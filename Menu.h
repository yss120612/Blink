// Menu.h



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
	YsMenu * subMenu;
	void (*selectFunc)();
	

public:
	YsMenuItem(int i, char * nm) {
		subMenu = NULL;
		name = nm;
		id = i;
	};

	YsMenu *  select();

	void setSelectFunc(void(*func) ()) { selectFunc = func; }

	void setSubMenu(YsMenu * m);



	~YsMenuItem() {
		//if (subMenu!=NULL) delete subMenu;
	}

	const char* get_name() const;

	
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

	const YsMenuItem * get_items() const;
	const YsMenu * get_parent() const;

	boolean haveParent() { return parent != NULL; }

	void draw(OLED * scr);

};