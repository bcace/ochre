#include "widgets/mainwidget.h"
#include "pine/color.h"
#include "pine/platform.h"

using namespace pine;



void Mainwidget::Paint() {

	Window::Paint();

	// draw mainbar separators

	mainbar.Paint();

	// shadows & gradients

	glBegin(GL_QUADS);

	glColor4ub(0, 0, 0, 60);
	glVertex2d(frame.min.x, frame.min.y + MAIN_MENU_H);
	glVertex2d(frame.max.x, frame.min.y + MAIN_MENU_H);
	glColor4ub(0, 0, 0, 0);
	glVertex2d(frame.max.x, frame.min.y + MAIN_MENU_H + SHADOW_W);
	glVertex2d(frame.min.x, frame.min.y + MAIN_MENU_H + SHADOW_W);

	glColor4ub(0, 0, 0, 0);
	glVertex2d(frame.min.x, frame.min.y + MAIN_MENU_H);
	glVertex2d(frame.max.x, frame.min.y + MAIN_MENU_H);
	glColor4ub(0, 0, 0, 40);
	glVertex2d(frame.max.x, frame.min.y);
	glVertex2d(frame.min.x, frame.min.y);

	glEnd();
}
