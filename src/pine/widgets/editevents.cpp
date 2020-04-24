#include "pine/widgets/edit.h"
#include "pine/fonts/font.h"
#include "pine/events.h"
#include "pine/platform.h"
#include "ochreapi.h"

using namespace pine;



void Edit::KeyPressEvent(KeyEvent &e) {

	RETURN_IF_INVISIBLE_OR_ACCEPTED

	bool ctrl = e.mods & mmCtrl, shift = e.mods & mmShift;

	switch (e.key) {
		case GLFW_KEY_LEFT:
			ArrowLeft(ctrl, shift);
			e.accepted = true;
			Widget::paint = true;
			return;
		case GLFW_KEY_RIGHT:
			ArrowRight(ctrl, shift);
			e.accepted = true;
			Widget::paint = true;
			return;
		case GLFW_KEY_DOWN:
			if (multiline) {
				ArrowDown(ctrl, shift);
				e.accepted = true;
				Widget::paint = true;
			}
			return;
		case GLFW_KEY_UP:
			if (multiline) {
				ArrowUp(ctrl, shift);
				e.accepted = true;
				Widget::paint = true;
			}
			return;
		case GLFW_KEY_BACKSPACE:
			Back(ctrl);
			e.accepted = true;
			Widget::paint = true;
			return;
		case GLFW_KEY_DELETE:
			Delete(ctrl);
			e.accepted = true;
			Widget::paint = true;
			return;
		case GLFW_KEY_ENTER:
			if (multiline) {
				Return(ctrl);
				e.accepted = true;
				Widget::paint = true;
			}
			return;
		case GLFW_KEY_TAB:
			Tab(shift);
			e.accepted = true;
			Widget::paint = true;
			return;
		case GLFW_KEY_HOME:
			Home(ctrl, shift);
			e.accepted = true;
			Widget::paint = true;
			return;
		case GLFW_KEY_END:
			End(ctrl, shift);
			e.accepted = true;
			Widget::paint = true;
			return;
		case GLFW_KEY_PAGE_UP:
			if (multiline) {
				PgUp(shift);
				e.accepted = true;
				Widget::paint = true;
			}
			return;
		case GLFW_KEY_PAGE_DOWN:
			if (multiline) {
				PgDown(shift);
				e.accepted = true;
				Widget::paint = true;
			}
			return;
		case GLFW_KEY_Z:
			if (ctrl) {
				Redo();
				e.accepted = true;
				Widget::paint = true;
			}
			return;
		case GLFW_KEY_Y:
			if (ctrl) {
				Undo();
				e.accepted = true;
				Widget::paint = true;
			}
			return;
		case GLFW_KEY_C:
			if (ctrl) {
				Copy();
				e.accepted = true;
				Widget::paint = true;
			}
			return;
		case GLFW_KEY_V:
			if (ctrl) {
				Paste();
				e.accepted = true;
				Widget::paint = true;
			}
			return;
		case GLFW_KEY_X:
			if (ctrl) {
				Cut();
				e.accepted = true;
				Widget::paint = true;
			}
			return;
		default:;
	}
}

void Edit::CharPressEvent(CharEvent &e) {

	RETURN_IF_INVISIBLE_OR_ACCEPTED

	if (e.mods & mmCtrl) {

		switch (e.code) {
			// case 'c':
			// 	Copy();
			// 	e.accepted = true;
			// 	Widget::paint = true;
			// 	break;
			// case 'v':
			// 	Paste();
			// 	e.accepted = true;
			// 	Widget::paint = true;
			// 	break;
			// case 'x':
			// 	Cut();
			// 	e.accepted = true;
			// 	Widget::paint = true;
			// 	break;
			// case 'z':
			// 	Undo();
			// 	e.accepted = true;
			// 	Widget::paint = true;
			// 	break;
			// case 'y':
			// 	Redo();
			// 	e.accepted = true;
			// 	Widget::paint = true;
			// 	break;
			default:;
		}
	}
	else {
		if (e.code == '}') {
			Block();
			e.accepted = true;
			Widget::paint = true;
		}
		else {
			Type(e.code);
			e.accepted = true;
			Widget::paint = true;
		}
	}
}

void Edit::MousePressEvent(MouseEvent &e) {

	RETURN_IF_INVISIBLE_OR_ACCEPTED

	if (frame.Contains(e.pos)) {

		if (e.button == mbLeft) {

			beg = end = FromPosition(e.pos);
			travel = 0;

			Widget::paint = true;
			e.accepted = true;
		}
		else {

			if (menu.Show(e.pos - frame.min)) {

				switch (menu.SelectedItemId()) {
					case CutActn:	Cut(); break;
					case CopyActn:	Copy(); break;
					case PasteActn:	Paste(); break;
					case UndoActn:	Undo(); break;
					case RedoActn:	Redo(); break;
					default:;
				}
			}
		}
	}
}

void Edit::MouseMoveEvent(MouseEvent &e) {

	RETURN_IF_INVISIBLE

	if (frame.Contains(e.pos)) {

		if (e.button == mbLeft) {

			if (travel > 1) {
				end = FromPosition(e.pos);
				AutoScroll();
				Widget::paint = true;
			}

			++travel;
		}
	}
}

void Edit::MouseScrollEvent(MouseEvent &e) {

	RETURN_IF_INVISIBLE_OR_ACCEPTED

	if (frame.Contains(e.pos)) {

		vScroll.IncreaseVal(-lineH * e.scroll);

		Widget::paint = true;
		e.accepted = true;
	}
}

void Edit::MouseHoverEvent(MouseEvent &e) {

	RETURN_IF_INVISIBLE_OR_ACCEPTED

	if (fileBased && frame.Contains(e.pos) && rev == par) {

		int c = ConservativeFromPosition(e.pos);
		if (c == -1)
			return;

		const char *info = OcPositionalInfo(tag.i, c);
		if (info) {
			Widget::paint = true;
			e.accepted = true;
			e.tag.s = info;
			e.tag.b = true;
		}
	}
}
