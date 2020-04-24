#include "token.h"
#include "type.h"
#include <iostream>

#define FRONT	args.front()
#define	BACK	args.back()



// Checks for mismatches between types and usages
// of operators (functions) and their operands (arguments).
// Resolves identifiers: variables, function prototypes and functions.
bool OcToken::ResolveIdentifiers(OcType *type, const OcVarses &varses, KeyType section, OcType *other, OcVar *refs, bool &err) {

	//
	// reference operator '.'
	//

	if (tt == ttRes && stt == otReference) {

		bool stat = FRONT.tt & ttStatic;

		// todo: defective case when FRONT == BACK

		if (!FRONT.ResolveIdentifiers(type, varses, section, other, refs, err))
			return false;

		if (FRONT.IsVoid())
			OC_ISSUE_ERR_RETURN("Left operand type is void", FRONT.Pos())	// todo: change text

		if (BACK.tt != ttUnknown)
			OC_ISSUE_ERR_RETURN("Illegal member token '" + BACK + "'", Pos())

		proto = OcProto(BACK, FRONT.use, FRONT.cl, stat);
		if (!proto.owner->Contains(proto)) {	// not a proto

			OcVar *v = FRONT.cl->Var(BACK, stat);
			if (v) {	// is var member

				var = v;
				cl = var->cl;
				use = var->Usage(section, FRONT.IsKey(ktThis), FRONT.use);

				// make the varname token point to this as its info token
				BACK.infoToken = this;
			}
			else
				OC_ISSUE_ERR_RETURN("Class '" + FRONT.cl->Name() + "' has no" + (stat ? " static " : " ") + "member named " + BACK, Pos())
		}
	}

	//
	// other operators
	//

	else if (tt == ttRes) {

		bool stat = FRONT.tt & ttStatic, unary = (args.size() == 1);

		if (!FRONT.ResolveIdentifiers(type, varses, section, other, refs, err))
			return false;

		if (FRONT.IsVoid())
			OC_ISSUE_ERR_RETURN(((unary) ? "Operand type is void" : "Left operand type is void"), FRONT.Pos())

		if (!unary && !BACK.ResolveIdentifiers(type, varses, section, other, refs, err))
			return false;

		if (!unary && BACK.IsVoid())
			OC_ISSUE_ERR_RETURN("Right operand type is void", Pos())

		OcOp *o = FRONT.cl->Operator((OpType)stt, (unary ? 0 : BACK.cl), stat);
		if (o) {

			if (!FRONT.VerifyUse(type, o->use, section, refs))
				OC_ISSUE_ERR_RETURN("Operation caller misuse", FRONT.Pos())

			if (!BACK.VerifyUse(type, o->arg.use, section, refs))
				OC_ISSUE_ERR_RETURN("Operation callee misuse", BACK.Pos())

			op = o;
			cl = op->res.cl;
			use = (op->res.use & ptCaller) ? FRONT.use : op->res.use;
		}
		else {
			if (unary)
				OC_ISSUE_ERR_RETURN("Unary operator '" + *this + "' not defined for type '" + FRONT.cl->Name() + "'", Pos())
			else
				OC_ISSUE_ERR_RETURN("Operator '" + *this + "' not defined for types '" + FRONT.cl->Name() + "' and '" + BACK.cl->Name() + "'" + (stat ? " (stat)" : ""), Pos())
		}
	}

	//
	// functions
	//

	else if (tt == ttFuncCall) {	// '(' token 	[[FRONT.FRONT].[FRONT.BACK]](arg1, arg2, ...)

		if (FRONT != BACK && BACK.IsRes(otComma)) {	// two args, second is ',', flatten commas
			OcTokens a;
			BACK.FlattenCommas(a);
			args.pop_back();
			args.splice(args.end(), a);
		}

		for (OcTokens::iterator arg = args.begin(); arg != args.end(); ++arg)
			if (!arg->ResolveIdentifiers(type, varses, section, other, refs, err))
				return false;

		if (!FRONT.proto.valid)
			OC_ISSUE_ERR_RETURN("Cannot be used as a function", Pos())

		OcFuncSign sgn;
		for (OcTokens::iterator arg = ++args.begin(); arg != args.end(); ++arg) {
			sgn.push_back(arg->cl);
			if (arg->IsVoid())
				OC_ISSUE_ERR_RETURN("Argument type is void", arg->Pos())
		}

		OcFunc *f = FRONT.proto.owner->Func(FRONT.proto, sgn);
		if (f) {

			if (f->bannedFrom & section)
				OC_ISSUE_ERR_RETURN("Cannot call function '" + f->name + "' in this section", Pos())

			if (!FRONT.FRONT.VerifyUse(type, f->use, section, refs))
				OC_ISSUE_ERR_RETURN("Function caller misuse", Pos())

			OcArgs::iterator req = f->args.begin();
			for (OcTokens::iterator arg = ++args.begin(); arg != args.end(); ++arg, ++req)
				if (!arg->VerifyUse(type, req->use, section, refs))
					OC_ISSUE_ERR_RETURN("Argument misuse", arg->Pos())

			func = f;
			cl = func->res.cl;
			use = (func->res.use & ptCaller) ? FRONT.proto.use : func->res.use;

			// make the funcname token point to this one for info
			FRONT.BACK.infoToken = this;
		}
		else
			OC_ISSUE_ERR_RETURN("No matching function for call to '" + FRONT.proto.name + "'", Pos())
	}

	//
	// variables
	//

	else if (tt == ttUnknown) {

		const OcVar *v = varses.Contains(*this);
		if (v) {
			var = v;
			cl = var->cl;
			use = var->Usage(section, true, ptNone);
		}
		else
			OC_ISSUE_ERR_RETURN("'" + *this + "' was not declared in this scope", Pos())
	}

	//
	// this & other
	//

	else if (IsKey(ktThis)) {

		if (section & ktStatic)
			OC_ISSUE_ERR_RETURN("Cannot use 'this' keyword in a static section", Pos())

		cl = type;
		use = ptRead;
	}
	else if (IsKey(ktOther)) {

		if (section & ~ktInter)
			OC_ISSUE_ERR_RETURN("Cannot use 'other' keyword in a non-interactive section", Pos())

		cl = other;
		use = ptRead;
	}

	//
	// other stuff...
	//

	else if (tt != ttClass && tt != ttType && tt != ttLiteral)
		OC_ISSUE_ERR_RETURN("Illegal token", Pos())

	return true;
}
