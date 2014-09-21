/*
 Adafruit i2c/SPI LCD backpack using MCP23008 I2C expander
( http://www.ladyada.net/products/i2cspilcdbackpack/index.html )

 This sketch prints "Hi," and the name found on the NFC tag to the LCD
 and shows the time.
 
  The circuit:
 * 5V to Arduino 5V pin
 * GND to Arduino GND pin
 * CLK to Analog #5
 * DAT to Analog #4
 
*/

// include the library code:
#include <Wire.h>
#include "LiquidCrystal.h"
#include <Keypad.h>  //needs to be in the library folder (Arduino/Libraries)
#include <Adafruit_NFCShield_I2C.h>

#define IRQ   (2)
#define RESET (3)  // Not connected by default on the NFC Shield

Adafruit_NFCShield_I2C nfc(IRQ, RESET);

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = { 9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 5, 4, 3}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// Connect via i2c, default address #0 (A0-A2 not jumpered)
LiquidCrystal lcd(0);

void setup() {
 	
  Serial.begin(9600); //initialise the serial port (9600 baud)
	nfc.begin();
	nfc.SAMConfig(); // configure board to read RFID tags
	lcd.begin(20, 4); // set up the LCD's number of rows and columns: 
	lcd.setBacklight(HIGH);  // Print a message to the LCD.       
	lcd.setCursor(2, 0);
	lcd.print("Waiting for Card");
       
}

void loop() {

  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  
  
  if (success) {
    // Display some basic information about the card
    lcd.print("Found an ISO14443A card");
    lcd.print("  UID Length: ");
	lcd.print(uidLength, DEC);
	lcd.print(" bytes");
    lcd.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    lcd.print("");
    
    if (uidLength == 4)
    {
      // We probably have a Mifare Classic card ... 
      lcd.print("Seems to be a Mifare Classic card (4 byte UID)");
	  
      // Now we need to try to authenticate it for read/write access
      // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
      lcd.print("Trying to authenticate block 4 with default KEYA value");
      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	  
	  // Start with block 4 (the first block of sector 1) since sector 0
	  // contains the manufacturer data and it's probably better just
	  // to leave it alone unless you know what you're doing
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
	  
      if (success)
      {
        lcd.print("Sector 1 (Blocks 4..7) has been authenticated");
        uint8_t data[16];
		
        // If you want to write something to block 4 to test with, uncomment
		// the following line and this text should be read back in a minute
        // data = { 'a', 'd', 'a', 'f', 'r', 'u', 'i', 't', '.', 'c', 'o', 'm', 0, 0, 0, 0};
        // success = nfc.mifareclassic_WriteDataBlock (4, data);

        // Try to read the contents of block 4
        success = nfc.mifareclassic_ReadDataBlock(4, data);
		
        if (success)
        {
          // Data seems to have been read ... spit it out
          lcd.print("Reading Block 4:");
          nfc.PrintHexChar(data, 16);
          lcd.print("");
		  
          // Wait a bit before reading the card again
          delay(1000);
        }
        else
        {
          lcd.print("Ooops ... unable to read the requested block.  Try another key?");
        }
      }
      else
      {
        lcd.print("Ooops ... authentication failed: Try another key?");
      }
    }
    
    if (uidLength == 7)
    {
      // We probably have a Mifare Ultralight card ...
      lcd.print("Seems to be a Mifare Ultralight tag (7 byte UID)");
	  
      // Try to read the first general-purpose user page (#4)
      lcd.print("Reading page 4");
      uint8_t data[32];
      success = nfc.mifareultralight_ReadPage (4, data);
      if (success)
      {
        // Data seems to have been read ... spit it out
        nfc.PrintHexChar(data, 4);
        lcd.print("");
		
        // Wait a bit before reading the card again
        delay(1000);
      }
      else
      {
        lcd.print("Ooops ... unable to read the requested page!?");
      }
    }
  }
 

  char key = keypad.getKey();
  
  if (key != NO_KEY){
    lcd.setCursor(0, 4);
	lcd.print(key); //print to serial the key that has been pressed
  } 
  
}

