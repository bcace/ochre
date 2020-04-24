#include "pine/widgets/menu.h"
#include "pine/fonts/font.h"
#include "pine/color.h"
#include "pine/platform.h"

using namespace pine;



Menu::Menu(Widget *_parent) : Popup(_parent),
	selItem(-1) {}

int Menu::SelectedItemId() const {
	return selItem;
}

int Menu::Show(const Point &pin) {

	Pinned(pin.x, pin.y, width, height);
	selItem = -1;

	return Exec();
}

void Menu::AddItem(const std::string &_name, int _id, bool _separator) {

	items.push_back(MenuItem(_id, _name, _separator));
	UpdateSize();
}

void Menu::SetItemEnabled(int _id, bool _enabled) {

	for (MenuItems::iterator i = items.begin(); i != items.end(); ++i)
		if (i->id == _id) {
			i->enabled = _enabled;
			return;
		}
}

void Menu::Paint() {

	RETURN_IF_INVISIBLE

	Schema::ApplyColor(scoFrgr);
	DrawRect(frame.min.x, frame.min.y, frame.max.x, frame.max.y);
	Widget::Paint();

	CiFont &font = CiFont::Small().Bind();

	double b = (MENU_ITEM_S - font.H()) * 0.5, y = frame.max.y - MENU_ITEM_S;

	for (MenuItems::iterator i = items.begin(); i != items.end(); ++i, y -= MENU_ITEM_S) {

		if (i->id == selItem) {

			Schema::ApplyColor(scoSelectionBkgr);

			glBegin(GL_QUADS);
			glVertex2d(frame.min.x, y);
			glVertex2d(frame.max.x, y);
			glVertex2d(frame.max.x, y + MENU_ITEM_S);
			glVertex2d(frame.min.x, y + MENU_ITEM_S);
			glEnd();
		}

		Schema::ApplyColor(scoBkgr);
		font.DrawText(i->name, frame.min.x + MENU_ITEM_S, y + b);

		if (i->separator) {

			Schema::ApplyColor(scoBkgr, 25);

			glBegin(GL_LINES);
			glVertex2d(frame.min.x, y);
			glVertex2d(frame.max.x, y);
			glEnd();
		}
	}
}

void Menu::UpdateSize() {

	width = 160.0;
	height = 0.0;

	CiFont &font = CiFont::Small();

	for (MenuItems::iterator i = items.begin(); i != items.end(); ++i) {

		double w = i->name.size() * font.W() + MENU_ITEM_S * 2.0;
		if (w > width)
			width = w;

		height += MENU_ITEM_S;
	}
}
