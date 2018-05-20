// DallasTerm.h

#ifndef _DALLASTERM_h
#define _DALLASTERM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class DallasTerm
{
public:
	DallasTerm();
	~DallasTerm();

private:
	uint8_t address[8];
};

