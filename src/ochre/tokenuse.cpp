#include "token.h"
#include "type.h"
#include <sstream>



// More detailed error reporting for use mismatches.
bool OcToken::VerifyUse(OcType *type, UsageType user, KeyType context, OcVar *refs) {

	// check the special 'specific section argument' rule first

	if (refs && var && refs == var && (user & ptModify))
		OC_ISSUE_RETURN("Cannot modify agent reference collection currently being iterated", Pos());

	// verify use

	UsageType problem = ptNone;

	if ((user & ptRead) && (use & ptRead) == 0)
		problem = ptRead;
	else if ((user & ptWrite) && (use & ptWrite) == 0)
		problem = ptWrite;
	else if ((user & ptModify) && (use & ptModify) == 0)
		problem = ptModify;
	else if ((user & ptAppend) && (use & ptAppend) == 0)
		problem = ptAppend;

	if (problem == ptNone)
		return true;

	std::stringstream error;

	error << "Cannot";

	switch (problem) {
		case ptRead:	error << " read from"; break;
		case ptWrite:	error << " write to"; break;
		case ptModify:	error << " modify"; break;
		case ptAppend:	error << " append to"; break;
		default:		error << " (unhandled use case)"; break;
	}

	if (var) {
		switch (var->owner) {
			case owtLocal:	error << " local variable"; break;
			case owtObject:	error << " object data member"; break;
			case owtAgent:
				if (use & ptReferenced)
					error << " referenced";
				switch (var->mod) {
					case vmPrivate:	error << " private"; break;
					case vmPublic:	error << " public"; break;
					case vmConst:	error << " const"; break;
					default:		error << " (unhandled modifier)"; break;
				}
				error << " agent variable";
				break;
			case owtStatic:	error << " static variable"; break;
			default:		error << " (unhandled owner)"; break;
		}
	}
	else
		error << " this token";

	error << " within the";

	switch (context) {
		case ktNoKey:		error << " declaration section"; break;
		case ktSet:			error << " 'set' section"; break;
		case ktSee:			error << " 'see' section"; break;
		case ktMod:			error << " 'mod' section"; break;
		case ktAct:			error << " 'act' section"; break;
		case ktDo:			error << " 'do' section"; break;
		default:			error << " (unhandled section)"; break;
	}

	OC_ISSUE_RETURN(error.str(), Pos());
}
