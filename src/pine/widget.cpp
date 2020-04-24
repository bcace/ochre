#include "widgets/mainwidget.h"
#include "pine/platform.h"

using namespace pine;



int Widget::mouseButtons = 0;
int Widget::modifiers = 0;
int Widget::hoverTime = 0;
bool Widget::paint = true;
Point Widget::windowSize;
Point Widget::mousePos;
std::vector<Window *> Widget::windowStack;

Widget::Widget(Widget *_parent) :
	parent(_parent),
	visible(true),
	face(&defFace),
	size(54.0, 54.0),
	subscribedActions(&face),
	defFace("Title", "...", -1) {}

Widget::Widget(Widget *_parent, int _icon, const std::string &_text) :
	parent(_parent),
	visible(true),
	face(&defFace),
	size(54.0, 54.0),
	subscribedActions(&face),
	defFace(_text, _text, _icon) {}

Widget::Widget(Widget *_parent, int _icon, const std::string &_text, const std::string &_desc) :
	parent(_parent),
	visible(true),
	face(&defFace),
	size(54.0, 54.0),
	subscribedActions(&face),
	defFace(_text, _desc, _icon) {}

bool Widget::SetVisible(bool _visible) {

	if (visible != _visible) {
		visible = _visible;
		Widget::paint = true;
		return true;
	}
	else
		return false;
}

const Point &Widget::Size() const {
	return size;
}

void Widget::SetSize(double w, double h) {
	size.x = w;
	size.y = h;
}

void Widget::EnableScissor() {
	glScissor(frame.min.x, frame.min.y, frame.W(), frame.H());
	glEnable(GL_SCISSOR_TEST);
}

void Widget::DisableScissor() {
	glDisable(GL_SCISSOR_TEST);
}

void Widget::ApplyViewport() {
	glViewport(frame.min.x, frame.min.y, frame.W(), frame.H());
}

void Widget::DrawRect(double minx, double miny, double maxx, double maxy) {
	glBegin(GL_QUADS);
	glVertex2d(minx, miny);
	glVertex2d(maxx, miny);
	glVertex2d(maxx, maxy);
	glVertex2d(minx, maxy);
	glEnd();
}
