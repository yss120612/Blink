// Beeper.h

#ifndef _BEEPER_h
#define _BEEPER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class Beeper
{
public:
	Beeper() {};
	void setup(uint8_t bp);
	void beep(uint16_t freq=1000, uint16_t dur = 1000);
	void process(long);
private:
	uint8_t beeper_pin;
	long tone_time;

};



