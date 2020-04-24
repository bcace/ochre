#include "pine/widgets/scroll.h"
#include "pine/color.h"

using namespace pine;



CiScroll::CiScroll(Widget *p) : Control(p),
	val(0.0),
	content(0.0) {}

double CiScroll::Val() const {
	return val;
}

void CiScroll::SetVal(double _val) {
	val = _val;
	FixVal();
}

void CiScroll::IncreaseVal(double _val) {
	val += _val;
	FixVal();
}

double CiScroll::Content() const {
	return content;
}

void CiScroll::SetContent(double _content) {
	content = _content;
	FixVal();
}

void CiScroll::FixVal() {
	if (content < size)
		val = 0.0;
	else if (val < 0.0)
		val = 0.0;
	else if (val > content - size)
		val = content - size;
}

//
// vertical
//

CiVScroll::CiVScroll(Widget *p) : CiScroll(p) {}

void CiVScroll::Resize(const Rect &_frame) {

	frame = _frame;

	frame.min.x = frame.max.x - SCROLL_W;
	size = frame.H();
}

void CiVScroll::Paint() {

	RETURN_IF_INVISIBLE

	if (content <= frame.H())
		return;

	Schema::ApplyColor(scoFrgr, 180);

	double topR = val / content;
	double bottomR = (val + frame.H()) / content;

	DrawRect(
		frame.min.x,
		frame.max.y - frame.H() * bottomR,
		frame.max.x,
		frame.max.y - frame.H() * topR
		);
}

//
// horizontal
//

CiHScroll::CiHScroll(Widget *p) : CiScroll(p) {}

void CiHScroll::Resize(const Rect &_frame) {

	frame = _frame;

	frame.max.y = frame.min.y + SCROLL_W;
	size = frame.W();
}

void CiHScroll::Paint() {

	RETURN_IF_INVISIBLE

	if (content <= frame.W())
		return;

	Schema::ApplyColor(scoFrgr, 180);

	double leftR = val / content;
	double rightR = (val + frame.W()) / content;

	DrawRect(
		frame.min.x + frame.W() * leftR,
		frame.min.y,
		frame.min.x + frame.W() * rightR,
		frame.max.y
		);
}
