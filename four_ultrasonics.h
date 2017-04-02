#ifndef FOUR_ULTRASONICS_H
#define FOUR_ULTRASONICS_H
#ifndef F_CPU
#define F_CPU 1000000UL
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Using no prescale, timer maxes out at ~65.5ms
// With prescale of 8 (next available prescale) max = 524 ms
// This delay will use a prescale of 8
#define DELAY_MS 100
#define CLKS_PER_DELAY (F_CPU>>3)*DELAY_MS/1000//F_CPU (clks/sec) * DELAY_MS(ms) / (1000 ms/sec) = clks
// These will not use any prescale
#define CLKS_PER_10MS F_CPU*10/1000 // 10 millis required to send ultrasonic pulse
#define TIMEOUT 20 // ms
#define CLKS_PER_TIMEOUT ((F_CPU)*TIMEOUT/1000) //

// Dist = uS/148 -> ms = Dist(in)*148(us/in)
#define PROX_DIST 5 // distance (inches)
#define PROX_TIME PROX_DIST*148

// TCCR1A
#define COM1A1 7 // Leave both ports to normal operation
#define COM1A0 6
#define COM1B1 5
#define COM1B0 4
#define WGM11 1
#define WGM10 0

// TCCR1B
#define ICNC1 7
#define ICES1 6
#define WGM13 4
#define WGM12 3
#define CS12 2
#define CS11 1
#define CS10 0

// TCCR1C
#define FOC1A 7
#define FOC1B 6

// TIMSK1
#define ICIE1 5 // Input capture IE
#define OCIE1B 2 // OCRB Match IE
#define OSIE1A 1 // OCRA Match IO
#define TOIE1 0  // Overflow IE

// Ultrasonic stuff
#define ECHO_PIN_PORT PIND
#define ECHO_PIN_PORT_DDR DDRD
#define ECHO_PIN 3			// Echo pin on INT1
#define TRIG_PORT PORTD
#define TRIG_PORT_DDR DDRD
#define TRIG1 4
#define TRIG2 2
#define TRIG3 1
#define TRIG4 0

// Interrupt
// EICRA
#define ISC11 3
#define ISC10 2
// EIMSK
#define INT1 1
// EIFR
#define INTF1 1


#define enable(port, pin) ((port) |= (1 << (pin)))
#define disable(port, pin) ((port) &= ~(1 << (pin)))
#define isEnabled(port, pin) (((port) & (1 << (pin))) != 0x00)

void initializeTimer16();
void initializeUltraSonic();

void stopTimer16();
void startTimer16();
void startTimer16_PS8();
void resetTimer16();
void setCompare1A(uint16_t comp);

//void enable(volatile unsigned char *port, uint8_t pin);
//void disable(volatile unsigned char *port, uint8_t pin);
//uint8_t isEnabled(uint8_t port, uint8_t pin);
/* Ultrasonic stages:
	1 - Enable trigger, reset timer, OCR1A = 10ms
	2 - Disable trigger, reset timer, OCR1A = TIMEOUT, enable INT1
	3 - Waiting for echo to go high
	  - Could also timeout -> ignore time/dist, continue to next sensor
	4 - Waiting for echo to go low
*/
uint8_t isStage(uint8_t stage);
void nextStage();
uint8_t getSensorTrigPin();
void moveToNextSensor();

void setDist();

uint16_t getDist1();
uint16_t getDist2();
uint16_t getDist3();
uint16_t getDist4();

#endif
