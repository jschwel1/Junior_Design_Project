#define F_CPU 1000000UL
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void left_motor(unsigned char dir){
	switch(dir){
		case 'F'://forward
		{
			PORTB |= (1<<1);
			break;
		}
		case 'R'://reverse
		{
			PORTB |= (1<<2);
			break;
		}
		case 'B'://brake;
		{
			PORTB &= ~(1<<1);
			PORTB &= ~(1<<2);
			break;
		}
		case 'C':
		{
			PORTB &= (0<<1|0<<2);
			break;
		}
	}
}

void right_motor(unsigned char dir){
	switch(dir){
		case 'F'://forward
		{
			PORTB |= (1<<3);
			break;
		}
		case 'R'://reverse
		{
			PORTB |= (1<<4);
			break;
		}
		case 'B'://brake;
		{
			PORTB &= ~(1<<3);
			PORTB &= ~(1<<4);
			break;
		}
		case 'C':
		{
			PORTB &= (0<<3|0<<4);
			break;
		}
	}
}

int main(){
  DDRB |=(1<<1|1<<2|1<<3|1<<4);
	DDRC = 0x00;
	uint8_t enc_count, enc_last,n;
	_delay_ms(2000);
	PORTB ^= (1<<1);
	PORTB ^= (4<<1);
	enc_count = 0;
  while(1){



			n = (PINC & (1<<1));
			if(enc_last == 0 && n == 1){
				if((PINC & (1<<2)) == 0){
					enc_count++;
				}
			}
			enc_last = n;

		PORTB ^= (1<<1);
		PORTB ^= (4<<1);
		// PORTB ^= (1<<1);
		// PORTB ^= (3<<1);
    // PORTB ^= (1<<1);
    // PORTB ^= (4<<1);
    // _delay_ms(100);
    // PORTB ^= (1<<1);
    // PORTB ^= (4<<1);
    // _delay_ms(1000);
  }
	return 0;
}
