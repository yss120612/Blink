// 
// 
// 

#include "Suvid.h"


void Suvid::process_suvid(long ms) {
	if (heater == NULL || term == NULL || targetT==0) return;
	if (ms - last_time < test_time) return;
	last_time = ms;

	if (work_mode && ms > time) {
		stop();
		beeper->beep(1000, 5000);
		return;
	}


	float tmp = term->getTemp();
	
	if (tmp >= targetT && !work_mode) {//вышли на рабочий режим
		work_mode = true;
		beeper->beep(1000, 500);
		time = ms + time * 60000L;
	}
	if (work_mode)
	{
		Serial.print("Curr=");
		Serial.print(ms);
		Serial.print("Time=");
		Serial.println(time);
	}

	uint8_t need_pw=100;

	if (targetT <= tmp) {
		need_pw = 0;
	}
	else if (targetT - tmp < 10)
	{
		need_pw = (targetT - tmp) * 10+5;
	}
	heater->setPower(need_pw);
	
}


void Suvid::start(uint16_t tm, uint16_t  min) {
	time = min;
	work_mode = false;
	targetT = tm;
	heater->start();
}

void Suvid::stop() {
	heater->setPower(0);
	heater->stop();
	targetT = 0;
}