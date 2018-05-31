// Mode.h
#include <U8glib.h>

#ifndef _MODE_h
#define _MODE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class Mode
{
public:
	Mode();
	virtual void draw(U8GLIB * u8) = 0;
	
private:
	Mode * parent;
};

Mode::Mode()
{
	parent = NULL;
}

