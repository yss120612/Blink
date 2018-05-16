// Termometer.h

#ifndef _TERMOMETER_h
#define _TERMOMETER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

const int TERMISTOR_B = 3435;
const int TERMISTOR_SECOND_RESISTOR = 9990;
const uint8_t avg_aray=5;

class Termometer
{
private:
	int counter;
	uint16_t data [avg_aray];
	float temperature;
	uint8_t term_pin;
	void calc();
public:
	Termometer() {}

	void setup(uint8_t tp) {
		term_pin = tp;
		pinMode(term_pin, INPUT);
		counter = 0;
	}
	void processTermometr();

	float get_temperature() { calc(); return temperature; }
	
};


