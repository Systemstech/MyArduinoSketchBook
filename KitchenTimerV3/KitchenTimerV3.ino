#include <Utility.h>

 /*
  Kitchen_Timer_V2
 
 This code requires the Utility Library.
 You can download it from here http://www.arduino.cc/playground/Code/Utility
 Full instructions on how to download and install are on the web site. (it is very easy)
 
 Counts up LED's on a bar graph to indicate 5 minute icrements being added to
 a timer circuit. A button is held to count up the LED's, and when the timer
 finishes the Bar Graph shows a graphic, and a piezo-electric buzzer sounds.
 
 The circuit:
 * LED bar graph attached from pins (13-3 excluding 9) to ground 
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground
 * piezo buzzer attached to pin 9 from ground
 
 * Note: I recommend doing this on an arduino shield, as this requires a good
 amount of wire jumpers.
 
 Original Code Created by
 Created in 2011 ©
 by Ari Cooper Davis [anonymouse197@gmail.com]
 http://www.instructables.com/member/anonymouse197/

This updated Code created by
Simon Carr 2011 ©
simonjcarr@gmail.com
http://www.arduino-mega.com

 */



const byte NUMBER_OF_PINS = 10;
byte ledPin[NUMBER_OF_PINS] = {3,4,5,6,7,8,10,11,12,13};
 
const int speakerOut = 9;

 // variables will change. They're used mostly for functions:
 
 int tastyTimeVariable = 0;   // variable for defining time to wait
 int countDownValue;
 
 void setup() {
   
  // initialize the LED pins as output pins:
foreach(ledPin, NUMBER_OF_PINS, pinMode, OUTPUT);
  
  // initialize the pushbutton pin as an input:
  pinMode(2, INPUT);
  // initialize the piezo pin as an output
  pinMode(9, OUTPUT);
  delay(1000);
}

void loop(){
 
//Keep reading the pushbutton while it is pressed.  
while(digitalRead(2) == HIGH){ 
  //Increment the time variable
  ++tastyTimeVariable;
  //Light the LED's in sync with value of tastyTimeVariable
  foreach(ledPin, 0, tastyTimeVariable-1, digitalWrite, HIGH);
  delay(500);

  //If count goes above 10 start at the beginning again
  if (tastyTimeVariable > 10){
   tastyTimeVariable = 0;
   foreach(ledPin, NUMBER_OF_PINS, digitalWrite, LOW);
  }
}

//When button is not pressed see if tastyTimeVariable has a value.
//If it has then we start the count down
if (tastyTimeVariable > 0){
  countDownValue = (tastyTimeVariable * 300)-1;
  while (countDownValue > 0){
  foreach(ledPin, NUMBER_OF_PINS, digitalWrite, LOW);
  //Because our foreach value list is zero based we normally have to minus 1 from our upper value. However we want our int to return a ceiling value.
  //So if count is 
  foreach(ledPin, 0, int((countDownValue/60)/5), digitalWrite, HIGH);
  //foreach(ledPin, 0, int(countDownValue/60), digitalWrite, HIGH);
  delay(1000);
  --countDownValue;
  }
  tastyTimeVariable = 0;
  
  //Sound the buzzer and Flash the LED's
  while(0 == 0){
  
    //Chase Up
  for(int tVariable = 0; tVariable < 9; tVariable++) {  // another loop function
    foreach(ledPin, NUMBER_OF_PINS, digitalWrite, LOW);
    beeper(0); //Call the beeper function
    delay(50);
    foreach(ledPin, tVariable, tVariable +1, digitalWrite, HIGH);
    beeper(10); //Call the beeper function
    delay(50);
  }
  //Chase Down
  
   for(int tVariable = 9; tVariable > 0; tVariable--) {  // another loop function
    foreach(ledPin, NUMBER_OF_PINS, digitalWrite, LOW);
    beeper(0); //Call the beeper function
    delay(50);
    foreach(ledPin, tVariable, tVariable +1, digitalWrite, HIGH);
    beeper(10); //Call the beeper function
    delay(50);
  }
  foreach(ledPin, NUMBER_OF_PINS, digitalWrite, LOW);
  }
}
 
}
//Beep the speaker function
void beeper(int beepdelay){
  analogWrite(speakerOut, beepdelay);
}
