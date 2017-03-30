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
#define LEFT_MOTOR_PORTA PORTD
#define LEFT_MOTOR_PORTA_DDR DDRD
#define LEFT_MOTOR_PORTB PORTD
#define LEFT_MOTOR_PORTB_DDR DDRD
#define LEFT_PWM_COMPA OCR0A
#define LEFT_PWM_COMPB OCR0B 
#define LEFT_MOTORA 6
#define LEFT_MOTORB 5
#define TCCR_LEFTA TCCR0A
#define TCCR_LEFTB TCCR0B
#define TIMSK_LEFT TIMSK0

#define RIGHT_MOTOR_PORTA PORTB
#define RIGHT_MOTOR_PORTA_DDR DDRB
#define RIGHT_MOTOR_PORTB PORTD
#define RIGHT_MOTOR_PORTB_DDR DDRD
#define RIGHT_PWM_COMPA OCR2A
#define RIGHT_PWM_COMPB OCR2B
#define RIGHT_MOTORA 3
#define RIGHT_MOTORB 3
#define TCCR_RIGHTA TCCR2A
#define TCCR_RIGHTB TCCR2B
#define TIMSK_RIGHT TIMSK2

// Encoder stuff (alternative sensors will likely be more accurate and useful)



// PWM stuff
#define PWM 0x7F

// TCCR0A/TCCR2A
#define COMnA1 7
#define COMnA0 6
#define COMnB1 5
#define COMnB0 4
#define WGMn1 1
#define WGMn0 0

// TCCR0B/TCCR2B
#define FOCnA 7
#define FOCnB 6
#define WGMn2 3
#define CSn2 2
#define CSn1 1
#define CSn0 0

// TIMSK0/TIMSK2
#define OCIEnB 2
#define OCIEnA 1
#define TOIEn 0


// Initialize rover ports/DDRs
void initializeRover();

/* void setPWM(uint8_t)
	Sets the pwm used to drive the motors at various speeds. This will used
	Timer0 and Timer2 (8-bit timer) and outputs A and B for each H-bridge driver.
	by inverting 1 or both outputs, we can aim to drive each wheel forward/backward.

	uint8_t pwm - pwm inversion point for A and B of both timers to keep them consistent

*/
void initializePWM();

void setLeftSpeed();

void setRightSpeed();

void driveForward();

void driveReverse();

void spinCW();

void spinCCW();

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
