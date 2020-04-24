#include "pine/widgets/tooltip.h"
#include "pine/fonts/font.h"
#include "pine/color.h"
#include "pine/platform.h"
#include <math.h>

using namespace pine;



Tooltip::Tooltip(Widget *p) : Control(p) {
	visible = false;
}

void Tooltip::Show(const std::string &_text, const Point &_pos, bool _multiline) {

	visible = true;
	TP_MARGIN = _multiline ? 12.0 : 3.0;

	text = _text;
	multiline = _multiline;
	Point pos = _pos;

	// set frame

	CiFont &font = CiFont::Small();

	int rows = 1, col = 0, cols = 0;
	if (multiline) {

		for (std::string::iterator c = text.begin(); c != text.end(); ++c) {
			if (*c == '\n') {
				++rows;
				col = 0;
			}
			else if (++col > cols)
				cols = col;
		}

		if (cols > 80) {	// wrap if text exceeds 80 columns

			cols = 0;
			rows = 1;
			col = 0;

			unsigned w = 0;
			for (unsigned c = 0; c < text.size(); ++c, ++col) {
				if (text[c] == '\n') {
					++rows;
					col = 0;
				}
				else if (col >= 80) {	// wrap!
					text.replace(w, 1, 1, '\n');
					++rows;
					cols = 80;
					col = 0;
					c = w;
				}
				else if (text[c] == ' ')
					w = c;
			}
		}
	}
	else
		cols = text.size();

	Point dims(font.W() * cols + TP_MARGIN * 2.0, font.H() * rows + TP_MARGIN * 2.0);

	if (dims.x + _pos.x > globalFrame.max.x)
		pos.x = globalFrame.max.x - dims.x;

	if (dims.y + _pos.y > globalFrame.max.y)
		pos.y = _pos.y - dims.y;

	frame.min = pos;
	frame.max = frame.min + dims;
}

bool Tooltip::Hide() {

	if (visible) {
		visible = false;
		return true;
	}
	else
		return false;
}

void Tooltip::Resize(const Rect &_frame) {
	globalFrame = _frame;
}

void Tooltip::Paint() {

	RETURN_IF_INVISIBLE

	Schema::ApplyColor(scoFrgr, 220);

	glBegin(GL_QUADS);
	glVertex2d(frame.min.x, frame.min.y);
	glVertex2d(frame.max.x, frame.min.y);
	glVertex2d(frame.max.x, frame.max.y);
	glVertex2d(frame.min.x, frame.max.y);
	glEnd();

	Schema::ApplyColor(scoBkgr);

	CiFont &font = CiFont::Small().Bind();
	if (multiline)
		font.DrawMultilineText(text, frame.min.x + TP_MARGIN, frame.max.y - TP_MARGIN);
	else
		font.DrawText(text, frame.min.x + TP_MARGIN, frame.min.y + TP_MARGIN);
}
