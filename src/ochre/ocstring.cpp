#include "ocstring.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

#define SLN_CMCHR	'#'
#define MLN_CMCHR	'$'
#define RPL_CMCHR	' '
#define DIV_SPCHR	'~'
#define NPOS 		std::string::npos



OcString::OcString() :
	pos(0) {}

OcString::OcString(const std::string &other, int _pos) :
	std::string(other), pos(_pos) {}

OcString OcString::operator+(const OcString &other) const {
	return OcString(std::string(*this) + std::string(other), pos);
}

// Returns the location of the string
// within the parent string.
int OcString::Pos() const {
	return pos;
}

// Returns the location of the string end
// within the parent string.
int OcString::EndPos() const {
	return pos + (int)size();
}

// Returns a 32 bit int with pos in the
// low 16 bits, and lenght in hi 16 bits.
int OcString::CompactPos() const {
	return pos | (int)size() << 16;
}

// Gets location (line and column) of the
// character at 'pos'.
OcStringLoc OcString::Loc(int _pos) const {
	OcStringLoc loc(_pos, 0, 0);

	if (_pos >= (int)length())
		return loc;

	for (int c = 0; c < _pos; ++c) {
		if (at(c) == '\n') {
			loc.ln++;
			loc.cl = 0;
		}
		else
			loc.cl++;
	}

	return loc;
}

bool OcString::ContainsPos(int _pos) const {
	return _pos >= pos && _pos < pos + (int)size();
}

// Copies the text and resets the pos.
void OcString::Copy(const char *text) {
	assign(text);
	pos = 0;
}

// Opens the file and copies its contents
// into the string.
void OcString::ReadFile(const std::string &path) {
	std::ifstream file(path.c_str());
	assign((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());
	file.close();
}

// Replaces the multi- and single-line comments
// with the RPL_CMCHR character.
void OcString::Uncomment() {

	unsigned int c = 0;
	while (c < size()) {

		bool multiline = false;
		if ((at(c) == SLN_CMCHR) || (multiline = (at(c) == MLN_CMCHR))) {

			at(c) = RPL_CMCHR;
			c++;
			while ((c < size()) &&
					((multiline && (at(c) != MLN_CMCHR)) ||
					(!multiline && (at(c) != '\n')))) {

				if (at(c) != '\n')
					at(c) = RPL_CMCHR;

				c++;
			}

			if (c < size() && at(c) == MLN_CMCHR) {
				at(c) = RPL_CMCHR;
				c++;
			}
		}
		else
			c++;
	}
}

// Splits the string into a vector of substrings.
// Requres a string of sep. and/or op. chars.
// Chars preceding (and including) DIV_SPCHR char
// are used as separators, and the ones following
// as operators (included as separate substrings).
OcStringVector OcString::Split(const std::string &seps) const {
	OcStringVector strings;

	bool wht = true;
	int start = 0, c = 0, div = seps.find(DIV_SPCHR);
	while (c <= (int)length()) {

		int sep = (int)NPOS;
		if ((c == (int)length()) || ((sep = seps.find(at(c))) != (int)NPOS)) {

			if (start != c && !wht)
				strings.push_back(OcString(substr(start, c - start), pos + start));

			if (sep > div)
				strings.push_back(OcString(std::string(1, at(c)), pos + c));

			if (c < (int)length() && at(c) == '"') {	// handle string literals

				unsigned int p = c;
				while (++p < length() && at(p) != '"');

				if (p < length()) {	// found closing '"'
					strings.push_back(OcString(substr(c, p - c + 1), pos + c));
					c = p;
				}
				else	// closing '"' not found, leave it in so lexer can report the error
					strings.push_back(OcString(std::string(1, at(c)), pos + c));
			}

			wht = true;
			start = c + 1;
		}
		else if (whtChars.find(at(c)) == NPOS)
			wht = false;

		++c;
	}

	return strings;
}

// Cannot contain whitespace or
// reserved char.
int OcString::IsValidName() const {
	if (size() == 0)
		return 1;

	for (std::string::const_iterator c = begin(); c != end(); ++c)
		if (whtChars.find(*c) != NPOS)
			return 2;
		else if (resChars.find(*c) != NPOS)
			return 3;

	return 0;
}

// Can contain only digits. Returns the
// integer value.
int OcString::IsValidInt() const {
	for (std::string::const_iterator c = begin(); c != end(); ++c)
		if (numChars.find(*c) == NPOS)
			return -1;

	return atoi(c_str());
}

// Name is public if it starts with
// a capital letter.
VarModifier OcString::GetModifier() const {

	if (at(0) < 'A' || at(0) > 'Z')
		return vmPrivate;

	if (size() == 1)
		return vmPublic;		// single-char public

	for (const_iterator c = begin(); c != end(); ++c)
		if (*c < 'A' || *c > 'Z')
			return vmPublic;

	return vmConst;
}

//
// static
//

std::string OcString::resChars(",.!-+/*=<>(){}[];:#$&|~");
std::string OcString::numChars("0123456789");
std::string OcString::whtChars(" \t\n\r\"");

//
// matching
//

// Tries to match the string with another string,
// returns the match level. todo: add case sensitivity
float OcString::Match(const std::string &str) const {

	if (str.size() > size())
		return OC_STR_NO_MATCH;

	int c = 0, d = 0, s = 0;	// skipped chars
	while (d < (int)str.size()) {
		while (c < (int)size() && tolower(str.at(d)) != tolower(at(c))) {
			++c;
			++s;
		}

		if (c == (int)size() && d < (int)str.size())
			return OC_STR_NO_MATCH;
		++d;
		++c;
	}

	return (float)str.size() / (float)size() + (s ? 0.0f : 2.0f);
}
