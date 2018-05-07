

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

const char* YsMenuItem::get_name() const {
	return name;
}

YsMenu * YsMenuItem::select() {
	if (subMenu != NULL) {
		return subMenu;
	}
	if (selectFunc != NULL) {
		selectFunc();
	}
	return NULL;

}

void YsMenuItem::setSubMenu(YsMenu * m) {
	subMenu = m;
}

///////////////////////////////////////
///////   Menu
//////////////////////////////////////

const YsMenuItem * YsMenu::get_items() const{
	return items;
}

void YsMenu::setItems(YsMenuItem * mi, int ic) {
	items = mi;
	items_count = ic;
	if (mi == NULL) current = -1;
	else current = 0;
	for (int i = 0; i < items_count; i++) {

	}
}

YsMenuItem * YsMenu::next() {
	if (current < 0) return NULL;
	if (current >= items_count) {
		current = 0;
	}
	else {
		current++;
	}
	return &items[current];
};

YsMenuItem * YsMenu::prev() {
	if (current < 0) return NULL;
	if (current>0) {
		current--;
	}
	else {
		current=items_count-1;
	}
	return &items[current];
};

YsMenuItem * YsMenu::open() {
	if (current < 0) return NULL;
	return &items[current];
};

void YsMenu::setParent(YsMenu * par) {
	parent = par;
};

const YsMenu * YsMenu::get_parent() const
{
	return parent;
}

void YsMenu::draw(OLED * scr) {
	for (int i = 0; i < items_count; i++)
	{
		scr->print(items[i].get_name, DRAW_X0, DRAW_Y0 + DRAW_DELTA_Y*i);
		if (i == current) {
			scr->drawRoundRect(DRAW_X0 - DRAW_FRAME_X, DRAW_Y0 + DRAW_DELTA_Y * i - DRAW_FRAME_Y0, DRAW_X0 - DRAW_FRAME_X, DRAW_Y0 + DRAW_DELTA_Y * i + DRAW_FRAME_Y1);
		}
	}
}