#include "pine/widgets/list.h"
#include "pine/fonts/font.h"
#include "pine/color.h"
#include "pine/platform.h"

#define SCR_BTN_S	16.0

using namespace pine;



void List::Paint() {

	RETURN_IF_INVISIBLE

	// list background

	if (parent == 0) {
		Schema::ApplyColor(scoPopupBkgr);
		DrawRect(frame.min.x, frame.min.y, frame.max.x, frame.max.y);
	}

	CiFont &fText = CiFont::Small();
	CiFont &fIcon = CiFont::SmallIcons();

	EnableScissor();

	double top = frame.max.y + scroll.Val(), bottom = top - itemHeight;
	for (ListItems::iterator i = items.begin(); i != items.end(); ++i) {

		// item background

		if (i->state != litNormal) {

			if (i->state == litHover)
				Schema::ApplyColor(scoFrgr, 40);
			else
				Schema::ApplyColor(scoSelectionBkgr);

			glBegin(GL_QUADS);
			glVertex2d(frame.min.x, bottom);
			glVertex2d(frame.max.x, bottom);
			glVertex2d(frame.max.x, top);
			glVertex2d(frame.min.x, top);
			glEnd();
		}

		// item content

		Schema::ApplyColor(scoFrgr);

		double left = frame.min.x, half = (top - bottom) * 0.5;

		if (style & Icon) {
			if (i->icon != -1)
				fIcon.Bind().DrawIconCentered(i->icon, left + half, bottom + half);
			left += itemHeight;
		}
		else
			left += LIST_ITEM_PADDING;

		fText.Bind().DrawText(i->text, left, bottom + half, taVert);

		top -= itemHeight;
		bottom -= itemHeight;
	}

	DisableScissor();

	scroll.Paint();
}
