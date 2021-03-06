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
 * 
 * This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with This software. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Authors: cirthix@gmail.com
 */
 
#include <Overclock.h>

uint32_t CalculateMaxClock(float VCC){  
	// See figure 29.3 "Speed Grades" in the atmega datasheet.
if(VCC>5.5) {return 0;}
if(VCC>=4.5) {return 20000000;}
if(VCC>=2.7) {return 10000000+(5555555)*(VCC-2.7);}
if(VCC>=1.8) {return 4000000+(6666666)*(VCC-1.8);}
return 0;
}

uint8_t Overclock::IsStock(){
if(OSCCAL==OSCCAL_STOCK) {return 1;}
else {return 0;}
}

uint32_t Overclock::GetClockSpeed(){
if(OSCCAL==OSCCAL_STOCK) {return F_CPU;}
else {return F_CPU_OC;}
}


uint32_t Overclock::GetSerialSpeed(){
if(OSCCAL==OSCCAL_STOCK) {return SERIAL_PORT_SPEED;}
else {return SERIAL_PORT_SPEED_OC;}
}


void Overclock::SetClockSpeed(uint32_t TargetSpeed){
	F_CPU_OC=Overclock::DetermineOC(TargetSpeed);
	SERIAL_PORT_SPEED_OC=Overclock::DetermineNewSerialSpeed();
	Overclock::SetClockSpeedOC();
}


void Overclock::SetClockSpeedStock(){
if(SERIAL_PORT_SPEED>0){
	Serial.flush();
	Serial.end();
}
	// TODO: add support for clock frequency dividers for underclocking
	OSCCAL=OSCCAL_STOCK;

if(SERIAL_PORT_SPEED>0){
	Serial.begin(SERIAL_PORT_SPEED);
}
}


void Overclock::SetClockSpeedOC(){	
if(SERIAL_PORT_SPEED>0){
	Serial.flush();
	Serial.end();
}
	// TODO: add support for clock frequency dividers for underclocking
	OSCCAL=OSCCAL_OC;

if(SERIAL_PORT_SPEED>0){
	Serial.begin(SERIAL_PORT_SPEED_OC);
}
}

void Overclock::SetClockSpeedOCMax(){	
OSCCAL_OC=OSCCAL_OC_MAX;
Overclock::SetClockSpeedOC();
}

uint32_t Overclock::DetermineOC(uint32_t TargetSpeed){
if(TargetSpeed==F_CPU){	OSCCAL_OC=OSCCAL_STOCK; return F_CPU;}
if(TargetSpeed==OVERCLOCK_MAX_SPEED){	OSCCAL_OC=OSCCAL_OC_MAX; return F_CPU;}
uint8_t osccal_min=OSCCAL_OC_MIN;
uint8_t osccal_max=OSCCAL_OC_MAX;
if(TargetSpeed>F_CPU){osccal_min=OSCCAL_STOCK;}
if(TargetSpeed<F_CPU){osccal_max=OSCCAL_STOCK;}
uint32_t clockspeed=0;
while(1){
	uint16_t osccal_sum=osccal_min+osccal_max;
  OSCCAL=0x00FF&(osccal_sum/2);
if((OSCCAL==osccal_min) || (OSCCAL==osccal_max)) {break;}
clockspeed=Overclock::MeasureCurrentClockSpeed();
if (clockspeed>TargetSpeed) {osccal_max=OSCCAL;} 
else {osccal_min=OSCCAL;}
}	
// Now that we have the best approximate osccal value, check above and below to minimize absolute error.

try_one_less:
if(OSCCAL>OSCCAL_OC_MIN) {
	OSCCAL=OSCCAL-1;
	int32_t abserr_p=clockspeed-TargetSpeed;
	uint32_t temp=Overclock::MeasureCurrentClockSpeed();
	int32_t abserr_n=temp-TargetSpeed;
	if (abs(abserr_n)<abs(abserr_p)) {clockspeed=temp; goto try_one_less;}
	else {OSCCAL=OSCCAL+1;}
}
	
try_one_more:
if(OSCCAL<OSCCAL_OC_MAX) {
	OSCCAL=OSCCAL+1;
	int32_t abserr_p=clockspeed-TargetSpeed;
	uint32_t temp=Overclock::MeasureCurrentClockSpeed();
	int32_t abserr_n=temp-TargetSpeed;
	if (abs(abserr_n)<abs(abserr_p)) {clockspeed=temp; goto try_one_more;}
	else {OSCCAL=OSCCAL-1;}
}
	OSCCAL_OC=OSCCAL;
	return clockspeed;
}


void Overclock::PrintAll(){
	Serial.print(F("NUM_CLOCKS           =")); Serial.println(NUM_CLOCKS			  	 );
	Serial.print(F("F_CPU                =")); Serial.println(F_CPU					 );
	Serial.print(F("F_CPU_OC             =")); Serial.println(F_CPU_OC				 );
	Serial.print(F("SERIAL_PORT_SPEED    =")); Serial.println(SERIAL_PORT_SPEED		 );
	Serial.print(F("SERIAL_PORT_SPEED_OC =")); Serial.println(SERIAL_PORT_SPEED_OC	 );
	Serial.print(F("OSCCAL               =")); Serial.println(OSCCAL  				 );
	Serial.print(F("OSCCAL_STOCK         =")); Serial.println(OSCCAL_STOCK			 );
	Serial.print(F("OSCCAL_OC            =")); Serial.println(OSCCAL_OC				 );
	Serial.print(F("OSCCAL_OC_MIN        =")); Serial.println(OSCCAL_OC_MIN	 		 );
	Serial.print(F("OSCCAL_OC_MAX        =")); Serial.println(OSCCAL_OC_MAX	 		 );
	Serial.flush();
}

 void Overclock::WatchdogFired(){	NUM_CLOCKS=TCNT1; wdt_disable();}


Overclock::~Overclock(){Overclock::SetClockSpeedStock();}
	
	
Overclock::Overclock(){Overclock::InitOC((uint32_t)F_CPU, (uint32_t)0);}

Overclock::Overclock(uint32_t TargetSpeed){Overclock::InitOC(TargetSpeed, (uint32_t)0);}

	Overclock::Overclock(uint32_t TargetSpeed, uint32_t SerialSpeed){Overclock::InitOC(TargetSpeed, SerialSpeed);}

void Overclock::InitOC(uint32_t TargetSpeed, uint32_t SerialSpeed){	
	OSCCAL_STOCK=OSCCAL;	
	Overclock::DetermineWatchdogClockSpeed();
	F_CPU_OC=Overclock::DetermineOC(TargetSpeed);
	Overclock::SetSerialSpeed(SerialSpeed);	
	Overclock::SetClockSpeedStock();
}


void Overclock::SetSerialSpeed(uint32_t SerialSpeed){
	SERIAL_PORT_SPEED=SerialSpeed;
	SERIAL_PORT_SPEED_OC=Overclock::DetermineNewSerialSpeed();
}


uint32_t Overclock::MeasureCurrentClockSpeed(){
	uint32_t retval=0;	
    cli();
  MCUSR &= ~(1<<WDRF);
  wdt_reset();
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  WDTCSR = ( (0<<WDE) | (1<<WDIE) | (0<<WDP3) | (0<<WDP2) | (0<<WDP1) | (0<<WDP0)  ); // 16 MILLISECOND TIMEOUT, INTERRUPT ONLY MODE
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;  
    TCCR1A= 0; // turn off PWM
    TCCR1B = 0x02; // set clock scaler to 1/8
   NUM_CLOCKS=0;
   TCNT1 = 0;
   wdt_reset();  
  sei();
  while(NUM_CLOCKS==0) {;} // Wait for interrupt to trigger
	  wdt_disable();
    TCCR1B = 0x00; // TURN OFF TIMER
  TIMSK1 = 0;  
    TCCR1A= 0; // turn off PWM
   TCNT1 = 0;
retval=NUM_CLOCKS;
retval=retval*EFFECTIVE_INTERRUPT_CLOCK_RATIO;
return retval;	
}

void Overclock::DetermineWatchdogClockSpeed(){
	// This function uses the factory-calibrated 8MHz oscillator to measure the watchdog clock against.
	// Run only when at stock speeds.
	EFFECTIVE_INTERRUPT_CLOCK_RATIO=1;
	uint32_t measured=Overclock::MeasureCurrentClockSpeed();
	EFFECTIVE_INTERRUPT_CLOCK_RATIO=F_CPU/measured;
}

uint32_t Overclock::DetermineNewSerialSpeed(){
if(F_CPU==F_CPU_OC) {return SERIAL_PORT_SPEED;}
	uint32_t retval=0;
if(SERIAL_PORT_SPEED>0){
uint32_t numerator=  (SERIAL_PORT_SPEED>>4) * (F_CPU>>4);  // Shifts here to avoid overflow
// eh, dunno, division just wasn't working properly here.
  while(numerator>F_CPU_OC){
    numerator-=F_CPU_OC;
    retval++;
  }
retval=retval<<8;
  }
  return retval;
}

