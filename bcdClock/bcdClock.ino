#include "Wire.h"
#include <Time.h>





int latchPin = 8;             //595 pin 12
int clockPin = 12;          //595 pin 11
int dataPin = 11;           //595 pin 14
//595 pin 16 has 5VDC
//595 pin 8 has GND

void setup() {
 

     pinMode(latchPin, OUTPUT);
     pinMode(clockPin, OUTPUT);
     pinMode(dataPin, OUTPUT);
}
void loop() {
   
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, ((second()/10 * 16) + (second() % 10)));
   digitalWrite(latchPin, HIGH);
   delay(1000);
}
