#include "TimerOne.h"
#include <Wire.h>

int sensPin = 5;
int c = 2;
boolean rising = true;
 
void setup(){
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
  Timer1.initialize(2000);
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
 if(rising){
  c = c << 1;
 } else {
  c = c >> 1;
 }

 if(c > 65535){
  rising = false;
 } else if(c == 1){
  rising = true;
 }
}
