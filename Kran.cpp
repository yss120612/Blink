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
{ //min==17 max==87
	inQuantum = true;
	forceClose();
	quantum = oq;
	state = oq;
}

void Kran::shiftQuantum(int8_t oq)
{
	if (state + oq > 100 || state + oq < 0) return;
	inQuantum = true;
	progress_time = millis();
	state += oq;
	if (oq < 0) {
		quantum = -oq;
		digitalWrite(close_pin, HIGH);
	}
	else {
		quantum = oq;
		digitalWrite(open_pin, HIGH);
	}
}

void Kran::process(long ms) {
	if (!inProgress && !inQuantum) return;
	if (inProgress) {
		if (ms - progress_time > switch_time) {
			if (digitalRead(open_pin) == HIGH) {
				state = 100;
			}
			else {
				state = 0;
			}
			digitalWrite(open_pin, LOW);
			digitalWrite(close_pin, LOW);
			inProgress = false;
			if (quantum > 0) {
					digitalWrite(open_pin, HIGH);
					progress_time = ms;
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


void Kran::setup(uint8_t c_pin, uint8_t o_pin, uint8_t m_pin, uint8_t r_pin) {
	close_pin = c_pin;
	open_pin = o_pin;
	measure_pin = m_pin;
	relay_pin = r_pin;

	pinMode(measure_pin, INPUT);
	pinMode(open_pin, OUTPUT);
	pinMode(close_pin, OUTPUT);
	pinMode(relay_pin, OUTPUT);
	digitalWrite(close_pin, LOW);
	digitalWrite(open_pin, LOW);
	digitalWrite(relay_pin, LOW);

	inProgress = false;
	inQuantum = false;
	quantum = 0;
	state = 0;
	progress_time = 0;
}