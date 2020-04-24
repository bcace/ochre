#ifndef octext_h
#define octext_h

#include "descriptors/descriptors.h"



class Ochre;
class OcClass;

OcClass *CreateTextClass();
void InitTextClass(Ochre *);

OCD(TextDestructor);

OCF(TextAdd);
OCF(TextAddBool);
OCF(TextAddInt);
OCF(TextAddReal);
OCF(TextAddPoint);
OCF(TextAssign);



struct OcPoint;

struct OcText
{
	char *t;
	int l;

	OcText();
	OcText(const std::string &);

	void Clear();

	void Add(const char *, int, OcText *);
	void Add(OcText *, OcText *);
	void Add(bool, OcText *);
	void Add(int, OcText *);
	void Add(double, OcText *);
	void Add(OcPoint *, OcText *);
	void Assign(OcText *);
};

#endif
