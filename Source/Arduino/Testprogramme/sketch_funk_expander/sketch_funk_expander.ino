#include <RCSwitch.h>
#include "Wire.h"
#include <stdlib.h>
#include <stdio.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySwitch.enableReceive(0); // Receiver onn inerrupt 0 =&gt; that is pin #2

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
  /*
  Wire.beginTransmission(0x27);
  Wire.write(0x12);
  Wire.write(0 & 0xff);              // Bitverschiebung ersten 8
  Wire.endTransmission();
  Wire.beginTransmission(0x20);
  Wire.write(0x12);
  Wire.write(0 >> 8);                // Bitverschiebung zweiten 8
  Wire.endTransmission();
  */
       
  // put your main code here, to run repeatedly:
  if (mySwitch.available()) {
    
    int value = mySwitch.getReceivedValue();

    if (value == 0) {
      Serial.print("Unknown encoding");
    } else {
      Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );


      Wire.beginTransmission(0x27);
      Wire.write(0x12);
      Wire.write(mySwitch.getReceivedValue() & 0xff);              // Bitverschiebung ersten 8
      Wire.endTransmission();
      Wire.beginTransmission(0x20);
      Wire.write(0x12);
      Wire.write(mySwitch.getReceivedValue() >> 8);                // Bitverschiebung zweiten 8
      Wire.endTransmission();
    }
    mySwitch.resetAvailable();

  }

}
