// Suvid.h

#ifndef _HEATER_h
#include "Heater.h"
#endif

#ifndef _DALLASTERM_h
#include "DallasTerm.h"
#endif

#ifndef _BEEPER_h
#include "Beeper.h"
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
	Beeper * beeper;
	float targetT;
	long last_time;
	boolean work_mode;
	long time;
public:
	Suvid(Heater * he, DallasTerm * tr, Beeper * bee) {
		heater = he;
		term = tr;
		beeper = bee;
		targetT = 0;
		last_time = 0;
	}
	/*void set_T(float t) {
		targetT = t;
	}*/
	void start(uint16_t tm,uint16_t min);
	void stop();
	void process_suvid(long);
	int getHeaterPower() { return heater->getPower(); };
};
