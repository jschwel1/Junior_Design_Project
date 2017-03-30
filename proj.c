#define F_CPU 1000000UL
#include "rover.h"
#include <avr/io.h>
#include <util/delay.h>


int main(){
	initializeRover();

	while(1){
		driveForward();
		_delay_ms(2000);
		driveReverse();
		_delay_ms(2000);
		coast();
		_delay_ms(2000);
		stop();
		_delay_ms(2000);
	}
	return 0;
}
