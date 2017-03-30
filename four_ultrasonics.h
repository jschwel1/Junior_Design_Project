#ifndef FOUR_ULTRASONICS_H
#define FOUR_ULTRASONICS_H

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#define DELAY_MS 100
#define CLKS_PER_DELAY F_CPU*DELAY_MS/1000//F_CPU (clks/sec) * DELAY_MS(ms) / (1000 ms/sec) = clks
#define CLKS_PER_10MS F_CPU*10/1000 // 10 millis required to send ultrasonic pulse
#define TIMEOUT 10 // ms
#define CLKS_PER_TIMEOUT (F_CPU*TIMEOUT/1000) // 1/2 second time-out

// Speed of sound, v= 1125.33fps*12ft/in = 13503.96in/s
// dist = v*time -> tme = dist/v, but there and back so t = 2*dist/v
#define PROX_DIST 5 // distance (inches)
#define PROX_TIME (PROX_DIST<<2)/13504

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


// used with uint8_t ultraSonicStatus in four_ultrasonics.c
#define USSS 7 // status bit for if ultrasonic is sending
#define UST1 0 // which sensor it is currently using
#define UST2 1
#define UST3 2
#define UST4 3

// Testing stuff
#define TEST_PORT PORTD
#define TEST_PORT_DDR DDRD
#define TEST_PIN 0


void initializeTimer16();
void initializeUltraSonic();
void stopTimer16();
void startTimer16();

uint16_t getDist1();
uint16_t getDist2();
uint16_t getDist3();
uint16_t getDist4();

#endif
