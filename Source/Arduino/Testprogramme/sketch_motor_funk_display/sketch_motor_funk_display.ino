#include <Wire.h>
#include "TimerOne.h"
#include <RCSwitch.h>

/*
 * Da es Probleme gibt, möglichst viele LEDs auf einmal anzusteuern (nach mehreren Wiederholungen
 * mit hoher Frequenz stürzt das Programm ab oder er wechselt die Pegel nicht mehr von HIGH auf LOW),
 * soll mithilfe von vorprogrammierten Bildern (gespeichert in long-Arrays) getestet werden, wo 
 * die Grenzen liegen.
 * 
 * Das Bild wird durch den Reed-Kontakt daran gehindert sich zu drehen, weil die Anzeige-Frequenz
 * durch die Umdrehungszeit angepasst wird.
 */
 
const int switchPin = 4;
const int ledPin = 12;
int c = 0;            // Winkelschritt-Counter
int roundCount = 0;
boolean pacManToggle = false;
boolean reedToggle = false;
unsigned long alt = 0;
unsigned long neu = 0;
unsigned long delta = 0;
unsigned long fix = 331;
// int roundCounter = 0;
// unsigned long roundAverage[10] = {0};
boolean rising = true;
int program = 1;
RCSwitch mySwitch = RCSwitch();

/*long picture[24] = { 65535,0,65535,0,65535,0,
                      65535,0,65535,0,65535,0,
                      65535,0,65535,0,65535,0,
                      65535,0,65535,0,65535,0,
                     };*/
                      
long picture[24] = { 8184,0,8184,0,8184,0,
                     8184,8184,8184,8184,8184,0,
                     8184,0,8184,0,8184,0,
                     8184,8184,8184,8184,8184,0
                    };

long smiley[24] = { 0,0,0,4080,0,0,
                    0,4080,4080,4080,4080,4080,
                    4080,4080,4080,4080,4080,0,
                    0,0,4080,0,0,0
                  };
long haw[24] = { 6528,2040,0,8184,24,8148,
                 0,0,0,0,0,0,
                 0,0,0,0,0,0,
                 0,8184,384,8184,0,2040
                 };
long fan[24] = {  1023,1023,0,0,0,0,
                  1023,1023,1023,1023,0,0,
                  0,0,1023,1023,1023,1023,  
                  0,0,0,0,1023,1023
                  };

long led[24] = { 6168,0,8184,6168,4080,0,
                 0,0,0,0,0,0,
                 0,0,0,0,0,0,
                 8184,48,48,0,8184,6552
                };
long fun[24] = { 8184,0,8184,6144,8184,0,
                 0,0,0,0,0,0,
                 0,0,0,0,0,0,
                 8184,6528,6144,0,8184,24 
};
long pacman[24] = {8191,8191,8191,0,0,0,
                   0,0,0,8191,8191,8191,
                   8191,8191,8191,8191,8191,8191,
                   8191,8191,8191,8191,8191,8191
};




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

  mySwitch.enableReceive(0); // Receiver on interrupt 0
  
  // Timer and attached interrupt 
  //Timer1.initialize(2000);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(display);
}

void loop() {
  //Serial.println(delta);
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
      neu = micros();
      reedToggle = true;          // WICHTIG!
      
      // Timer-Period Update
      
      delta = neu - alt;
      Serial.println(delta);
      roundCount++;
      if(roundCount % 50 == 0){
        toggle();
      }
      delta = delta + fix;
      
      //Serial.println();
      c = 0;
      Timer1.setPeriod(delta/24);     // neue timePerRound (delta) durch Winkelschritte
      //roundAverage[roundCounter] = delta;
      //Serial.println(roundCounter);
      //roundCounter++;
      alt = micros();
    } 
  }

  if(mySwitch.available()){
    int value = mySwitch.getReceivedValue();

    if(value == 0){
      Serial.print("unknown encoding");
    } else {
      Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );
      program = value;
    }
  }
}

void display(){
  sei();
  // allow interrupt
  /*
  if(stepCounter == 10){
    pendulum();
  }
  Serial.print(stepCounter);
  stepCounter++;
  if(stepCounter > 10){
    stepCounter = 1;
  }*/
  //pendulum();
  showPicture();
}

void toggle(){
  if(pacManToggle){
    
  } else {
    
  }
}

void showPicture(){
   if(program == 1){
     Wire.beginTransmission(0x27);
     Wire.write(0x12);
     Wire.write(picture[c] & 0xff);
     Wire.endTransmission();
     Wire.beginTransmission(0x20);
     Wire.write(0x12);
     Wire.write(picture[c] >> 8);
     Wire.endTransmission(); 
   } else if(program == 2){
     Wire.beginTransmission(0x27);
     Wire.write(0x12);
     Wire.write(smiley[c] & 0xff);
     Wire.endTransmission();
     Wire.beginTransmission(0x20);
     Wire.write(0x12);
     Wire.write(smiley[c] >> 8);
     Wire.endTransmission();
   } else if(program == 3){
    Wire.beginTransmission(0x27);
     Wire.write(0x12);
     Wire.write(haw[c] & 0xff);
     Wire.endTransmission();
     Wire.beginTransmission(0x20);
     Wire.write(0x12);
     Wire.write(haw[c] >> 8);
     Wire.endTransmission();
   } else if(program == 4){
    Wire.beginTransmission(0x27);
    Wire.write(0x12);
    Wire.write(fan[c] & 0xff);
    Wire.endTransmission();
    Wire.beginTransmission(0x20);
    Wire.write(0x12);
    Wire.write(fan[c] >> 8);
    Wire.endTransmission();
   }
   c++;
   if(c >= 24){
    c = 0;
   }
}



