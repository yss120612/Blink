
//#include <RTClib.h>


#include <EEPROM.h>
//#include <OLED_I2C.h>
#include <OneWire.h>
#include <U8glib.h>

#include "Clock.h"
#include "Beeper.h"
#include "DallasTerm.h"
#include "Uroven.h"
#include "Termometer.h"
#include "Menu.h"
#include "YssBtn.h"
#include "Kran.h"
#include "Heater.h"


#include "Discill.h"
#include "Suvid.h"






//extern uint8_t SmallFont[];
//extern uint8_t MediumNumbers[];
//extern uint8_t BigNumbers[];


const uint8_t COOLER_PIN = 3;
const uint8_t LEFT_BTN_PIN = 4;
const uint8_t WATER_OPEN_PIN = 5;
const uint8_t WATER_CLOSE_PIN = 6;
const uint8_t CENTER_BTN_PIN = 7;
const uint8_t RIGHT_BTN_PIN = 8;
const uint8_t RELAY_PIN = 9;
const uint8_t HEATER_PIN = 9;
const uint8_t TEMPERATURE_PIN = 10;
const uint8_t UROVEN_VCC_PIN = 11;
const uint8_t BUZZER_PIN = 12;

const uint8_t CLOCK_RST_PIN = 3;
const uint8_t CLOCK_DAT_PIN = 5;
const uint8_t CLOCK_CLK_PIN = 6;

const uint8_t TERMISTOR_PIN = A0;
const uint8_t WATER_MEASURE_PIN = A2;
const uint8_t UROVEN_PIN = A3;

const char * names [] = {"Sd","Ba","Reify","Setup","Strt","Pae","Clo","Meajure","SS121","SS122","SS123","Sub21","Sub22","Sub23","Sub24"};

volatile int kranStat;

long scrLoop = 0;


U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);

OneWire ds(TEMPERATURE_PIN);
uint8_t  tkube[] = { 0x28,0xFF,0x73,0x37,0x67,0x14,0x02,0x11 };

//OLED  myOLED(SDA, SCL, 8);
//OLED  myOLED(SDA, SCL);
YssBtn bLeft, bRight, bOK;
Kran kran;
Uroven ur;
DallasTerm trm(tkube,&ds,2.5);
Beeper beeper;

Heater heater;
Suvid suvid(&heater,&trm,&beeper);
//DS1302 clock(CLOCK_RST_PIN, CLOCK_DAT_PIN, CLOCK_CLK_PIN);

//iarduino_RTC clock(RTC_DS1302, CLOCK_RST_PIN, CLOCK_DAT_PIN, CLOCK_CLK_PIN);
Clock rtc;
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
	//clock.begin();
	
}

void setup() {
	

  pinMode(LED_BUILTIN, OUTPUT);
  
  

  pinMode(RELAY_PIN, OUTPUT);
  //pinMode(HEATER_PIN, OUTPUT);
  pinMode(COOLER_PIN, OUTPUT);
  
  
  

  digitalWrite(LED_BUILTIN, HIGH);   
  digitalWrite(UROVEN_VCC_PIN, HIGH);
  digitalWrite(UROVEN_PIN, HIGH);
  //digitalWrite(RELAY_PIN, LOW);
  digitalWrite(COOLER_PIN, LOW);
  
  Serial.begin(57600); 
  
  kran.setup(WATER_CLOSE_PIN, WATER_OPEN_PIN, WATER_MEASURE_PIN, RELAY_PIN);
  ur.setup(UROVEN_PIN, UROVEN_VCC_PIN);
  //trm.setup(TERMISTOR_PIN);
  

  initMenu();
  beeper.setup(BUZZER_PIN);

  bLeft.init(LEFT_BTN_PIN);
  bRight.init(RIGHT_BTN_PIN);
  bOK.init(CENTER_BTN_PIN);

  //bOK.initBeep(12,1000,30);
  bOK.initBeep(&beeper);
  bOK.setClickFunc(onCClick);
  bOK.setClickLongFunc(onOKLong);



  bLeft.setClickFunc(onLClick);
  bRight.setClickFunc(onRClick);
  
  
  //pui.setup(10,50,10,2);
  pf.setup(29.9, 50, 10, 0.2);
  //myOLED.begin();
  //myOLED.setFont(SmallFont);
  heater.setup(HEATER_PIN, -1);
 // detachInterrupt(0);
  attachInterrupt(0, onheaterProcess, RISING);

  interrupts();
  trm.set12bit();
  rtc.setup(CLOCK_RST_PIN, CLOCK_DAT_PIN, CLOCK_CLK_PIN);
  rtc.halt(false);
 // heater.start();
 /* Time t(2018, 5, 28, 23, 00, 00, Time::kSunday);

  rtc.writeProtect(false);
  rtc.halt(false);
  rtc.time(t);*/
  //heater.setPower(45);
  //suvid.start(60,1);
 // clock.begin();
  //u8g.setColorIndex(1);

  //u8g.setFont(u8g_font_10x20);
  //u8g.setFont(u8g_font_4x6);
  //u8g.setFont(u8g_font_5x7);
  //u8g.setFont(u8g_font_5x8);
  u8g.setFontPosTop();
  u8g.setFont(u8g_font_6x10);//norm
							 //u8g.setFont(u8g_font_6x12);
							 //u8g.setFont(u8g_font_6x13);
							 //u8g.setFont(u8g_font_7x13);
							 //u8g.setFont(u8g_font_7x14);
							 //u8g.setFont(u8g_font_8x13);
							 //u8g.setFont(u8g_font_);
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
	//heater.setPower(heater.getPower() - 10);
	//return;
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

//	heater.setPower(heater.getPower()+1);
  selectMenu();
  scrLoop = 0;
}

void onKClose() {

//	kran.close();
	kran.shiftQuantum(-2);
}

void onKOpen() {

	//kran.open();
	kran.shiftQuantum(2);
}



void onKMeajure() {

//	kranState=kran.measureState();
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

//boolean Bras(int power, int diap)
//{
//	bool res = false;
//	int err = diap / 2;
//	for (int j = 0; j < 10; j++)
//	{
//
//		for (int i = 0; i < diap; i++)
//		{
//			err -= power;
//			if (err < 0)
//			{
//				err += diap;
//				res = true;
//			}
//			else
//			{
//				res = false;
//			}
//
//		}
//	}
//	return res;
//}

//volatile byte cn[100];
//volatile int counter = 0;
void onheaterProcess() {
	//noInterrupts();
	kranStat++;
	//Serial.println(kranStat);
	//cn[counter]=
		heater.processHeater();
	//if (counter < 99) counter++;
	//else counter = 0;
	//interrupts();
}

boolean mejj=false;
void loop() {

   long mls=millis();
   uint8_t addr[8];
   float temperature = 0;
   if (scrLoop+1000-mls<0){
	   digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	   ////char buf[32];

	   ////// Print the RAM byte values as hex using the byte interface.
	   ////for (int i = 0; i < DS1302::kRamSize; ++i) {
		  //// snprintf(buf, sizeof(buf), "[%02X]", rtc.readRam(i));
		  //// Serial.print(buf);
	   ////}
	   ////Serial.println();
	   

  // Name the day of the week.
  //const String day = dayAsString(t.day);

  // Format the time and date and insert into the temporary buffer.
  

  // Print the formatted string to serial so we can see the time.
  //Serial.println(buf);
	   //Serial.println(clock.gettime("d-m-Y, H:i:s, D"));
	  /* Serial.print(clock.now().hour());
	   Serial.print(":");
	   Serial.print(clock.now().minute());
	   Serial.print(":");
	   Serial.println(clock.now().second());*/
		   //byte data[2]; // Место для значения температуры
	   //if (mejj) {
		  // ds.reset(); // Начинаем взаимодействие со сброса всех предыдущих команд и параметров
		  // while (!ds.search(addr)) {
			 //  for (int i = 0; i < 8; i++) {
				//   Serial.print(addr[i], HEX);
				//   Serial.print(":");
			 //  }
			 //  Serial.println("");
		  // }
		  // Serial.println("No mode sensors");
	//	   ds.reset();
	//	   //ds.search(addr);
	//	   //ds.select(addr);

	//	   ds.write(0xCC); // Даем датчику DS18b20 команду пропустить поиск по адресу. В нашем случае только одно устрйоство 
	//	   ds.write(0x44); // Даем датчику DS18b20 команду измерить температуру. Само значение температуры мы еще не получаем - датчик его положит во внутреннюю память
	//}
	//   else {
	//	   ds.reset(); // Теперь готовимся получить значение измеренной температуры
	//	   ds.write(0xCC);
	//	   ds.write(0xBE); // Просим передать нам значение регистров со значением температуры

	//					   // Получаем и считываем ответ
	//	   data[0] = ds.read(); // Читаем младший байт значения температуры
	//	   data[1] = ds.read(); // А теперь старший

	//							// Формируем итоговое значение: 
	//							//    - сперва "склеиваем" значение, 
	//							//    - затем умножаем его на коэффициент, соответсвующий разрешающей способности (для 12 бит по умолчанию - это 0,0625)
	//	   temperature = ((data[1] << 8) | data[0]) * 0.0625+2.4;
	   //}
	//   mejj = (!mejj);
	   

	   

  u8g.firstPage();
  do {
	  draw();
  } while (u8g.nextPage());


	   

	//float tm = trm.get_temperature();
	//boolean vlaj = ur.isActive()?ur.process():false;
	//uint8_t speed = tm > 60 ? 255 : tm < pf.get() ? 0 : 105 + 150 / pf.get() * (tm - pf.get());
	//analogWrite(COOLER_PIN, speed);

	//pty = pty1 + 12 * MenuSelected;
	
   // myOLED.clrScr();
	//myOLED.setFont(SmallFont);
	//myOLED.setFont(MediumNumbers);
	//if (temperature>1) myOLED.printNumF(temperature,1, LEFT, 0);
	//myOLED.printNumI(vlaj, RIGurrT,0);
//int x = 0;
	//for (int i = 0; i < 100; i++) x += cn[i];
	//myOLED.printNumI(suvid.getHeaterPower(),LEFT , 0);
	//myOLED.printNumF(trm.getTemp(),1, RIGHT, 0);
	//Serial.println(kranStat);
	//myOLED.print(buf,0, 0);
	//myOLED.setFont(SmallFont);

	//if (menu != NULL) menu->draw(&myOLED);
	
   // myOLED.update();
	
    scrLoop=millis();
    }
	beeper.process(mls);
	ur.process(mls);
	kran.process(mls);
	bLeft.process(mls);
	bRight.process(mls);
	bOK.process(mls);
	trm.process(mls);
	//suvid.process_suvid(mls);
//trm.processTermometr();
}


void draw(void) {
	
	char buf[20];
	
	// graphic commands to redraw the complete screen should be placed here  
	//u8g.setFont(u8g_font_unifont);
	//u8g.setFont(u8g_font_osb21);
	//u8g.setPrintPos(0,20);
	u8g.drawStr(0,10,rtc.get_time_str());
}