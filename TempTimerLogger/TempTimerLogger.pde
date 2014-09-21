#include <SD.h>
#include <Wire.h>
#include <max6675.h>
#include "RTClib.h"
#include "LiquidCrystal.h"

RTC_DS1307 RTC;

// A simple data logger for the Arduino analog pins

// how many milliseconds between grabbing data and logging it. 1000 ms is once a second
#define LOG_INTERVAL  1000 // mills between entries (reduce to take more/faster data)

// how many milliseconds before writing the logged data permanently to disk
// set it to the LOG_INTERVAL to write each time (safest)
// set it to 10*LOG_INTERVAL to write all data every 10 datareads, you could lose up to 
// the last 10 reads if power is lost but it uses less power and is much faster!
#define SYNC_INTERVAL 1000 // mills between calls to flush() - to write data to the card
uint32_t syncTime = 0; // time of last sync()

#define ECHO_TO_SERIAL      1 // echo data to SERIAL
#define WAIT_TO_START       0 // Wait for serial input in setup()

// the digital pins that connect to the LEDs
//#define redLEDpin 2
#define greenLEDpin 13

int thermoDO = 5;
int thermoCS = 6;
int thermoCLK = 7;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

// Connect via SPI. Data pin is #3, Clock is #2 and Latch is #4
LiquidCrystal lcd(3, 2, 4);

// make a cute degree symbol
uint8_t degree[8]  = {140,146,146,140,128,128,128,128};

// connect to RTC
  Wire.begin(); 
   RTC.begin();
  if (!RTC.begin()) {
    lcd.print("RTC failed");
#if ECHO_TO_SERIAL
    Serial.println("RTC failed");
#endif  //ECHO_TO_SERIAL
  }

void error(char *str)
{
  lcd.print("error: ");
  lcd.print(str);
  
  // red LED indicates error
  //digitalWrite(redLEDpin, HIGH);

  while(1);
}


void setup() {
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.createChar(0, degree);
   // wait for MAX chip to stabilize
  delay(500);
}

void loop(){
  // basic readout test, just print the current temp
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time ");
  
  // go to line #1
  lcd.setCursor(0,1);
  lcd.print(thermocouple.readCelsius());
#if ARDUINO >= 100
  lcd.write((byte)0);
#else
  lcd.print(0, BYTE);
#endif
  lcd.print("C ");
  lcd.print(thermocouple.readFarenheit());
#if ARDUINO >= 100
  lcd.write((byte)0);
#else
  lcd.print(0, BYTE);
#endif
  lcd.print('F');
  
  delay(1000);
  
  
  DateTime now;

  // delay for the amount of time we want between readings
  delay((LOG_INTERVAL -1) - (millis() % LOG_INTERVAL));
  
  digitalWrite(greenLEDpin, HIGH);
  
  // fetch the time
  now = RTC.now();
  // log time
  lcd.print(now.unixtime()); // seconds since 1/1/1970
  lcd.print(", ");
  lcd.print('"');
  lcd.print(now.year(), DEC);
  lcd.print("/");
  lcd.print(now.month(), DEC);
  lcd.print("/");
  lcd.print(now.day(), DEC);
  lcd.print(" ");
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);
  lcd.print(":");
  lcd.print(now.second(), DEC);
  lcd.print('"');
 
}



