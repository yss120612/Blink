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
}

void Uroven::reset() {
	digitalWrite(power_pin, HIGH);
	events = 0;
}

boolean Uroven::process() {
	if (analogRead(uroven_pin) > tolerance) {
		events++;
	if (events >= event_count) {
		digitalWrite(power_pin, LOW);
		return true;
	}
	}
	return false;
}

boolean Uroven::isActive() {
	return 	digitalRead(power_pin)==HIGH;
}