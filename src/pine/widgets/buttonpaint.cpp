#include "pine/widgets/button.h"
#include "pine/fonts/font.h"
#include "pine/color.h"
#include "pine/platform.h"

using namespace pine;



void Button::Paint() {

	RETURN_IF_INVISIBLE

	// background

	if (state == bsPressed) {

		Schema::ApplyColor(scoFrgr);

		glBegin(GL_QUADS);
		glVertex2d(frame.min.x, frame.min.y);
		glVertex2d(frame.min.x, frame.max.y);
		glVertex2d(frame.max.x, frame.max.y);
		glVertex2d(frame.max.x, frame.min.y);
		glEnd();
	}
	else if (state == bsHover || drawBkgr) {

		Schema::ApplyColor(scoFrgr, (state == bsHover) ? 40 : 20);

		glBegin(GL_QUADS);
		glVertex2d(frame.min.x, frame.min.y);
		glVertex2d(frame.min.x, frame.max.y);
		glVertex2d(frame.max.x, frame.max.y);
		glVertex2d(frame.max.x, frame.min.y);
		glEnd();
	}

	// icon & text

	if (!face->enabled)
		Schema::ApplyColor(scoDisabledFrgr);
	else if (state == bsPressed)
		Schema::ApplyColor(scoBkgr);
	else
		Schema::ApplyColor(useSpecColor ? specColor : scoFrgr);

	Point center = frame.Center();

	if (style == IconOnly) {

		CiFont::SmallIcons().Bind().DrawIconCentered(face->icon, center.x, center.y);
	}
	else if (style == TextOnly) {

		CiFont &font = CiFont::Small().Bind();
		font.DrawText(face->text, center.x, center.y, taAll);
	}
	else if (style == IconAndText) {

		CiFont::SmallIcons().Bind().DrawIconCentered(face->icon, frame.min.x + frame.H() * 0.5, center.y);
		CiFont &font = CiFont::Small().Bind();
		font.DrawText(face->text, frame.min.x + frame.H() + 3.0, center.y, taVert);
	}
	else if (style == LargeIconOnly) {

		CiFont &icons = CiFont::LargeIcons();
		icons.Bind().DrawIconCentered(face->icon, center.x, center.y);
	}
	else {

		CiFont &icons = CiFont::LargeIcons();
		CiFont &font = CiFont::Small();
		double hSum = (icons.H() + font.H()) * 0.5 + 4.0;
		icons.Bind().DrawIconCentered(face->icon, center.x, center.y + hSum - icons.H() * 0.5);
		font.Bind().DrawText(face->text, center.x, center.y - hSum + font.H() * 0.5, taAll);
	}
}
