/*
 This is the example project for demonstrating operation of the Overclock library.
Made by cirthix (cirthix@gmail.com)
Please see https://github.com/cirthix/Overclock for more information.


 Expected output should be roughly this (using atmega168p at 3.3v at 20C:
Hello from setup().
Default Clock: 8000000
OC Clock: 15625272
16M Clock: 15639912
14M Clock: 14091976
12M Clock: 12006264
10M Clock: 9984968
8M Clock: 8000000
6M Clock: 5960432
4M Clock: 5111312
*/




#include <avr/wdt.h>
#include "Overclock.h"

const uint32_t SERIAL_BAUD = 115200 ;  // Recommend 38400, 19200, or 9600 for better stability.  115200 works for me, but may be less stable in some setups.


Overclock my_Overclock_object=Overclock((uint32_t)16000000, (uint32_t)SERIAL_BAUD);
//Overclock my_Overclock_object=Overclock((uint32_t)F_CPU);
//Overclock my_Overclock_object=Overclock();

ISR(WDT_vect) {
my_Overclock_object.WatchdogFired();
// Add your other watchdog code here.
}
 
void setup() {
 Serial.begin(SERIAL_BAUD); 
  Serial.println(F("Hello from setup()."));    Serial.flush();
//  my_Overclock_object.PrintAll();
// Serial.println(F("Goodbye from setup()."));    Serial.flush();
}

void loop() {
  my_Overclock_object.SetClockSpeedStock();        
  Serial.print(F("Default Clock: ")); Serial.println(my_Overclock_object.GetClockSpeed());  Serial.flush();
  delay(100);  
  my_Overclock_object.SetClockSpeedOCMax();
  Serial.print(F("OC Clock: ")); Serial.println(my_Overclock_object.GetClockSpeed());  Serial.flush();
  delay(100);
  my_Overclock_object.SetClockSpeed((uint32_t)16000000);
  Serial.print(F("16M Clock: ")); Serial.println(my_Overclock_object.GetClockSpeed());  Serial.flush();
  delay(100);
  my_Overclock_object.SetClockSpeed((uint32_t)14000000);
  Serial.print(F("14M Clock: ")); Serial.println(my_Overclock_object.GetClockSpeed());  Serial.flush();
  delay(100);
  my_Overclock_object.SetClockSpeed((uint32_t)12000000);
  Serial.print(F("12M Clock: ")); Serial.println(my_Overclock_object.GetClockSpeed());  Serial.flush();
  delay(100);
  my_Overclock_object.SetClockSpeed((uint32_t)10000000);
  Serial.print(F("10M Clock: ")); Serial.println(my_Overclock_object.GetClockSpeed());  Serial.flush();
  delay(100);
  my_Overclock_object.SetClockSpeed((uint32_t)8000000);
  Serial.print(F("8M Clock: ")); Serial.println(my_Overclock_object.GetClockSpeed());  Serial.flush();
  delay(100);
  my_Overclock_object.SetClockSpeed((uint32_t)6000000);
  Serial.print(F("6M Clock: ")); Serial.println(my_Overclock_object.GetClockSpeed());  Serial.flush();
  delay(100);
  my_Overclock_object.SetClockSpeed((uint32_t)4000000);
  Serial.print(F("4M Clock: ")); Serial.println(my_Overclock_object.GetClockSpeed());  Serial.flush();
  delay(100);
  while(1){;}
}
