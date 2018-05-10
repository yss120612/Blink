// Menu.h

#include <OLED_I2C.h>

#ifndef _MENU_h
#define _MENU_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class  YsMenuComponent;
class  YsMenu;

class YsMenuItem {
private:
	int id;
	char * name;
protected:
	YsMenuComponent * subMenu;
	void (*selectFunc)();

public:
	YsMenuItem(int i, const char * nm) {
		subMenu = NULL;
		name = nm;
		id = i;
	};



	YsMenuComponent *  select();

	void setSelectFunc(void(*func) ()) { selectFunc = func; }

	void setSubMenu(YsMenuComponent * m);

	int getId() { return id; };

	~YsMenuItem() {
		//if (subMenu!=NULL) delete subMenu;
	}

	const char* get_name();

	void set_name(char * nm) {
		name = nm;
	}

	void PrintState();
};

class YsMenuComponent {
protected:
	int id;
	YsMenuComponent * parent;
public:
	YsMenuComponent(int i) {
		id = i;
		parent = NULL;
	};
	virtual void next()=0;
	virtual void prev()=0;
	virtual YsMenuComponent * open()=0;
	const YsMenuComponent * get_parent();
	void setParent(YsMenuComponent * par);
	boolean haveParent() { return parent != NULL; }
	virtual void draw(OLED * scr)=0;
	virtual uint8_t iAmIs() { return 0; }
};

class YsMenu  : public YsMenuComponent {
private:
	
	YsMenuItem * items;
	int items_count;
	int current;

public:
	void setItems(YsMenuItem * mi, int ic);
	
	void next();
	void prev();
	YsMenuComponent * open();

	YsMenu(int i):YsMenuComponent(i){
	};

	const YsMenuItem * get_items();
		

	void draw(OLED * scr);
	uint8_t iAmIs() { return 1; }
};


class YsMenuParameter : public YsMenuComponent{
protected:
	char * name;
public:
	YsMenuParameter(int i, char * nm) :YsMenuComponent(i) {
		name = nm;
	};
	void set_name(char * nm);
	const char * get_name();
	uint8_t iAmIs() { return 2; }
	virtual void begin_edit()=0;
	virtual void apply_value()=0;
	YsMenuComponent * YsMenuParameter::open();
};

class YsMenuParameterB : public YsMenuParameter {
protected:
	boolean value;
	boolean curr_value;
public:
	YsMenuParameterB(int i, char * nm) :YsMenuParameter(i,nm) {
	};
	void next();
	void prev();
	void begin_edit() { curr_value = value; };
	void apply_value() { value = curr_value; };
	boolean get_value() { return curr_value; };
	void set_value(boolean b) { curr_value = b; };
	uint8_t iAmIs() { return 3; }
	void draw(OLED * scr);
};

class YsMenuParameterUI8 : public YsMenuParameter {
protected:
	uint8_t value;
	uint8_t curr_value;
	uint8_t up_limit;
	uint8_t down_limit;
	uint8_t step;
public:
	YsMenuParameterUI8(int i, char * nm) :YsMenuParameter(i, nm) {
		value = 0;
		curr_value = 0;
		up_limit = 100;
		down_limit = 0;
		step = 1;

	};
	void setup(uint8_t val, uint8_t up, uint8_t down, uint8_t stp);
	void next();
	void prev();
	void begin_edit() { curr_value = value; };
	void apply_value() { value = curr_value; };
	uint8_t get_value() { return curr_value; };
	void set_value(uint8_t b) { curr_value = b; };
	uint8_t iAmIs() { return 4; }
	void draw(OLED * scr);
};


class YsMenuParameterF : public YsMenuParameter {
protected:
	float value;
	float curr_value;
	float up_limit;
	float down_limit;
	float step;
public:
	YsMenuParameterF(int i, char * nm) :YsMenuParameter(i, nm) {
		value = 50.0;
		curr_value = 50.0;
		up_limit = 100.0;
		down_limit = 0.0;
		step = 0.1;

	};
	void setup(float  val, float  up, float  down, float stp);
	void next();
	void prev();
	void begin_edit() { curr_value = value; };
	void apply_value() { value = curr_value; };
	float  get_value() { return curr_value; };
	void set_value(float  b) { curr_value = b; };
	uint8_t iAmIs() { return 5; }
	void draw(OLED * scr);
};
