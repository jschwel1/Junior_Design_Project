#include <avr/io.h>
#include <avr/interrupt.h>
#include "four_ultrasonics.h"

// The stage in getting distance from a sensor is stored in the 2 MSB
// The current sensor is the 2 LSB. (this method can be expanded to more
// sensors with only slight modifications);
uint8_t ultraSonicStatus = 0x00;

volatile uint16_t dist1 = 0x0000;
volatile uint16_t dist2 = 0x0000;
volatile uint16_t dist3 = 0x0000;
volatile uint16_t dist4 = 0x0000;



void initializeTimer16(){
	TCCR1A &= ~(1 << COM1A1) & ~(1 << COM1A0) & ~(1 << COM1B1) & ~(1 << COM1B0) // Normal Port ops
		& ~(1 << WGM11) & ~(1 << WGM10); // Normal Counter

	TCCR1B &= ~(1 << WGM13) & ~(1 << WGM12) // Normal Counter
		& ~(1 << ICNC1) & ~(1 << ICES1) // No input capture 
		& ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS10); // Timer initially stopped

	TCCR1C &= ~(1 << FOC1A) & ~(1 << FOC1B);// They need to be 0 anyway
	
	TIMSK1 &= ~(1 << ICIE1) & ~(1 << TOIE1) & ~(1 << OCIE1B); // No Input Capture or overflow interrupts
	TIMSK1 |= (1 << OCIE1A);// | (1 << OCIO1B); // Interrupt on compare outputs with A and B
						 // Stored in OCR1AH|ORCR1AL, OCR1BH|OCR1BL
}

void initializeUltraSonic(){
	// External Interrupt & ECHO Pin
	ECHO_PIN_PORT_DDR &= ~(1 << ECHO_PIN);
	ECHO_PIN_PORT |= (1 << ECHO_PIN); // Set pull up resistor high

	EICRA |= (1 << ISC10);	// Set it to interrupt on any logical change on INT1
	EICRA &= ~(1 << ISC11);
	
	EIMSK |= (1 << INT1); // enable interrupts
	// TRIG Port
	TRIG_PORT_DDR |= (1 << TRIG1) | (1 << TRIG2) | (1 << TRIG3) | (1 << TRIG4); // set TRIGs to
										    // outputs	
	TRIG_PORT &= ~(1 << TRIG1) & ~(1 << TRIG2) & ~(1 << TRIG3) & ~(1 << TRIG4); // Set all trigger low	

	TEST_PORT_DDR |= (1 << TEST_PIN);
	TEST_PORT &= ~(1 << TEST_PIN);


	// clear timer, set compare to delay time, and start timer
	resetTimer16();
	setCompare1A(CLKS_PER_DELAY);
	startTimer16_PS8();
}

void stopTimer16(){
	TCCR1B &= ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS10);
}
void startTimer16(){
	TCCR1B |= (1 << CS10);
	TCCR1B &= ~(1 << CS12) & ~(1 << CS11);
}
void startTimer16_PS8(){
	// CS1(2:0) = 010
	TCCR1B |= (1 << CS11);
	TCCR1B &= ~(1 << CS12) & ~(1 << CS10);

}
void setCompare1A(uint16_t comp){
	OCR1AH = (comp >> 8);
	OCR1AL = comp;
}
void resetTimer16(){
	TCNT1H = 0x00;
	TCNT1L = 0x00;
}

void enable(uint8_t port, uint8_t pin){
	port |= (1 << pin);
}
void disable(uint8_t port, uint8_t pin){
	port &= ~(1 << pin);
}
uint8_t isEnabled(uint8_t port, uint8_t pin){
	return port&(1<<pin);
}
/* Ultrasonic stages:
	0 - Enable trigger, reset timer, OCR1A = 10ms
	1 - Disable trigger, reset timer, OCR1A = TIMEOUT, enable INT1
	2 - Waiting for echo to go high 
	  - Could also timeout -> ignore time/dist, continue to next sensor
	3 - Waiting for echo to go low
*/
uint8_t isStage(uint8_t stage){
	return (((ultraSonicStatus&0xC0) >> 6) == stage);
}
void nextStage(){
	uint8_t next = 0x03&(((ultraSonicStatus&0xC0) >> 6)+1); // increment the current stage
	ultraSonicStatus &= 0x3F;	//clear the stage section
	ultraSonicStatus |= (next << 6); // place the stage in the correct position

}

uint8_t getSensorTrigPin(){
	switch (ultraSonicStatus&0x03){ //current sensor is stored in 2 LSB
		case 0: return TRIG1;
		case 1: return TRIG2;
		case 2: return TRIG3;
		case 3: return TRIG4;
	}
	// default
	return TRIG1;
}
void setDist(){
	switch (ultraSonicStatus&0x03){ //current sensor is stored in 2 LSB
		case 0: dist1 = TCNT1L | (TCNT1H << 8);
				break;
		case 1: dist2 = TCNT1L | (TCNT1H << 8);
				break;
		case 2: dist3 = TCNT1L | (TCNT1H << 8);
				break;
		case 3: dist4 = TCNT1L | (TCNT1H << 8);
				break;
	}
}

void moveToNextSensor(){
	uint8_t next = 0xFC&((ultraSonicStatus&0x03)+1);// (++sensor)%4
	ultraSonicStatus &= 0xFC;
	ultraSonicStatus |= next;
}



// INT1 ISR -> for echo pin
ISR(INT1_vect){
	cli(); // prevent other things from happening

	// waiting for echo to go high (just went high)
	if (isStage(2)){
		stopTimer16();
		resetTimer16();
		setCompare1A(CLKS_PER_TIMEOUT);
		nextStage();
		startTimer16(); // start clk with no prescale
	}
	// if waiting for echo to go low (just went low)
	else if(isStage(3)){
		stopTimer16();
		setDist();
		setCompare1A(CLKS_PER_DELAY);
		nextStage();
		moveToNextSensor();
		startTimer16_PS8(); // slow timer for longer delay
	}


	sei();
}

// TIMER1_COMPA ISR -> For sending out a trigger and checking for timeouts
ISR(TIMER1_COMPA_vect){
	cli(); //ensure no interrupts
		// delay has completed, turn on trigger and get pulse started
		if(isStage(0)){
			stopTimer16();
			enable(TRIG_PORT, getSensorTrigPin());
			setCompare1A(CLKS_PER_10MS);
			nextStage();
			startTimer16();
		}
		// 10ms pulse completed, clear trigger and set up for echo/timeout
		else if (isStage(1)){
			stopTimer16();
			disable(TRIG_PORT, getSensorTrigPin());
			setCompare1A(CLKS_PER_TIMEOUT);
			nextStage();
			startTimer16();
		}
		// check for timeout
		else if (isStage(2) || isStage(3)){
			stopTimer16();
			setCompare1A(CLKS_PER_DELAY);
			nextStage();
			moveToNextSensor();
			startTimer16_PS8();
		}

	sei();
}


uint16_t getDist1(){
	return dist1;
}
uint16_t getDist2(){
	return dist2;
}
uint16_t getDist3(){
	return dist3;
}
uint16_t getDist4(){
	return dist4;
}
