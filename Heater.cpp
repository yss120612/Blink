// 
// 
// 

#include "Heater.h"

Heater::Heater() {
	have_relay = false;
	heater_stopped = true;
};


boolean Heater::processHeater() {
	if (heater_stopped) { 
		if (digitalRead(heater_pin)==HIGH) digitalWrite(heater_pin, LOW);
		return false;
	}
	if (!relayIsOn()) return false;
	
	curr -= power;
	if (curr < 0) {
		curr += max_power;
		digitalWrite(heater_pin, HIGH);
		return true;
	}
	else
	{
		digitalWrite(heater_pin, LOW);
		return false;
	}
	//Serial.println(curr);
}

void Heater::setup(uint8_t hp, int8_t rp) {
	heater_pin = hp;
	pinMode(heater_pin, OUTPUT);
	if (rp < 0) {
		have_relay = false;
	}
	else
	{
		have_relay = true;
		relay_pin = rp;
		pinMode(relay_pin, OUTPUT);
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

uint8_t Heater::getPower()
{
	return power;
}

int16_t Heater::getCurr()
{
	return curr;
}

void Heater::setPower(int pw) {
	if (pw == power) return;
	boolean hs = heater_stopped;
	heater_stopped = true;
	power = max(min(pw, max_power), 0);
	curr = max_power / 2;
	heater_stopped = hs;
}