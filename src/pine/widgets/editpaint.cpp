#include "pine/widgets/edit.h"
#include "pine/fonts/font.h"
#include "pine/color.h"
#include "ochreapi.h"
#include "pine/platform.h"
#include <sstream>
#include <iostream>

using namespace pine;



void Edit::Paint() {

	RETURN_IF_INVISIBLE

	EnableScissor();

	Schema::ApplyColor(scoFrgr);

	CiFont &font = CiFont::Normal().Bind();

	hilight.Reset();

	unsigned int b = beg, e = end;
	if (e < b)
		std::swap<unsigned int>(b, e);

	Point curPos, brkPos;
	double left = frame.min.x + gutter.tx - hScroll.Val();
	double top = frame.max.y + vScroll.Val() - fontH - TOP_MARGIN;
	double x = left, y = top;
	int col = 0, row = 0, i = 0;
	int brk = (fileBased && OcGetState() == OC_STATE_SUSPENDED && OcBreakType() == tag.i) ? OcBreakPosition() : -1;

	for (std::string::iterator c = text.begin(); c != text.end(); ++c, ++i) {

		if (hilight.Update(c))
			Schema::ApplyColor(hilight.color);

		if (i == end)
			curPos = Point(x, y);

		if (i == brk)
			brkPos = Point(x, y);

		if (*c == '\n') {

			if (i >= b && i < e) {
				Schema::ApplyColor(scoSelectionBkgr);
				DrawRect(x, y, x + fontW, y + lineH);
				Schema::ApplyColor(hilight.color);
			}

			x = left;
			y -= lineH;
			col = 0;
			++row;
		}
		else if (*c == '\t') {
			int tab = 4 - col % 4;

			if (i >= b && i < e) {
				Schema::ApplyColor(scoSelectionBkgr);
				DrawRect(x, y, x + tab * fontW, y + lineH);
				Schema::ApplyColor(hilight.color);
			}

			x += tab * fontW;
			col += tab;
		}
		else {

			if (i >= b && i < e) {
				Schema::ApplyColor(scoSelectionBkgr);
				DrawRect(x, y, x + fontW, y + lineH);
				Schema::ApplyColor(hilight.color);
			}

			font.DrawChar(*c, (int)x, (int)y);
			x += fontW;
			++col;
		}
	}

	// cursor

	Schema::ApplyColor(scoFrgr);
	if (end == text.size())
		curPos = Point(x, y);
	DrawRect(curPos.x - 1.0, curPos.y, curPos.x + 1.0, curPos.y + lineH);

	// break

	if (brk != -1) {
		Schema::ApplyColor(scoFrgr, 140);
		DrawRect(brkPos.x - 1.0, brkPos.y, brkPos.x + 1.0, brkPos.y + lineH);
		Schema::ApplyColor(scoFrgr, 30);
		DrawRect(left, brkPos.y, frame.max.x, brkPos.y + lineH);
	}

	// gutter

	if (lineNumbers) {

		Schema::ApplyColor(scoPopupBkgr);
		DrawRect(frame.min.x, frame.min.y, frame.min.x + gutter.br, frame.max.y);

		Schema::ApplyColor(scoDisabledFrgr);

		std::stringstream ss;
		for (int l = 0; l < lines; ++l) {
			ss << l + 1;
			font.DrawText(ss.str(), frame.min.x + gutter.ln - ss.str().size() * fontW, top - l * lineH);
			ss.str("");
		}
	}

	// unsaved/unparsed changes hint

	if (fileBased && (sav != rev || par != rev)) {
		Schema::ApplyColor(scoFrgr, 180);
		CiFont::LargeIcons().Bind().DrawIcon((sav != rev) ? 10 : 1, frame.max.x - 30.0, frame.max.y - 30.0);
	}

	// clear text

	if (text.size() == 0 && clearText.size() != 0) {
		Schema::ApplyColor(scoString, 180);
		CiFont::Normal().Bind().DrawText(clearText, left, top);
	}

	DisableScissor();

	hScroll.Paint();
	vScroll.Paint();
//	menu.Paint();
}
