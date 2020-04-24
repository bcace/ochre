#include "type.h"
#include "runtime/env.h"
#include "classes/text.h"
#include <iostream>



// Recursively generates instructions from the
// syntax tree. Assumes parse executed with no issues.
/*
todo:
	- index op
*/
void OcToken::Generate(OcEnv *env, OcRoutine *r, int &refCount) {

	// resolve args first

	for (OcTokens::iterator arg = args.begin(); arg != args.end(); ++arg)
		arg->Generate(env, r, refCount);

	// operations

	if (tt == ttRes) {

		// references

		if (stt == otReference) {

			if (var) {

				if (var->owner == owtAgent) {	// p->x

					// if 'this.x' or 'other.x' do not create instruction

					if (args.front().IsKey((KeyType)(ktThis | ktOther)))
						return;

					// create referencing instruction

					ins = r->NewInstruction(Pos());
					args.front().SetInstructionArg(env, r, ins);

					ins->exec = GetMember;
					ins->SetRef(refCount, var->off);
					++refCount;	// todo: test if exceeds frame capacity
				}
				else if (var->owner == owtObject) {	// p.x

					// don't create a new instruction, forward the old one

					ins = args.front().ins;

					// determine owner & offset

					if (ins) {
						memberOwner = owtResult;
						memberOff = args.front().memberOff + var->off;
					}
					else if (args.front().var && args.front().var->owner != owtObject) {
						memberOwner = args.front().var->owner;
						memberOff = args.front().var->off + var->off;
					}
					else {
						memberOwner = args.front().memberOwner;
						memberOff = args.front().memberOff + var->off;
					}
				}
				else if (var->owner == owtStatic) {	// T.x

					// don't create a new instruction, forward the old one

					ins = args.front().ins;
				}
				else
					std::cerr << "OcToken::Generate() var->owner not handled" << std::endl;
			}
			else if (proto.valid) {
			}
			else
				std::cerr << "OcToken::Generate() reference not var or proto" << std::endl;
		}

		// index op

		else if (stt == otIndex) {

			ins = r->NewInstruction(Pos());
			for (OcTokens::iterator a = args.begin(); a != args.end(); ++a)
				a->SetInstructionArg(env, r, ins);

			ins->exec = op->exec;
			ins->SetRef(refCount, 0);
			++refCount;	// todo: test if exceeds frame capacity
		}

		// regular ops

		else {

			ins = r->NewInstruction(Pos());
			for (OcTokens::iterator a = args.begin(); a != args.end(); ++a)
				a->SetInstructionArg(env, r, ins);

			ins->exec = op->exec;

			if (op->res.cl) {
				if (op->returnsThis)
					ins->SetSelfRes();
				else
					ins->SetRes(r->resScheme.Add(op->res.cl));
			}
		}
	}

	// funcs

	else if (tt == ttFuncCall) {

		ins = r->NewInstruction(Pos());

		// implicit 'this' argument

		args.front().args.front().SetInstructionArg(env, r, ins);

		// explicit arguments

		for (OcTokens::iterator a = ++args.begin(); a != args.end(); ++a)
			a->SetInstructionArg(env, r, ins);

		ins->exec = func->exec;

		if (func->res.cl) {
			if (func->returnsThis)
				ins->SetSelfRes();
			else
				ins->SetRes(r->resScheme.Add(func->res.cl));
		}
	}
}

void OcToken::SetInstructionArg(OcEnv *env, OcRoutine *r, OcIns *i) {

	// operators

	if (tt == ttRes) {

		// reference

		if (stt == otReference) {

			if (ins)	// p->x
				i->SetNextArg(ins->ResChk(), ins->ResOff() + memberOff);
			else if (var->owner == owtObject) {	// p.x

				if (memberOwner == owtLocal)
					i->SetNextArg(fcLocal, memberOff);
				else if (memberOwner == owtStatic)
					i->SetNextArg(fcType, memberOff);
				else if (memberOwner == owtAgent)
					i->SetNextArg(fcThis, memberOff);
				else
					std::cerr << "SetInstructionArg() unhandled owner error" << std::endl;
			}
			else if (var->owner == owtAgent) {	// 'this.x' or 'other.x'

				if (args.front().IsKey(ktThis))
					i->SetNextArg(fcThis, var->off);
				else if (args.front().IsKey(ktOther))
					i->SetNextArg(fcOther, var->off);
				else
					std::cerr << "SetInstructionArg() reference/owner error" << std::endl;
			}
			else if (var->owner == owtStatic) {	// T.x

				i->SetNextArg(fcStatic + env->Pool(((OcType *)args.front().cl)->Id())->Index(), var->off);
			}
			else
				std::cerr << "SetInstructionArg() reference/owner error" << std::endl;
		}

		// regular operator

		else
			i->SetNextArg(ins->ResChk(), ins->ResOff() + memberOff);
	}
	else if (tt == ttUnknown) {

		// variables

		if (var) {

			if (var->owner == owtLocal)
				i->SetNextArg(fcLocal, var->off);
			else if (var->owner == owtStatic)
				i->SetNextArg(fcType, var->off);
			else if (var->owner == owtAgent)
				i->SetNextArg(fcThis, var->off);
		}
	}

	// funcs

	else if (tt == ttFuncCall) {

		if (ins)
			i->SetNextArg(ins->ResChk(), ins->ResOff() + memberOff);
		else
			std::cerr << "SetInstructionArg() error: func with no instruction set" << std::endl;
	}

	// this & other

	else if (tt == ttKey) {

		if (stt == ktThis)
			i->SetNextArg(fcThis, 0);
		else if (stt == ktOther)
			i->SetNextArg(fcOther, 0);
		else
			std::cerr << "SetInstructionArg() error: keyword as argument not handled" << std::endl;
	}

	// literals

	else if (tt == ttLiteral) {

		if (cl->Name() == "int")
			i->SetNextArg(fcLit, r->AddLiteral(cl, &intL));
		else if (cl->Name() == "bool")
			i->SetNextArg(fcLit, r->AddLiteral(cl, &boolL));
		else if (cl->Name() == "real")
			i->SetNextArg(fcLit, r->AddLiteral(cl, &realL));
		else if (cl->Name() == "text") {
			OcText text(textL);
			i->SetNextArg(fcLit, r->AddLiteral(cl, &text));
		}
		else
			std::cerr << "SetInstructionArg() error: literal type not handled: " << cl->Name() << std::endl;
	}

	// types (static type funcs)

	else if (tt == ttType) {
		i->SetNextArg(fcPools, env->Pool(((OcType *)cl)->Id())->Offset());
	}

	// classes (static class funcs)

	else if (tt == ttClass) {
	}

	// unhandled

	else
		std::cerr << "Unhandled SetInstructionArg() case" << std::endl;
}
