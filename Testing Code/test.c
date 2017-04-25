#define F_CPU 1000000UL
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdlib.h>
#include "sonics_driver.h"
#include "rover.h"

#define thresh 300

volatile uint16_t sent_time = 0;
volatile uint8_t pulse_sent = 0;
volatile uint8_t echo_hist = 0;

volatile uint16_t distance1 = 0;
volatile uint16_t distance2 = 0;
volatile uint16_t distance3 = 0;

volatile uint8_t isr_hist = 0xFF;

void send_pulse1();
void send_pulse2();
void send_pulse3();
uint8_t check_limit();
void initializeSonics();
uint16_t currentTimerVal();
void check_pins();
void sensor_fix();


int main(){

		initializeTimer16();
		initializeSonics();
		LED_PORT_DDRC |= (1<<LED1);
    DDRD &= ~(1<<3);
    enable(LED_PORT,LED1);
    _delay_ms(500);
    disable(LED_PORT,LED1);

    sei();
		while(1){
			if((PIND & (1<<3))){
				send_pulse1();
				_delay_ms(60);

				// send_pulse3();
				// _delay_ms(60);

				if(distance1 < 1500 & distance1 > 0){
					stop();
					send_pulse2();
					_delay_ms(60);
					if(distance2 == 0){
						sensor_fix();
						send_pulse2();
						_delay_ms(60);
					}
					else if(distance2 < 1500){
						spinCW();
					}
					else if(distance2> 1500){
						spinCCW();
					}
				}


				// else if(distance3 > 0 && distance3 < 1500){
				// 	stop();
				// 	// spinCW();
				// }
				// else if(distance3 == 0){
				// 	sensor_fix(ECHO3);
				// }


				else{
					driveForward();
				}
			}else{
				stop();
			}

			//

		}

 return 0;
}


ISR(PCINT2_vect){
	cli();
	if(ECHO_PIN & (1<<echo_hist)){
		resetTimer16();
		startTimer16();
	}
	else{
		stopTimer16();
		switch(echo_hist){
			case ECHO1:{
				distance1 = TCNT1L | (TCNT1H << 8);
				break;
			}
			case ECHO2:{
				distance2 = TCNT1L | (TCNT1H << 8);
				break;
			}
			case ECHO3:{
				distance3 = TCNT1L | (TCNT1H << 8);
				break;
			}
		}

	}
 sei();
}

void initializeSonics(){
	disable(ECHO_PIN_DDR, ECHO1);
	disable(ECHO_PIN_DDR, ECHO2);
	disable(ECHO_PIN_DDR, ECHO3);
	disable(ECHO_PIN_DDR, ECHO4);
	// Set the pull up resistor high
	enable(ECHO_PIN, ECHO1);
	enable(ECHO_PIN, ECHO1);
	enable(ECHO_PIN, ECHO1);
	enable(ECHO_PIN, ECHO1);

	//Interrupt Enable Sequence
	enable(PCICR, PCIE2);
	enable(PCMSK2, ECHO1_PCINT); // ECHO1
	enable(PCMSK2, ECHO2_PCINT); // ECHO2
	enable(PCMSK2, ECHO3_PCINT); // ECHO3
	enable(PCMSK2, ECHO4_PCINT); // ECHO4

	TRIG_PORT_DDR |= (1 << TRIG1) | (1 << TRIG2) | (1 << TRIG3) | (1 << TRIG4); // set TRIGs to
										    // outputs
	// Turn off all trigger pins
	disable(TRIG_PORT, TRIG1);
	disable(TRIG_PORT, TRIG2);
	disable(TRIG_PORT, TRIG3);
	disable(TRIG_PORT, TRIG4);
}
uint16_t currentTimerVal(){
	return TCNT1L | (TCNT1H << 8);
}
void sensor_fix(pin){
	stop();
	enable(ECHO_PIN_DDR,pin);
	disable(ECHO_PIN_DDR,pin);
	_delay_ms(200);
	disable(ECHO_PIN_DDR,pin);
}

void send_pulse1(){
	echo_hist = ECHO1;
	enable(TRIG_PORT, TRIG1);
  _delay_us(10);
	disable(TRIG_PORT,TRIG1);

}
void send_pulse2(){
	echo_hist = ECHO2;
	enable(TRIG_PORT, TRIG2);
  _delay_us(10);
	disable(TRIG_PORT,TRIG2);
}
void send_pulse3(){
	echo_hist = ECHO3;
	enable(TRIG_PORT, TRIG3);
  _delay_us(10);
	disable(TRIG_PORT,TRIG3);
}
