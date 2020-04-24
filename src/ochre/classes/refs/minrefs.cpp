#include "classes/refs/minrefs.h"
#include "runtime/agent.h"
#include <string.h>
#include <limits>



OcClass *CreateMinRefsClass(OcClass *templCl, OcClass *intCl, OcClass *realCl) {

	OcClass *refsCl = new OcClass("min_refs", sizeof(OcMinRefs), templCl, 0, 0, rqAgentRefs);

	refsCl->fns.push_back(OcFunc(
		"clear",
		"removes all agent references from the collection",
		OcArg(refsCl, ptReadModifyAppend),
		ptModify,
		MinRefsClear,
		true));

	refsCl->fns.push_back(OcFunc(
		"add",
		"adds the agent reference at the end of the collection",
		OcArg(refsCl, ptReadModifyAppend),
		ptAppend,
		MinRefsAdd,
		true));
	refsCl->fns.back().args.push_back(OcArg("item", templCl, ptRead));
	refsCl->fns.back().args.push_back(OcArg("grade", realCl, ptRead));

	refsCl->fns.push_back(OcFunc(
		"count",
		"returns the number of agent references in the collection",
		OcArg(intCl, ptRead),
		ptRead,
		MinRefsCount));

	return refsCl;
}

OCF(MinRefsClear) {
	OCR(OcMinRefs, 0)->Clear();
}

OCF(MinRefsAdd) {
	OCR(OcMinRefs, 0)->Add(OCR(OcAgentPtr, 1), OCP(double, 2));
}

OCF(MinRefsCount) {
	OCP(int, ARG_RES) = OCR(OcMinRefs, 0)->Count();
}

//
// struct
//

OcMinRefs::OcMinRefs() :
	min(std::numeric_limits<double>::max()) {}

void OcMinRefs::Clear() {

	if (count) {

		delete[] c;
		c = 0;
		count = 0;
	}

	min = std::numeric_limits<double>::max();
}

void OcMinRefs::Add(OcAgentPtr *p, double v) {

	if (v < min) {

		min = v;

		if (count != 1) {

			if (count)
				delete[] c;

			c = new OcAgentPtr[1];
			count = 1;
		}

		c[0] = *p;
	}
	else if (v == min) {

		OcAgentPtr *n = new OcAgentPtr[count + 1];

		if (count) {
			memcpy(n, c, count * sizeof(OcAgentPtr));
			delete[] c;
		}

		c = n;
		c[count] = *p;
		++count;
	}
}
