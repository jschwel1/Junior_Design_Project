#include "rover_pwm.h"


void initializeRover(){
	// set up Motor DDRs
	LEFT_MOTOR_PORTA_DDR |= (1 << LEFT_MOTORA);
	LEFT_MOTOR_PORTB_DDR |= (1 << LEFT_MOTORB);
	RIGHT_MOTOR_PORTA_DDR |= (1 << RIGHT_MOTORA);
	RIGHT_MOTOR_PORTB_DDR |= (1 << RIGHT_MOTORB);

	
}


void initializePWM(){
	// set up for fast pwm, set on compare match -> PWM 0 = ON
	TCCR_LEFTA |= 	(1 << COMnA1) | (1 << COMnA0) |
					(1 << COMnB1) | (1 << COMnB0) |
					(1 << WGMn1) | (1 << WGMn0);
	TCCR_LEFTB &= 	~(1 << FOCnA) & ~(1 << FOCnB) &
					~(1 << WGMn2) & ~(1 << CSn2) & ~(1 << CSn1);
	TCCR_LEFTB |= 	(1 << CSn0);
	TIMSK_LEFT &= 	~(1 << OCIEnB) & ~(1 << OCIEnA) & ~(1 << TOIEn);

	TCCR_RIGHTA |= 	(1 << COMnA1) | (1 << COMnA0) |
					(1 << COMnB1) | (1 << COMnB0) |
					(1 << WGMn1) | (1 << WGMn0);
	TCCR_RIGHTB &= 	~(1 << FOCnA) & ~(1 << FOCnB) &
					~(1 << WGMn2) & ~(1 << CSn2) & ~(1 << CSn1);
	TCCR_RIGHTB |= 	(1 << CSn0);
	TIMSK_RIGHT &= 	~(1 << OCIEnB) & ~(1 << OCIEnA) & ~(1 << TOIEn);

	// set up compares in OC0nA/B
	setLeftSpeed();
	setRightSpeed();
	// Turn off motors so it does not move until it is supposed to
	stop();
}

void setLeftSpeed(){
	LEFT_PWM_COMPA = PWM;
	LEFT_PWM_COMPA = PWM;
}

void setRightSpeed(){
	RIGHT_PWM_COMPA = PWM;
	RIGHT_PWM_COMPB = PWM;
}

void driveForward(){
	setLeftForward();
	setRightForward();
}

void driveReverse(){
	setLeftReverse();
	setRightReverse();
}

void spinCW(){
	setLeftForward();
	setRightReverse();
}

void spinCCW(){
	setLeftReverse();
	setRightForward();
}

void stop(){
	setLeftStop();
	setRightStop();
}

void coast(){
	setLeftCoast();
	setRightCoast();
}

void setLeftForward(){
	//	Forward <= (xIN1=PWM) & (xIN0=0);
	// turn off PWM function in port B, then set it low
	// turn on PWM function  in port A
	TCCR_LEFTA &= ~(1 << COMnB1) & ~(1 << COMnB0);
	TCCR_LEFTA |= (1 << COMnA1) | (1 << COMnA0);

	LEFT_MOTOR_PORTB &= ~(1 << LEFT_MOTORB);

	return;
}

void setRightForward(){
	//	Forward <= (xIN1=PWM) & (xIN0=0);
	// turn off PWM function in port B, then set it low
	// turn on PWM function  in port A
	TCCR_RIGHTA &= ~(1 << COMnB1) & ~(1 << COMnB0);
	TCCR_RIGHTA |= (1 << COMnA1) | (1 << COMnA0);

	RIGHT_MOTOR_PORTB &= ~(1 << RIGHT_MOTORB);
	return;
}

void setLeftReverse(){
	// Reverse <= (xIN1=0) & (xIN0=PWM);
	// turn on PWM function in port B
	// turn off PWM function  in port A, then set it low
	TCCR_LEFTA &= ~(1 << COMnA1) & ~(1 << COMnA0);
	TCCR_LEFTA |= (1 << COMnB1) | (1 << COMnB0);

	LEFT_MOTOR_PORTA &= ~(1 << LEFT_MOTORA);
	return;
}

void setRightReverse(){
	// Reverse <= (xIN1=0) & (xIN0=PWM);
	// turn on PWM function in port B
	// turn off PWM function  in port A, then set it low
	TCCR_RIGHTA &= ~(1 << COMnA1) & ~(1 << COMnA0);
	TCCR_RIGHTA |= (1 << COMnB1) | (1 << COMnB0);

	RIGHT_MOTOR_PORTA &= ~(1 << RIGHT_MOTORA);
	return;
}
void setLeftCoast(){
	// Coast <= (xIN1=0) & (xIN0=0);
	// Turn off both PWMs, set both outputs low
	TCCR_LEFTA &= ~(1 << COMnA1) & ~(1 << COMnA0) &
				  ~(1 << COMnB1) & ~(1 << COMnB0);
	LEFT_MOTOR_PORTA &= ~(1 << LEFT_MOTORA);
	LEFT_MOTOR_PORTB &= ~(1 << LEFT_MOTORB);
	
	return;
}

void setRightCoast(){
	// Coast <= (xIN1=0) & (xIN0=0);
	// Turn off both PWMs, set both outputs low
	TCCR_RIGHTA &= ~(1 << COMnA1) & ~(1 << COMnA0) &
				  ~(1 << COMnB1) & ~(1 << COMnB0);
	RIGHT_MOTOR_PORTA &= ~(1 << RIGHT_MOTORA);
	RIGHT_MOTOR_PORTB &= ~(1 << RIGHT_MOTORB);
	
	return;
}

void setLeftStop(){
	// Stop <= (xIN1=1) & (xIN0=1);
	// Turn off both PWMs, set both outputs high
	TCCR_LEFTA &= ~(1 << COMnA1) & ~(1 << COMnA0) &
				  ~(1 << COMnB1) & ~(1 << COMnB0);
	LEFT_MOTOR_PORTA |= (1 << LEFT_MOTORA);
	LEFT_MOTOR_PORTB |= (1 << LEFT_MOTORB);
	
	return;	
}

void setRightStop(){
	// Stop <= (xIN1=1) & (xIN0=1);
	// Turn off both PWMs, set both outputs high
	TCCR_RIGHTA &= ~(1 << COMnA1) & ~(1 << COMnA0) &
				  ~(1 << COMnB1) & ~(1 << COMnB0);
	RIGHT_MOTOR_PORTA |= (1 << RIGHT_MOTORA);
	RIGHT_MOTOR_PORTB |= (1 << RIGHT_MOTORB);
	
	return;
}
