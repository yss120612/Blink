// 
// 
// 

#include "Kran.h"


void Kran::close() {
	if (!inProgress && measureState()) {
		inProgress = true;
		progress_time = millis();
		digitalWrite(close_pin, HIGH);
	}
}
void Kran::forceClose() {
	inProgress = true;
	progress_time = millis();
	digitalWrite(close_pin, HIGH);
}

void Kran::openQuantum(int8_t oq)
{
	inQuantum = true;
	forceClose();
	quantum = oq;
}

void Kran::shiftQuantum(int8_t oq)
{
	//if (quantum == 0) return;
	inQuantum = true;
	
	progress_time = millis();
	
	quantum = oq;
	if (oq < 0) {
		quantum = -oq;
		digitalWrite(close_pin, HIGH);
	}
	else {
		digitalWrite(open_pin, HIGH);
	}
}

void Kran::process(uint16_t ms) {
	if (!inProgress && !inQuantum) return;
	if (inProgress) {
		if (ms - progress_time > switch_time) {
			if (digitalRead(open_pin) == HIGH) {
				kran_opened = true;
			}
			else {
				kran_opened = false;
			}
			digitalWrite(open_pin, LOW);
			digitalWrite(close_pin, LOW);
			inProgress = false;
			if (quantum > 0) {
					digitalWrite(open_pin, HIGH);
					progress_time = millis();
			}
		}
	}
	else {
		if (ms - progress_time > quantum*quantumT) {
			digitalWrite(open_pin, LOW);
			digitalWrite(close_pin, LOW);
			inQuantum = false;
			quantum = 0;
		}
	}

}

void Kran::open() {
	if (!inProgress && !measureState()) {
		inProgress = true;
		progress_time = millis();
		digitalWrite(open_pin, HIGH);
	}
}

boolean Kran::measureState() {
	digitalWrite(relay_pin, HIGH);
	delay(200);
	boolean result = analogRead(measure_pin)<100;
	//int result= analogRead(measure_pin);
	digitalWrite(relay_pin, LOW);
	delay(200);
	return result;
}