

#include "Menu.h"
const uint8_t DRAW_X0=3;
const uint8_t DRAW_X1 = 128-3;
const uint8_t DRAW_Y0=18;
const uint8_t DRAW_DELTA_Y=12;//пункты меню друг от друга

const uint8_t DRAW_FRAME_X = 2;
const uint8_t DRAW_FRAME_Y0 = 2;
const uint8_t DRAW_FRAME_Y1 = 9;

const uint8_t MAX_POS = 4;// помещаетса максимально на экране


///////////////////////////////////////
///////   Menu Item
//////////////////////////////////////

const char * YsMenuItem::get_name() 
{
	return name;
}

YsMenuComponent * YsMenuItem::select() {
	//Serial.print(name);
	//Serial.println(" selected");
	//Serial.print("init submenu ");
	//Serial.println(subMenu->get_items()[0].get_name());

	
	
	if (subMenu != NULL ) {
		return subMenu;
	}
	
	if (selectFunc)
	{
		(*selectFunc)();
		//Serial.println("function");
		return NULL;
	}
	//Serial.println("all is null");
	return NULL;

}

void YsMenuItem::setSubMenu(YsMenuComponent * m) {
	subMenu = m;
}

void YsMenuItem::PrintState() {
	Serial.print("State for:");
	Serial.print(name);
	Serial.print(" id:");
	Serial.print(id);
	Serial.print(" function:");
	if (*selectFunc)
	{
		Serial.print("HAVE");
	}
	else {
		Serial.print("NULL");
	}

	Serial.print(" SubMenu:");
	if (subMenu)
	{
		Serial.println("HAVE");
	}
	else {
		Serial.println("NULL");
	}

}
///////////////////////////////////////
///////   Menu Component
//////////////////////////////////////

void YsMenuComponent::setParent(YsMenuComponent * par) {
	parent = par;
};

const YsMenuComponent * YsMenuComponent::get_parent() {
	return parent;
}

///////////////////////////////////////
///////   Menu Parametr
//////////////////////////////////////

void YsMenuParameter::set_name(char * nm) {
	name = nm;
}
const char * YsMenuParameter::get_name() {
	return name;
}


///////////////////////////////////////
///////   Menu ParametrB
//////////////////////////////////////

void YsMenuParameterB::draw(OLED * scr) {
	scr->print(get_name(), DRAW_X0, DRAW_Y0 + DRAW_DELTA_Y);
	if (get_value())
	{
		scr->print(" : YES", DRAW_X0+50, DRAW_Y0 + DRAW_DELTA_Y);
	}
	else {
		scr->print(" : NO", DRAW_X0 + 50, DRAW_Y0 + DRAW_DELTA_Y);
	}
}

void YsMenuParameterB::next() {
	set_value(!get_value());
	//return &items[current];
};

void YsMenuParameterB::prev() {
	set_value(!get_value());
};

YsMenuComponent * YsMenuParameterB::open() {
	apply_value();
		
	//if (&items[current].select())
	return get_parent();
};


///////////////////////////////////////
///////   Menu
//////////////////////////////////////

const YsMenuItem * YsMenu::get_items(){
	return items;
}

void YsMenu::setItems(YsMenuItem * mi, int ic) {
	items = mi;
	items_count = ic;
	if (mi == NULL) current = -1;
	else current = 0;
	
}

void YsMenu::next() {
	if (current < 0) return;
	
	if (current == items_count-1) {
		current = 0;
	}
	else {
		current++;
	}
	//return &items[current];
};

void YsMenu::prev() {
	if (current < 0) return;
	if (current>0) {
		current--;
	}
	else {
		current=items_count-1;
	}
	//return &items[current];
};

YsMenuComponent * YsMenu::open() {
	if (current < 0) return NULL;
	//if (&items[current].select())
	return items[current].select();
};


void YsMenu::draw(OLED * scr) {
	for (int i = 0; i < items_count; i++)
	{
		
		scr->print(items[i].get_name(), DRAW_X0, DRAW_Y0 + DRAW_DELTA_Y*i);
		if (i == current) {
			scr->drawRoundRect(DRAW_X0 - DRAW_FRAME_X, DRAW_Y0 + DRAW_DELTA_Y * i - DRAW_FRAME_Y0, DRAW_X1 + DRAW_FRAME_X, DRAW_Y0 + DRAW_DELTA_Y * i + DRAW_FRAME_Y1);
		}
	}
}

