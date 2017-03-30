#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#ifndef F_CPU
#define F_CPU 1000000 // Switch to 8000000 for Dom
#endif

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// HC-SR04 Ultrasonic Ranging Module
#define TRIGGER 4
#define LED 0
#define TRIGGER_PORT PORTD
#define TRIGGER_PORT_DDR DDRC
#define TRIGGER_PORT_DDR_MASK 0x11;

#define ECHO 3 // D2 => INT1
#define ECHO_PIN PIND
#define ECHO_PIN_DDR DDRD
#define ECHO_PIN_DDR_MASK 0x00 // all inputs

#define PROXIMITY 174

#define CYCLE_DELAY_TIME 65 // Anything over 60ms should work according to datasheet
#define PULSE_TIME 10 // datasheet specifies 10us pulse
// Add timer stuff to count times
#define com1a1 7
#define com1a0 6
#define com1b1 5
#define com1b0 4

#define wgm10 0
#define wgm11 1
#define wgm12 3
#define wgm13 4

#define icnc1 7
#define ices1 6

#define cs10 0
#define cs11 1
#define cs12 2

// Interrupt Stuff
#define ISC11 3
#define ISC10 2

// EIMSK
#define INT1 1

// EIFR
#define INTF1 1

void initializeUltraSonic();
void trigger();
void turnOffTimer();
void turnOnTimer();


void ledOn();
void ledOff();

uint16_t getDist();

#endif
