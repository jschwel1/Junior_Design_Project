#include <avr/io.h>
#include <avr/interrupt.h>
#include "sonics_driver.h"



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
void stopTimer16(){
	TCCR1B &= ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS10);
}
void startTimer16(){
	TCCR1B |= (1 << CS10);
	TCCR1B &= ~(1 << CS12) & ~(1 << CS11);
}
void startTimer16_PS8(){
	// CS1(2:0) = 010
	TCCR1B |= (1 << CS11);
	TCCR1B &= ~(1 << CS12) & ~(1 << CS10);

}
void resetTimer16(){
	TCNT1H = 0x00;
	TCNT1L = 0x00;
}
