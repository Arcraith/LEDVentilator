#include <Wire.h>

const int switchPin = 4;
const int ledPin = 12;
int c = 0;
boolean reedToggle = false;

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
      
      // Expander
      Wire.beginTransmission(0x27);
      Wire.write(0x12);
      Wire.write(c & 0xff);
      Wire.endTransmission();
      Wire.beginTransmission(0x20);
      Wire.write(0x12);
      Wire.write(c >> 8);
      Wire.endTransmission();
    } else {
      //Expander
      Wire.beginTransmission(0x27);
      Wire.write(0x12);
      Wire.write(c & 0xff);
      Wire.endTransmission();
      Wire.beginTransmission(0x20);
      Wire.write(0x12);
      Wire.write(c >> 8);
      Wire.endTransmission();
    }
  }
}
