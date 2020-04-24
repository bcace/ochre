#ifndef ocstring_h
#define ocstring_h

#include "enums.h"
#include <string>
#include <vector>

#define OC_STR_NO_MATCH	0.0f



struct OcStringLoc
{
	int ps, ln, cl;

	OcStringLoc(int _ps, int _ln, int _cl) :
		ps(_ps), ln(_ln), cl(_cl) {}
};



class OcString : public std::string
{
public:
	static std::string conChars, resChars, numChars, whtChars;

	OcString();
	OcString(const std::string &, int);

	OcString operator+(const OcString &) const;

	int Pos() const;
	int EndPos() const;
	int CompactPos() const;
	OcStringLoc Loc(int) const;
	bool ContainsPos(int) const;

	void Copy(const char *);
	void ReadFile(const std::string &);
	void Uncomment();
	std::vector<OcString> Split(const std::string &) const;
	int IsValidName() const;
	int IsValidInt() const;
	VarModifier GetModifier() const;
	float Match(const std::string &) const;

private:
	int pos;
};

typedef std::vector<OcString> OcStringVector;

#endif
