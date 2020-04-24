#include "pine/control.h"
#include "pine/widgets/layout.h"
#include "pine/color.h"
#include "pine/platform.h"

using namespace pine;



HLayout &HLayout::AddWidget(Control &w, bool expand) {
	elems.push_back(LayoutElement(&w, expand));
	return *this;
}

HLayout &HLayout::AddSeparator() {
	elems.push_back(LayoutElement());
	return *this;
}

HLayout &HLayout::AddExpander() {
	elems.push_back(LayoutElement(true));
	return *this;
}

void HLayout::Resize(const Rect &_frame) {

	frame = _frame;

	// update elems layout

	double min = 0.0;
	unsigned expanders = 0;
	for (LayoutElements::iterator e = elems.begin(); e != elems.end(); ++e) {

		if (e->widget) {
			if (e->expander)
				++expanders;
			min += e->widget->Size().x;
		}
		else if (e->expander)
			++expanders;
	}

	double padding = CiMax(expanders ? (frame.W() - min) / (double)expanders : 0.0, 0.0);

	double p = _frame.min.x;
	for (LayoutElements::iterator e = elems.begin(); e != elems.end(); ++e) {
		if (e->separator)
			e->pos = p;
		else {
			double w = e->expander ? padding : 0.0;

			if (e->widget) {
				w += e->widget->Size().x;
				e->widget->Resize(Rect(p, frame.min.y, p + w, frame.max.y));
			}

			p += w;
		}
	}
}

void HLayout::Paint() {

	for (LayoutElements::iterator e = elems.begin(); e != elems.end(); ++e)
		if (e->separator) {
			glBegin(GL_LINES);
			Schema::ApplyColor(scoFrgr, 60);
			glVertex2d(e->pos, frame.min.y);
			Schema::ApplyColor(scoFrgr, 5);
			glVertex2d(e->pos, frame.max.y);
			glEnd();
		}
}
