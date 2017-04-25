#include "rover.h"
#include <util/delay.h>

void initializeRover(){
	// set up Motor DDRs
	LEFT_MOTOR_PORTA_DDR |= (1 << LEFT_MOTORA);
	LEFT_MOTOR_PORTB_DDR |= (1 << LEFT_MOTORB);
	RIGHT_MOTOR_PORTA_DDR |= (1 << RIGHT_MOTORA);
	RIGHT_MOTOR_PORTB_DDR |= (1 << RIGHT_MOTORB);


}


void driveForward(){
	setLeftForward();
	setRightForward();
}

void driveReverse(){
	setLeftReverse();
	setRightReverse();
}

void stop(){
	setLeftStop();
	setRightStop();
}

void coast(){
	setLeftCoast();
	setRightCoast();
}

void spinCW(){
	setLeftForward();
	setRightReverse();
}
void spinCCW(){
	setLeftReverse();
	setRightForward();
}
void rightDrift(){
	setLeftForward();
	setRightCoast();
}
void leftDrift(){
	setRightForward();
	setLeftCoast();
}


void setLeftForward(){
	//	Forward <= (xIN1=1) & (xIN0=0);
	LEFT_MOTOR_PORTA |= (1 << LEFT_MOTORA);
	LEFT_MOTOR_PORTB &= ~(1 << LEFT_MOTORB);

	return;
}

void setRightForward(){
	//	Forward <= (xIN1=1) & (xIN0=0);
	RIGHT_MOTOR_PORTA |= (1 << RIGHT_MOTORA);
	RIGHT_MOTOR_PORTB &= ~(1 << RIGHT_MOTORB);
	return;
}

void setLeftReverse(){
	// Reverse <= (xIN1=0) & (xIN0=1);
	LEFT_MOTOR_PORTA &= ~(1 << LEFT_MOTORA);
	LEFT_MOTOR_PORTB |= (1 << LEFT_MOTORB);
	return;
}

void setRightReverse(){
	// Reverse <= (xIN1=0) & (xIN0=1);
	RIGHT_MOTOR_PORTA &= ~(1 << RIGHT_MOTORA);
	RIGHT_MOTOR_PORTB |= (1 << RIGHT_MOTORB);
	return;
}
void setLeftCoast(){
	// Coast <= (xIN1=0) & (xIN0=0);
	LEFT_MOTOR_PORTA &= ~(1 << LEFT_MOTORA);
	LEFT_MOTOR_PORTB &= ~(1 << LEFT_MOTORB);

	return;
}

void setRightCoast(){
	// Coast <= (xIN1=0) & (xIN0=0);
	RIGHT_MOTOR_PORTA &= ~(1 << RIGHT_MOTORA);
	RIGHT_MOTOR_PORTB &= ~(1 << RIGHT_MOTORB);

	return;
}

void setLeftStop(){
	// Stop <= (xIN1=1) & (xIN0=1);
	LEFT_MOTOR_PORTA |= (1 << LEFT_MOTORA);
	LEFT_MOTOR_PORTB |= (1 << LEFT_MOTORB);

	return;
}

void setRightStop(){
	// Stop <= (xIN1=1) & (xIN0=1);
	RIGHT_MOTOR_PORTA |= (1 << RIGHT_MOTORA);
	RIGHT_MOTOR_PORTB |= (1 << RIGHT_MOTORB);

	return;
}
