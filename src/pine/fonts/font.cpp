#include "pine/fonts/font.h"
#include "pine/platform.h"



int CiFont::W() const {
	return w;
}

int CiFont::H() const {
	return h;
}

CiFont &CiFont::Bind() {
	glBindTexture(GL_TEXTURE_2D, id);
	return *this;
}

void CiFont::Load(int _w, int _h, short *codedBytes, int l, int iw, int ih) {

	w = _w;
	h = _h;

	fcw = w / (double)iw;
	fch = h / (double)ih;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	unsigned char *bytes = new unsigned char[iw * ih * 4];

	int k = 0;
	for (int i = 0; i < l; ++i) {

		if (codedBytes[i] > 255)
			for (int j = 0; j < codedBytes[i] - 255; ++j) {
				bytes[k] = bytes[k + 1] = bytes[k + 2] = 255;
				bytes[k + 3] = 0;
				k += 4;
			}
		else {
			bytes[k] = bytes[k + 1] = bytes[k + 2] = 255;
			bytes[k + 3] = codedBytes[i];
			k += 4;
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iw, ih, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	delete[] bytes;
}

//
// static
//

CiFont CiFont::fonts[5];

bool CiFont::loaded = false;

CiFont &CiFont::Small() {
	LoadFonts();
	return fonts[cfsSmall];
}

CiFont &CiFont::Normal() {
	LoadFonts();
	return fonts[cfsNormal];
}

CiFont &CiFont::Large() {
	LoadFonts();
	return fonts[cfsLarge];
}

CiFont &CiFont::SmallIcons() {
	LoadFonts();
	return fonts[cfsSmallIcons];
}

CiFont &CiFont::LargeIcons() {
	LoadFonts();
	return fonts[cfsLargeIcons];
}

void CiFont::LoadFonts() {

	if (loaded)
		return;

	CreateUbuntuMono7x15(fonts[cfsSmall]);
	CreateUbuntuMono8x16(fonts[cfsNormal]);
	CreateUbuntuMono7x15(fonts[cfsLarge]);
	CreateSmallIcons(fonts[cfsSmallIcons]);
	CreateLargeIcons(fonts[cfsLargeIcons]);

	loaded = true;
}
