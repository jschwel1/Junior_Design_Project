#ifndef NUS_H
#define NUS_H




// HC-SR04 Ultrasonic Ranging Module
#define TRIG1 6
#define LED 5
#define TRIG_PORT PORTC
#define TRIG_PORT_DDR DDRC


#define ECHO 3 // D3 => INT1
#define ECHO_PIN PIND
#define ECHO_PIN_DDR DDRD
#define ECHO_PIN_DDR_MASK 0x00 // all inputs

#define PROXIMITY 174

#define CYCLE_DELAY_TIME 65 // Anything over 60ms should work according to datasheet
#define PULSE_TIME 10 // datasheet specifies 10us pulse
// Add timer stuff to count times
#define COM1a1 7
#define COM1a0 6
#define COM1b1 5
#define COM1b0 4

#define WGM10 0
#define WGM11 1
#define WGM12 3
#define WGM13 4

#define ICNC1 7
#define ICES1 6

#define CS10 0
#define CS11 1
#define CS12 2

// Interrupt Stuff
#define ISC11 3
#define ISC10 2

// EIMSK
#define INT1 1

// EIFR
#define INTF1 1

void setup();
void trigger();
void turnOffTimer();
void turnOnTimer();






#endif
