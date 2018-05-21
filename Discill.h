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
/*
stage=0 ������
stage=1 ������ �� temp_start
stage=6 ������������� �� ������
stage=7 ��������� ���, ���� ���������� ����
stage=8 ���������� ���������� �� ������� � ����
stage=9 ���������� ���������� �� ������
stage=10 ��������� ���������� (������ err)
*/

const uint16_t check_time=1000;
const uint8_t t_tolerance=5;
class Discill {
private:
	Termometer *temp_kub;//��������� ����
	Termometer *temp_tsarga;//��������� �� ����� (���� �� �����)
	Termometer *temp_tsa;//��������� �� ������
	Uroven * uroven;
	Heater * heater;//���
	Kran * kran;//������� ����
	uint8_t stage;//������
	uint16_t last_check;
	//������� �����������
	uint8_t temp_start;
	uint8_t temp_tsa_error;
	uint8_t temp_end_process;

	uint8_t err;

	uint8_t work_power;//�������� ������������ �� ����� ��������
	uint8_t work_water;//�������� ������ ���� 17-87 �� ����� ��������
	uint8_t kran_off_pause;//���������� (������) ����� ����� ���������� ���� (���� ��� ��������)
	uint16_t kran_process_active;
	
public:
	Discill() { stage = 0; temp_start = 60; temp_tsa_error = 55; temp_end_process = 98; kran_off_pause = 2 * 60; err = 0; work_power = 50; work_water = 25; };
	void start();
	void stop();
	
	void initPeregon();
	void error(uint8_t);
	void setup(Heater *h,Kran *k,Termometer *tk, Termometer *tc, Termometer *ta,Uroven *ur) {
		temp_kub = tk;
		temp_tsarga = tc;
		temp_tsa = ta;
		heater = h;
		kran = k;
		uroven = ur;
		stage = 1;
		kran_process_active = 0;
	}

	void process(uint16_t ms);
};