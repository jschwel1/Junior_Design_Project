#include <IRremote.h>


/*  IR Settings*/
#define IR_RECEIVE_PIN 11 	// where the IR receiver is connected
#define EN_OUTPUT_PIN 12	// The output pin connected to the ATMega328 to signal it to stop
#define POWER_BUTTON_VALUE 0xFD00FF	// The signal from the power button on the remote

IRrecv irrecv(IR_RECEIVE_PIN); 	// Set up the receiver on the receiver pin
decode_results results;			// Where the results are stored

/* Encoders */
#define ENC_IN 10 // one input signal from one of the encoders
#define COUNT_PER_INCH 60 // Number of ticks of the encoder per inch


/* Bluetooth or other method of displaying distance*/



void setup()
{
	// remove this line for implementation in HW <============================
  	Serial.begin(9600); // ===================================================
  	// =======================================================================
  	irrecv.enableIRIn(); // Start the receiver

  	pinMode(EN_OUTPUT_PIN, OUTPUT); 	// set the stop signal pin to an output
  	digitalWrite(EN_OUTPUT_PIN, LOW);	// drive it low


  	pinMode(ENC_IN, INPUT);		// input for the encoder
  	digitalWrite(ENC_IN, HIGH); // set the pull up resistor high
}


void loop() {
	static uint8_t last_enc_val = LOW; 	// first value won't impact final results
	static uint8_t tick_count = 0; 		// might need to be uint16_t
	static uint8_t inches_moved = 0; 	// Distance travelled in inches
	// if the receiver detects a signal
  	if (irrecv.decode(&results)) {
  		// for debugging purposes--remove in final version <=======================
    	Serial.println(results.value, HEX); //=====================================
    	// ========================================================================
    	// if the received signal is from the power button
    	if (results.value == POWER_BUTTON_VALUE){
    		// flip the value of the output signal
    		if (digitalRead(EN_OUTPUT_PIN) == HIGH) digitalWrite(EN_OUTPUT_PIN, LOW);
    		else digitalWrite(EN_OUTPUT_PIN, HIGH);
    	}

    	irrecv.resume(); // Receive the next value
  	}

  	// encoders
  	if (digitalRead(ENC_IN) != last_enc_val){
  		last_enc_val = digitalRead(ENC_IN);
  		if (++tick_count == COUNT_PER_INCH){
  			inches_moved++;
  			tick_count = 0;
  		}
  	}
}




