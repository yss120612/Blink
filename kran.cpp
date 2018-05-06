// 
// 
// 

#include "kran.h"


void Kran::close() {
	if (kran_opened) {
		inProgress = true;
		progress_time = millis();
		digitalWrite(close_pin, HIGH);
	}
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
}

void Kran::open() {
	if (!kran_opened) {
		inProgress = true;
		progress_time = millis();
		digitalWrite(open_pin, HIGH);
	}
}

boolean Kran::measureState() {
	digitalWrite(relay_pin, HIGH);
	delay(200);
	boolean result = analogRead(measure_pin)<100;
	digitalWrite(relay_pin, LOW);
	return result;
}