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

class Discill {
private:
	Termometer *temp_kub;//��������� ����
	Termometer *temp_tsarga;//��������� �� �����
	Termometer *temp_tsa;//��������� �� ������
	Heater * heater;//���
	Kran * kran;//������� ����
	uint8_t stage;//������
public:
	Discill() {};
	void process();
};