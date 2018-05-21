// 
// 
// 

#include "Discill.h"

void Discill::error(uint8_t e) {
	err = e;
}


void Discill::stop() {
	kran_process_active = millis()+kran_off_pause*1000;
	heater->stop();
}

void Discill::start() {
	stage = 0;
	float tk=0;
	err = 0;
	if (temp_tsarga != NULL) {
		tk = temp_tsarga->get_temperature();
	}

	if (tk >= temp_tsa_error) {
		error(11);//перегрев ТСА
		return;
	}

	tk = 0;

	if (temp_kub == NULL) {//обязательно
		error(10);//нед градусника в кубе - досвидос
		return;
	}
	else {

		tk=temp_kub->get_temperature();
	}

	if (tk >= temp_end_process) {
		stop();
		return;
	}
	
	if (tk >= temp_start) {
		initPeregon();
		return;
	}

	if (uroven != NULL) {
		uroven->reset();
	}
	
	kran->forceClose();
	heater->setPower(100);
	heater->start();
	stage = 1;
}

void Discill::initPeregon() {
	heater->setPower(work_power);
	kran->openQuantum(work_water);
	stage = 2;
}


void Discill::process(uint16_t ms) {
	

	if (kran_process_active > 0) {
		if (ms > kran_process_active) {
			kran_process_active = 0;
			kran->forceClose();
			if (err > 0) stage = 10;
		}
	}

	if (stage == 0 || stage>7) return;

	if (err > 0) {
		stop();
	}

	if (ms - last_check > check_time) {
		float tmp;
		if (temp_kub == NULL) {//обязательно
			error(10);
		}
		else {

			temp_kub->get_temperature();
		}
		
		
		
		if (temp_tsa != NULL) {
			temp_tsa->get_temperature();
		}

		//if (uroven)

		last_check = millis();

	}
}