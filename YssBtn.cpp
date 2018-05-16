#include "YssBtn.h"
void YssBtn::init(int8_t p) {
	pin = p;
	beep_pin = 0;
	pinMode(pin, INPUT);
	digitalWrite(pin, HIGH);
	last = false;
	lastTime = 0;
	downTime = 0;
	statePrint = 0;
	clickTime = 0;
	longOnce = true;
}

void YssBtn::initBeep(int8_t p, int16_t f, int16_t d) {
	beep_pin = p;
	freq = f;
	duration = d;
}

void YssBtn::beep() {
	tone(beep_pin, freq); // включаем звук 
	delay(duration); // 
	noTone(beep_pin); // выключаем звук
}

void YssBtn::process() {
	boolean curr = digitalRead(pin);
	uint16_t mls = millis();
	/* if (mls-statePrint>5000){
	statePrint=mls;
	Serial.print("last:");
	Serial.print(last);
	Serial.print(" curr:");
	Serial.print(curr);
	Serial.print(" lastTime=");
	Serial.print(lastTime);
	Serial.print(" mls:");
	Serial.println(mls);
	}*/

	if (downTime>0 && mls - downTime>longPress) {
		if (clickLongFunc) (*clickLongFunc)();
		downTime = (longOnce ? 0 : mls);
	}

	if (clickTime>0 && mls - clickTime>dblClick) {
		clickTime = 0;
	}


	if (lastTime>0 && mls - lastTime>drebb) {
		if (curr == last) {
			if (!curr)
			{
				downTime = mls;
			}
			else
			{
				if (downTime>0) {
					if (clickTime>0 && mls - clickTime<dblClick) {
						if (beep_pin>0) beep();
						if (clickFunc) (*clickFunc)();
						if (clickDblFunc) (*clickDblFunc)();
						clickTime = 0;
					}
					else {
						if (beep_pin>0) beep();
						if (clickFunc) (*clickFunc)();
						clickTime = mls;
					}
					downTime = 0;
				}
			}
		}
		last = curr;
		lastTime = 0;
		return;
	}

	if (curr != last && lastTime == 0) {
		last = curr;
		lastTime = mls;
	}
}


