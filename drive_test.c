#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "rover.h"
#include "four_ultrasonics.h"
#include "shift_out.h"


#define LED_PORT PORTD
#define LED_PORT_DDRD DDRD
#define LED1 5

int main(){


	LED_PORT_DDRD |= (1<<LED1);
	while(1){
		enable(&LED_PORT,LED1);
		_delay_ms(200);
		disable(&LED_PORT,LED1);
		_delay_ms(200);
	}

// 	initializeTimer16();
// 	initializeUltraSonic();
// //	initializeShiftReg();
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
