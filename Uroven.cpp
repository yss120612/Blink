// 
// 
// 

#include "Uroven.h"

void Uroven::setup(uint8_t ui, uint8_t pp){
	uroven_pin = ui;
	power_pin = pp;
	pinMode(uroven_pin, INPUT);
	pinMode(power_pin, OUTPUT);
	digitalWrite(power_pin, HIGH);
	events = 0;
	warning = false;
}

void Uroven::reset() {
	warning = false;
	digitalWrite(power_pin, HIGH);
	events = 0;
}

void Uroven::process(long ms) {
	if (warning) return;
	if (ms - last_meajure, meajure_time) return;
	last_meajure = ms;
	if (analogRead(uroven_pin) > tolerance) {
		events++;
	if (events >= event_count) {
		digitalWrite(power_pin, LOW);
		warning = true;
	}
	}
}

boolean Uroven::isActive() {
	return 	digitalRead(power_pin)==HIGH;
}

boolean Uroven::isWarning() {
	return 	warning;
}