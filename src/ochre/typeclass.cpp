#include "classes/refs/refs.h"
#include "classes/refs/minrefs.h"
#include "runtime/agent.h"
#include "ochre.h"
#include <cstring>



void OcType::InitTypeClass(Ochre *ochre) {
	OcClass *intCl = ochre->Class("int");
	OcClass *realCl = ochre->Class("real");
	OcClass *pointCl = ochre->Class("point");
	OcClass *boolCl = ochre->Class("bool");

	// static funcs

	sfns.push_back(OcFunc("new", "", OcArg(this, ptReadModify), ptNone, NewAgent, false, ktInter));

	sfns.push_back(OcFunc("new", "", OcArg(this, ptReadModify), ptNone, NewAgentPointPos, false, ktInter));
	sfns.back().args.push_back(OcArg("initial position", pointCl, ptRead));

	sfns.push_back(OcFunc("new", "", OcArg(this, ptReadModify), ptNone, NewAgentRealPos, false, ktInter));
	sfns.back().args.push_back(OcArg("initial position x", realCl, ptRead));
	sfns.back().args.push_back(OcArg("initial position y", realCl, ptRead));
	sfns.back().args.push_back(OcArg("initial position z", realCl, ptRead));

	sfns.push_back(OcFunc("begin", "", OcArg(this, ptReadModify), ptNone, Begin, false, ktInter | ktAct));
	sfns.push_back(OcFunc("next", "", OcArg(this, ptReadModify), ptNone, Next, false, ktInter | ktAct));

	// instance funcs

	fns.push_back(OcFunc("delete", "", OcArg(0, ptNone), ptNone, DeleteAgent, true, ktInter));
	fns.push_back(OcFunc("isNull", "", OcArg(boolCl, ptRead), ptRead, IsNull));

	// instance ops

	ops.push_back(OcOp(otEqual, "", OcArg(boolCl, ptRead), ptRead, OcArg(this, ptRead), RefEqual));
	ops.push_back(OcOp(otAssign, "", OcArg(this, ptCaller), ptWrite, OcArg(this, ptRead), AssignRef));
	ops.push_back(OcOp(otIf, "", OcArg(0, ptNone), ptRead, ReferenceIf, true));
	ops.push_back(OcOp(otWhile, "", OcArg(0, ptNone), ptRead, ReferenceIf, true));

	ochre->AddClass(CreateRefsClass(this, intCl, false));
	ochre->AddClass(CreateRefsClass(this, intCl, true));
	ochre->AddClass(CreateMinRefsClass(this, intCl, realCl));
}

// static funcs

OCF(NewAgent) {
	OCP(OcAgentPtr, ARG_RES) = OCP(OcPool *, 0)->Grid().New(OCP(int, ARG_THR));
}

OCF(NewAgentPointPos) {
	OCP(OcAgentPtr, ARG_RES) = OCP(OcPool *, 0)->Grid().New(OCP(int, ARG_THR), OCP(OcPoint, 1));
}

OCF(NewAgentRealPos) {
	OCP(OcAgentPtr, ARG_RES) = OCP(OcPool *, 0)->Grid().New(OCP(int, ARG_THR), OcPoint(OCP(double, 1), OCP(double, 2), OCP(double, 3)));
}

OCF(Begin) {
	OCP(OcAgentPtr, ARG_RES) = OCP(OcPool *, 0)->Grid().Begin();
}

OCF(Next) {
	OCP(OcAgentPtr, ARG_RES) = OCP(OcPool *, 0)->Grid().Next();
}

// instance funcs

OCF(DeleteAgent) {
	OcAgentPtr ref = OCP(OcAgentPtr, 0);
	if (ref.IsNull())
		return;

	ref.agent->Delete();
}

OCF(IsNull) {
	OCP(bool, ARG_RES) = OCP(OcAgentPtr, 0).IsNull();
}

// instance ops

OCF(GetMember) {
	OcAgentPtr ref = OCP(OcAgentPtr, 0);
	if (ref.IsNull())
		return;	// todo: should be exception

	*((CHUNK **)v[1]) = ref.agent->c;
}

OCF(RefEqual) {
	OCP(bool, ARG_RES) = OCP(OcAgentPtr, 0) == OCP(OcAgentPtr, 1);
}

OCF(AssignRef) {
	OCP(OcAgentPtr, ARG_RES) = OCP(OcAgentPtr, 0) = OCP(OcAgentPtr, 1);
}

OCF(ReferenceIf) {
	if (OCP(OcAgentPtr, 0).IsNull())
		OCP(void *, 1) = OCP(void *, 2);
}

// vector ops
/*
OCD(RefVectorDestructor) {
	((OcVector *)v)->Clear(sizeof(OcAgentPtr), 0);
}

OCF(RefVectorAdd) {
	OCR(OcVector, 0)->Add(sizeof(OcAgentPtr), OCR(OcAgentPtr, 1), 0);
}

OCF(RefVectorAt) {
	CHUNK *at = OCR(OcVector, 0)->At(sizeof(OcAgentPtr), OCP(int, 1));

	if (at)
		*((CHUNK **)v[2]) = at;
//	else
//		todo: exception!
}

OCF(RefVectorClear) {
	OCR(OcVector, 0)->Clear(sizeof(OcAgentPtr), 0);
}
*/
