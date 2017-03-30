#include <avr/io.h>
#include <avr/interrupt.h>
#include "four_ultrasonics.h"


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
	TCNT1H = 0x00;
	TCNT1L = 0x00;
	OCR1AH = (uint8_t)(((uint16_t)CLKS_PER_DELAY) >> 8);
	OCR1AL = (uint8_t)((CLKS_PER_DELAY));
	startTimer16();
}

void stopTimer16(){
	TCCR1B &= ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS10);
}
void startTimer16(){
	TCCR1B |= (1 << CS10);
	TCCR1B &= ~(1 << CS12) & ~(1 << CS11);
}

// INT1 ISR -> for echo pin
ISR(INT1_vect){
	TEST_PORT &= (1 << TEST_PIN); //flag 
	cli();

	// if the echo pin is high
	if (ECHO_PIN_PORT & (1 << ECHO_PIN)){
		// clear and start the timer
		TCNT1H = 0x00;
		TCNT1L = 0x00;
		startTimer16();
	}	
	else{
		stopTimer16();

		
		// correct ultraSonicStatus Status & get distance
		ultraSonicStatus |= (1 << USSS);
		if (ultraSonicStatus & (1 << UST1)){
			ultraSonicStatus &= ~(1 << UST1);
			ultraSonicStatus |= (1 << UST2);
			dist1 = (TCNT1H << 8)|TCNT1L;
		}
		if (ultraSonicStatus & (1 << UST2)){
			ultraSonicStatus &= ~(1 << UST2);
			ultraSonicStatus |= (1 << UST3);
			dist2 = (TCNT1H << 8)|TCNT1L;
		}
		if (ultraSonicStatus & (1 << UST3)){
			ultraSonicStatus &= ~(1 << UST3);
			ultraSonicStatus |= (1 << UST4);
			dist3 = (TCNT1H << 8)|TCNT1L;
		}
		if (ultraSonicStatus & (1 << UST4)){
			ultraSonicStatus &= ~(1 << UST4);
			ultraSonicStatus |= (1 << UST1);
			dist4 = (TCNT1H << 8)|TCNT1L;
		}
		// clear the timer and start it again, so it can trigger another sensor		
		TCNT1H = 0x00;
		TCNT1L = 0x00;
		OCR1AH = (uint8_t)(((uint16_t)CLKS_PER_DELAY) >> 8);
		OCR1AL = (uint8_t)((CLKS_PER_DELAY));
		startTimer16();

	}

	sei();
}

// TIMER1_COMPA ISR -> For sending out a trigger and checking for timeouts
ISR(TIMER1_COMPA_vect){
	cli(); //ensure no interrupts

	TEST_PORT |= (1 << TEST_PIN); //flag 

	// if sending a pulse
	if (ultraSonicStatus & (1 << USSS)){
		
		// choose the correct trigger,
		// set high, change new delay for 10ms
		// reset tnct1h/l and dddclear USSS bit
		if (ultraSonicStatus & (1 << UST1)){
			// if the trigger pin is already high
			if (TRIG_PORT & (1 << TRIG1)){
				TRIG_PORT &= ~(1 << TRIG1);
				OCR1AH = (uint8_t)(((uint16_t)(CLKS_PER_TIMEOUT)));
				OCR1AL = (uint8_t)(CLKS_PER_TIMEOUT);
			}
			else{
				TRIG_PORT |= (1 << TRIG1);
				OCR1AH = (uint8_t)(((uint16_t)(CLKS_PER_10MS)));
				OCR1AL = (uint8_t)(CLKS_PER_10MS);
	
			}
		}
		if (ultraSonicStatus & (1 << UST2)){
			if (TRIG_PORT & (1 << TRIG2)){
				TRIG_PORT &= ~(1 << TRIG2);
				OCR1AH = (uint8_t)(((uint16_t)(CLKS_PER_TIMEOUT)));
				OCR1AL = (uint8_t)(CLKS_PER_TIMEOUT);
			}
			else{
				TRIG_PORT |= (1 << TRIG2);
				OCR1AH = (uint8_t)(((uint16_t)(CLKS_PER_10MS)));
				OCR1AL = (uint8_t)(CLKS_PER_10MS);
	
			}
	
		}
		if (ultraSonicStatus & (1 << UST3)){
			if (TRIG_PORT & (1 << TRIG3)){
				TRIG_PORT &= ~(1 << TRIG3);
				OCR1AH = (uint8_t)(((uint16_t)(CLKS_PER_TIMEOUT)));
				OCR1AL = (uint8_t)(CLKS_PER_TIMEOUT);
			}
			else{
				TRIG_PORT |= (1 << TRIG3);
				OCR1AH = (uint8_t)(((uint16_t)(CLKS_PER_10MS)));
				OCR1AL = (uint8_t)(CLKS_PER_10MS);
	
			}
	
		}
		if (ultraSonicStatus & (1 << UST4)){
			if (TRIG_PORT & (1 << TRIG4)){
				TRIG_PORT &= ~(1 << TRIG4);
				OCR1AH = (uint8_t)(((uint16_t)(CLKS_PER_TIMEOUT)));
				OCR1AL = (uint8_t)(CLKS_PER_TIMEOUT);
			}
			else{
				TRIG_PORT |= (1 << TRIG4);
				OCR1AH = (uint8_t)(((uint16_t)(CLKS_PER_10MS)));
				OCR1AL = (uint8_t)(CLKS_PER_10MS);
	
			}
	
		}

		// regardless of whether the trigger pin was high or low
		// the timer needs to be reset and change the status to 
		// waiting for an echo
		TCNT1H = 0x00;
		TCNT1L = 0x00;
		startTimer16();
		ultraSonicStatus &= ~(1 << USSS);
		
	}
	// if waiting for a the echo, check for timeout
	// Move sensor to the next one hold old value since it likely won't 
	// change too much and if something is wrong with that sensor, we 
	// cannot have it continuously timing out and miss data from the other sensors
	
	else{
		TCNT1H = 0x00;
		TCNT1L = 0x00;
		OCR1AH = (uint8_t)(((uint16_t)CLKS_PER_DELAY) >> 8);
		OCR1AH = (uint8_t)((CLKS_PER_DELAY));
		// correct ultraSonicStatus Status
		ultraSonicStatus |= (1 << USSS);
		if (ultraSonicStatus & (1 << UST1)){
			ultraSonicStatus &= ~(1 << UST1);
			ultraSonicStatus |= (1 << UST2);
		}
		if (ultraSonicStatus & (1 << UST2)){
			ultraSonicStatus &= ~(1 << UST2);
			ultraSonicStatus |= (1 << UST3);
		}
		if (ultraSonicStatus & (1 << UST3)){
			ultraSonicStatus &= ~(1 << UST3);
			ultraSonicStatus |= (1 << UST4);
		}
		if (ultraSonicStatus & (1 << UST4)){
			ultraSonicStatus &= ~(1 << UST4);
			ultraSonicStatus |= (1 << UST1);
		}

		startTimer16();
			
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
