#define zerocross 2       //pin for the external interrupt (Input with Pullup)                         
#define MOSFET 4           //pin to trigger the MOSFET (Output)
int setvalue = 0;             //variable to store the current potentiometer value
int stapdown; 
bool zero = 0;            //variable as a status flag which shows whether a zero crossing point occured
#include <Wire.h>

void setup() {
  Serial.begin(9600);
  pinMode(zerocross, INPUT_PULLUP);
  pinMode(MOSFET, OUTPUT);
  digitalWrite(MOSFET, LOW);                  //pull the MOSFET pin at the beginning low so that the MOSFET is off
  TCCR0A = 0;                                 //clear timer0 register (TCCR0A)
  TCCR0A |= (1 << WGM01);                     //set the CTC mode for timer0 with OCRA as top
  TCCR0B = 0;                                 //clear timer0 register (TCCR0B)
  TCCR0B |= (1 << CS00) | (1 << CS02);        //set prescaler of timer0 to 1024
  TIMSK0 |= (1 << OCIE0B);                     //activate timer compare match interrupt for OCR0A and OCR0B
  attachInterrupt(0, zerocrossing, FALLING);  //activate external interrupt on pin 2 at a falling edge
  sei();                                      //activate interrupts      //read in potentiometer value and store it in potvalue
  
  //Wire settings
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
}

void zerocrossing() {                         //external interrupt, occurs after every zero crossing point
  OCR0B = map(setvalue, 0, 100, 3, 70);   //set OCR0B value between 3 and 70 (0.384ms - 8.96ms) according to the potentiometer value
  TCNT0 = 0;                                  //clear timer0 counter
  zero = 1;                                   //set zero crossing point flag to 1
  digitalWrite(MOSFET, HIGH);
}

ISR(TIMER0_COMPB_vect) {                      //compare match interrupt for OCR0B
  if (zero == 1) {        
    digitalWrite(MOSFET, LOW);                //if a zero crossing occurred, deactivate the MOSFET
    zero = 0;                                 //set the zero crossing point flag to 0
  }
}

//Receive event 
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
  setvalue = x;
}

void loop() { 

  delayMicroseconds(500000);
}