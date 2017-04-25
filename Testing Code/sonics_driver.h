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
// #define COM1A1 7 // Leave both ports to normal operation
// #define COM1A0 6
// #define COM1B1 5
// #define COM1B0 4
// #define WGM11 1
// #define WGM10 0
//
// // TCCR1B
// #define ICNC1 7
// #define ICES1 6
// #define WGM13 4
// #define WGM12 3
// #define CS12 2
// #define CS11 1
// #define CS10 0

// TCCR1C
// #define FOC1A 7
// #define FOC1B 6

// TIMSK1
// #define ICIE1 5 // Input capture IE
// #define OCIE1B 2 // OCRB Match IE
// #define OSIE1A 1 // OCRA Match IO
// #define TOIE1 0  // Overflow IE

// Ultrasonic stuff -- Using PORTD
// PORTD contains PCINT(23:16) -> PCMSK2
#define ECHO_PIN PIND
#define ECHO_PIN_DDR DDRD
#define ECHO1 4 // PCINT20
#define ECHO1_PCINT PCINT20
#define ECHO2 5 // PCINT21
#define ECHO2_PCINT PCINT21
#define ECHO3 6 // PCINT22
#define ECHO3_PCINT PCINT22
#define ECHO4 7 // PCINT23
#define ECHO4_PCINT PCINT23
#define TRIG_PORT PORTD
#define TRIG_PORT_DDR DDRD

#define TRIG1 0
#define TRIG2 1
#define TRIG3 2
#define TRIG4 3

// Interrupt
// EICRA
// #define ISC11 3
// #define ISC10 2
// // EIMSK
// #define INT1 1
// // EIFR
// #define INTF1 1


#define LED_PORT PORTC
#define LED_PORT_DDRC DDRC
#define LED1 2
#define LED2 1

// Macro functions to easily set or clear pins
#define enable(port, pin) ((port) |= (1 << (pin)))
#define disable(port, pin) ((port) &= ~(1 << (pin)))

// Macro function to easily see if a pin is high or low
#define isEnabled(port, pin) (((port) & (1 << (pin))))


void initializeTimer16();
void initializeUltraSonic();

void stopTimer16();
void startTimer16();
void startTimer16_PS8();
void resetTimer16();
