// 
// 
// 

#include "Suvid.h"


void Suvid::process_suvid() {
	if (heater == NULL || term == NULL) return;
	float tmp = term->get_temperature();
	uint8_t pw = heater->getPower();
	uint8_t need_pw=100;
	if (targetT <= tmp) {
		need_pw = 0;
	}
	else if (targetT - tmp < 10)
	{
		need_pw = (targetT - tmp) * 10+5;
	}
	if (need_pw != pw) heater->setPower(need_pw);
}