#include "pine/widgets/list.h"
#include "pine/fonts/font.h"
#include <math.h>

using namespace pine;



List::List(Widget *p, bool _multiselect, ItemStyle _style) : Control(p),
	multiselect(_multiselect), permaselect(false), chooseOnClick(false),
	style(_style),
	scroll(this),
	itemHeight(CiMax(CiFont::Small().H(), CiFont::SmallIcons().H()) + LIST_ITEM_PADDING * 2.0) {}

List::List(Widget *p, bool _multiselect, bool _permaselect, ItemStyle _style) : Control(p),
	multiselect(_multiselect), permaselect(_permaselect), chooseOnClick(!_multiselect),
	style(_style),
	scroll(this),
	itemHeight(CiMax(CiFont::Small().H(), CiFont::SmallIcons().H()) + LIST_ITEM_PADDING * 2.0) {}

void List::Clear() {
	items.clear();
	scroll.SetContent(0.0);
}

void List::Add(const ListItem &item) {

	if (style & (Group | Alphabetize)) {

		ListItems::iterator i = items.begin();
		for (; i != items.end() && item > *i; ++i);

		items.insert(i, item);
	}
	else
		items.push_back(item);

	// if list is permaselect, and no item is selected, select the added item

	if (permaselect) {
		ClearPressed();
		items.back().state = litPressed;
	}

	scroll.SetContent(items.size() * itemHeight);
}

void List::RemoveSelected() {

	ListItems::iterator i = items.begin(), l = items.end();
	while (i != items.end()) {

		if (i->state == litPressed)
			l = i = items.erase(i);
		else
			++i;
	}

	if (permaselect) {

		if (l != items.end())
			l->state = litPressed;
		else if (items.size())
			items.front().state = litPressed;
	}

	scroll.SetContent(items.size() * itemHeight);
}

bool List::SelectNext() {

	if (items.size() == 0)
		return false;

	ListItems::iterator s = items.end();
	for (ListItems::iterator i = items.begin(); i != items.end(); ++i)
		if (i->state == litPressed)
			s = i;

	if (s == items.end())
		items.front().state = litPressed;
	else {

		ClearPressed();

		if (++s == items.end())
			items.front().state = litPressed;
		else
			s->state = litPressed;
	}

	AutoScroll();

	return true;
}

bool List::SelectPrevious() {

	if (items.size() == 0)
		return false;

	ListItems::reverse_iterator s = items.rend();
	for (ListItems::reverse_iterator i = items.rbegin(); i != items.rend(); ++i)
		if (i->state == litPressed)
			s = i;

	if (s == items.rend())
		items.back().state = litPressed;
	else {

		ClearPressed();

		if (++s == items.rend())
			items.back().state = litPressed;
		else
			s->state = litPressed;
	}

	AutoScroll();

	return true;
}

Tags List::SelectedItems(int group) const {

	Tags selItems;

	for (ListItems::const_iterator i = items.begin(); i != items.end(); ++i)
		if ((i->state == litPressed) && (i->group & group))
			selItems.push_back(i->tag);

	return selItems;
}

int List::SelectedIndex() const {

	for (int i = 0; i < items.size(); ++i)
		if (items[i].state == litPressed)
			return i;

	return -1;
}

void List::SelectByIndex(int index) {

	if (index < 0 || index >= (int)items.size())
		return;

	for (int i = 0; i < items.size(); ++i)
		items[i].state = (i == index) ? litPressed : litNormal;
}

unsigned int List::Count() const {
	return items.size();
}

double List::ItemHeight() const {
	return itemHeight;
}

void List::AutoScroll() {

	int i = SelectedIndex();
	if (i != -1) {

		if (i * itemHeight < scroll.Val())
			scroll.SetVal(i * itemHeight);
		else if ((i + 1) * itemHeight - frame.H() > scroll.Val())
			scroll.SetVal((i + 1) * itemHeight - frame.H());
	}
}

unsigned int List::ItemFromPos(const Point &pos) {
	return floor((frame.max.y - pos.y + scroll.Val()) / itemHeight);
}

void List::Resize(const Rect &_frame) {
	frame = _frame;
	scroll.Resize(frame);
}

//
// private
//

bool List::UpdateHover(const Point &pos) {

	if (frame.Contains(pos)) {

		unsigned int item = ItemFromPos(pos);
		if (item < items.size()) {

			if (items[item].state == litNormal) {

				for (ListItems::iterator i = items.begin(); i != items.end(); ++i)
					if (i->state == litHover)
						i->state = litNormal;

				items[item].state = litHover;
				return true;
			}
			else if (items[item].state == litHover) {
				return false;
			}
		}
	}

	return ClearHover();
}

bool List::ClearHover() {

	bool cleared = false;
	for (ListItems::iterator i = items.begin(); i != items.end(); ++i)
		if (i->state == litHover) {
			i->state = litNormal;
			cleared = true;
		}

	return cleared;
}

bool List::ClearPressed() {

	bool cleared = false;
	for (ListItems::iterator i = items.begin(); i != items.end(); ++i)
		if (i->state == litPressed) {
			i->state = litNormal;
			cleared = true;
		}

	return cleared;
}

//
// item
//

bool ListItem::operator>(const ListItem &o) const {

	if (group > o.group)
		return true;
	else if (group < o.group)
		return false;
	else
		return TextIsGreaterThan(o.text);
}

static unsigned int _min(unsigned int a, unsigned int b) {
	return (a < b) ? a : b;
}

// String a is less than b.
bool ListItem::TextIsGreaterThan(const std::string &other) const {

	unsigned int s = _min(text.size(), other.size());

	for (unsigned int i = 0; i < s; ++i) {

		int a = tolower(text[i]), b = tolower(other[i]);
		if (a > b)
			return true;
		else if (a < b)
			return false;
	}

	return text.size() > other.size();
}
