#include "block.h"
#include <iostream>



void OcBlock::Generate(OcEnv *env, OcRoutine *r) {

	OcInses ifs;
	for (OcBlock::iterator stat = begin(); stat != end(); ++stat) {

		if (stat->IsRes(otElseIf) == otNoOp && stat->IsOp(otElse) == otNoOp)
			ConnectIfElses(ifs, r);

		OpType t = stat->IsRes(otIf | otElseIf | otWhile);

		bool r_empty = r->empty();
		OcRoutine::iterator f;
		if (!r_empty)
			f = --r->end();

		stat->Generate(env, r);
		OcIns *eval = r->LastInstruction();
		OcIns *evalExp = r_empty ? r->FirstInstruction() : &(*(++f));

		if (stat->Block())
			stat->Block()->Generate(env, r);

		if (t == otWhile) {

			r->NewInstruction(0)->exec = Jump;
			r->LastInstruction()->SetJumpArgs(r->AddJump(evalExp));

			eval->SetJumpArgs(r->AddJump(r->NewInstruction(0)));
		}
		else if (t == otIf || t == otElseIf) {

			r->NewInstruction(0)->exec = Jump;
			ifs.push_back(r->LastInstruction());

			eval->SetJumpArgs(r->AddJump(r->NewInstruction(0)));
		}
	}

	ConnectIfElses(ifs, r);
}

void OcBlock::ConnectIfElses(OcInses &ifs, OcRoutine *r) {

	if (ifs.size()) {

		r->NewInstruction(0);
		for (OcInses::iterator i = ifs.begin(); i != ifs.end(); ++i)
			(*i)->SetJumpArgs(r->AddJump(r->LastInstruction()));

		ifs.clear();
	}
}
