#include "pine/widgets/container.h"
#include "pine/color.h"
#include "pine/platform.h"

using namespace pine;



void CiContainer::Paint() {

	RETURN_IF_INVISIBLE

	// toolbar

	glBegin(GL_QUADS);

	Schema::ApplyColor(scoBkgr);
	glVertex2d(frame.min.x, frame.max.y);
	glVertex2d(frame.max.x, frame.max.y);
	glVertex2d(frame.max.x, frame.max.y - TOOLBAR_H);
	glVertex2d(frame.min.x, frame.max.y - TOOLBAR_H);

	glEnd();

	Control::Paint();

	glBegin(GL_QUADS);

	glColor4ub(0, 0, 0, 60);
	glVertex2d(frame.min.x, frame.max.y - TOOLBAR_H);
	glVertex2d(frame.max.x, frame.max.y - TOOLBAR_H);
	glColor4ub(0, 0, 0, 0);
	glVertex2d(frame.max.x, frame.max.y - TOOLBAR_H - SHADOW_W);
	glVertex2d(frame.min.x, frame.max.y - TOOLBAR_H - SHADOW_W);

	glColor4ub(0, 0, 0, 0);
	glVertex2d(frame.min.x, frame.max.y);
	glVertex2d(frame.max.x, frame.max.y);
	glColor4ub(0, 0, 0, 20);
	glVertex2d(frame.max.x, frame.max.y - TOOLBAR_H);
	glVertex2d(frame.min.x, frame.max.y - TOOLBAR_H);

	glEnd();
}
