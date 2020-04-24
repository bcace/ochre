#include "token.h"
#include "context.h"
#include <sstream>



OcToken::OcToken() :
	OcString("", 0), tt(0), stt(0), itt(0), cl(0), use(ptNone), func(0), var(0), op(0), infoToken(0), ins(0),
	memberOff(0), memberOwner(owtUndefined) {}

OcToken::OcToken(const OcString &str) :
	OcString(str), tt(1), stt(0), itt(0), cl(0), use(ptNone), func(0), var(0), op(0), infoToken(0), ins(0),
	memberOff(0), memberOwner(owtUndefined) {}

OcToken::OcToken(const OcString &str, int _tt) :
	OcString(str), tt(_tt), stt(0), itt(0), cl(0), use(ptNone), func(0), var(0), op(0), infoToken(0), ins(0),
	memberOff(0), memberOwner(owtUndefined) {}

OcToken::OcToken(const OcString &str, int _tt, int _stt) :
	OcString(str), tt(_tt), stt(_stt), itt(0), cl(0), use(ptNone), func(0), var(0), op(0), infoToken(0), ins(0),
	memberOff(0), memberOwner(owtUndefined) {}

OcToken::OcToken(const OcString &str, int _tt, OcClass *_cl) :
	OcString(str), tt(_tt), stt(0), itt(0), cl(_cl), use(ptNone), func(0), var(0), op(0), infoToken(0), ins(0),
	memberOff(0), memberOwner(owtUndefined) {}

OcToken::OcToken(const OcString &str, int _tt, OcClass *_cl, int _intL) :
	OcString(str), tt(_tt), stt(0), itt(0), cl(_cl), use(ptRead), func(0), var(0), op(0), infoToken(0), intL(_intL), ins(0),
	memberOff(0), memberOwner(owtUndefined) {}

OcToken::OcToken(const OcString &str, int _tt, OcClass *_cl, bool _boolL) :
	OcString(str), tt(_tt), stt(0), itt(0), cl(_cl), use(ptRead), func(0), var(0), op(0), infoToken(0), boolL(_boolL), ins(0),
	memberOff(0), memberOwner(owtUndefined) {}

OcToken::OcToken(const OcString &str, int _tt, OcClass *_cl, double _realL) :
	OcString(str), tt(_tt), stt(0), itt(0), cl(_cl), use(ptRead), func(0), var(0), op(0), infoToken(0), realL(_realL), ins(0),
	memberOff(0), memberOwner(owtUndefined) {}

OcToken::OcToken(const OcString &str, int _tt, OcClass *_cl, const std::string &_textL) :
	OcString(str), tt(_tt), stt(0), itt(0), cl(_cl), use(ptRead), func(0), var(0), op(0), infoToken(0), textL(_textL), ins(0),
	memberOff(0), memberOwner(owtUndefined) {}

std::string OcToken::ToString(const std::string &pre) const {
	std::stringstream ss;

	ss << pre;
	switch (tt) {
		case ttUnknown:		ss << "UN~" << *this; break;
		case ttKey:			ss << "KY~" << *this; break;
		case ttLiteral:		ss << "LT~"; break;
		case ttClass:		ss << "CL~" << *this; break;
		case ttType:		ss << "TP~" << *this << ((cl) ? "" : "?"); break;
		case ttRes:			ss << "RS~" << *this; break;
		case ttFuncCall:	ss << "FN~CALL"; break;
		case ttStruct:		ss << "ST~" << *this; break;
		case ttOp:			ss << "OP~" << *this; break;
		default: 			ss << "UNHANDLED_TT"; break;
	}

	if (args.size()) {
		for (OcTokens::const_iterator arg = args.begin(); arg != args.end(); ++arg)
			ss << std::endl << arg->ToString(pre + "    ");
	}

	return ss.str();
}

int OcToken::Tt() const {
	return tt;
}

int OcToken::Stt() const {
	return stt;
}

OcClass *OcToken::Cl() const {
	return cl;
}

bool OcToken::IsStruct(StructType st) const {
	return (tt == ttStruct) && (stt & st);
}

bool OcToken::IsKey(KeyType kt) const {
	return (tt == ttKey) && (stt & kt);
}

bool OcToken::IsOp(OpType ot) const {
	return (tt == ttOp) && (OC_BNDL(stt) == OC_BNDL(ot)) && (OC_ENUM(stt) & OC_ENUM(ot));
}

bool OcToken::IsRes(OpType ot) const {
	return (tt == ttRes) && (OC_BNDL(stt) == OC_BNDL(ot)) && (OC_ENUM(stt) & OC_ENUM(ot));
}

bool OcToken::IsOperand() const {
	return tt & ttOperand;
}

bool OcToken::IsPrefix() const {
	return (tt == ttOp) && OC_PREF(stt);
}

bool OcToken::IsPrefixRes() const {
	return (tt == ttRes) && OC_PREF(stt);
}

bool OcToken::IsInfix() const {
	return (tt == ttOp) && !OC_PREF(stt);
}

bool OcToken::IsInfixRes() const {
	return (tt == ttRes) && !OC_PREF(stt);
}

bool OcToken::IsVoid() const {
	return cl == 0;
}

const OcToken *OcToken::Info() const {
	return infoToken ? infoToken : this;
}

void OcToken::CommitElse() {
	tt = ttRes;
}

void OcToken::CommitPrefix(OcTokens &tokens, OcTokens::iterator &arg) {
	tt = ttRes;
	args.splice(args.end(), tokens, arg);
}

void OcToken::CommitInfix(OcTokens &tokens, OcTokens::iterator &arg1, OcTokens::iterator &arg2) {
	tt = ttRes;
	args.splice(args.end(), tokens, arg1);
	args.splice(args.end(), tokens, arg2);
}

void OcToken::CommitFunc(OcTokens &tokens, OcTokens::iterator &arg) {
	tt = ttFuncCall;
	args.splice(args.end(), tokens, arg);
}

void OcToken::CommitArgs(OcTokens &tokens, OcTokens::iterator &arg) {
	args.splice(args.end(), tokens, arg);
}

void OcToken::FlattenCommas(OcTokens &f) {

	if (args.front().IsRes(otComma)) {
		f.splice(f.begin(), args, --args.end());	// splice arg from end
		args.front().FlattenCommas(f);				// process ','
	}
	else
		f.splice(f.begin(), args);					// splice both ending arg, arg
}

void OcToken::SetTemplateInstance(OcClass *_cl) {
	cl = _cl;
}
