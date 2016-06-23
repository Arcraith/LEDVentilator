#include <Wire.h>
#include "TimerOne.h"

const int switchPin = 4;
const int ledPin = 12;
int c = 1;
int stepCounter = 1;
int angleCounter = 0;
boolean reedToggle = false;
unsigned long alt = 0;
unsigned long neu = 0;
unsigned long delta = 0;
unsigned long fix = 30;
boolean rising = true;

unsigned long h[7] = {65535,128,128,128,128,128,65535};
unsigned long a[7] = {8191,24576,32768,0,32768,24576,8191};
unsigned long w[7] = {65535,3,252,0,252,3,65535};
unsigned long space[2] = {0,0};

void setup() {
  Serial.begin(9600);
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
  //Timer1.initialize(2000);
  Timer1.initialize(1000000);
  //Timer1.attachInterrupt(display);
}

void loop() {

  show();
  delay(1000);
  /*Serial.println(delta);
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
   /* if(reedToggle == false){
      neu = micros();
      reedToggle = true;          // WICHTIG!
      
      // Timer-Period Update
      
      delta = neu - alt;
      //Serial.print(delta);
      //Serial.println();
      //Timer1.setPeriod(delta);     // neue timePerRound (delta) durch Winkelschritte
      alt = micros();
    } 
  }*/
}

void display(){
  sei();  // allow interrupt
  
  show();
}

void show(){
  for(int i = 0; i < 7; i++){
    Wire.beginTransmission(0x27);
     Wire.write(0x12);
     Wire.write(h[i] & 0xff);
     Wire.endTransmission();
     Wire.beginTransmission(0x20);
     Wire.write(0x12);
     Wire.write(h[i] >> 8);
     Wire.endTransmission();
  }
  for(int i = 0; i < 2; i++){
    Wire.beginTransmission(0x27);
     Wire.write(0x12);
     Wire.write(space[i] & 0xff);
     Wire.endTransmission();
     Wire.beginTransmission(0x20);
     Wire.write(0x12);
     Wire.write(space[i] >> 8);
     Wire.endTransmission();
  }
  delay(50);
  for(int i = 0; i < 7; i++){
    Wire.beginTransmission(0x27);
     Wire.write(0x12);
     Wire.write(a[i] & 0xff);
     Wire.endTransmission();
     Wire.beginTransmission(0x20);
     Wire.write(0x12);
     Wire.write(a[i] >> 8);
     Wire.endTransmission();
  }
  delay(50);
  for(int i = 0; i < 2; i++){
    Wire.beginTransmission(0x27);
     Wire.write(0x12);
     Wire.write(space[i] & 0xff);
     Wire.endTransmission();
     Wire.beginTransmission(0x20);
     Wire.write(0x12);
     Wire.write(space[i] >> 8);
     Wire.endTransmission();
  }
  delay(50);
  for(int i = 0; i < 7; i++){
    Wire.beginTransmission(0x27);
     Wire.write(0x12);
     Wire.write(w[i] & 0xff);
     Wire.endTransmission();
     Wire.beginTransmission(0x20);
     Wire.write(0x12);
     Wire.write(w[i] >> 8);
     Wire.endTransmission();
  }
  delay(50);
  for(int i = 0; i < 9; i++){
    Wire.beginTransmission(0x27);
     Wire.write(0x12);
     Wire.write(0 & 0xff);
     Wire.endTransmission();
     Wire.beginTransmission(0x20);
     Wire.write(0x12);
     Wire.write(0 >> 8);
     Wire.endTransmission();
  }
  delay(50);
}

