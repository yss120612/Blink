// 
// 
// 

#include "Heater.h"


void Heater::processHeater() {
	if (heater_stopped) {
		digitalWrite(heater_pin, LOW);
		return;
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
void Heater::setPower(int pw) {
	boolean hs = heater_stopped;
	heater_stopped = true;
	power = pw;
	curr = max_power / 2;
	counter = 0;
	heater_stopped = hs;
}