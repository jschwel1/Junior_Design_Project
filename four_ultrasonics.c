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
	// ECHO_PIN_PORT_DDR &= ~(1 << ECHO_PIN) & ~(1<<2);
	// ECHO_PIN_PORT |= (1 << ECHO_PIN) | (1<<2); // Set pull up resistor high
	//
	//
	// EICRA |= (1 << ISC10);	// Set it to interrupt on any logical change on INT1
	// EICRA &= ~(1 << ISC11);
	//
	// EICRA |= (1 << ISC00) | ~(1<< ISC01);
	// EIMSK |= (1 << INT0);  //
	//
	// EIMSK |= (1 << INT1); // enable interrupts

	// Set all echos to inputs (DDR = '0')
	disable(ECHO_PIN_DDR, ECHO1);
	disable(ECHO_PIN_DDR, ECHO2);
	disable(ECHO_PIN_DDR, ECHO3);
	disable(ECHO_PIN_DDR, ECHO4);
	// Set the pull up resistor high
	/*
	enable(ECHO_PIN, ECHO1);
	enable(ECHO_PIN, ECHO1);
	enable(ECHO_PIN, ECHO1);
	enable(ECHO_PIN, ECHO1);
	*/
	disable(ECHO_PIN, ECHO1);
	disable(ECHO_PIN, ECHO2);
	disable(ECHO_PIN, ECHO3);
	disable(ECHO_PIN, ECHO4);
	// Set the PCINT settings
	// Pin Change Interrupt Control Register for Pins 23->16
	enable(PCICR, PCIE2);
	enable(PCMSK2, ECHO1_PCINT); // ECHO1
	enable(PCMSK2, ECHO2_PCINT); // ECHO2
	enable(PCMSK2, ECHO3_PCINT); // ECHO3
	enable(PCMSK2, ECHO4_PCINT); // ECHO4

	// TRIG Port
	TRIG_PORT_DDR |= (1 << TRIG1) | (1 << TRIG2) | (1 << TRIG3) | (1 << TRIG4); // set TRIGs to
										    // outputs
	// Turn off all trigger pins
	disable(TRIG_PORT, TRIG1);
	disable(TRIG_PORT, TRIG2);
	disable(TRIG_PORT, TRIG3);
	disable(TRIG_PORT, TRIG4);


	ultraSonicStatus = 0x00;
	// clear timer, set compare to delay time, and start timer
	resetTimer16();
	setCompare1A(CLKS_PER_DELAY);
	startTimer16_PS8();

	DDRB |= (0x02);
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
uint8_t getSensorEchoPin(){
	switch (ultraSonicStatus&0x03){ //current sensor is stored in 2 LSB
		case 0: return ECHO1;
		case 1: return ECHO2;
		case 2: return ECHO3;
		case 3: return ECHO4;
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
	uint8_t next = 0x03&((ultraSonicStatus&0x03)+1);// (++sensor)%4
	ultraSonicStatus &= 0xFC;
	ultraSonicStatus |= next;
}

void trigger(){
	enable(TRIG_PORT, getSensorTrigPin());
	_delay_ms(10);
	disable(TRIG_PORT, getSensorTrigPin());
}

ISR(PCINT2_vect){
	// Check the correct
	
	cli(); // prevent other things from happening
	if (isEnabled(ECHO_PIN, ECHO1) || isEnabled(ECHO_PIN, ECHO2) || isEnabled(ECHO_PIN, ECHO3) || isEnabled(ECHO_PIN, ECHO4)){
		//enable(PORTB, 1);
		PORTB ^= 0x02;
		resetTimer16();
		startTimer16();
	}
	else{
		stopTimer16();
		//disable(PORTB, 1);
		PORTB ^= 0x02;
		// setDist();
//		dist2 = TCNT1L | (TCNT1H << 8);
		setDist();
		
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
