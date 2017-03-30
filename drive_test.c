#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "rover.h"
#include "four_ultrasonics.h"
#include "shift_out.h"

int main(){
	initializeTimer16();
	initializeUltraSonic();
//	initializeShiftReg();
	sei();

//	uint8_t sig = 0x00;

	while(1){
		sei();
			
	}

	return 0;

}
