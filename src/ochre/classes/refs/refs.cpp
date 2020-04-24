#include "classes/refs/refs.h"
#include "runtime/agent.h"
#include <string.h>



OcClass *CreateRefsClass(OcClass *templCl, OcClass *intCl, bool local) {

	OcClass *refsCl = new OcClass(local ? "local_refs" : "refs", sizeof(OcRefs), templCl, 0, 0, local ? rqLocalRefs : rqAgentRefs);

	refsCl->fns.push_back(OcFunc(
		"clear",
		"removes all agent references from the collection",
		OcArg(refsCl, ptReadModifyAppend),
		ptModify,
		RefsClear,
		true));

	refsCl->fns.push_back(OcFunc(
		"add",
		"adds the agent reference at the end of the collection",
		OcArg(refsCl, ptReadModifyAppend),
		ptAppend,
		RefsAdd,
		true));
	refsCl->fns.back().args.push_back(OcArg("item", templCl, ptRead));

	refsCl->fns.push_back(OcFunc(
		"count",
		"returns the number of agent references in the collection",
		OcArg(intCl, ptRead),
		ptRead,
		RefsCount));

	if (local) {

		refsCl->ops.push_back(OcOp(
			otIndex,
			"returns the element at specified index",
			OcArg(templCl, ptCaller),
			ptNone,
			OcArg(intCl, ptRead),
			RefsAt));

		refsCl->ops.push_back(OcOp(
			otAssign,
			"copies all elements from one agent reference colection to another",
			OcArg(refsCl, ptCaller),
			ptNone,
			OcArg(refsCl, ptRead),
			RefsAssign,
			true));
	}

	return refsCl;
}

//
// ops
//

OCF(RefsClear) {
	OCR(OcRefs, 0)->Clear();
}

OCF(RefsAdd) {
	OCR(OcRefs, 0)->Add(OCR(OcAgentPtr, 1));
}

OCF(RefsCount) {
	OCP(int, ARG_RES) = OCR(OcRefs, 0)->Count();
}

OCF(RefsAt) {
	OcAgentPtr *at = OCR(OcRefs, 0)->At(OCP(int, 1));

	if (at)
		*((OcAgentPtr **)v[2]) = at;
//	else
//		todo: exception!
}

OCF(RefsAssign) {
	OCR(OcRefs, 0)->Assign(OCR(OcRefs, 1));
}

//
// struct
//

void OcRefs::Clear() {

	if (count) {

		delete[] c;
		c = 0;
		count = 0;
	}
}

void OcRefs::Add(OcAgentPtr *p) {

	OcAgentPtr *n = new OcAgentPtr[count + 1];

	if (count) {
		memcpy(n, c, count * sizeof(OcAgentPtr));
		delete[] c;
	}

	c = n;
	c[count] = *p;
	++count;
}

void OcRefs::Assign(OcRefs *o) {

	if (count)
		delete[] c;

	count = o->count;

	c = new OcAgentPtr[count];
	memcpy(c, o->c, count * sizeof(OcAgentPtr));
}

OcAgentPtr *OcRefs::At(unsigned int i) {
	return (i < count) ? c + i : 0;
}
