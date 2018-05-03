// YssBtn.h

#ifndef _YSSBTN_h
#define _YSSBTN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class YssBtn {
private:

	int8_t pin;
	const uint8_t drebb = 20;
	const uint16_t longPress = 1500;
	const uint16_t dblClick = 300;
	boolean last;
	uint16_t lastTime;
	uint16_t downTime;
	uint16_t clickTime;
	uint16_t statePrint;
	boolean longOnce;
	//beeper
	uint16_t freq;
	uint16_t duration;
	uint8_t beep_pin;
protected:
	//void writeState();
	void(*clickFunc)();
	void(*clickLongFunc)();
	void(*clickDblFunc)();
	void beep();
public:
	void init(int8_t p);
	void initBeep(int8_t p, int16_t f, int16_t d);
	void process();
	void setClickFunc(void(*func) ()) { clickFunc = func; }
	void setClickLongFunc(void(*func) ()) { clickLongFunc = func; }
	void setClickDblFunc(void(*func) ()) { clickDblFunc = func; }
};