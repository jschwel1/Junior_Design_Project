#ifndef F_CPU
	#define F_CPU 1000000UL
#endif

#include "new_ultrasonic.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t time = 0x00;


void setup(){
	// TRIGGER/LED
	TRIG_PORT_DDR = 0xFF;
	TRIG_PORT &= ~(1 << TRIG1) & ~(1 << LED);

	// ECHO 
	ECHO_PIN_DDR = 0x00;
	ECHO_PIN |= (1 << ECHO);

	// TIMER
	TCCR1A &= ~(1 << COM1A1) & ~(1 << COM1A0) & ~(1 << COM1B1) & ~(1 << COM1B0) & ~(1 << WGM11) & ~(1 << WGM10);
	TCCR1B &= ~(1 << WGM13) & ~(1 << WGM12) & ~(1 << ICNC1) & ~(1 << ICES1) & ~(1 << CS12) & ~(1 << CS10);
	TCNT1L = 0x00;
	TCNT1H = 0x00;

	// INTERRUPT 

	EICRA |= (1 << ISC10);
	EICRA &= (1 << ISC11);

	EIMSK |= (1 << INT0);
	EIFR |= (1 << INTF0);
}
void trigger(){
	cli();
	TRIG_PORT |= (1 << TRIG1);
	_delay_ms(10);
	TRIG_PORT &= ~(1 << TRIG1);
	sei();
}

void turnOffTimer(){
	TCCR1B &= ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS10);
}
void turnOnTimer(){
	TCCR1B &= ~(1 << CS12) & ~(1 << CS11);
	TCCR1B |=  (1 << CS10);
}

ISR(INT1_vect){
	cli();

	TRIG_PORT |= (1 << LED);

	if (!(ECHO_PIN & (1 << ECHO))){
		turnOffTimer();
		time = ((TCNT1H << 8) | TCNT1L);
	}
	else {
		TCNT1L = 0x00;
		TCNT1H = 0x00;
		turnOnTimer();
	}
	TRIG_PORT &= ~(1 << LED);
	sei();
}
