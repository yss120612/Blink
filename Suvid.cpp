// 
// 
// 

#include "Suvid.h"


void Suvid::process_suvid(uint16_t ms) {
	if (heater == NULL || term == NULL) return;
	if (ms - last_time < test_time) return;
	last_time = ms;

	float tmp = term->getTemp();
	//uint8_t pw = heater->getPower();
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