// Uroven.h
//датчик уровня
#ifndef _UROVEN_h
#define _UROVEN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif
const uint16_t tolerance=100;//чувствительность
const uint8_t event_count = 3;//количество срабатываний для верности
const uint16_t meajure_time = 1000;
class Uroven
{
public:
	Uroven() {};
	void process(long ms);
	boolean isActive();
	boolean isWarning();
	void setup(uint8_t ui, uint8_t pp);
	void reset();
private:
	uint8_t events;
	uint8_t uroven_pin;
	uint8_t power_pin;
	boolean warning;
	long last_meajure;
};
