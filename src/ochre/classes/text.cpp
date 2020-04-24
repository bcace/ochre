#include "classes/text.h"
#include "descriptors/class.h"
#include "ochre.h"
#include <string.h>
#include <sstream>
#include <string>



OcClass *CreateTextClass() {
	return new OcClass("text", sizeof(OcText), false, TextDestructor);
}

void InitTextClass(Ochre *ochre) {
	OcClass *textCl = ochre->Class("text");
	OcClass *boolCl = ochre->Class("bool");
	OcClass *intCl = ochre->Class("int");
	OcClass *realCl = ochre->Class("real");
	OcClass *pointCl = ochre->Class("point");

	// instance ops

	textCl->ops.push_back(OcOp(otPlus, "returns a composite string of *this* and *arg*", OcArg(textCl, ptRead), ptRead, OcArg(textCl, ptRead), TextAdd));
	textCl->ops.push_back(OcOp(otPlus, "returns a composite string of *this* and *arg*, bool values are formatted as: \"true\", \"false\"", OcArg(textCl, ptRead), ptRead, OcArg(boolCl, ptRead), TextAddBool));
	textCl->ops.push_back(OcOp(otPlus, "returns a composite string of *this* and *arg*", OcArg(textCl, ptRead), ptRead, OcArg(intCl, ptRead), TextAddInt));
	textCl->ops.push_back(OcOp(otPlus, "returns a composite string of *this* and *arg*", OcArg(textCl, ptRead), ptRead, OcArg(realCl, ptRead), TextAddReal));
	textCl->ops.push_back(OcOp(otPlus, "returns a composite string of *this* and *arg*, point values are formatted as: \"{ 0.0, 30.4, 123.4 }\"", OcArg(textCl, ptRead), ptRead, OcArg(pointCl, ptRead), TextAddPoint));

	textCl->ops.push_back(OcOp(otAssign, "copies the *arg* text into *this*", OcArg(textCl, ptCaller), ptWrite, OcArg(textCl, ptRead), TextAssign, true));
}

// ctors

OCD(TextDestructor) {
	((OcText *)v)->Clear();
}

// instance ops

OCF(TextAdd) {
	OCR(OcText, 0)->Add(OCR(OcText, 1), OCR(OcText, ARG_RES));
}

OCF(TextAddBool) {
	OCR(OcText, 0)->Add(OCP(bool, 1), OCR(OcText, ARG_RES));
}

OCF(TextAddInt) {
	OCR(OcText, 0)->Add(OCP(int, 1), OCR(OcText, ARG_RES));
}

OCF(TextAddReal) {
	OCR(OcText, 0)->Add(OCP(double, 1), OCR(OcText, ARG_RES));
}

OCF(TextAddPoint) {
	OCR(OcText, 0)->Add(OCR(OcPoint, 1), OCR(OcText, ARG_RES));
}

OCF(TextAssign) {
	OCR(OcText, 0)->Assign(OCR(OcText, 1));
}

//
// struct
//

OcText::OcText() :
	t(0), l(0) {}

OcText::OcText(const std::string &s) :
	t(new char[s.length() + 1]), l(s.length()) {

	strcpy_s(t, s.length(), s.c_str());
}

void OcText::Clear() {
	if (t)
		delete[] t;
	t = 0;
	l = 0;
}

void OcText::Add(const char *at, int al, OcText *r) {

	r->Clear();

	r->l = l + al;
	r->t = new char[r->l + 1];

	strncpy_s(r->t, l, t, l);
	strncpy_s(r->t + l * sizeof(char), al + 1, at, al + 1);
}

void OcText::Add(OcText *a, OcText *r) {
	Add(a->t, a->l, r);
}

void OcText::Add(bool v, OcText *r) {
	v ? Add("true", 4, r) : Add("false", 5, r);
}

void OcText::Add(int v, OcText *r) {
	std::stringstream ss;
	ss << v;
	Add(ss.str().c_str(), ss.str().length(), r);
}

void OcText::Add(double v, OcText *r) {
	std::stringstream ss;
	ss << v;
	Add(ss.str().c_str(), ss.str().length(), r);
}

void OcText::Add(OcPoint *v, OcText *r) {
	std::stringstream ss;
	ss << "{" << v->x << ", " << v->y << ", " << v->z << "}";
	Add(ss.str().c_str(), ss.str().length(), r);
}

void OcText::Assign(OcText *a) {

	Clear();

	if (a->t == 0 || a->l == 0)
		return;

	l = a->l;
	t = new char[l + 1];
	strcpy_s(t, l, a->t);
}
