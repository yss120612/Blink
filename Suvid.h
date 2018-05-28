// Suvid.h

#include <OLED_I2C.h>

#ifndef _HEATER_h
#include "Heater.h"
#endif

#ifndef _DALLASTERM_h
#include "DallasTerm.h"
#endif

#ifndef _BEEPER_h
#include "Beeper.h"
#endif

#ifndef _SUVID_h
#define _SUVID_h
//

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

#define SU_OFF  0 //начало
#define SU_FORSAJ  1 //разгон до temp_start
#define SU_WORK 2

#define SUEND_NO 0 //работаем
#define SUEND_TIME 1 //закончили по времени
#define SUEND_ERROR 2 //закончили аварийно

#define SUERR_OK 0 //нет ошибок
#define SUERR_NOTKUB 1 //нет градусника в кубе
#define SUERR_NOHEATER 2 //нет тена


const uint16_t test_time=1000;

class Suvid {
private:
	Heater * heater;
	DallasTerm * term;
	Beeper * beeper;
	int8_t targetT;
	long last_time;
	int8_t work_mode;
	int8_t end_reason;
	int8_t err;
	long time;
	void error(uint8_t);
public:
	Suvid(Heater * he, DallasTerm * tr, Beeper * bee) {
		heater = he;
		term = tr;
		beeper = bee;
		targetT = 0;
		last_time = 0;
	}
	/*void set_T(float t) {
		targetT = t;
	}*/
	void start(int8_t tm,uint16_t min);
	void stop();
	
	void display(OLED * disp);
	void process_suvid(long);
	int getHeaterPower() { return heater->getPower(); };
};
