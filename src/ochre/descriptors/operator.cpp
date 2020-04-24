#include "descriptors/class.h"



std::string OcOp::Description() const {

	std::string desc;

	// result

	if (res.cl)
		desc += (returnsThis ? "[this, " : "[") + res.cl->Name();
	else
		desc += "[void";

	desc += "] this ";

	switch (type) {
	case otReference:		desc += "."; break;
	case otIndex:			desc += ":"; break;

	case otPrefMinus:		desc += "-"; break;
	case otCast:			desc += "::"; break;

	case otMultiply:		desc += "*"; break;
	case otDivide:			desc += "/"; break;

	case otPlus:			desc += "+"; break;
	case otMinus:			desc += "-"; break;

	case otLess:			desc += "<"; break;
	case otLessOrEqual:		desc += "<="; break;
	case otGreater:			desc += ">"; break;
	case otGreaterOrEqual:	desc += ">="; break;

	case otEqual:			desc += "=="; break;
	case otNotEqual:		desc += "<>"; break;

	case otAnd:				desc += "&"; break;

	case otOr:				desc += "|"; break;

	case otAssign:			desc += "="; break;
	case otPlusAssign:		desc += "+="; break;
	case otMinusAssign:		desc += "-="; break;
	case otMultiplyAssign:	desc += "*="; break;
	case otDivideAssign:	desc += "/="; break;

	case otIf:				desc += "if"; break;
	case otElseIf:			desc += "else if"; break;
	case otWhile:			desc += "while"; break;
	default:				desc += "??"; break;
	}

	if (arg.cl)
		desc += " " + arg.cl->Name();

	return desc;
}
