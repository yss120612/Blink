// 
// 
// 

#include "kran.h"


void Kran::close() {
	if (kran_opened) {
		inProgress = true;
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
		digitalWrite(open_pin, HIGH);
	}
}