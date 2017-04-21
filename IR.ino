#include <IRremote.h>

#define IR_RECEIVE_PIN 11
#define EN_OUTPUT_PIN 12
#define POWER_BUTTON_VALUE 0xFD00FF


  	IRrecv irrecv(IR_RECEIVE_PIN);
	decode_results results;

void setup()
{
  	Serial.begin(9600);
  	irrecv.enableIRIn(); // Start the receiver

  	pinMode(EN_OUTPUT_PIN, OUTPUT);
  	digitalWrite(EN_OUTPUT_PIN, LOW);
}


void loop() {
  	if (irrecv.decode(&results)) {
    	Serial.println(results.value, HEX);
    	if (results.value == POWER_BUTTON_VALUE){
    		if (digitalRead(EN_OUTPUT_PIN) == HIGH) digitalWrite(EN_OUTPUT_PIN, LOW);
    		else digitalWrite(EN_OUTPUT_PIN, HIGH);
    	}

    	irrecv.resume(); // Receive the next value
  }
}




