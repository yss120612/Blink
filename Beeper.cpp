// 
// 
// 

#include "Beeper.h"


void Beeper::setup(uint8_t bp)
{
	beeper_pin = bp;
	pinMode(beeper_pin, OUTPUT);
	tone_time = 0;
}


void Beeper::beep(uint16_t freq = 1000, uint16_t dur = 1000) {
	tone(beeper_pin, freq); // включаем звук 
	tone_time = millis() + dur;
}

void Beeper::process(long ms) {
	if (tone_time > 0 && tone_time - ms < 0)
	{
		tone_time = 0;
		noTone(beeper_pin); // выключаем звук
	}
}