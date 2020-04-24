#include "pine/widgets/edit.h"
#include "pine/fonts/font.h"
#include "ochreapi.h"
#include <fstream>
#include <math.h>

using namespace pine;



Edit::Edit(Widget *_parent, const estring &_clearText, bool _multiline) : Control(_parent),
	multiline(_multiline),
	editable(true),
	lineNumbers(multiline),
	fileBased(false),
	spacing(1.0),
	beg(0),
	end(0),
	rev(0),
	sav(0),
	par(0),
	vScroll(this),
	hScroll(this),
	menu(this),
	clearText(_clearText),
	hilight(text) {

	Initialize();
	UpdateTextStats();
}

Edit::Edit(Widget *_parent, const Tag &_tag) : Control(_parent),
	multiline(true),
	editable(true),
	lineNumbers(true),
	fileBased(true),
	spacing(1.0),
	beg(0),
	end(0),
	rev(0),
	sav(0),
	par(0),
	vScroll(this),
	hScroll(this),
	menu(this),
	hilight(text) {

	tag = _tag;	// todo: can i move this somewhere more elegant?

	Initialize();
}

Edit::~Edit() {
	OcSelectType(tag.i);
	OcRemoveType();
}

void Edit::Initialize() {

	fontW = CiFont::Normal().W();
	fontH = CiFont::Normal().H();
	lineH = fontH + spacing;

	menu.AddItem("Cut", CutActn);
	menu.AddItem("Copy", CopyActn);
	menu.AddItem("Paste", PasteActn, true);
	menu.AddItem("Undo", UndoActn);
	menu.AddItem("Redo", RedoActn);
}

const estring &Edit::Text() const {
	return text;
}

void Edit::SetEditable(bool _editable) {
	editable = _editable;
}

bool Edit::Unsaved() const {
	return sav != rev;
}

void Edit::LoadFromFile(const estring &path) {

	std::ifstream file(path.c_str());
	if (file.is_open()) {
		text.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
	}

	sav = ++rev;

	UpdateTextStats();
}

void Edit::SaveToFile(const estring &path) {

	if (sav == rev)
		return;

	std::ofstream file(path.c_str(), std::ofstream::trunc);
	if (file.is_open()) {

		file << text;
		file.close();
	}

	sav = rev;
}

bool Edit::BuildEventPrologue() {

	if (par == rev)
		return false;

	OcSelectType(tag.i);
	OcReloadType(text.c_str(), 1);

	par = rev;

	return true;
}

void Edit::FocusLocation(unsigned int c) {

	beg = end = ((c < text.size()) ? c : 0);
	AutoScroll();
}

unsigned Edit::CaretPos() const {
	return end;
}

void Edit::Resize(const Rect &_frame) {
	Widget::Resize(_frame);
	vScroll.Resize(frame);
	hScroll.Resize(Rect(frame.min.x + gutter.tx, frame.min.y, frame.max.x, frame.max.y));
}

void Edit::UpdateTextStats() {

	int col = 0;
	columns = 0;
	lines = 1;
	for (std::string::iterator c = text.begin(); c != text.end(); ++c) {

		if (*c == '\n') {
			++lines;
			col = 0;
		}
		else if (*c == '\t') {
			int tab = 4 - col % 4;
			col += tab;
			if (col > columns)
				columns = col;
		}
		else {
			++col;
			if (col > columns)
				columns = col;
		}
	}

	gutter.ln = 0.0;
	if (lineNumbers)
		gutter.ln += fontW * ((int)floor(log10(lines)) + 3);

	gutter.br = gutter.ln + 6.0;
	gutter.tx = gutter.br + 6.0;

	vScroll.SetContent(lines * lineH);
	hScroll.SetContent(columns * fontW);
	hScroll.Resize(Rect(frame.min.x + gutter.tx, frame.min.y, frame.max.x, frame.max.y));
}

void Edit::AutoScroll() {

	Point pos = Position(end); // text system

	if (pos.y < 0)
		vScroll.IncreaseVal(pos.y);
	else if (pos.y + lineH > frame.H())
		vScroll.IncreaseVal(pos.y + lineH - frame.H());

	if (pos.x < 0)
		hScroll.IncreaseVal(pos.x);
	else if (pos.x + fontW > frame.W() - gutter.tx)
		hScroll.IncreaseVal(pos.x + fontW - (frame.W() - gutter.tx));
}

void Edit::ScrollUp() {
	vScroll.IncreaseVal(-lineH);
}

void Edit::ScrollDown() {
	vScroll.IncreaseVal(lineH);
}

// In char coordinates (row, col).
Loc Edit::Location(unsigned int p) {

	Loc loc;
	for (unsigned int c = 0; c < p; ++c)
		if (text[c] == '\n') {
			++loc.y;
			loc.x = 0;
		}
		else
			loc.x += (text[c] == '\t') ? TabMod(loc.x) : 1;

	return loc;
}

// In absolute pixel coords (x, y).
Point Edit::Position(unsigned int p) {

	Loc loc = Location(p);

	return Point(
		loc.x * fontW - hScroll.Val(),
		loc.y * lineH - vScroll.Val());
}

unsigned int Edit::FromPosition(const Point &pos) {

	Point p(pos.x - frame.min.x - gutter.tx, frame.max.y - pos.y - TOP_MARGIN);

	int l = (int)floor((p.y + vScroll.Val()) / lineH);
	if (l < 0)
		l = 0;
	else if (l > lines)
		l = lines - 1;

	unsigned int c = 0;

	// if not first line, find the first line char

	if (l) {
		unsigned int _l = 0;
		for (; c < text.size(); ++c)
			if (text[c] == '\n' && ++_l == l)
				break;
		++c;	// skip newline
	}

	// find char in line

	double x = -hScroll.Val();
	unsigned int col = 0;
	for (; c < text.size() && text[c] != '\n'; ++c) {

		double r = p.x - x;

		if (text[c] == '\t') {

			unsigned int tab = TabMod(col);

			if (r > 0.0 && r <= fontW * tab)
				return (r <= fontW * tab * 0.5) ? c : (c + 1);

			col += tab;
		}
		else {

			if (r > 0.0 && r <= fontW)
				return (r <= fontW * 0.5) ? c : (c + 1);

			++col;
		}

		x = fontW * col - hScroll.Val();
	}

	return c;
}

int Edit::ConservativeFromPosition(const Point &pos) {

	Point p(pos.x - frame.min.x - gutter.tx, frame.max.y - pos.y - TOP_MARGIN);

	int l = (int)floor((p.y + vScroll.Val()) / lineH);
	if (l < 0 || l >= lines)
		return -1;

	unsigned int c = 0;

	// if not first line, find the first line char

	if (l) {
		unsigned int _l = 0;
		for (; c < text.size(); ++c)
			if (text[c] == '\n' && ++_l == l)
				break;
		++c;	// skip newline
	}

	// find char in line

	double x = -hScroll.Val();
	for (unsigned int col = 0; c < text.size() && text[c] != '\n'; ++c) {

		double r = p.x - x;

		if (text[c] == '\t') {
			unsigned int tab = TabMod(col);
			if (r > 0.0 && r <= fontW * tab)
				return c;
			col += tab;
		}
		else {
			if (r > 0.0 && r <= fontW)
				return c;
			++col;
		}

		x = fontW * col - hScroll.Val();
	}

	return -1;
}

//
// static
//

unsigned int Edit::tabW = 4;

unsigned int Edit::TabMod(unsigned int s) {
	return Edit::tabW - (s % Edit::tabW);
}
