#include "TimerOne.h"
#include <Wire.h>

/*
 * Ein Timer-Interrupt basiertes Pendel, welches die leuchtende LED von innen nach außen nach innen
 * laufen lässt.
 */

int sensPin = 5;
unsigned long c = 1;
boolean rising = true;
 
void setup(){
  Serial.begin(9600);
  
  // Expander
  
  Wire.begin();
  Wire.beginTransmission(0x27);         // 0x27 Adresse für den ersten Expander
  Wire.write(0x00);                     // IODIRA register
  Wire.write(0x00);                     // set all of port A to outputs
  Wire.endTransmission();
  Wire.beginTransmission(0x20);         // 0x20 Adresse für den zweiten Expander
  Wire.write(0x00);                     // IODIRA register
  Wire.write(0x00);                     // set all of port A to outputs
  Wire.endTransmission();
  
  pinMode(sensPin, INPUT);              // Set Sensor as Input

  // Timer and interrupt
  Timer1.initialize(100000);
  Timer1.attachInterrupt(display);
}
 
void loop()    {
  /*
   * Da display() an den Timer attached ist, wird es unabhängig von loop() ausgeführt
   */
}

void display() {
 sei();                                           // allow interrupt
 
 Wire.beginTransmission(0x27);
 Wire.write(0x12);
 Wire.write(c & 0xff);
 Wire.endTransmission();
 Wire.beginTransmission(0x20);
 Wire.write(0x12);
 Wire.write(c >> 8);
 Wire.endTransmission();
 Serial.print(c);
 if(c >= 32768){
  rising = false;
 } else if(c == 1){
  rising = true;
 }
 
 if(rising){
  c = c << 1;
 } else {
  c = c >> 1;
 }
}
