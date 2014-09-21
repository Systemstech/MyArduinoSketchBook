// Working with Noritake Itron VFD modules - model CU40026SCPB-T20A
// John Boxall 2013
 
#include <SoftwareSerial.h>
SoftwareSerial VFD(6,7); // rx, tx
 
void setup()
{
  VFD.begin(9600); // set speed for software serial port 
  resetVFD();  
  VFDclearsceen();
//  VFD.write(0x12); // vertical scroll mode (on)
}
 
void resetVFD()
// performs a software reset on the VFD controller
{
  VFD.write(0x1B); // ESC
  VFD.write(0x49); // software reset
}
 
void VFDnewline()
// moves cursor to start of next line
{
  VFD.write(0x0D); // carriage return
  VFD.write(0x0A); // line feed
}
 
void VFDclearsceen()
// moves cursor to top-left and clears display
{
  VFD.write(0x0E); // clear display 
  VFD.write(0x0C); // form feed - cursor to top-left
}
 
void VFDbrightness(int amount)
// sets VFD brightness - 25/50/75/100%
// uses ESC sequences
{
  switch(amount)
  {
  case 25:
    VFD.write(0x1B); // ESC
    VFD.write(0x4C); // brightness
    VFD.print(0); // 25% brightness
    break;
  case 50:
    VFD.write(0x1B); // ESC
    VFD.write(0x4C); // brightness
    VFD.write(0x40); // 50% brightness
    break;
  case 75:
    VFD.write(0x1B); // ESC
    VFD.write(0x4C); // brightness
    VFD.write(0x80); // 75% brightness
    break;
  case 100:
    VFD.write(0x1B); // ESC
    VFD.write(0x4C); // brightness
    VFD.write(0xC0); // 100% brightness
  }
}
 
void VFDchars()
// run through characters for selected font
{
  for (int i = 21 ; i < 256; i++)
  {
    VFD.write(0x16); // underline cursor off
    VFD.write(i);
    delay(100);
  }
}
 
void moveCursor(byte position)
// moves the cursor - top row is 0~39, bottom row is 40~79
// vertical scroll mode must be turned off if used
{
    VFD.write(0x1B); // ESC
    VFD.write(0x48); // move cursor 
    VFD.write(position); // location
}
 
void loop()
{
  VFD.write(0x16); // underline cursor off
  VFD.print("Hello, Mats."); // You can blast out text 
  delay(1000);      
  VFDnewline();
  VFD.print("www.smallroomlabs.com."); 
  delay(1000);    
  VFDclearsceen();
  VFDbrightness(25);
  VFD.print("*** 25% brightness ***");   
  delay(1000);
  VFDclearsceen();  
  VFDbrightness(50);
  VFD.print("*** 50% brightness ***");     
  delay(1000);
  VFDclearsceen();   
  VFDbrightness(75);
  VFD.print("*** 75% brightness ***");       
  delay(1000);
  VFDclearsceen();   
  VFDbrightness(100);
  VFD.print("*** 100% brightness ***");         
  delay(1000);
  VFDclearsceen();
 
  VFDchars();
  VFDclearsceen();
 
  for (int i = 0; i < 80; i++)
  {
    VFD.write(0x16); // underline cursor off
    moveCursor(i);
    VFD.print("X");
    delay(100);
    moveCursor(i++);    
    VFD.print(" ");    
  }
  VFDclearsceen();
}
