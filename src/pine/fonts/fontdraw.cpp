#include "pine/fonts/font.h"
#include "pine/platform.h"



void CiFont::DrawChar(unsigned char c, double i, double j) {
	double tx1 = (c - 32) * fcw;
	double tx2 = tx1 + fcw;

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2d(tx1, 0.0);
	glVertex2d(i, j);
	glTexCoord2d(tx1, fch);
	glVertex2d(i, j + h);
	glTexCoord2d(tx2, fch);
	glVertex2d(i + w, j + h);
	glTexCoord2d(tx2, 0.0);
	glVertex2d(i + w, j);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void CiFont::DrawIcon(unsigned char c, double i, double j) {
	DrawChar(c + 32, i, j);
}

void CiFont::DrawIconCentered(unsigned char c, double i, double j) {
	DrawChar(c + 32, (int)(i - h * 0.5), (int)(j - w * 0.5));
}

// If not centered, drawn from lower left corner.
void CiFont::DrawText(const std::string &t, double x, double y, CiTextAlign a) {

	if (a & taHorz)
		x -= t.length() * w * 0.5;
	if (a & taVert)
		y -= h * 0.5;

	for (std::string::const_iterator c = t.begin(); c != t.end(); ++c) {
		DrawChar(*c, (int)x, (int)y);
		x += w;
	}
}

// If not centered, drawn from upper left corner.
void CiFont::DrawMultilineText(const std::string &t, double x, double y, CiTextAlign a) {

	unsigned int rows = 1, col = 0, cols = 0;
	for (std::string::const_iterator c = t.begin(); c != t.end(); ++c) {
		if (*c != '\n') {
			++col;
			if (col > cols)
				cols = col;
		}
		else {
			++rows;
			col = 0;
		}
	}

	double left = (a & taHorz) ? (x - cols * w * 0.5) : x;

	x = left;
	y = ((a & taVert) ? y + rows * h * 0.5 : y) - h;

	for (std::string::const_iterator c = t.begin(); c != t.end(); ++c) {
		if (*c != '\n') {
			DrawChar(*c, (int)x, (int)y);
			x += w;
		}
		else {
			y -= h;
			x = left;
		}
	}
}
