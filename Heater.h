// Heater.h

#ifndef _HEATER_h
#define _HEATER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class Heater
{
public:
	Heater();
	~Heater();
	void processHeater();
	void setPower(int pw);
	void start();
	void stop();
	void setup(uint8_t hp, int8_t rp);

private:
	uint8_t heater_pin;
	uint8_t relay_pin;
	uint8_t detect_0_pin;
	uint8_t power;
	const uint8_t max_power = 100;
	int curr;
	boolean heater_stopped;
	boolean have_relay;
	void switchRelay(boolean on);
	boolean relayIsOn();
};


Heater::Heater()
{
	heater_stopped = true;
}

Heater::~Heater()
{
}