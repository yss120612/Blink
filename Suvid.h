// Suvid.h

#ifndef _HEATER_h
#include "Heater.h"
#endif
#ifndef _DALLASTERM_h
#include "DallasTerm.h"
#endif


#ifndef _SUVID_h
#define _SUVID_h
//

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif
const uint16_t test_time=1000;

class Suvid {
private:
	Heater * heater;
	DallasTerm * term;
	float targetT;
	uint16_t last_time;
public:
	Suvid(Heater * he, DallasTerm * tr) {
		heater = he;
		term = tr;
		targetT = 0;
		last_time = 0;
	}
	void set_T(float t) {
		targetT = t;
	}
	void process_suvid(uint16_t);
	int getHeaterPower() { return heater->getPower(); };
};
