#include "pine/widgets/list.h"
#include "pine/platform.h"
#include "pine/events.h"

using namespace pine;



void List::MousePressEvent(MouseEvent &e) {

	RETURN_IF_INVISIBLE_OR_ACCEPTED

	if (frame.Contains(e.pos)) {

		unsigned int i = ItemFromPos(e.pos);

		if ((!(e.modifier & mmCtrl) || !multiselect) && (i < items.size() || !permaselect))
			Widget::paint = ClearPressed();

		if (i < items.size()) {
			items[i].state = litPressed;
			Widget::paint = true;
		}

		e.accepted = true;
	}
}

void List::MouseMoveEvent(MouseEvent &e) {

	RETURN_IF_INVISIBLE

	if (UpdateHover(e.pos))
		Widget::paint = true;
}

void List::MouseReleaseEvent(MouseEvent &e) {

	RETURN_IF_INVISIBLE_OR_ACCEPTED

	if (frame.Contains(e.pos)) {

		if (e.dblclk) {

			unsigned int i = ItemFromPos(e.pos);
			if (i < items.size()) {
				Emit(ActionEvent(Choose));
				Widget::paint = true;
			}
		}

		e.accepted = true;
	}

	if (chooseOnClick) {
		e.accepted = true;
		Emit(ActionEvent(Choose));
		SetVisible(false);
	}
}

void List::MouseScrollEvent(MouseEvent &e) {

	RETURN_IF_INVISIBLE_OR_ACCEPTED

	if (frame.Contains(e.pos)) {

		scroll.IncreaseVal(-e.scroll * itemHeight);

		UpdateHover(e.pos);

		Widget::paint = true;
		e.accepted = true;
	}
}

void List::KeyPressEvent(KeyEvent &e) {

	RETURN_IF_INVISIBLE_OR_ACCEPTED

	switch (e.key) {
		case keyUp:
			SelectPrevious();
			e.accepted = true;
			Widget::paint = true;
			break;
		case keyDown:
			SelectNext();
			e.accepted = true;
			Widget::paint = true;
			break;
		case keyEnter:
			e.accepted = true;
			Emit(ActionEvent(Choose));
			Widget::paint = true;
			break;
		default:;
	}
}
