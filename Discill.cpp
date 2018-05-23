// 
// 
// 

#include "Discill.h"

void Discill::error(uint8_t e) {
	err = e;
	stage = 10;
}


void Discill::stop() {
	kran_process_active = millis()+time_wait_kran*1000*60;
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
		stage = 8;
		stop();
		return;
	}
	
	modeCorrected = 0;

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

void Discill::correctMode() {
	if (modeCorrected >= max_mode_corrected) {
		error(11);
		return;
	}
	modeCorrected++;
	heater->setPower(heater->getPower() - shift_power);
	kran->shiftQuantum(-shift_water);
	correction_process_active= millis() + time_wait_correct * 1000 * 60;
}

void Discill::process(uint16_t ms) {
	

	if (kran_process_active > 0) {
		if (ms > kran_process_active) {
			kran_process_active = 0;
			kran->forceClose();
		}
	}

	if (correction_process_active > 0) {
		if (ms > kran_process_active) {
			correction_process_active = 0;
		}
	}

	if (stage == 0 || stage>7) return;

	if (err > 0) {
		stop();
	}

	switch (stage)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
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

