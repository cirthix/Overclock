/*
 * Overclock, a simple library for adjusting OSCCAL on atmega168 and similar
 * 
 * Copyright (C) 2015 cirthix@gmail.com
 * 
 * This file is part of Overclock.
 * 
 * This software is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.  In addition to or superseding this license, this
 * software may not be used in any form which is not completely free without the author's written 
 * consent.
 * 
 * This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with This software. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Authors: cirthix@gmail.com
 */

/*
 This is the example project for demonstrating operation of the Overclock library.
Made by cirthix (cirthix@gmail.com)
Please see https://github.com/cirthix/Overclock for more information.
Note the scaled delay as used here:   delay(CLOCK_RATIO*100);
 Expected output should be roughly this (using atmega168p at 3.3v at 20C:
Hello from setup().
OC Clock: 15094328
Default Clock: 8000000
16M Clock: 15644304
14M Clock: 14057328
12M Clock: 11965760
10M Clock: 10057192
8M Clock: 8000000
6M Clock: 6000936
4M Clock: 5096184
*/




#include <avr/wdt.h>
#include "Overclock.h"

#define OVERCLOCKING


const uint32_t SERIAL_BAUD = 115200 ;  // Recommend 38400, 19200, or 9600 for better stability.  115200 works for me, but may be less stable in some setups.
const uint32_t OVERCLOCKED_SPEED = 15000000; // Note: setting this outside of the OCCAL tuning range will break serial communications.

#ifdef OVERCLOCKING
const uint32_t REAL_SPEED=OVERCLOCKED_SPEED;
#else
const uint32_t REAL_SPEED=F_CPU;
#endif
const float CLOCK_RATIO=REAL_SPEED/F_CPU;



Overclock my_Overclock_object=Overclock(REAL_SPEED, SERIAL_BAUD);
//Overclock my_Overclock_object=Overclock(REAL_SPEED);
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
  my_Overclock_object.SetClockSpeedOC();
  Serial.print(F("OC Clock: ")); Serial.println(my_Overclock_object.GetClockSpeed());  Serial.flush();
  delay(CLOCK_RATIO*100);
  my_Overclock_object.SetClockSpeedStock();        
  Serial.print(F("Default Clock: ")); Serial.println(my_Overclock_object.GetClockSpeed());  Serial.flush();
  delay(CLOCK_RATIO*100);  
  my_Overclock_object.SetClockSpeed(16000000);
  Serial.print(F("16M Clock: ")); Serial.println(my_Overclock_object.GetClockSpeed());  Serial.flush();
  delay(CLOCK_RATIO*100);
  my_Overclock_object.SetClockSpeed(14000000);
  Serial.print(F("14M Clock: ")); Serial.println(my_Overclock_object.GetClockSpeed());  Serial.flush();
  delay(CLOCK_RATIO*100);
  my_Overclock_object.SetClockSpeed(12000000);
  Serial.print(F("12M Clock: ")); Serial.println(my_Overclock_object.GetClockSpeed());  Serial.flush();
  delay(CLOCK_RATIO*100);
  my_Overclock_object.SetClockSpeed(10000000);
  Serial.print(F("10M Clock: ")); Serial.println(my_Overclock_object.GetClockSpeed());  Serial.flush();
  delay(CLOCK_RATIO*100);
  my_Overclock_object.SetClockSpeed(8000000);
  Serial.print(F("8M Clock: ")); Serial.println(my_Overclock_object.GetClockSpeed());  Serial.flush();
  delay(CLOCK_RATIO*100);
  my_Overclock_object.SetClockSpeed(6000000);
  Serial.print(F("6M Clock: ")); Serial.println(my_Overclock_object.GetClockSpeed());  Serial.flush();
  delay(CLOCK_RATIO*100);
  my_Overclock_object.SetClockSpeed(4000000);
  Serial.print(F("4M Clock: ")); Serial.println(my_Overclock_object.GetClockSpeed());  Serial.flush();
  delay(CLOCK_RATIO*100);
  while(1){;}
}
