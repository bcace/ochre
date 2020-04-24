#include "pine/widgets/hilight.h"
#include <iostream>

#define NPOS 	estring::npos

using namespace pine;



CiHilight::CiHilight(estring &_text) :
	text(_text),
	end(text.begin()) {}

void CiHilight::Reset() {
	color = scoText;
	end = text.begin();
}

bool CiHilight::Update(estring::iterator &c) {

	if (c < end)
		return false;

	estring sep = ",.-+/*=<>(){}[];:#$&|~ \t\n\r", num = "0123456789";

	if (*c == '$') {

		estring::iterator l = ++estring::iterator(c);
		while (l != text.end() && *l != '$')
			++l;

		if (l != text.end())	// include last '$'
			++l;

		color = scoComment;
		end = l;

		return true;
	}
	else if (*c == '#') {

		estring::iterator l = c;
		while (l != text.end() && *l != '\n')
			++l;

		color = scoComment;
		end = l;

		return true;
	}
	else if (*c == '"') {

		estring::iterator l = ++estring::iterator(c);
		while (l != text.end() && *l != '"')
			++l;

		if (l != text.end())	// include last '"'
			++l;

		color = scoString;
		end = l;

		return true;
	}
	else if (num.find(*c) != NPOS) {

		estring::iterator l = c;
		while (l != text.end() && num.find(*l) == NPOS)
			++l;

		color = scoNumber;
		end = l;

		return true;
	}
	else if (sep.find(*c) == NPOS) {

		estring::iterator l = Lookahead(c);
		unsigned int length = l - c;

		if (
			Matches(c, l, length, "if") ||
			Matches(c, l, length, "else") ||
			Matches(c, l, length, "while") ||
			Matches(c, l, length, "requires") ||
			Matches(c, l, length, "this") ||
			Matches(c, l, length, "other") ||
			Matches(c, l, length, "type") ||
			Matches(c, l, length, "false") ||
			Matches(c, l, length, "true"))
			color = scoKeyword;
		else if (
			Matches(c, l, length, "set") ||
			Matches(c, l, length, "see") ||
			Matches(c, l, length, "mod") ||
			Matches(c, l, length, "act") ||
			Matches(c, l, length, "do"))
			color = scoSection;
		else if (
			Matches(c, l, length, "refs") ||
			Matches(c, l, length, "local_refs") ||
			Matches(c, l, length, "min_refs") ||
			Matches(c, l, length, "env") ||
			Matches(c, l, length, "int") ||
			Matches(c, l, length, "bool") ||
			Matches(c, l, length, "real") ||
			Matches(c, l, length, "point") ||
			Matches(c, l, length, "text") ||
			Matches(c, l, length, "vector") ||
			Matches(c, l, length, "box"))
			color = scoClass;
		else
			color = scoText;

		end = l;

		return true;
	}
	else {
		color = scoText;
		return true;
	}

}

estring::iterator CiHilight::Lookahead(estring::iterator c) {

	estring sep = ",.-+/*=<>(){}[];:#$&|~ \t\n\r";

	estring::iterator l = c;
	while (sep.find(*l) == NPOS && l != text.end())
		++l;

	return l;
}

bool CiHilight::Matches(estring::iterator c, estring::iterator l, unsigned int length, const estring &w) {

	if (length != w.length())
		return false;

	for (estring::const_iterator i = w.begin(); i != w.end(); ++c, ++i)
		if (*c != *i)
			return false;

	return true;
}
