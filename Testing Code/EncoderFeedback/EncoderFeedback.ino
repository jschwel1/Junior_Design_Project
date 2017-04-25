
int encoder_count;
int encoder_last;
int encoder0PinA = 10;
int encoder0PinB = 11;
int n;
int incoming;
int last_tenth = 0;
int foot_count;
void setup()
{
    Serial.begin(9600);   //Sets the baud for serial data transmission                               
    pinMode(10, INPUT);  //Sets digital pin 13 as output pin
    pinMode(11, INPUT);
    pinMode(7,OUTPUT);
//    pinMode(12, INPUT);
//    pinMode(13, INPUT);
}
void loop()
{
  n = digitalRead(encoder0PinA);
  if ((encoder_last == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      encoder_count++;
      
    }
  }
  if(encoder_count - last_tenth == 20){
    foot_count++;
    println(foot_count);
  }
  encoder_last = n;

  if(Serial.available()){
    incoming = Serial.read();
    if(incoming == 49){
      digitalWrite(7,HIGH);
    }
    else if(incoming == 50){
      digitalWrite(7,LOW);
      encoder_count = 0;
    }
    Serial.println(incoming);
  }


}
