
#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ultrasonic.h"

volatile uint16_t time;



void initializeUltraSonic(){

	// Settings for Ultrasonic Sensor
	TRIGGER_PORT_DDR = TRIGGER_PORT_DDR_MASK;
	TRIGGER_PORT &= ~(1<<TRIGGER); // Clear trigger ppin
	
	
	ECHO_PIN_DDR = ECHO_PIN_DDR_MASK;
	ECHO_PIN |= (1<<ECHO); // set pull-up resistor
	
	// Timer settings

	TCCR1A &= ~(1 << com1a1) & ~(1 << com1b0) & ~(1 << com1b1) & ~(1 << com1b0) & ~(1 << wgm11) & ~(1 << wgm10);

	TCCR1B &= ~(1 << wgm13) &~(1 << wgm12) & ~(1 << icnc1) & ~( 1 << ices1) &
			  ~(1 << cs12) & ~(1 << cs10);
	
	TCNT1L = 0x0000;
	TCNT1H = 0x0000;

	EICRA |= (1 << ISC10);
	EICRA &= ~(1 << ISC11);

	EIMSK |= (1 << INT1);
	EIFR |= ( 1 << INTF1); 

	turnOffTimer();

	return;
}

void turnOffTimer(){

	TCCR1B &= ~(1 << cs12) & ~( 1 << cs11) 
			& ~(1 << cs10);	
	return;
}

void turnOnTimer(){

	TCCR1B &= ~(1 << cs12) & ~( 1 << cs11); 
	TCCR1B |= (1 << cs10);

	return;
}

void trigger(){
	cli(); // Turn off interrupts while sending the pulse

	// From Datasheet:
	// Send a 10us pulse of the Trigger to send out an 8 cycle burst
	// of ultrasound at 40kHz.
	TRIGGER_PORT |= (1<<TRIGGER);
	_delay_us(PULSE_TIME);
	TRIGGER_PORT &= ~(1<<TRIGGER);

	// Turn interrupts back on
	sei();
}

uint16_t getDist(){
	return time;
}

void ledOn(){
	TRIGGER_PORT |= (1 << LED);
}
void ledOff(){
	TRIGGER_PORT &= ~(1 << LED);
}

ISR(INT1_vect) {
	cli();
	if ( !(ECHO_PIN & (1 << ECHO))){
		turnOffTimer();
		time =  ((TCNT1H << 8)| TCNT1L) ;
	
	//	TRIGGER_PORT &= ~(1 << LED);
	}
	else{
		TCNT1L = 0x00;
		TCNT1H = 0x00;
		turnOnTimer();
		
//		TRIGGER_PORT |= (1 << LED);
	}	
	sei();
}
