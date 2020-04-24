#ifndef font_h
#define font_h

#include <string>



enum CiFontSize { cfsSmall = 0, cfsNormal, cfsLarge, cfsSmallIcons, cfsLargeIcons };
enum CiTextAlign { taNone = 0x0, taHorz = 0x1, taVert = 0x2, taAll = 0x3 };

class CiFont
{
public:
	static CiFont &Small();
	static CiFont &Normal();
	static CiFont &Large();
	static CiFont &SmallIcons();
	static CiFont &LargeIcons();

	int W() const;
	int H() const;

	void DrawChar(unsigned char, double, double);
	void DrawIcon(unsigned char, double, double);
	void DrawIconCentered(unsigned char, double, double);
	void DrawText(const std::string &t, double x, double y, CiTextAlign a = taNone);
	void DrawMultilineText(const std::string &t, double x, double y, CiTextAlign a = taNone);

	void Load(int, int, short *, int, int, int);
	CiFont &Bind();

private:
	static CiFont fonts[5];
	static bool loaded;

	static void LoadFonts();

	int w, h;
	double fcw, fch;
	unsigned int id;
};

void CreateUbuntuMono6x12(CiFont &);
void CreateUbuntuMono7x15(CiFont &);
void CreateUbuntuMono8x16(CiFont &);
void CreateSmallIcons(CiFont &);
void CreateLargeIcons(CiFont &);

#endif
