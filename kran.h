// kran.h

#ifndef _KRAN_h
#define _KRAN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class Kran {
private:
	boolean kran_opened;
	const int switch_time = 5000;
	boolean inProgress;
	uint16_t progress_time;
	uint8_t close_pin;
	uint8_t open_pin;

public:
	Kran(uint8_t c_pin, uint8_t o_pin) {
		close_pin = c_pin;
		open_pin = o_pin;
		digitalWrite(close_pin, LOW);
		digitalWrite(open_pin, LOW);
		inProgress = false;
		kran_opened = false;
		progress_time=0;
	};
	void process(uint16_t ms);

	boolean isOpened() {
		return kran_opened;
	}

	void close();
	void open();
};