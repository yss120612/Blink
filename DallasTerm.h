// DallasTerm.h


#include <OneWire.h>
#ifndef _DALLASTERM_h
#define _DALLASTERM_h



#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif
const uint8_t dim = 5;
const uint16_t interval = 1000;
class DallasTerm
{
public:
	DallasTerm(uint8_t addr[], OneWire *d, float del=0) {
		//for (uint8_t i=0;i<8;i++) address[i] = addr[i];
		address = addr;
		counter = 0;
		ds = d;
		delta = del;
		meajured = false;
	}

	
	boolean isMy(uint8_t addr[]) {
		for (uint8_t i = 0; i < 8; i++) {
			if (addr[i] != address[i]) return false;
		}
		return true;
	}

	float getTemp();
	void process(uint16_t ms);
	void set12bit();
private:
	OneWire * ds;
	uint8_t * address;
	uint16_t lastWork;
	boolean meajured;
	float temp[dim];
	float delta;
	uint8_t counter;

};

