// Discill.h
//������� ���������� �����

#ifndef _HEATER_h
#include "Heater.h"
#endif
#ifndef _TERMOMETER_h
#include "Termometer.h"
#endif

#ifndef _UROVEN_h
#include "Uroven.h"
#endif

#ifndef _KRAN_h
#include "Kran.h"
#endif


#ifndef _DISCILL_h
#define _DISCILL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif


#define DS_OFF  0 //������
#define DS_WAITOFF 1 //��������� ���, ���� ���������� ����
#define DS_FORSAJ  2 //������ �� temp_start
#define DS_WORK 3

#define DSEND_NO 0 //��������
#define DSEND_TKUB 1 //��������� �� � ����
#define DSEND_UROVEN 2 //��������� �� ������
#define DSEND_ERROR 3 //��������� ��������

#define DSERR_OK 0 //��� ������
#define DSERR_NOTKUB 1 //��� ���������� � ����
#define DSERR_NOTTSA 2 //��� ���������� � ���
#define DSERR_NOHEATER 3 //��� ����
#define DSERR_NOUROVEN 4 //��� ������� ������
#define DSERR_NOKRAN 5 //��� ����� ����
#define DSERR_HOTTSA 6 //�������� �� ���


const uint16_t check_time=1000;
const uint8_t t_tolerance=5;
//const uint8_t t_tsa_avary = 60;//����������� ��� ��� ������� �������� �����������������
const uint8_t max_mode_corrected = 3;//���������� ������� �����������������

const uint8_t shift_water = 3;//��� ������������� ��������� ���� ��
const uint8_t shift_power = 5;//��� ������������� �������� �������� ��

const uint8_t time_wait_correct = 5;//����� ���� ��������� T TCA ����� ���������
const uint8_t time_wait_kran=2;//���������� (������) ����� ����� ���������� ���� (���� ��� ��������)

const uint8_t temp_tsa_error = 60;

class Discill {
private:
	Termometer *temp_kub;//��������� ����
	//Termometer *temp_tsarga;//��������� �� ����� (���� �� �����)
	Termometer *temp_tsa;//��������� �� ������
	Uroven * uroven;
	Heater * heater;//���
	Kran * kran;//������� ����
	uint8_t stage;//������
	uint8_t end_reason;//������� ����������
	uint8_t err;//��� ������

	uint16_t last_check;

	//������� �����������
	uint8_t temp_start;
	uint8_t temp_end_process;

	

	uint8_t work_power;//�������� ������������ �� ����� ��������
	uint8_t work_water;//�������� ������ ���� 17-87 �� ����� ��������

	
	long kran_process_active;
	long correction_process_active;

	

	uint8_t modeCorrected;//������� ��� �������������� ���� + ������
	void correctMode();
public:
	Discill() { stage = 0; temp_start = 60; temp_end_process = 98; err = 0; work_power = 50; work_water = 25; };
	void start();
	void stop();
	
	void initWorkMode();
	void initForceMode();
	void error(uint8_t);
	void setup(Heater *h,Kran *k,Termometer *tk, Termometer *tc, Termometer *ta,Uroven *ur) {
		temp_kub = tk;
		temp_tsa = ta;
		heater = h;
		kran = k;
		uroven = ur;
		stage = 1;
		kran_process_active = 0;
		correction_process_active = 0;
		
	}

	void process(long ms);
};