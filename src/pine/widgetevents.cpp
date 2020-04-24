#include "control.h"
#include "events.h"

using namespace pine;



void Widget::KeyPressEvent(KeyEvent &e) {

	if (!visible || e.accepted)
		return;

	if (actions.exec_key(e.key, e.mods & mmCtrl, e.mods & mmShift)) {
		e.accepted = true;
		return;
	}

	for (std::list<Control *>::iterator c = controls.begin(); c != controls.end(); ++c) {
		(*c)->KeyPressEvent(e);
		if (e.accepted)
			return;
	}
}

void Widget::CharPressEvent(CharEvent &e) {

	if (!visible || e.accepted)
		return;

	if (actions.exec_char(e.code, e.mods & mmCtrl, e.mods & mmShift)) {
		e.accepted = true;
		return;
	}

	for (std::list<Control *>::iterator c = controls.begin(); c != controls.end(); ++c) {
		(*c)->CharPressEvent(e);
		if (e.accepted)
			return;
	}
}

void Widget::MousePressEvent(MouseEvent &e) {

	if (!visible || e.accepted || !frame.Contains(e.pos))
		return;

	for (std::list<Control *>::iterator c = controls.begin(); c != controls.end(); ++c) {
		(*c)->MousePressEvent(e);
		if (e.accepted)
			return;
	}
}

void Widget::MouseReleaseEvent(MouseEvent &e) {

	if (!visible || e.accepted || !frame.Contains(e.pos))
		return;

	for (std::list<Control *>::iterator c = controls.begin(); c != controls.end(); ++c) {
		(*c)->MouseReleaseEvent(e);
		if (e.accepted)
			return;
	}
}

void Widget::MouseMoveEvent(MouseEvent &e) {

	if (!visible)
		return;

	for (std::list<Control *>::iterator c = controls.begin(); c != controls.end(); ++c)
		(*c)->MouseMoveEvent(e);
}

void Widget::MouseScrollEvent(MouseEvent &e) {

	if (!visible || e.accepted || !frame.Contains(e.pos))
		return;

	for (std::list<Control *>::iterator c = controls.begin(); c != controls.end(); ++c) {
		(*c)->MouseScrollEvent(e);
		if (e.accepted)
			return;
	}
}

void Widget::MouseHoverEvent(MouseEvent &e) {

	if (!visible || e.accepted || !frame.Contains(e.pos))
		return;

	for (std::list<Control *>::iterator c = controls.begin(); c != controls.end(); ++c) {
		(*c)->MouseHoverEvent(e);
		if (e.accepted)
			return;
	}
}

void Widget::Update(UpdateEvent &e) {
	for (std::list<Control *>::iterator c = controls.begin(); c != controls.end(); ++c)
		(*c)->Update(e);
}

void Widget::Resize(const Rect &_frame) {
	frame = _frame;
}

void Widget::Paint() {

	if (!visible)
		return;

	for (std::list<Control *>::iterator c = controls.begin(); c != controls.end(); ++c)
		(*c)->Paint();
}

/*
private funcs for (un)registering controls
for automatic event propagation.
called implicitly for controls when
created, copied or deleted.
*/

void Widget::RegisterControl(Control *c) {
	controls.push_back(c);
}

void Widget::UnregisterControl(Control *c) {
	controls.remove(c);
}

bool Widget::IsParent(Control *c) {
	for (Widget *p = c->parent; p; p = p->parent)
		if (p == this)
			return true;
	return false;
}

/*
representation stuff
*/

std::string &Widget::Text() {
	return face->text;
}

std::string &Widget::Desc() {
	return face->desc;
}

int &Widget::Icon() {
	return face->icon;
}

bool &Widget::Enabled() {
	return face->enabled;
}
