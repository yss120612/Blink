// Suvid.h

#include "Heater.h"
#include "Termometer.h"
#ifndef _SUVID_h
#define _SUVID_h
//

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class Suvid {
private:
	Heater * heater;
	Termometer * term;
	float targetT;
public:
	Suvid(Heater * he, Termometer * tr) {
		heater = he;
		term = tr;
		targetT = 0;
	}
	void set_T(float t) {
		targetT = t;
	}
	void process_suvid();
};
