// 
// 
// 

#include "DallasTerm.h"


float DallasTerm::getTemp() {
	float summ;
	for (uint8_t i = 0; i < dim; i++) {
		summ += temp[i];
	}
	return summ / dim;
}

void DallasTerm::process(uint16_t ms) {
	if ((ms - lastWork < interval) || meajured && (ms - lastWork < interval*2)) return;

	ds->reset(); // Теперь готовимся
	ds->select(address);

	if (meajured) {
	   ds->write(0xBE); // Просим передать нам значение регистров со значением температуры
       temp[counter]= (ds->read() | (ds->read()<<8)) * 0.0625+delta;
	   counter = (counter < dim - 1) ? counter + 1:0;
	   meajured = false;
	   lastWork = ms;
	}
	else {
		ds->write(0x44); // Даем датчику DS18b20 команду измерить температуру. Само значение температуры мы еще не получаем - датчик его положит во внутреннюю память
	}
}

void DallasTerm::set12bit() {
	ds->reset(); // запрос шины 1-Wire
	ds->select(address); // выбор нашего DS18B20
	ds->write(0x4E); // запись в  scratchPad
	ds->write(0x00); // User byte 0 – Unused
	ds->write(0x00); // User byte 1 – Unused
	ds->write(0x7F); // выставляем 12 бит (0x7F)
}

