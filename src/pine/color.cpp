#include "pine/color.h"
#include "pine/platform.h"

using namespace pine;



void Color::Apply() {
	glColor4ubv(c);
}

void Color::Apply(unsigned char a) {
	glColor4ub(c[0], c[1], c[2], a);
}

//
// scheme
//

Scheme::Scheme(const std::string &_name) :
	name(_name),
	id(Schema::availableId++) {

	resize(scoEnd);
}

//
// manager
//

Schema *Schema::ins = 0;
unsigned Schema::availableId = 0;

void Schema::CreateSchema() {
	if (ins == 0)
		ins = new Schema();
}

Scheme &Schema::SelectedScheme() {
	return ins->at(ins->selScheme);
}

void Schema::SelectScheme(unsigned int _selScheme) {
	if (_selScheme < ins->size())
		ins->selScheme = _selScheme;
}

void Schema::SelectScheme(const std::string &name) {
	for (unsigned int s = 0; s < ins->size(); ++s)
		if (ins->at(s).name == name) {
			ins->selScheme = s;
			break;
		}
}
/*
unsigned int CiSchema::SelectedScheme() {
	return ins->selScheme;
}
*/
Color &Schema::GetColor(int i) {
	return SelectedScheme()[i];
}

void Schema::ApplyColor(int i) {
	SelectedScheme()[i].Apply();
}

void Schema::ApplyColor(int i, unsigned char a) {
	SelectedScheme()[i].Apply(a);
}

std::vector<Scheme>::iterator Schema::Begin() {
	return ins->begin();
}

std::vector<Scheme>::iterator Schema::End() {
	return ins->end();
}

Schema::Schema() : selScheme(0) {

	push_back(Scheme("Terminal"));

	back()[scoBkgr] = Color(0, 0, 0);
	back()[scoPopupBkgr] = Color(0, 0, 0);
	back()[scoFrgr] = Color(0, 255, 0);
	back()[scoDisabledFrgr] = Color(0, 100, 0);

	back()[scoSelectionBkgr] = Color(0, 80, 0);

	back()[scoText] = Color(0, 160, 0);
	back()[scoKeyword] = Color(0, 255, 0);
	back()[scoSection] = Color(0, 255, 0);
	back()[scoClass] = Color(0, 255, 0);
	back()[scoComment] = Color(0, 100, 0);
	back()[scoNumber] = Color(0, 255, 0);
	back()[scoString] = Color(0, 255, 0);

	back()[scoError] = Color(220, 50, 47);

	push_back(Scheme("Slush"));

	back()[scoBkgr] = Color(248, 248, 248);
	back()[scoPopupBkgr] = Color(230, 230, 230);
	back()[scoFrgr] = Color(40, 40, 40);
	back()[scoDisabledFrgr] = Color(140, 140, 140);

	back()[scoSelectionBkgr] = Color(160, 200, 255);

	back()[scoText] = Color(30, 30, 30);
	back()[scoKeyword] = Color(0x00, 0x80, 0xA0);
	back()[scoSection] = Color(0x00, 0x80, 0xA0);
	back()[scoClass] = Color(0xA0, 0x80, 0x00);
	back()[scoComment] = Color(0xA0, 0xA0, 0xA0);
	back()[scoNumber] = Color(0xA0, 0x00, 0x00);
	back()[scoString] = Color(220, 50, 47);

	back()[scoError] = Color(220, 50, 47);

	push_back(Scheme("Monokai"));

	back()[scoBkgr] = Color(35, 38, 33);
	back()[scoPopupBkgr] = Color(46, 48, 40);
	back()[scoFrgr] = Color(180, 180, 180);
	back()[scoDisabledFrgr] = Color(100, 100, 100);

	back()[scoSelectionBkgr] = Color(0, 100, 160);

	back()[scoText] = Color(230, 230, 230);
	back()[scoKeyword] = Color(0xF9, 0x26, 0x72);
	back()[scoSection] = Color(204, 223, 50);
	back()[scoClass] = Color(0x66, 0xd9, 0xef);
	back()[scoComment] = Color(0x75, 0x75, 0x75);
	back()[scoNumber] = Color(0xAE, 0x81, 0xFF);
	back()[scoString] = Color(220, 50, 47);

	back()[scoError] = Color(220, 50, 47);

	push_back(Scheme("Solarized dark"));

	back()[scoBkgr] = Color(0, 43, 54);
	back()[scoPopupBkgr] = Color(7, 54, 66);
	back()[scoFrgr] = Color(147, 161, 161);
	back()[scoDisabledFrgr] = Color(88, 110, 117);

	back()[scoSelectionBkgr] = Color(0, 100, 160);

	back()[scoText] = Color(147, 161, 161);
	back()[scoKeyword] = Color(133, 153, 0);
	back()[scoSection] = Color(203, 75, 22);
	back()[scoClass] = Color(181, 137, 0);
	back()[scoComment] = Color(88, 110, 117);
	back()[scoNumber] = Color(42, 161, 152);
	back()[scoString] = Color(220, 50, 47);

	back()[scoError] = Color(220, 50, 47);

	push_back(Scheme("Solarized light"));

	back()[scoBkgr] = Color(253, 246, 227);
	back()[scoPopupBkgr] = Color(238, 232, 213);
	back()[scoFrgr] = Color(88, 110, 117);
	back()[scoDisabledFrgr] = Color(147, 161, 161);

	back()[scoSelectionBkgr] = Color(38, 139, 210);

	back()[scoText] = Color(88, 110, 117);
	back()[scoKeyword] = Color(133, 153, 0);
	back()[scoSection] = Color(203, 75, 22);
	back()[scoClass] = Color(181, 137, 0);
	back()[scoComment] = Color(88, 110, 117);
	back()[scoNumber] = Color(42, 161, 152);
	back()[scoString] = Color(220, 50, 47);

	back()[scoError] = Color(220, 50, 47);
}
