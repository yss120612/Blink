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
void Kran::process(uint16_t ms) {
	if (!inProgress) return;
	if (ms - progress_time > switch_time) {
		if (digitalRead(open_pin) == HIGH) {
			kran_opened = true;
	 	}
		else {
			kran_opened = false;
		}
		digitalWrite(open_pin, LOW);
		digitalWrite(close_pin, LOW);
	}
	inProgress = false;

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