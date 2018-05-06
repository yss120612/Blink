// 
// 
// 

#include "Heater.h"


void Heater::processHeater() {
	if (heater_stopped) {
		digitalWrite(heater_pin, LOW);
		return;
	}
	if (!relayIsOn()) return;

	curr -= power;
	if (curr < 0) {
		curr += max_power;
		digitalWrite(heater_pin, LOW);
	}
	else
	{
		digitalWrite(heater_pin, HIGH);
	}
}

void Heater::setup(uint8_t hp, int8_t rp) {
	heater_pin = hp;
	if (rp < 0) {
		have_relay = false;
	}
	else
	{
		have_relay = true;
		relay_pin = rp;
	}
}



void Heater::switchRelay(boolean on) {
	if (have_relay)
	{
		digitalWrite(relay_pin, (on ? HIGH : LOW));
	}
}

//реле включено если его нет вообще или оно есть и включено
boolean Heater::relayIsOn() {
	return !have_relay || digitalRead(relay_pin) == HIGH;
}

void Heater::start() {
	switchRelay(true);
	heater_stopped = false;
}

void Heater::stop() {
	switchRelay(false);
	heater_stopped = true;
}

void Heater::setPower(int pw) {
	boolean hs = heater_stopped;
	heater_stopped = true;
	power = max(min(pw, max_power),0);
	curr = max_power / 2;
	heater_stopped = hs;
}