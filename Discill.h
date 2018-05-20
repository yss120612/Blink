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
	//Termometer *temp_kub;//термометр куба
	//Termometer *temp_tsarga;//термометр на царге
	//Termometer *temp_tsa;//аварийный на трубке
	//Uroven * uroven;
	//Heater * heater;//тен
	//Kran * kran;//водяной кран
	//uint8_t stage;//стадия
	//uint16_t last_check;
public:
	//Discill() { stage = 0; };
	//void error(uint8_t);
	//void setup(Heater *h,Kran *k,Termometer *tk, Termometer *tc, Termometer *ta,Uroven *ur) {
	//	temp_kub = tk;
	//	temp_tsarga = tc;
	//	temp_tsa = ta;
	//	heater = h;
	//	kran = k;
	//	uroven = ur;
	//	stage = 1;
	//}

	void process(uint16_t ms);
};