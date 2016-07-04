#include "TimerOne.h"
#include <Wire.h>


/*
 * Um zu garantieren, dass die LEDs in immer gleichen Abständen an/ausgeschaltet werden, werden 
 * diese durch Timer-attached Interrupts gesteuert. Dadurch kann Code, der unterschiedlich lange dauern kann
 * in die main-loop() gepackt werden und verzögert somit nicht die LEDs.
 * 
 * Dieses Beispiel zählt lediglich eine Zahl hoch und stellt sie binär mit den Expandern dar.
 */
 
int c = 0;
 
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

  // Timer and interrupt
  Timer1.initialize(2000);  // 1000000 entspricht 1 Sekunde
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
 c++;
}
