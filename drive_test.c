#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "rover.h"
#include "four_ultrasonics.h"
#include "shift_out.h"


#define LED_PORT PORTB
#define LED_PORT_DDRD DDRB
#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4


void startupSequence();

int main(){

	initializeTimer16();
 	initializeUltraSonic();
	// initializeShiftReg();

	//LED_PORT_DDRD |= (1<<LED1) | (1<<LED2) | (1<<LED3) | (1<<LED4);

	// startupSequence();
	sei();
	// trigger();
	while(1){
		// push(getDist1());

		_delay_ms(100);
		moveToNextSensor();
/*		disable(LED_PORT, LED1);
		disable(LED_PORT, LED2);
		disable(LED_PORT, LED3);
		disable(LED_PORT, LED4);
*/		trigger();
/*
		if((getDist1()<200)) enable(LED_PORT,LED1);
		else disable(LED_PORT,LED1);
		
		if((getDist2()<200)) enable(LED_PORT,LED2);
		else disable(LED_PORT,LED2);

		if((getDist3()<200)) enable(LED_PORT,LED3);
		else disable(LED_PORT,LED3);
		
		if((getDist4()<200)) enable(LED_PORT,LED4);
		else disable(LED_PORT,LED4);
*/		
	}

// 	sei();

//	uint8_t sig = 0x00;

	// while(1){
	// 	sei();
	// 	_delay_ms(75);
	// 	trigger();
	//
	// 	if (getDist1() < 200) enable(LED_PORT, LED1);
	// 	else disable(LED_PORT, LED1);
	//
	// }

	return 0;

}



void startupSequence(){
	enable(LED_PORT, LED1);
	_delay_ms(200);
	disable(LED_PORT, LED1);
	_delay_ms(200);

	enable(LED_PORT, LED1);
	_delay_ms(100);
	disable(LED_PORT, LED1);
	_delay_ms(100);

	enable(LED_PORT, LED1);
	_delay_ms(50);
	disable(LED_PORT, LED1);
	_delay_ms(50);

	enable(LED_PORT, LED1);
	_delay_ms(25);
	disable(LED_PORT, LED1);
	_delay_ms(25);


	push(0x11);
	_delay_ms(200);
	push(0x33);
	_delay_ms(200);
	push(0x77);
	_delay_ms(200);
	push(0xFF);
	_delay_ms(200);
	push(0xEE);
	_delay_ms(200);
	push(0xCC);
	_delay_ms(200);
	push(0x88);
	_delay_ms(200);
	push(0x00);
	_delay_ms(200);

	//uint8_t i;
	// for (i = 0; i < 255; i++){
	// 	push(i);
	// 	_delay_ms(5);
	// }
	push(0x00);



	return;

}
