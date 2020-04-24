#include "ochre.h"
#include "section.h"
#include "context.h"
#include <iostream>
#include <sstream>
#include <cmath>



OcStat::OcStat(OcStat &x, OcStat::iterator f, OcStat::iterator l) : block(0) {
	splice(begin(), x, f, l);
}

OcStat::OcStat(Ochre *ochre, OcType *type, const OcString &line) : block(0) {

	OcStringVector words = line.Split(OcString::whtChars + "~" + OcString::resChars);

	OcClass *realClass = ochre->Class("real");
	OcClass *intClass = ochre->Class("int");
	OcClass *textClass = ochre->Class("text");

	OcClass *cl;
	OcType *tp;
	int intPart, fractPart;
	for (OcStringVector::iterator word = words.begin(); word != words.end(); ++word) {

		// keywords

		if (*word == "set")				push_back(OcToken(*word, ttKey, ktSet));
		else if (*word == "see")		push_back(OcToken(*word, ttKey, ktSee));
		else if (*word == "mod")		push_back(OcToken(*word, ttKey, ktMod));
		else if (*word == "act")		push_back(OcToken(*word, ttKey, ktAct));
		else if (*word == "do")			push_back(OcToken(*word, ttKey, ktDo));
		else if (*word == "requires")	push_back(OcToken(*word, ttKey, ktRequires));
		else if (*word == "this")		push_back(OcToken(*word, ttKey, ktThis));
		else if (*word == "other")		push_back(OcToken(*word, ttKey, ktOther));
		else if (*word == "type")		push_back(OcToken(*word, ttKey, ktType));

		// classes

		else if ((cl = ochre->Class(*word, 0, true)))
			push_back(OcToken(*word, ttClass, cl));

		// types

		else if ((tp = ochre->Type(*word)))
			push_back(OcToken(*word, ttType, tp));

		// operators

		else if (*word == "{")			push_back(OcToken(*word, ttStruct, stBlockBegin));
		else if (*word == "}")			push_back(OcToken(*word, ttStruct, stBlockEnd));
		else if (*word == "(")			push_back(OcToken(*word, ttStruct, stOpenBrace));
		else if (*word == ")")			push_back(OcToken(*word, ttStruct, stCloseBrace));
		else if (*word == ",")			push_back(OcToken(*word, ttOp, otComma));
		else if (*word == "&")			push_back(OcToken(*word, ttOp, otAnd));
		else if (*word == "|")			push_back(OcToken(*word, ttOp, otOr));
		else if (*word == ".") {
			if ((++word != words.end()) && ((intPart = word->IsValidInt()) != -1))	// .2
				push_back(OcToken(*(word - 1), ttLiteral, realClass, (double)intPart * pow(0.1, (double)word->size())));
			else
				push_back(OcToken(*(--word), ttOp, otReference));
		}
		else if (*word == ":") {
			if ((++word != words.end()) && (*word == ":"))
				push_back(OcToken(*(word - 1) + *word, ttOp, otCast));
			else
				push_back(OcToken(*(--word), ttOp, otIndex));
		}
		else if (*word == "+") {
			if ((++word != words.end()) && (*word == "="))
				push_back(OcToken(*(word - 1) + *word, ttOp, otPlusAssign));
			else
				push_back(OcToken(*(--word), ttOp, otPlus));
		}
		else if (*word == "-") {
			if ((++word != words.end()) && (*word == "="))
				push_back(OcToken(*(word - 1) + *word, ttOp, otMinusAssign));
			else
				push_back(OcToken(*(--word), ttOp, otMinus));
		}
		else if (*word == "*") {
			if ((++word != words.end()) && (*word == "="))
				push_back(OcToken(*(word - 1) + *word, ttOp, otMultiplyAssign));
			else
				push_back(OcToken(*(--word), ttOp, otMultiply));
		}
		else if (*word == "/") {
			if ((++word != words.end()) && (*word == "="))
				push_back(OcToken(*(word - 1) + *word, ttOp, otDivideAssign));
			else
				push_back(OcToken(*(--word), ttOp, otDivide));
		}
		else if (*word == "<") {
			if (++word != words.end()) {
				if (*word == "=")
					push_back(OcToken(*(word - 1) + *word, ttOp, otLessOrEqual));
				else if (*word == ">")
					push_back(OcToken(*(word - 1) + *word, ttOp, otNotEqual));
				else
					push_back(OcToken(*(--word), ttOp, otLess));
			}
			else
				push_back(OcToken(*(--word), ttOp, otLess));
		}
		else if (*word == ">") {
			if ((++word != words.end()) && (*word == "="))
				push_back(OcToken(*(word - 1) + *word, ttOp, otGreaterOrEqual));
			else
				push_back(OcToken(*(--word), ttOp, otGreater));
		}
		else if (*word == "=") {
			if ((++word != words.end()) && (*word == "="))
				push_back(OcToken(*(word - 1) + *word, ttOp, otEqual));
			else
				push_back(OcToken(*(--word), ttOp, otAssign));
		}

		// control flow

		else if (*word == "while")		push_back(OcToken(*word, ttOp, otWhile));
		else if (*word == "if")			push_back(OcToken(*word, ttOp, otIf));
		else if (*word == "else") {
			if ((++word != words.end()) && (*word == "if"))
				push_back(OcToken(*(word - 1) + *word, ttOp, otElseIf));
			else
				push_back(OcToken(*(--word), ttOp, otElse));
		}

		// literals

		else if ((intPart = word->IsValidInt()) != -1) {

			if ((++word != words.end()) && (*word == ".")) {		// 2.
				if (++word != words.end()) {
					if ((fractPart = word->IsValidInt()) != -1)		// 2.3
						push_back(OcToken(*(word - 2), ttLiteral, realClass, intPart + fractPart * pow(0.1, word->size())));
					else if (word->IsValidName() > 0)				// 2.+
						push_back(OcToken(*(--word), ttLiteral, realClass, (double)intPart));
					else {											// 2.i
						--word;
						push_back(OcToken(*(--word), ttLiteral, intClass, intPart));
					}
				}
				else												// 2.(eol)
					push_back(OcToken(*(--word - 1), ttLiteral, realClass, (double)intPart));
			}
			else
				push_back(OcToken(*(--word), ttLiteral, intClass, intPart));	// 2
		}
		else if (*word == "true")		push_back(OcToken(*word, ttLiteral, ochre->Class("bool"), true));
		else if (*word == "false")		push_back(OcToken(*word, ttLiteral, ochre->Class("bool"), false));
		else if (word->length() && word->at(0) == '"') {
			if (word->length() == 1)
				type->AddIssue("No closing '\"' found for string literal", itError, word->Pos());
			else
				push_back(OcToken(*word, ttLiteral, textClass, word->substr(1, word->length() - 2)));
		}

		// undefined

		else
			push_back(OcToken(*word));
	}

	//
	// determine section
	//

	SectionDesc desc(ktNoKey);
	bool newSect = false;

	OcStat::const_iterator token = begin();
	if (token->Tt() == ttKey) {

		// direct sections

		if (token->Stt() & (ktSet | ktAct | ktDo)) {

			desc.type = (KeyType)token->Stt();
			desc.beg = token->Pos();
			newSect = true;

			if (++token != end())
				type->AddIssue("Unexpected tokens following the section declaration", itWarning, token->Pos());
		}

		// interactive sections

		else if (token->Stt() & (ktSee | ktMod)) {

			desc.type = (KeyType)token->Stt();
			desc.beg = token->Pos();
			newSect = true;

			if (++token != end()) {

				if (token->Tt() == ttUnknown || token->Tt() == ttType) {

					desc.argument = *token;

					if (++token != end())
						type->AddIssue("Unexpected tokens following the desc declaration", itError, token->Pos());
				}
				else
					type->AddIssue("Expecting agent type name or agent reference collection", itError, token->Pos());
			}
			else
				type->AddIssue("Expecting agent type name or agent reference collection", itError, (--token)->EndPos());
		}
	}

	OcSection &section = type->GetSection(desc, newSect);
	if (!newSect) {
		section.push_back(*this);
		section.desc.end = this->back().EndPos();
	}
}

OcStat::~OcStat() {
	if (block)
		delete block;
}

OcBlock *OcStat::AddBlock() {
	if (block)
		delete block;
	return block = new OcBlock();
}

OcBlock *OcStat::Block() const {
	return block;
}

std::string OcStat::ToString(const std::string &pre) const {
	std::stringstream ss;

	for (OcStat::const_iterator token = begin(); token != end(); ++token)
		ss << token->ToString(pre) << std::endl;

	if (block)
		ss << block->ToString(pre + "    ");

	return ss.str();
}

bool OcStat::RequiresBlock() const {
	return size() && (front().IsKey(ktFlow) || front().IsOp(otFlow));
}

bool OcStat::IsBlockStart() const {
	return size() && front().IsStruct(stBlockBegin);
}

bool OcStat::IsBlockEnd() const {
	return size() && front().IsStruct(stBlockEnd);
}

bool OcStat::IsRequires() const {
	return size() && front().IsKey(ktRequires);
}

// Assumes statement already passed  the
// 'IsRequires()' check.
bool OcStat::CheckRequires(OcType *type) const {

	for (OcStat::const_iterator token = ++OcStat::const_iterator(begin()); token != end(); ++token)
		if (token->Tt() == ttUnknown)
			return false;
		else if (token->Tt() != ttType)
			type->AddIssue("Illegal type name, skipping token", itError, token->Pos());

	return true;
}

OpType OcStat::IsIfElse() {

	if (size() && front().IsOp(otIfElse))
		return (OpType)front().Stt();

	return otNoOp;
}

OpType OcStat::IsOp(int ot) {

	if (size() && front().IsOp((OpType)ot))
		return (OpType)front().Stt();

	return otNoOp;
}

OpType OcStat::IsRes(int ot) {

	if (size() && front().IsRes((OpType)ot))
		return (OpType)front().Stt();

	return otNoOp;
}

//
// generate
//

void OcStat::Generate(OcEnv *env, OcRoutine *r) {

	// generate stat instructions & calculate the required stat results chunk size

	int rC = 0;
	front().Generate(env, r, rC);
}

//
// debug
//

const OcToken *OcStat::GetTokenAt(int pos) const {

	for (const_iterator i = begin(); i != end(); ++i) {
		const OcToken *t = i->GetTokenAt(pos);
		if (t)
			return t;
	}

	return 0;
}
