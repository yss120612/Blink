

#include "Menu.h"
const uint8_t DRAW_X0=3;
const uint8_t DRAW_X1 = 128-3;
const uint8_t DRAW_Y0=18;
const uint8_t DRAW_DELTA_Y=12;//пункты меню друг от друга
const uint8_t DRAW_LETTER_X = 6;//ДЛИНА буквы по Х
const uint8_t DRAW_LETTER_Y = 6;//ДЛИНА буквы по Х

const uint8_t DRAW_FRAME_X = 2;
const uint8_t DRAW_FRAME_Y0 = 2;
const uint8_t DRAW_FRAME_Y1 = 9;

const uint8_t MAX_POS = 4;// помещаетса максимально на экране

#pragma region Menu Item

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

#pragma endregion

#pragma region Menu Component
///////////////////////////////////////
///////   Menu Component
//////////////////////////////////////

void YsMenuComponent::setParent(YsMenuComponent * par) {
	parent = par;
};

const YsMenuComponent * YsMenuComponent::get_parent() {
	return parent;
}
#pragma endregion

#pragma region Menu Parametr

///////////////////////////////////////
///////   Menu Parametr
//////////////////////////////////////

void YsMenuParameter::set_name(char * nm) {
	name = nm;
}
const char * YsMenuParameter::get_name() {
	return name;
}

YsMenuComponent * YsMenuParameter::open() {
	apply_value();

	//if (&items[current].select())
	return get_parent();
};
#pragma endregion

#pragma region Boolean Parametr
///////////////////////////////////////
///////   Menu ParametrB
//////////////////////////////////////

void YsMenuParameterB::draw(OLED * scr) {
	scr->print(name, DRAW_X0, DRAW_Y0 + DRAW_DELTA_Y);
	if (get_value())
	{
		scr->print(" : YES", DRAW_X0+DRAW_LETTER_X * (strlen(name)+1), DRAW_Y0 + DRAW_DELTA_Y);
	}
	else {
		scr->print(" : NO", DRAW_X0 + DRAW_LETTER_X * (strlen(name)+1), DRAW_Y0 + DRAW_DELTA_Y);
	}
}

void YsMenuParameterB::next() {
	set_value(!get_value());
	//return &items[current];
};

void YsMenuParameterB::prev() {
	set_value(!get_value());
};

#pragma endregion

#pragma region UINT Parametr

///////////////////////////////////////
///////   Menu Parametr UINT-8
//////////////////////////////////////


void YsMenuParameterUI8::setup(uint8_t val, uint8_t up, uint8_t down, uint8_t stp) {
	up_limit = up;
	down_limit = down;
	value = val;
	curr_value = val;
	step = stp;
}

void YsMenuParameterUI8::next() {
	if (curr_value + step <= up_limit)
		curr_value += step;
};

void YsMenuParameterUI8::prev() {
	if (curr_value - step >= down_limit)
		curr_value -= step;
};

void YsMenuParameterUI8::draw(OLED * scr) {
	scr->print(name, DRAW_X0, DRAW_Y0 + DRAW_DELTA_Y);
	scr->print(" : ", DRAW_X0 + DRAW_LETTER_X * strlen(name), DRAW_Y0 + DRAW_DELTA_Y);
	scr->printNumI(curr_value, DRAW_X0 + DRAW_LETTER_X * (strlen(name) + 3), DRAW_Y0 + DRAW_DELTA_Y);
	
}
#pragma endregion

#pragma region Float Parametr



///////////////////////////////////////
///////   Menu Parametr FLOAT
//////////////////////////////////////


void YsMenuParameterF::setup(float val, float up, float down, float stp) {
	up_limit = up;
	down_limit = down;
	value = val;
	curr_value = val;
	step = stp;
}

void YsMenuParameterF::next() {
	if (curr_value + step <= up_limit)
		curr_value += step;
};

void YsMenuParameterF::prev() {
	if (curr_value - step>=down_limit)
		curr_value -= step;
};

void YsMenuParameterF::draw(OLED * scr) {
	scr->print(name, DRAW_X0, DRAW_Y0 + DRAW_DELTA_Y);
	scr->print(" : ", DRAW_X0 + DRAW_LETTER_X * strlen(name), DRAW_Y0 + DRAW_DELTA_Y);
	scr->printNumF(curr_value,1, DRAW_X0 + DRAW_LETTER_X * (strlen(name) + 3), DRAW_Y0 + DRAW_DELTA_Y);
}

#pragma endregion

#pragma region Time Parametr
///////////////////////////////////////
///////   Menu Parametr TIME
//////////////////////////////////////


void YsMenuParameterT::setup(uint8_t  ho, uint8_t  mi, uint8_t  se, uint8_t  mo){
	set_csec(se);
	set_cmin(mi);
	set_chour(ho);
	edit_mode = mo;
	apply_value();
	edit_mode = 0;
}

void YsMenuParameterT::next() {
	switch (edit_mode)
	{
	case 0:
		set_csec(curr_sec + 1);
		break;
	case 1:
		set_cmin(curr_min + 1);
		break;
	case 2:
		set_chour(curr_hour + 1);
		break;
	default:
		break;
	}
};

void YsMenuParameterT::prev() {
	switch (edit_mode)
	{
	case 0:
		set_csec(curr_sec - 1);
		break;
	case 1:
		set_cmin(curr_min - 1);
		break;
	case 2:
		set_chour(curr_hour - 1);
		break;
	default:
		break;
	}
};

YsMenuComponent * YsMenuParameterT::open() {
	edit_mode++;
	if (edit_mode >= 3)
	{
		apply_value();
		edit_mode = 0;
		return parent;
	}
	else
	{
		
		return this;
	}
};

void YsMenuParameterT::draw(OLED * scr) {
	
	scr->print(name, DRAW_X0, DRAW_Y0 + DRAW_DELTA_Y);
	uint8_t sl = strlen(name);
	scr->print(" ", DRAW_X0 + DRAW_LETTER_X * sl, DRAW_Y0 + DRAW_DELTA_Y);
	
	scr->printNumI(curr_hour, DRAW_X0 + DRAW_LETTER_X * (sl + 1), DRAW_Y0 + DRAW_DELTA_Y, 2, '0');
	
	scr->print(":", DRAW_X0 + +DRAW_LETTER_X * (sl + 3), DRAW_Y0 + DRAW_DELTA_Y);
	
	scr->printNumI(curr_min, DRAW_X0 + DRAW_LETTER_X * (sl + 4), DRAW_Y0 + DRAW_DELTA_Y,2,'0');
	
	scr->print(":", DRAW_X0 + +DRAW_LETTER_X * (sl + 6), DRAW_Y0 + DRAW_DELTA_Y);
	
	scr->printNumI(curr_sec,  DRAW_X0 + DRAW_LETTER_X * (sl + 7), DRAW_Y0 + DRAW_DELTA_Y, 2, '0');
	


	scr->drawCircle(DRAW_X0 + DRAW_LETTER_X * ((sl + 1) + (2-edit_mode) * 3) + DRAW_LETTER_X,
		DRAW_Y0 + DRAW_DELTA_Y + DRAW_LETTER_Y/2,
		8
	);

	//scr->drawRoundRect(DRAW_X0 + DRAW_LETTER_X *(sl+3*edit_mode) - DRAW_FRAME_X,
	//				   DRAW_Y0 + DRAW_DELTA_Y - DRAW_FRAME_Y0, 
	//				   DRAW_X0 + DRAW_LETTER_X * (sl + 3 * edit_mode +2 ) + DRAW_FRAME_X,
	//				   DRAW_Y0 + DRAW_DELTA_Y + DRAW_FRAME_Y1);
}
#pragma endregion

#pragma region Menu
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

#pragma endregion