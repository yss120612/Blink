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
	const int switch_time = 7000;
	boolean inProgress;
	uint16_t progress_time;
	uint8_t close_pin;
	uint8_t open_pin;
	uint8_t measure_pin;
	uint8_t relay_pin;

public:
	Kran(uint8_t c_pin, uint8_t o_pin, uint8_t m_pin, uint8_t r_pin) {
		close_pin = c_pin;
		open_pin = o_pin;
		measure_pin = m_pin;
		relay_pin = r_pin;

		inProgress = false;
		kran_opened = false;
		progress_time=0;
	};
	void forceClose();
	void process(uint16_t ms);
	boolean measureState();
	boolean isOpened() {
		return kran_opened;
	}
	void setup() {
		pinMode(measure_pin, INPUT);
		pinMode(open_pin, OUTPUT);
		pinMode(close_pin, OUTPUT);
		pinMode(relay_pin, OUTPUT);
		digitalWrite(close_pin, LOW);
		digitalWrite(open_pin, LOW);
		digitalWrite(relay_pin, LOW);
	}
	void close();
	void open();
};
