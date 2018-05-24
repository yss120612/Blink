// 
// 
// 

#include "Beeper.h"


void Beeper::setup(uint8_t bp)
{
	beeper_pin = bp;
	pinMode(beeper_pin, OUTPUT);
}


void Beeper::beep(uint16_t freq = 1000, uint16_t dur = 1000) {
	tone(beeper_pin, freq); // включаем звук 
	delay(dur); 
	noTone(beeper_pin); // выключаем звук
}