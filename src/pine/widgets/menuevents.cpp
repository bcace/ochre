#include "pine/widgets/menu.h"
#include "pine/events.h"
#include <math.h>

using namespace pine;



void Menu::MouseMoveEvent(MouseEvent &e) {

	int newItem = -1;
	if (frame.Contains(e.pos)) {
		unsigned idx = floor((frame.max.y - e.pos.y) / MENU_ITEM_S);
		if (idx < items.size())
			newItem = items[idx].id;
	}

	if (newItem != selItem) {
		selItem = newItem;
		Widget::paint = true;
	}
}

void Menu::MouseReleaseEvent(MouseEvent &e) {

	if (frame.Contains(e.pos) && e.button == mbLeft) {
		selItem = items[floor((frame.max.y - e.pos.y) / MENU_ITEM_S)].id;
		Exit(selItem);
	}
	else
		Popup::MouseReleaseEvent(e);
}
