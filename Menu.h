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

class YsMenu;

class YsMenuItem {
private:
	int id;
	char * name;
protected:
	YsMenu * subMenu;
	void (*selectFunc)();

public:
	YsMenuItem(int i, const char * nm) {
		subMenu = NULL;
		name = nm;
		id = i;
	};



	YsMenu *  select();

	void setSelectFunc(void(*func) ()) { selectFunc = func; }

	void setSubMenu(const YsMenu * const m);

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

class YsMenu {
private:
	int id;
	YsMenuItem * items;
	int items_count;
	int current;
	YsMenu * parent;

public:
	void setItems(YsMenuItem * mi, int ic);

	void setParent(YsMenu * par);

	YsMenuItem * next();
	YsMenuItem * prev();
	YsMenuItem * open();

	YsMenu(int i) {
		id = i;
		parent = NULL;
	};

	const YsMenuItem * get_items();
	const YsMenu * get_parent();

	boolean haveParent() { return parent != NULL; }

	void draw(OLED * scr);

};