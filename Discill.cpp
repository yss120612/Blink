// 
// 
// 

#include "Discill.h"


void Discill::process(uint16_t ms) {
	if (ms - last_check > check_time) {
		
		last_check = ms;
	}
}