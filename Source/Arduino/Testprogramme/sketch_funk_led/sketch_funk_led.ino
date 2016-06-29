#include <RCSwitch.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Wenn der Code 123 per Funk erhalten wird, soll die LED, die am PIN 12 vom Arduino 
 * angeschlossen ist, aufleuchten.
 */

RCSwitch mySwitch = RCSwitch();
int ledpin = 12;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySwitch.enableReceive(0); // Receiver onn inerrupt 0 =&gt; that is pin #2
  pinMode(ledpin, OUTPUT);
}

void loop() {
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

      if(mySwitch.getReceivedValue() == 123){
        Serial.print("Code 123 erhalten");
        digitalWrite(ledpin, HIGH);
        delay(50);
        digitalWrite(ledpin, LOW);
        delay(50);
      }
    }
    
    mySwitch.resetAvailable();

  }
}
