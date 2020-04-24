#include "widgets/runtimeview.h"
#include "pine/events.h"

using namespace pine;



RuntimeView::RuntimeView(Widget *p) : CiContainer(p),
	mainview(this) {}

void RuntimeView::Resize(const Rect &f) {

	Widget::Resize(f);

	mainview.Resize(Rect(f.min, Point(f.max.x, f.max.y - TOOLBAR_H)));

	// resize state buttons (use the hlayout)
}
