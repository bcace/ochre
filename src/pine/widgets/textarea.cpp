#include "pine/widgets/textarea.h"
#include "pine/color.h"
#include "pine/fonts/font.h"
#include "pine/platform.h"

using namespace pine;



CiTextArea::CiTextArea(Widget *p) : Control(p),
	margin(16.0, 16.0),
	rows(0),
	cols(0) {}

const Point &CiTextArea::TextSize() const {
	return textSize;
}

void CiTextArea::SetText(const std::string &_text) {

	text = _text;

	rows = 1;
	cols = 0;

	unsigned int col = 0;
	for (std::string::iterator c = text.begin(); c != text.end(); ++c)
		if (*c != '\n') {
			if (++col > cols)
				cols = col;
		}
		else {
			++rows;
			col = 0;
		}

	CiFont &font = CiFont::Small();
	textSize.x = cols * font.W() + margin.x * 2.0;
	textSize.y = rows * font.H() + margin.y * 2.0;
}

void CiTextArea::Paint() {

	RETURN_IF_INVISIBLE

	// draw text

	EnableScissor();

	Schema::ApplyColor(scoFrgr);

	CiFont &font = CiFont::Small().Bind();

	double x = frame.min.x + margin.x, left = x;
	double y = frame.max.y - margin.y - font.H();

	for (std::string::iterator c = text.begin(); c != text.end(); ++c) {

		if (*c != '\n') {
			font.DrawChar(*c, (int)x, (int)y);
			x += font.W();
		}
		else {
			x = left;
			y -= font.H();
		}
	}

	DisableScissor();
}
