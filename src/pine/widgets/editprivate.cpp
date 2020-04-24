#include "pine/widgets/edit.h"
#include "pine/fonts/font.h"
#include <math.h>

using namespace pine;



unsigned int Edit::SpacesToBol(unsigned int c) {

	unsigned int d = c;
	MoveToBol(d);

	unsigned int s = 0;
	for (; d < c; ++d)
		if (text[d] == '\t')
			s += TabMod(s);
		else
			++s;

	return s;
}

unsigned int Edit::SpacesToEol(unsigned int c) {

	unsigned int s = 0;
	for (; c < text.size() && text[c] != '\n'; ++c)
		if (text[c] == '\t')
			s += TabMod(s);
		else
			++s;

	return s;
}

bool Edit::MoveToEol(unsigned int &e) {
	while (e < text.size() && text[e] != '\n')
		++e;

	return e < text.size();
}

bool Edit::MoveToBol(unsigned int &e) {
	while (e > 0 && text[e - 1] != '\n')
		--e;

	return e > 0;
}

void Edit::MoveRight(unsigned int &e, unsigned int s) {

	unsigned int c = 0;
	for (; e < text.size() && c < s; ++e)
		if (text[e] == '\t')
			c += TabMod(c);
		else
			++c;
}

void Edit::MoveToEow(unsigned int &e) {
	unsigned int p = e;
	while (++e < text.size())
		if (CharType(text[p]) != CharType(text[e]))
			return;
}

void Edit::MoveToBow(unsigned int &e) {
	unsigned int p = --e;

	if (e == 0)
		return;

	while (--e > 0)
		if (CharType(text[p]) != CharType(text[e])) {
			++e;
			return;
		}
}

int Edit::CharType(unsigned char c) {
	if (c == ' ' || c == '\t' || c == '\n')
		return 0;
	else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return 1;
	else
		return 2;
}

unsigned int Edit::PgSize() {
	return (unsigned int)floor((float)frame.H() / (float)CiFont::Normal().H());
}

unsigned int Edit::IndentLevel(unsigned int e) {

	unsigned int c = e, w = WhitespacesAtBol(e);
	bool open = false, close = false;

	MoveToBol(c);
	while (c < e && !open && !close) {
		if (text[c] == '{')
			open = true;
		else if (text[c] == '}')
			close = true;
		++c;
	}

	if (open && !close)
		w += tabW;

	return (unsigned int)ceil((double)w / (double)tabW);
}

int Edit::TabsOnlyToBol(int e) {

	if (e < 0)
		return -1;

	int t = 0;
	for (unsigned int c = e; c >= 0 && text[c] != '\n'; --c)
		if (text[c] == '\t')
			++t;
		else
			return -1;

	return t;
}

unsigned int Edit::WhitespacesAtBol(unsigned int e) {

	unsigned int c = e;
	MoveToBol(c);

	unsigned int w = 0;
	while (c < e) {
		if (text[c] == '\t')
			w += tabW;
		else if (text[c] == ' ')
			++w;
		else
			break;
		++c;
	}

	return w;
}
