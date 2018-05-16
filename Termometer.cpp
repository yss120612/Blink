// 
// 
// 

#include "Termometer.h"


void Termometer::processTermometr() {
	data[counter] = analogRead(term_pin);
	counter++;
	if (counter == avg_aray) counter = 0;
}

void Termometer::calc() {
	uint16_t i = 0;
	for (uint8_t j = 0; j < avg_aray; j++) {
		i += data[j];
	}
	i /= avg_aray;
	temperature = TERMISTOR_SECOND_RESISTOR / (1023.0 / i - 1.0);
	temperature = log(temperature / 10000.0) / TERMISTOR_B + 1.0 / (25.0 + 273.15);
	temperature = 1.0 / temperature;
	temperature -= 273.15;   // Kelvin to Celcius
}