#include "Menu.h"
#include "Kran.h"
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

char * names[] = {"Main1","Main2","Main3","Main4","Sub11","Sub12","Sub13","SubSub121","SubSub122","SubSub123","Sub21","Sub22","Sub23","Sub24"};




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
Kran kran(WATER_CLOSE_PIN, WATER_OPEN_PIN, WATER_MEASURE_PIN,RELAY_PIN);
int MenuSelected = 0;

YsMenu * menu;

YsMenuItem mi1(0, names[0]);
YsMenuItem mi2(1, names[1]);
YsMenuItem mi3(2, names[2]);
YsMenuItem mi4(3, names[3]);


YsMenuItem  mi11(4, names[4]);
YsMenuItem  mi12(5, names[5]);
YsMenuItem  mi13(6, names[6]);

YsMenuItem mi121(11, names[4]);
YsMenuItem mi122(12, names[5]);
YsMenuItem mi123(13, names[6]);

YsMenuItem  mi21(7, names[4]);
YsMenuItem  mi22(8, names[5]);
YsMenuItem  mi23(9, names[6]);
YsMenuItem  mi24(10, names[6]);

YsMenu menu0(1);
YsMenu menu1(2);
YsMenu menu2(3);
YsMenu menu3(4);

void initMenu() {
	
	YsMenuItem mm0i[] = { mi1, mi2, mi3, mi4 };
	YsMenuItem  mm1i[] = { mi11, mi12,mi13 };
	YsMenuItem  mm12i[] = { mi121, mi122,mi123 };
	YsMenuItem  mm2i[] = { mi21, mi22,mi23 ,mi24 };

	mi4.setSelectFunc(onMenuSelect);

	menu0.setItems(mm0i, 4);
	
	menu1.setItems(mm1i, 3);

	menu2.setItems(mm2i, 4);

	menu3.setItems(mm12i, 3);

	mi1.setSubMenu(& menu1);
	mi12.setSubMenu(& menu3);
	mi2.setSubMenu(& menu2);

	menu1.setParent(& menu0);
	menu2.setParent(& menu0);
	menu3.setParent(& menu1);

	menu = &menu0;
	
}

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
  
  kran.setup();
  initMenu();


  bLeft.init(LEFT_BTN_PIN);
  bRight.init(RIGHT_BTN_PIN);
  bOK.init(CENTER_BTN_PIN);

  bOK.initBeep(12,1000,30);
  bOK.setClickFunc(onCClick);
  bOK.setClickLongFunc(onOKLong);



  bLeft.setClickFunc(onLClick);
  bRight.setClickFunc(onRClick);
  
  
  myOLED.begin();
  myOLED.setFont(SmallFont);
}

void onLClick(){
  digitalWrite(LED_BUILTIN,LOW ); 
  menu->prev();
  if (kran.isOpened()) {
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
  menu->next();
 // Serial.print("State=");
 // Serial.println(kr.measureState());
  if (MenuSelected <3) {
	  MenuSelected++;
	  scrLoop = 0;
  }
}

void onOKLong() {
	if (menu->haveParent()) {
		menu = menu->get_parent();
	}
}

void selectMenu() {
	YsMenuItem * ymi = menu->open();
	if (ymi != NULL) {
		YsMenu * m = ymi->select();
		if (m != NULL) {
			menu = m;
		}
		scrLoop = 0;
	}
	
}

void onCClick(){
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN)   );   
  selectMenu();
}

void onMenuSelect() {
	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
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

	double tm = tTermoRes(TERMISTOR_PIN);
	int vlaj = analogRead(UROVEN_PIN);
	uint8_t speed = tm > 60 ? 255 : tm < 30 ? 0 : 105 + 150 / 30 * (tm - 30);
	analogWrite(COOLER_PIN, speed);

	//pty = pty1 + 12 * MenuSelected;
	
    myOLED.clrScr();
	myOLED.setFont(MediumNumbers);
	myOLED.printNumF(tm,1, LEFT, 0);
	myOLED.printNumI(vlaj, RIGHT,0);
	myOLED.setFont(SmallFont);

	menu->draw(&myOLED);
	//myOLED.printNumI(mls, ptx, pty1);
	

    //myOLED.print("Braga", ptx, pty1+12);
	//myOLED.drawRoundRect(ptx-2, pty2-2, ptx2, pty2 + 9);

	//myOLED.print("Menu item", ptx, pty2);
	//myOLED.drawRoundRect(ptx - 2, pty3 - 2, ptx2, pty3 + 9);

	

	

	//myOLED.printNumF(tm,1, ptx, pty1+24);
	///Serial.println(Temp);
	//myOLED.drawRoundRect(ptx - 2, pty3 - 2, ptx2, pty3 + 9);
	
	
	

	if (vlaj > 40)
	{
		digitalWrite(UROVEN_VCC_PIN, LOW);
		digitalWrite(RELAY_PIN, HIGH);
	}

	//myOLED.printNumI(vlaj, ptx, pty1+36);
	//myOLED.drawRoundRect(ptx - 2, pty4 - 2, ptx2, pty4 + 9);

	//myOLED.drawRoundRect(ptx - 2, pty - 2, ptx2, pty + 9);

    myOLED.update();
    scrLoop=millis();
    }

kran.process(mls);
bLeft.process();
bRight.process();
bOK.process();
}
