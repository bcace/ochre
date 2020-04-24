#include "runtime/instruction.h"
#include "descriptors/class.h"
#include <cstring>



void OcIns::SetNextArg(int chk, int argO) {
	argReg[argNo] = argNo;	// argument register location (automatic)
	argChk[argNo] = chk;	// chunk where the argument value is located
	argOff[argNo] = argO;	// location of resulting value in chunk
	++argNo;
}

void OcIns::SetRes(int argO) {
	argReg[argNo] = ARG_RES;	// register pointer index (void pointer to result)
	argChk[argNo] = fcRes;		// frame chunk index where result is written
	argOff[argNo] = argO;		// offset of data in result chunk
	resNo = argNo;
	++argNo;
}

void OcIns::SetRef(int refI, int argO) {
	argReg[argNo] = argNo;								// frame pointer index (void pointer to arg)
	argChk[argNo] = fcFrame;							// frame chunk index where referenced chunk ptr will be stored
	argOff[argNo] = (fcRef + refI) * sizeof(void *);	// offset of referenced chunk ptr in frame chunk
	++argNo;
	// following is just a hint for the next instruction where to look for the chunk with data
	argReg[argNo] = ARG_RES;		// not used
	argChk[argNo] = fcRef + refI;	// frame chunk index where referenced agent's chunk is
	argOff[argNo] = argO;			// offset of data in referenced agent's chunk
	resNo = argNo;					// mark arg as result for instructions that use this ref, but do not execute (no ++argNo)
}

void OcIns::SetSelfRes() {
	resNo = 0;
}

void OcIns::SetJumpArgs(int argO) {
	argReg[argNo] = argNo;		// register pointer index (void pointer to result)
	argChk[argNo] = fcNext;		// frame chunk index where result is written
	argOff[argNo] = 0;			// offset of data in result chunk
	++argNo;
	argReg[argNo] = argNo;	// argument register location (automatic)
	argChk[argNo] = fcJump;	// chunk where the argument value is located
	argOff[argNo] = argO;	// location of resulting value in chunk
	++argNo;
}

void OcIns::MapArgs(OcFrame &f) {
	for (int a = 0; a < argNo; ++a)
		f.v[argReg[a]] = (void *)(f.c[argChk[a]] + argOff[a]);
}

int OcIns::ResChk() {
	return argChk[resNo];
}

// Returns result offset.
int OcIns::ResOff() {
	return argOff[resNo];
}

void OcIns::ToOutput(std::stringstream &out, CHUNK *jmp) {

	out << "ins " << id << ":";

	for (int a = 0; a < argNo; ++a) {

		out << " (";

		switch (argChk[a]) {
			case fcRes:		out << "RES "; break;
			case fcLocal:	out << "LOC "; break;
			case fcLit:		out << "LIT "; break;
			case fcThis:	out << "THS "; break;
			case fcOther:	out << "OTH "; break;
			case fcType:	out << "TYP "; break;
			case fcPools:	out << "PLS "; break;
			case fcNext:	out << "NXT "; break;
			case fcJump:	out << "JMP "; break;
			default:;
		}

		if (argChk[a] == fcJump)
			out << (*((OcIns **)(jmp + argOff[a])))->id << "@";

		out << argOff[a] << ")";
	}

	out <<
		" res_arg: " << resNo <<
		" next_ins: " << (next ? next->id : 0) <<
		std::endl;
}

//
// routine
//

OcRoutine::OcRoutine(int _beg, int _end) :
	lit(0), jmp(0), begPos(_beg), endPos(_end), breakIns(0) {}

OcRoutine::~OcRoutine() {
	ChkDelete(&lit, litScheme);
	ChkDelete(&jmp, jmpScheme);
}

void OcRoutine::Clear() {
	clear();
	litScheme.Clear();
	jmpScheme.Clear();
	locScheme.Clear();
	resScheme.Clear();
}

OcIns *OcRoutine::NewInstruction(int pos) {
	push_back(OcIns(size(), pos));
	return &back();
}

OcIns *OcRoutine::FirstInstruction() {
	return size() ? &front() : 0;
}

OcIns *OcRoutine::LastInstruction() {
	return size() ? &back() : 0;
}

// Connects 'next' instruction pointers and
// sets the appropriate register function.
void OcRoutine::ConnectInstructions() {

	OcIns *ins = 0;
	for (OcRoutine::iterator i = begin(); i != end(); ++i) {

		if (ins) {
			if (ins->next)
				ins = 0;
			else
				ins = ins->next = &(*i);
		}
		else
			ins = &(*i);
	}
}

/* Adds a literal to the literals chunk and returns
the literal's offset in the chunk. */
int OcRoutine::AddLiteral(OcClass *cl, void *l) {

	int off = litScheme.Add(cl);

	ChkAppendValue(&lit, off, cl->size, l);

	return off;
}

/* Adds a jump address to the jumps chunk and returns
the jump's offset in the chunk. */
int OcRoutine::AddJump(OcIns *i) {

	int off = jmpScheme.Add(sizeof(OcIns *));

	ChkAppendValue(&jmp, off, sizeof(OcIns *), &i);

	return off;
}

/* Set first instruction after 'pos' as break instruction.
Use negative pos value to unset break instruction. */
void OcRoutine::SetBreakInstruction(int pos) {

	breakIns = 0;

	if (pos < begPos || pos >= endPos)
		return;

	unsigned min = 100000;
	for (OcRoutine::iterator i = begin(); i != end(); ++i) {
		unsigned d = i->pos - pos;
		if (d < min) {
			min = d;
			breakIns = &(*i);
		}
	}
}

//
// generic instructions
//

OCN(NullOp) {}

OCF(Jump) {
	OCP(OcIns *, 0) = OCP(OcIns *, 1);
}
