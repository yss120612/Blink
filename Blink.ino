#include "kran.h"
#include "Heater.h"
#include "YssBtn.h"
#include <EEPROM.h>
#include <OLED_I2C.h>

OLED  myOLED(SDA, SCL, 8);

extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

const uint8_t BUZZER_PIN = 12;
const uint8_t COOLER_PIN = 3;
const uint8_t RELAY_PIN = 9;
const uint8_t TERMISTOR_PIN = A0;
const uint8_t UROVEN_PIN = A3;
const uint8_t UROVEN_VCC_PIN = 11;
const int TERMISTOR_B = 3435;
const int TERMISTOR_SECOND_RESISTOR= 9990;
const uint8_t WATER_OPEN_PIN = 5;
const uint8_t WATER_CLOSE_PIN = 6;
const uint8_t WATER_MEASURE_PIN = A2;
const uint8_t LEFT_BTN_PIN = 4;
const uint8_t RIGHT_BTN_PIN = 8;
const uint8_t CENTER_BTN_PIN = 7;

 
uint16_t scrLoop = 0;

const uint16_t ptx = 3;
const uint16_t ptx2 = 128 - 3;

const uint16_t pty1 = 18;
//const uint16_t pty2 = 30;
//const uint16_t pty3 = 42;
//const uint16_t pty4 = 54;
uint16_t pty = 0;

int period = 0;
int curr = 0;
const int sdOn = 2000;
const int sdOff = 3000;
boolean isOn;
YssBtn bLeft, bRight, bOK;
Kran kr(WATER_CLOSE_PIN, WATER_OPEN_PIN, WATER_MEASURE_PIN,RELAY_PIN);
int MenuSelected = 0;

//class Menu{
//	MenuItem itms[];
//};
//
//class MenuItem{
//  private:
//  uint8_t id;
//  char* mname[];
//  Menu submenu;
//  
//  public:
//};


void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(UROVEN_VCC_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(COOLER_PIN, OUTPUT);
  pinMode(TERMISTOR_PIN, INPUT);
  pinMode(UROVEN_PIN, INPUT);
  

  digitalWrite(LED_BUILTIN, HIGH);   
  digitalWrite(UROVEN_VCC_PIN, HIGH);
  digitalWrite(UROVEN_PIN, HIGH);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(COOLER_PIN, LOW);
  
  Serial.begin(57600); 
  
  kr.setup();

  bLeft.init(LEFT_BTN_PIN);
  bRight.init(RIGHT_BTN_PIN);
  bOK.init(CENTER_BTN_PIN);

  bOK.initBeep(12,1000,30);
  bOK.setClickFunc(onCClick);

  bLeft.setClickFunc(onLClick);
  bRight.setClickFunc(onRClick);
  bRight.setClickDblFunc(onRDblClick);
  
  myOLED.begin();
  myOLED.setFont(SmallFont);
}

void onLClick(){
  digitalWrite(LED_BUILTIN,LOW ); 
  if (kr.isOpened()) {
	//  kr.close();
  }
  else {
	//  kr.open();
  }
  if (MenuSelected > 0)
  {
	  MenuSelected--;
	  scrLoop = 0;
  }
}

void onRClick(){
  digitalWrite(LED_BUILTIN,HIGH);   
  
 // Serial.print("State=");
 // Serial.println(kr.measureState());
  if (MenuSelected <3) {
	  MenuSelected++;
	  scrLoop = 0;
  }
}

void onRDblClick() {
	
}

void onCClick(){
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN)   );   
}

double tTermoRes(int pin) {
	int i = analogRead(pin);
	//сопротивление термопары
	double Temp = TERMISTOR_SECOND_RESISTOR / (1023.0 / i - 1.0);
	Temp = log(Temp / 10000.0) / TERMISTOR_B + 1.0 / (25.0 + 273.15);
	Temp = 1.0 / Temp;
	Temp = Temp - 273.15;   // Kelvin to Celcius
	return Temp;
}

boolean Bras(int power, int diap)
{
	bool res = false;
	int err = diap / 2;
	for (int j = 0; j < 10; j++)
	{

		for (int i = 0; i < diap; i++)
		{
			err -= power;
			if (err < 0)
			{
				err += diap;
				res = true;
			}
			else
			{
				res = false;
			}

		}
	}
	return res;
}


void loop() {
   uint16_t mls=millis();
   
   if (mls-scrLoop>1000){

	pty = pty1 + 12 * MenuSelected;
	
    myOLED.clrScr();
	myOLED.setFont(MediumNumbers);
	myOLED.printNumI(mls, LEFT, 0);

	myOLED.setFont(SmallFont);

	myOLED.printNumI(mls, ptx, pty1);
	

    myOLED.print("Braga", ptx, pty1+12);
	//myOLED.drawRoundRect(ptx-2, pty2-2, ptx2, pty2 + 9);

	//myOLED.print("Menu item", ptx, pty2);
	//myOLED.drawRoundRect(ptx - 2, pty3 - 2, ptx2, pty3 + 9);

	

	double tm = tTermoRes(TERMISTOR_PIN);
	int vlaj = analogRead(UROVEN_PIN);

	myOLED.printNumF(tm,1, ptx, pty1+24);
	///Serial.println(Temp);
	//myOLED.drawRoundRect(ptx - 2, pty3 - 2, ptx2, pty3 + 9);
	
	
	uint8_t speed = tm > 60 ? 255 : tm < 30 ? 0 : 105 + 150 / 30 * (tm - 30);
	analogWrite(COOLER_PIN, speed);

	if (vlaj > 40)
	{
		digitalWrite(UROVEN_VCC_PIN, LOW);
		digitalWrite(RELAY_PIN, HIGH);
	}

	myOLED.printNumI(vlaj, ptx, pty1+36);
	//myOLED.drawRoundRect(ptx - 2, pty4 - 2, ptx2, pty4 + 9);

	myOLED.drawRoundRect(ptx - 2, pty - 2, ptx2, pty + 9);

    myOLED.update();
    scrLoop=millis();
    }

kr.process(mls);
bLeft.process();
bRight.process();
bOK.process();
}
