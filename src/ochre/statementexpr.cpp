#include "statement.h"
#include "type.h"
#include <iterator>
#include <iostream>

#define OC_PARSE_RESTART	0xFFFFFFFF


/*
Parses statement using a precedence-climbing parser with restarting.
After parsing, type & use check is preformed on the resulting abstract
syntax tree (token/parse tree).
*/
void OcStat::ParseExpression(OcType *type, const OcVarses &varses, KeyType section, OcType *other, OcVar *refs) {

	err = false;

	if (begin()->IsOp(otElse)) {	// special 'else' case
		if (size() > 1)
			OC_ISSUE_ERR("Unexpected tokens following the 'else' keyword", begin()->Pos())
		return;
	}

	for (OcTokens::iterator token = begin(); token != end(); ++token)	// parse
		ParseOperand(type, *this, token, OC_PARSE_RESTART);

	for (OcTokens::iterator token = begin(); token != end(); ++token)	// type check
		token->ResolveIdentifiers(type, varses, section, other, refs, err);
}

/*
If any of the following functions returns false, the returned token is defective
and should not be consumed (must restart parse).
*/

bool OcStat::ParseOperand(OcType *type, OcTokens &tokens, OcTokens::iterator &t, int callerStt) {

	if (t->IsOperand())
		return ParseInfix(type, tokens, t, callerStt);

	if (t->IsPrefix() || t->IsOp(otMinus))
		return ParsePrefix(type, tokens, t, callerStt);

	if (t->IsStruct(stOpenBrace))
		return ParseSub(type, tokens, t, callerStt);

	OC_ISSUE_ERR("Expecting valid operand, operator or '('", t->Pos())
	return false;
}

bool OcStat::ParseInfix(OcType *type, OcTokens &tokens, OcTokens::iterator &t, int callerStt) {

	OcTokens::iterator i = t;
	if (++i == tokens.end())	// single token: legal end of statement
		return true;

	if (i->IsStruct(stCloseBrace))	// legal ')'
		return true;

	if (i->IsInfix()) {	// infix test
		if (!LeftPrecedes(callerStt, i->Stt())) {

			OcTokens::iterator a = i;	// other infix operand
			if (++a == tokens.end()) {
				OC_ISSUE_ERR("Missing infix operator second argument", i->EndPos())
				return true;
			}

			if (ParseOperand(type, tokens, a, i->Stt()))	// try to parse lookahead
				i->CommitInfix(tokens, t, a);
			else
				return false;	// todo: add exception for '.'

			return ParseOperand(type, tokens, t = i, callerStt);	// continue parsing
		}
	}
	else if (i->IsStruct(stOpenBrace)) {	// legal '(', function args list
		if (!LeftPrecedes(callerStt, otArgList)) {

			i->CommitFunc(tokens, t);	// transform '(' into function call

			OcTokens::iterator s = i;
			for (++s; s != tokens.end() && !s->IsStruct(stCloseBrace); ++s)
				ParseOperand(type, tokens, s, OC_PARSE_RESTART);

			if (s == tokens.end()) {	// no matching ')'
				t = --s;	// jump to last token for restart
				OC_ISSUE_ERR("Expecting argument list end ')'", t->EndPos())
				return false;
			}
			else if (std::distance(i, s) > 2) {
				t = s;	// jump to ')' for restart
				return false;
			}

			s = --tokens.erase(s);	// erase the ')' token
			if (std::distance(i, s) == 1)	// not '()' case
				i->CommitArgs(tokens, s);

			return ParseOperand(type, tokens, t = i, callerStt);	// continue parsing
		}
	}
	else
		OC_ISSUE_ERR("Expecting infix operator", i->Pos())

	return true;	// default case, when nothing has been done to token
}

bool OcStat::ParsePrefix(OcType *type, OcTokens &tokens, OcTokens::iterator &t, int callerStt) {

	if (t->IsOp(otMinus)) // '-' infix to prefix exception
		t->stt = otPrefMinus;

	OcTokens::iterator a = t;	// prefix argument
	if (++a == tokens.end()) {
		OC_ISSUE_ERR("Missing prefix operator argument", t->EndPos())
		return false;
	}
/*	todo: do!
	if (t->stt == otPrefMinus && a->tt == ttLiteral) {	// -100[.0] exception

	}
*/
	if (ParseOperand(type, tokens, a, t->Stt()))	// parse preceding operations
		t->CommitPrefix(tokens, a);
	else
		return false;

	return ParseOperand(type, tokens, t, callerStt);	// continue parsing
}

bool OcStat::ParseSub(OcType *type, OcTokens &tokens, OcTokens::iterator &t, int callerStt) {

	OcTokens::iterator s = t;
	for (++s; s != tokens.end() && !s->IsStruct(stCloseBrace); ++s)
		ParseOperand(type, tokens, s, OC_PARSE_RESTART);

	if (s == tokens.end()) {	// no matching ')'
		t = --s;	// jump to last token for restart
		OC_ISSUE_ERR("Expecting ')'", tokens.back().EndPos())
		return false;
	}
	else if (std::distance(t, s) == 1) {	// '()' case
		t = s;	// jump to ')' for restart
		OC_ISSUE_ERR("Expecting expression before ')'", s->Pos())
		return false;
	}
	else if (std::distance(t, s) == 2) {	// '(token)' case
		s = tokens.erase(s);	// erase the ')' token
		t = tokens.erase(t);	// erase the '(' token
		return ParseOperand(type, tokens, t, callerStt);	// continue parsing
	}

	t = s; //	jump to ')' for parse restart
	return false;
}

//
// misc
//

bool OcStat::LeftPrecedes(int t1, int t2) {
	int oc_prec = OC_PREC(t1) - OC_PREC(t2);

	if (oc_prec < 0)
		return true;
	else if (oc_prec > 0)
		return false;
	else
		return OC_ASSC(t1);
}
