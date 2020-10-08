#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
//Wire settings
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("run");  
  delayMicroseconds(500000);
}
void receiveEvent(int howMany)
{
    // nog te bekijken 
  while(1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.println(c);         // print the character
  }

  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}