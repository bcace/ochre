#include "pine/widgets/popuplist.h"

using namespace pine;



PopupList::PopupList(Widget *_parent, bool _permaselect) : Popup(_parent),
	list(this, false, _permaselect, List::Text),
	orientation(Down),
	offset(0.0),
	maxSize(80.0) {

	AddAction("Choose", "Choose an item from the list", -1, &PopupList::ChooseAction);
	list.Connect(actions["Choose"], List::Choose);
}

void PopupList::AddItem(const std::string &itemText) {
	list.Add(ListItem(-1, itemText, Tag()));
}

void PopupList::SelectItem(int i) {
	list.SelectByIndex(i);
}

int PopupList::Show(double _offset, double _maxSize, Orientation _orientation) {

	offset = _offset;
	maxSize = _maxSize;
	orientation = _orientation;

	double listContentHeight = list.Count() * list.ItemHeight();
	double listHeight = (listContentHeight < maxSize) ? listContentHeight : maxSize;

	switch (orientation) {
	case Down:	SlideDown(offset, listHeight); break;
	case Up:	SlideUp(offset, listHeight); break;
	default:;
	}

	return Exec();
}

void PopupList::ChooseAction() {
	Exit(list.SelectedIndex());
}

void PopupList::Resize(const Rect &_frame) {
	Popup::Resize(_frame);
	list.Resize(frame);
}
