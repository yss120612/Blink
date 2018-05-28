// Discill.h
//Процесс дистиляции сырца

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


#define DS_OFF  0 //начало
#define DS_WAITOFF 1 //выключили тен, ждем отключение воды
#define DS_FORSAJ  2 //разгон до temp_start
#define DS_WORK 3

#define DSEND_NO 0 //работаем
#define DSEND_TKUB 1 //закончили по т куба
#define DSEND_UROVEN 2 //закончили по уровню
#define DSEND_ERROR 3 //закончили аварийно

#define DSERR_OK 0 //нет ошибок
#define DSERR_NOTKUB 1 //нет градусника в кубе
#define DSERR_NOTTSA 2 //нет градусника в ТСА
#define DSERR_NOHEATER 3 //нет тена
#define DSERR_NOUROVEN 4 //нет датчика уровня
#define DSERR_NOKRAN 5 //нет крана воды
#define DSERR_HOTTSA 6 //перегрев по ТСА


const uint16_t check_time=1000;
const uint8_t t_tolerance=5;
//const uint8_t t_tsa_avary = 60;//температура ТСА при которой пытаемся скорректироваться
const uint8_t max_mode_corrected = 3;//количество попыток скорректироваться

const uint8_t shift_water = 3;//при корректировке добавляем воду на
const uint8_t shift_power = 5;//при корректировке сбавляем мощность на

const uint8_t time_wait_correct = 5;//минут ждем коррекцию T TCA между попытками
const uint8_t time_wait_kran=2;//отключение (минуты) крана после выключения тена (если нет протечек)

const uint8_t temp_tsa_error = 60;

class Discill {
private:
	Termometer *temp_kub;//термометр куба
	//Termometer *temp_tsarga;//термометр на царге (пока не юзаем)
	Termometer *temp_tsa;//аварийный на трубке
	Uroven * uroven;
	Heater * heater;//тен
	Kran * kran;//водяной кран
	uint8_t stage;//стадия
	uint8_t end_reason;//причина завершения
	uint8_t err;//код ошибки

	uint16_t last_check;

	//границы температуры
	uint8_t temp_start;
	uint8_t temp_end_process;

	

	uint8_t work_power;//мощность обогревателя во время перегона
	uint8_t work_water;//мощность потока воды 17-87 во время перегона

	
	long kran_process_active;
	long correction_process_active;

	

	uint8_t modeCorrected;//сколько раз корректировали воду + нагрев
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