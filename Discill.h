// Discill.h
//Процесс дистиляции сырца
#ifndef _DISCILL_h
#define _DISCILL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif
const uint16_t check_time=1000;
class Discill {
private:
	Termometer *temp_kub;//термометр куба
	Termometer *temp_tsarga;//термометр на царге
	Termometer *temp_tsa;//аварийный на трубке
	Heater * heater;//тен
	Kran * kran;//водяной кран
	uint8_t stage;//стадия
	uint16_t last_check;
public:
	Discill() {};
	void process(uint16_t ms);
};