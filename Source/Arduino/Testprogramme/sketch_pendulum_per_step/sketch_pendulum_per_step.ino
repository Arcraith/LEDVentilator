#include <Wire.h>
#include "TimerOne.h"

/*
 * Pundulum erzeugt das Pendelhafte Aufleuchten der LEDs (einmal vor und zurück). Dieser soll
 * einmal pro Timer-Interrupt ausgeführt werden, sodass durch die interne Verzögerung durch die 
 * for-Schleife ein Dreieck entsteht.
 * 
 * Dieses wird durch den Reed-Kontakt und der damit errechneten Runden-Laufzeit immer an der gleichen
 * Position gehalten.
 */

const int switchPin = 4;
const int ledPin = 12;
int c = 1;
int stepCounter = 1;
boolean reedToggle = false;
unsigned long alt = 0;
unsigned long neu = 0;
unsigned long delta = 0;
//int roundCounter = 0;
//unsigned long roundAverage[10] = {0};
boolean rising = true;

/*long picture[36] = { 65535,0,65535,0,63535,
                     0,65535,0,65535,0,
                     65535,0,65535,0,63535,
                     0,65535,0,65535,0,
                     65535,0,65535,0,63535,0};*/

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
      //Serial.println();
      Timer1.setPeriod(delta);     // neue timePerRound (delta) durch Winkelschritte
      //roundAverage[roundCounter] = delta;
      //Serial.println(roundCounter);
      //roundCounter++;
      alt = micros();
    } 
  }
  /*
  if(roundCounter == 10){
    unsigned long sum = 0;
    for(int i = 0; i < 10; i++){
      sum += roundAverage[i];
    }
    sum = sum / 10;
    Serial.println(sum);
    Timer1.setPeriod(sum);
    roundCounter = 0;
    
  }*/
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
  pendulum();
}

void pendulum(){
  unsigned long d = 1;
  for(int i = 0; i < 31; i++){
    
    Wire.beginTransmission(0x27);
     Wire.write(0x12);
     Wire.write(d & 0xff);
     Wire.endTransmission();
     Wire.beginTransmission(0x20);
     Wire.write(0x12);
     Wire.write(d >> 8);
     Wire.endTransmission();
     
     if(d >= 32768){
      rising = false;
     } else if(d == 1){
      rising = true;
     }
     
     if(rising){
      d = d << 1;
     } else {
      d = d >> 1;
     }
     
  }
}

