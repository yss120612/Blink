// 
// 
// 

#include "Discill.h"

void Discill::error(uint8_t e) {
	err = e;
	end_reason = DSEND_ERROR;
}

void Discill::stop() {
	
	kran_process_active = millis()+time_wait_kran*60000;
	heater->stop();
	stage = DS_WAITOFF;
}

void Discill::start() {
	stage = DS_OFF;
	err = DSERR_OK;
	end_reason = DSEND_NO;

	if (heater == NULL) {
		error(DSERR_NOHEATER);
		return;
	}
	if (temp_tsa == NULL) {
		error(DSERR_NOTTSA);
		return;
	}
	if (kran == NULL) {
		error(DSERR_NOKRAN);
		return;
	}
	if (temp_kub == NULL) {
		error(DSERR_NOTKUB);
		return;
	}
	if (uroven == NULL) {
		uroven->reset();
		error(DSERR_NOUROVEN);
		return;
	}
	initForceMode();
}

void Discill::initForceMode() {
	heater->setPower(100);
	heater->start();
	kran->forceClose();
	stage = DS_FORSAJ;
}

void Discill::initWorkMode() {
	heater->setPower(work_power);
	kran->openQuantum(work_water);
	stage = DS_WORK;
}

void Discill::correctMode() {
	//поджимаем мощу и расширяем кран
	//так можно сделать прои перегреве ТСА max_mode_corrected раз
	if (modeCorrected >= max_mode_corrected) {
		error(DSERR_HOTTSA);
		return;
	}
	modeCorrected++;
	heater->setPower(heater->getPower() - shift_power);
	kran->shiftQuantum(-shift_water);
	correction_process_active= millis() + time_wait_correct * 60000;
}

void Discill::process(long ms) {
	

	if (kran_process_active > 0) {
		if (kran_process_active - ms<0) {
			kran_process_active = 0;
			kran->forceClose();
			stage = DS_OFF;
		}
	}

	if (correction_process_active > 0) {
		if (correction_process_active-ms<0) {
			correction_process_active = 0;
		}
	}

	if (stage <=DS_WAITOFF) return;

	if (err > DSERR_OK) {
		stop();
		return;
	}

	if (last_check + check_time - ms > 0) return;
	last_check = ms;

	if (correction_process_active==0 && temp_tsa->get_temperature() > temp_tsa_error)
	{
		correctMode();
	}
	
	if (uroven->isActive && uroven->isWarning) {
		stop();
		stage = DSEND_UROVEN;
		return;
	}

	if (temp_kub->get_temperature() > temp_end_process) {
		stop();
		stage = DSEND_TKUB;
		return;
	}
	

	switch (stage)
	{
	case DS_FORSAJ:
		if (temp_kub->get_temperature() > temp_start) {
			initWorkMode();
		}
		break;
	case DS_WORK:
		break;
	
	default:
		break;
	}

	
	

	}


