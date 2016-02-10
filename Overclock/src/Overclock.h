// License: eh, not sure yet.  Free to use for all noncommercial purposes.  If you modify it a little, don't claim it as your own.
// cirthix@gmail.com
// VERSION 0.9
// VERY IMPORTANT NOTE: THIS LIBRARY RELIES ON THE WATCHDOG INTERRUPT SERVICE REQUEST.  THE WDT_ISR MUST CONTAIN THE FOLLOWING:
//
//		ISR(WDT_vect) {
//			my_Overclock_object.WatchdogFired();
//			// Add your other watchdog code here.
//		}
//	This lib will also zero-out the timer1 configuration.  Because you should call this as one of the first lines in setup(), this is OK.
// TODO: add support for clock frequency dividers for underclocking

#ifndef OVERCLOCK_h
#define OVERCLOCK_h

#include <Arduino.h>
#include <inttypes.h>
#include <avr/wdt.h>

const uint32_t OVERCLOCK_MAX_SPEED = 0xFFFFFFFF;


// Please only ever call this with a compile-time-known constant.
uint32_t CalculateMaxClock(float VCC);											// Calculates max cpu speed based on voltage.  Not part of the class so you can call it in constructor.

class Overclock
{
	public:  
						Overclock();											// Default constructor creates the object.
						Overclock(uint32_t TargetSpeed);						// Constructor creates the object and sets cpu speed.
						Overclock(uint32_t TargetSpeed, uint32_t SerialSpeed);	// Constructor creates the object, sets cpu speed, and sets Serial() speed.
						~Overclock();											// Destroys the object, reverts clock speed and serial speed to stock speeds.
			void  		SetClockSpeed(uint32_t TargetSpeed);					// Determines new clock settings to match requested speed and applies them.
			void  		SetClockSpeedStock();									// Reverts clock speed and serial speed to stock speeds
			void  		SetClockSpeedOC();										// Sets clock speed and serial speed to overclocked speeds
			void  		SetClockSpeedOCMax();									// Overclocks to the highest possible setting.
			uint8_t		IsStock();												// Returns zero if currently overclocked, one if at default speed.
			void 		WatchdogFired();										// Function to call during ISR(WDT_vect);  Saves TCNT1 to NUM_CLOCKS.
			void	 	SetSerialSpeed(uint32_t SerialSpeed);					// Sets the desired(real) speed for Serial.begin()
			uint32_t 	GetSerialSpeed();										// Returns the correct Serial.begin() speed for the current clock speed.
			uint32_t 	GetClockSpeed();										// Returns the current cpu speed.
			uint32_t	DetermineOC(uint32_t TargetSpeed);						// Determines new clock settings to attempt (best-effort, not exact) to match the requested speed.
			void		PrintAll();												// Prints out all state of the library.  Debugging only.
			uint16_t	AdjustDelay(uint16_t target);							// Adjusts a delay value to match the new clock.  Example usage:   delay(my_Overclock_object.AdjustDelay(1000)); Should attempt to delay 1 second.
					private:  			                                                // 
			void		DetermineWatchdogClockSpeed();							// Uses the factory calibration at 8MHz to determine with decent accuracy the watchdog timer clock speed ratio.
					void			InitOC(uint32_t TargetSpeed, uint32_t SerialSpeed);	// Does the work of the constructor.
					float			EFFECTIVE_INTERRUPT_CLOCK_RATIO;
		volatile	uint16_t		NUM_CLOCKS;							// 
					uint32_t		F_CPU_OC;							// 
					uint32_t		SERIAL_PORT_SPEED;					// 
					uint32_t		SERIAL_PORT_SPEED_OC;				// 
					uint8_t 		OSCCAL_STOCK;						// 
					uint8_t 		OSCCAL_OC;							// 
					uint8_t 		OSCCAL_OC_MAX = 0xFF;				// 
					uint8_t 		OSCCAL_OC_MIN = 0x00;				// 
					uint32_t		MeasureCurrentClockSpeed();			// Uses timer1, the watchdog timer, and the watchdog interrupt to measure cpu speed.  Note: can be inaccurate.  Seems to work OK on atmega168p at 3.3v at 20C.
					uint32_t 		DetermineNewSerialSpeed();
};		
#endif
