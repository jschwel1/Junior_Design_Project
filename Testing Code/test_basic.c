#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

int main() {


  DDRD = 0x00;
  DDRB = 0xFF;

  // uint8_t v1 = 0x00;

//    PORTD |= (1<<1);
  while(1) {
    // if((PIND & (1<<1))){ // != 0x00){
    //   PORTB = v1;
    //   v1++;
    //
    // }
    PORTB ^= 0xFF;
    _delay_ms(500);


  }
  return 1;

}
