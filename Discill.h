// Discill.h
//������� ���������� �����
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
	//Termometer *temp_kub;//��������� ����
	//Termometer *temp_tsarga;//��������� �� �����
	//Termometer *temp_tsa;//��������� �� ������
	//Uroven * uroven;
	//Heater * heater;//���
	//Kran * kran;//������� ����
	//uint8_t stage;//������
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