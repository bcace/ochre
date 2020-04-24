#include "pine/widgets/button.h"
#include "pine/events.h"

#define BTN_R	4.0

using namespace pine;



Button::Button(Widget *p, ButtonStyle _style, bool _drawBkgr) : Control(p),
	drawBkgr(_drawBkgr), useSpecColor(false),
	state(bsNormal),
	style(_style) {}

Button::Button(Widget *p, int _icon, const std::string &_text, ButtonStyle _style, bool _drawBkgr) : Control(p, _icon, _text),
	drawBkgr(_drawBkgr), useSpecColor(false),
	state(bsNormal),
	style(_style) {}

Button::Button(Widget *p, int _icon, const std::string &_text, const std::string &_desc, ButtonStyle _style, bool _drawBkgr) : Control(p, _icon, _text, _desc),
	drawBkgr(_drawBkgr), useSpecColor(false),
	state(bsNormal),
	style(_style) {}

void Button::SetSpecColor(int _specColor) {
	specColor = _specColor;
	useSpecColor = true;
}

//
// events
//

void Button::MousePressEvent(MouseEvent &e) {

	RETURN_IF_INVISIBLE_OR_ACCEPTED

	if (e.button == mbLeft) {
		if (frame.Contains(e.pos) && state != bsPressed) {
			state = bsPressed;
			Widget::paint = true;
			e.accepted = true;
		}
	}
}

void Button::MouseMoveEvent(MouseEvent &e) {

	RETURN_IF_INVISIBLE

	if (frame.Contains(e.pos)) {

		if (state == bsNormal) {
			state = bsHover;
			Widget::paint = true;
		}

		e.accepted = true;
	}
	else {

		if (state == bsHover) {
			state = bsNormal;
			Widget::paint = true;
		}
	}
}

void Button::MouseReleaseEvent(MouseEvent &e) {

	RETURN_IF_INVISIBLE_OR_ACCEPTED

	if (state == bsPressed) {

		if (frame.Contains(e.pos)) {
			Widget::paint = true;
			Emit(ActionEvent(Clicked));
		}

		state = bsNormal;
	}
}

void Button::MouseHoverEvent(MouseEvent &e) {

	RETURN_IF_INVISIBLE_OR_ACCEPTED

	if (frame.Contains(e.pos)) {
		Widget::paint = true;
		e.accepted = true;
		e.tag.s = Desc();
	}
}
