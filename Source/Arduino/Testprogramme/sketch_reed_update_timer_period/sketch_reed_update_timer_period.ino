#include <Wire.h>
#include "TimerOne.h"

const int switchPin = 4;
const int ledPin = 12;
int c = 0;
boolean reedToggle = false;
unsigned long start = 0;
unsigned long ende = 0;
unsigned long delta = 0;

long picture[36] = { 65535,0,65535,0,63535,
                     0,65535,0,65535,0,
                     65535,0,65535,0,63535,
                     0,65535,0,65535,0,
                     65535,0,65535,0,63535,0};

void setup() {
  // put your setup code here, to run once:
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(switchPin, HIGH);  // turn on the internal pull-up resistor

  Wire.begin();
  Wire.beginTransmission(0x27);         // 0x27 Adresse für den ersten Expander
  Wire.write(0x00);                     // IODIRA register
  Wire.write(0x00);                     // set all of port A to outputs
  Wire.endTransmission();

  Wire.beginTransmission(0x20);         // 0x20 Adresse für den zweiten Expander
  Wire.write(0x00);                     // IODIRA register
  Wire.write(0x00);                     // set all of port A to outputs
  Wire.endTransmission();

  // Timer and attached interrupt 
  Timer1.initialize(2000);
  Timer1.attachInterrupt(display);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(switchPin) == LOW){
    //digitalWrite(ledPin, LOW);
    reedToggle = false;
  } else {
    //digitalWrite(ledPin, HIGH);
    /* 
     *  Magnetfeld ist ein ganzer Bereich; bedeutet, dass man unterscheiden muss zwischen
     *  - Pegel ist gerade auf HIGH gesprungen (Magnet in der Nähe)
     *  - Pegel ist immernoch HIGH
     *  
     *  --> Toggle-Boolean, wenn man HIGH betritt
     */
    if(reedToggle == false){
      c++;
      reedToggle = true;          // WICHTIG!
      
      // Timer-Period Update
      ende = micros();
      delta = ende - start;
      Timer1.setPeriod(delta/36);     // neue timePerRound (delta) durch Winkelschritte
    } 
  }
}

void display(){
  sei();                                           // allow interrupt
 
  Wire.beginTransmission(0x27);
  Wire.write(0x12);
  Wire.write(picture[c] & 0xff);
  Wire.endTransmission();
  Wire.beginTransmission(0x20);
  Wire.write(0x12);
  Wire.write(picture[c] >> 8);
  Wire.endTransmission();
  c++;

  if(c >= 35){
    c = 0;
  }
}

