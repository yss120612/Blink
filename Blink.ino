//#include "Termometer.h"
//#include "Heater.h"
#include "Uroven.h"
#include "Discill.h"
#include "Suvid.h"
#include "Menu.h"
#include "Kran.h"
#include "YssBtn.h"
#include <EEPROM.h>
#include <OLED_I2C.h>

OLED  myOLED(SDA, SCL, 8);

extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
//extern uint8_t BigNumbers[];

const uint8_t BUZZER_PIN = 12;
const uint8_t COOLER_PIN = 3;
const uint8_t RELAY_PIN = 9;
const uint8_t TERMISTOR_PIN = A0;
const uint8_t UROVEN_PIN = A3;
const uint8_t UROVEN_VCC_PIN = 11;

const uint8_t WATER_OPEN_PIN = 5;
const uint8_t WATER_CLOSE_PIN = 6;
const uint8_t WATER_MEASURE_PIN = A2;
const uint8_t LEFT_BTN_PIN = 4;
const uint8_t RIGHT_BTN_PIN = 8;
const uint8_t CENTER_BTN_PIN = 7;

const char * names [] = {"Solod","Braga","Rectify","Setup","Open","Close","Meajure","SubSub121","SubSub122","SubSub123","Sub21","Sub22","Sub23","Sub24"};

//float ft;



uint16_t scrLoop = 0;

//const uint16_t ptx = 3;
//const uint16_t ptx2 = 128 - 3;

//const uint16_t pty1 = 18;
//const uint16_t pty2 = 30;
//const uint16_t pty3 = 42;
//const uint16_t pty4 = 54;
//uint16_t pty = 0;

//int period = 0;
//int curr = 0;
//const int sdOn = 2000;
//const int sdOff = 3000;
//boolean isOn;
YssBtn bLeft, bRight, bOK;
Kran kran;
Uroven ur;
Termometer trm;
Heater heater;
Suvid suvid(&heater,&trm);


YsMenuComponent * menu;
//YsMenuParameter *par;

YsMenu menu0(1);
YsMenu menu1(2);
YsMenu menu2(3);
YsMenu menu3(4);



//YsMenuItem  mi11(4, names[4]);
//YsMenuItem  mi12(5, names[5]);
//YsMenuItem  mi13(6, names[6]);
//
//YsMenuItem mi121(11, names[7]);
//YsMenuItem mi122(12, names[8]);
//YsMenuItem mi123(13, names[9]);
//
//YsMenuItem  mi21(7, names[10]);
//YsMenuItem  mi22(8, names[11]);
//YsMenuItem  mi23(9, names[12]);
//YsMenuItem  mi24(10, names[13]);

//miXY menu items X level 0.1.2....; Y 0.1.2.... parent item number
YsMenuItem  mi00[] = { YsMenuItem(0, names[0]), YsMenuItem(1, names[1]), YsMenuItem(2, names[2]),YsMenuItem(3, names[3]) };
YsMenuItem  mi10[] = { YsMenuItem(4, names[4]), YsMenuItem(5, names[5]),YsMenuItem(6, names[6]) };
YsMenuItem  mi21[] = { YsMenuItem(7, names[7]), YsMenuItem(8, names[8]),YsMenuItem(9, names[9]) };
YsMenuItem  mi11[] = { YsMenuItem(10, names[10]),YsMenuItem(11, names[11]),YsMenuItem(12, names[12]) ,YsMenuItem(13, names[13]) };

YsMenuParameterT pt(1,"Boolean");
YsMenuParameterF pf(2, "Float");
//YsMenuParameterUI8 pui(3, "Integer");

void initMenu() {
	//ft = 0;
	mi00[3].setSelectFunc(onMenuSelect);
	mi21[1].setSelectFunc(onMenuSelect);

	mi10[0].setSelectFunc(onKOpen);
	mi10[1].setSelectFunc(onKClose);
	mi10[2].setSelectFunc(onKMeajure);

	menu0.setItems(mi00, 4);
	
	menu1.setItems(mi10, 3);

	menu2.setItems(mi11, 4);

	menu3.setItems(mi21, 3);

	mi00[0].setSubMenu(& menu1);
	mi00[2].setSubMenu(&pt);
	mi00[3].setSubMenu(&pf);

	//mi10[1].setSubMenu(& menu3);
	//mi10[0].setSubMenu(&pui);

	mi00[1].setSubMenu(& menu2);

	menu1.setParent(& menu0);
	menu2.setParent(& menu0);
	menu3.setParent(& menu1);

	pt.setParent(& menu0);
	pf.setParent(&menu0);
	//pui.setParent(&menu1);

	menu = NULL;// &menu0;
	
}

void setup() {
	

  pinMode(LED_BUILTIN, OUTPUT);
  
  

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(COOLER_PIN, OUTPUT);
  
  
  

  digitalWrite(LED_BUILTIN, HIGH);   
  digitalWrite(UROVEN_VCC_PIN, HIGH);
  digitalWrite(UROVEN_PIN, HIGH);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(COOLER_PIN, LOW);
  
  Serial.begin(57600); 
  
  kran.setup(WATER_CLOSE_PIN, WATER_OPEN_PIN, WATER_MEASURE_PIN, RELAY_PIN);
  ur.setup(UROVEN_PIN, UROVEN_VCC_PIN);
  trm.setup(TERMISTOR_PIN);


  initMenu();


  bLeft.init(LEFT_BTN_PIN);
  bRight.init(RIGHT_BTN_PIN);
  bOK.init(CENTER_BTN_PIN);

  bOK.initBeep(12,1000,30);
  bOK.setClickFunc(onCClick);
  bOK.setClickLongFunc(onOKLong);



  bLeft.setClickFunc(onLClick);
  bRight.setClickFunc(onRClick);
  
  
  //pui.setup(10,50,10,2);
  pf.setup(29.9, 50, 10, 0.2);
  myOLED.begin();
  myOLED.setFont(SmallFont);
}

void onLClick(){
 // digitalWrite(LED_BUILTIN,LOW ); 
  menu->prev();
  scrLoop = 0;
 // if (kran.isOpened()) {
	//  kr.close();
  //}
 // else {
	//  kr.open();
//  }
 
}

void onRClick(){
  //digitalWrite(LED_BUILTIN,HIGH);   
  menu->next();
  scrLoop = 0;
 }

void onOKLong() {
	if (menu == NULL) {
		menu = &menu0;
		scrLoop = 0;
		return;
	}
	if (menu->haveParent()) {
		menu = menu->get_parent();
		scrLoop = 0;
	}
	else {
		menu = NULL;
	}

}

void selectMenu() {
	//YsMenuItem * ymi = menu->open();
	//if (ymi != NULL) {
	YsMenuComponent * m = menu->open();
		if (m != NULL) {
		//	//Serial.println(m->iAmIs());
			menu = m;
			if (menu->iAmIs() >= 3 && menu->iAmIs()<=6) ((YsMenuParameter *)menu)->begin_edit();
		}
		scrLoop = 0;
	//}
	
}

void onCClick(){

  selectMenu();
 // scrLoop = 0;
}

void onKClose() {

//	kran.close();
	kran.shiftQuantum(-2);
}

void onKOpen() {

	//kran.open();
	kran.shiftQuantum(2);
}

boolean kranState;

void onKMeajure() {

	kranState=kran.measureState();
	//kran.openQuantum(17);
	//kran.openQuantum(87);
	kran.openQuantum(17);
}



void onMenuSelect() {
	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

}

//double tTermoRes(int pin) {
//	int i = analogRead(pin);
//	//сопротивление термопары
//	double Temp = TERMISTOR_SECOND_RESISTOR / (1023.0 / i - 1.0);
//	Temp = log(Temp / 10000.0) / TERMISTOR_B + 1.0 / (25.0 + 273.15);
//	Temp = 1.0 / Temp;
//	Temp = Temp - 273.15;   // Kelvin to Celcius
//	return Temp;
//}

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

	float tm = trm.get_temperature();
	boolean vlaj = ur.isActive()?ur.process():false;
	uint8_t speed = tm > 60 ? 255 : tm < pf.get() ? 0 : 105 + 150 / pf.get() * (tm - pf.get());
	analogWrite(COOLER_PIN, speed);

	//pty = pty1 + 12 * MenuSelected;
	
    myOLED.clrScr();
	myOLED.setFont(MediumNumbers);
	myOLED.printNumF(tm,1, LEFT, 0);
	//myOLED.printNumI(vlaj, RIGHT,0);
	myOLED.printNumI(kranState, RIGHT, 0);
	myOLED.setFont(SmallFont);

	if (menu != NULL) menu->draw(&myOLED);
	
    myOLED.update();
    scrLoop=millis();
    }

kran.process(mls);
bLeft.process();
bRight.process();
bOK.process();
trm.processTermometr();
}
