// kran.h

#ifndef _KRAN_h
#define _KRAN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif
const int switch_time = 7000;
const uint8_t quantumT=70;

class Kran {
private:
	boolean inQuantum;
	boolean inProgress;
	uint16_t progress_time;
	uint8_t close_pin;
	uint8_t open_pin;
	uint8_t measure_pin;
	uint8_t relay_pin;
	uint8_t state;
	int8_t quantum;
public:
	Kran() {
	};
	void forceClose();
	void openQuantum(int8_t oq);
	void shiftQuantum(int8_t oq);
	void process(uint16_t ms);
	boolean measureState();
	
	void setup(uint8_t c_pin, uint8_t o_pin, uint8_t m_pin, uint8_t r_pin);
	void close();
	void open();
};
