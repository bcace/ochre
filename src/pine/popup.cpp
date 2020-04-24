#include "popup.h"
#include "pine/color.h"
#include "pine/events.h"
#include "pine/platform.h"

using namespace pine;



Popup::Popup(Widget *_parent, const Point &_margin) : Window(_parent),
	margin(_margin) {}

void Popup::Resize(const Rect &_frame) {
	xResizer.Resize(_frame.min.x, _frame.max.x, frame.min.x, frame.max.x);
	yResizer.Resize(_frame.min.y, _frame.max.y, frame.min.y, frame.max.y);
}

void Popup::Paint() {

	RETURN_IF_INVISIBLE

	Schema::ApplyColor(scoPopupBkgr);
	DrawRect(frame.min.x, frame.min.y, frame.max.x, frame.max.y);
	Widget::Paint();
}

void Popup::KeyPressEvent(KeyEvent &e) {

	RETURN_IF_INVISIBLE_OR_ACCEPTED

	if (e.key == keyEscape)
		Exit(-1);

	Widget::KeyPressEvent(e);
}

void Popup::MouseReleaseEvent(MouseEvent &e) {

	RETURN_IF_INVISIBLE_OR_ACCEPTED

	if (!frame.Contains(e.pos))
		Exit(-1);

	Widget::MouseReleaseEvent(e);
}

//
// resizer
//

void Popup::ResizedCentered(double xmargin, double ymargin) {
	xResizer.style = PopupResizerSingle::FixedMargin;
	yResizer.style = PopupResizerSingle::FixedMargin;
	xResizer.d1 = xResizer.d2 = xmargin;
	yResizer.d1 = yResizer.d2 = ymargin;
}

void Popup::FixedCentered(double width, double height) {
	xResizer.style = PopupResizerSingle::FixedSize;
	yResizer.style = PopupResizerSingle::FixedSize;
	xResizer.d1 = xResizer.d2 = width;
	yResizer.d1 = yResizer.d2 = height;
}

void Popup::Pinned(double x, double y, double width, double height) {
	xResizer.style = PopupResizerSingle::AnchorLo;
	yResizer.style = PopupResizerSingle::AnchorLo;
	xResizer.d1 = x;
	xResizer.d2 = width;
	yResizer.d1 = y;
	yResizer.d2 = height;
}

void Popup::SlideDown(double y, double height) {
	xResizer.style = PopupResizerSingle::FixedMargin;
	yResizer.style = PopupResizerSingle::AnchorHi;
	xResizer.d1 = xResizer.d2 = 0.0;
	yResizer.d1 = height;
	yResizer.d2 = y;
}

void Popup::SlideUp(double y, double height) {
	xResizer.style = PopupResizerSingle::FixedMargin;
	yResizer.style = PopupResizerSingle::AnchorLo;
	xResizer.d1 = xResizer.d2 = 0.0;
	yResizer.d1 = y;
	yResizer.d2 = height;
}

void PopupResizerSingle::Resize(double fmin, double fmax, double &min, double &max) {

	if (style == FixedSize) {
		min = (fmin + fmax - d1) * 0.5;
		max = min + d1;
	}
	else if (style == AnchorLo) {
		double p = fmin + d1;
		if (p + d2 < fmax) {
			min = p;
			max = p + d2;
		}
		else {
			min = p - d2;
			max = p;
		}
	}
	else if (style == AnchorHi) {
		double p = fmax - d2;
		if (p - d1 > fmin) {
			min = p - d1;
			max = p;
		}
		else {
			min = p;
			max = p + d1;
		}
	}
	else {
		min = fmin + d1;
		max = fmax - d2;
	}
}
