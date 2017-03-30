#ifndef ROVER_H
#define ROVER_H

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>


// MACROS to allow more reusable code
// Motor driver outputs
/*
	OC0A - PD6 <- LA
	OC0B - PD5 <- LB
	OC2A - PB3 <- RA **** This might be more annoying
	OC2B - PD3 <- RB
*/
#define LEFT_MOTOR_PORTA PORTB
#define LEFT_MOTOR_PORTA_DDR DDRB
#define LEFT_MOTOR_PORTB PORTB
#define LEFT_MOTOR_PORTB_DDR DDRB
#define LEFT_MOTORA 1
#define LEFT_MOTORB 2


#define RIGHT_MOTOR_PORTA PORTB
#define RIGHT_MOTOR_PORTA_DDR DDRB
#define RIGHT_MOTOR_PORTB PORTB
#define RIGHT_MOTOR_PORTB_DDR DDRB
#define RIGHT_MOTORA 3
#define RIGHT_MOTORB 4


// Encoder stuff for distance tracking




// Initialize rover ports/DDRs
void initializeRover();


void setLeftSpeed();

void setRightSpeed();

void driveForward();

void driveReverse();

void stop();

void coast();

void spinCW();

void spinCCW();

void setLeftForward();

void setRightForward();

void setLeftReverse();

void setRightReverse();

void setLeftCoast();

void setRightCoast();

void setLeftStop();

void setRightStop();



#endif
