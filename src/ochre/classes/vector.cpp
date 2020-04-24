#include "classes/vector.h"
#include <string.h>



OcClass *CreateVectorClass(OcClass *templCl, OcClass *intCl, CTOR dltor, COPY cptor, EXEC addExec, EXEC atExec, EXEC clearExec) {

	OcClass *vectorCl = new OcClass("vector", sizeof(OcVector), templCl, dltor, cptor);

	if (clearExec)
		vectorCl->fns.push_back(OcFunc("clear", "removes all elements from the vector", OcArg(vectorCl, ptReadModifyAppend), ptModify, clearExec, true));

	if (addExec) {
		vectorCl->fns.push_back(OcFunc("add", "adds a new element at the end of the vector", OcArg(vectorCl, ptReadModifyAppend), ptAppend, addExec, true));
		vectorCl->fns.back().args.push_back(OcArg("item", templCl, ptRead));
	}

	if (atExec)
		vectorCl->ops.push_back(OcOp(otIndex, "returns the element at specified index", OcArg(templCl, ptCaller), ptNone, OcArg(intCl, ptRead), atExec));

	// non-template members

	vectorCl->fns.push_back(OcFunc("count", "returns the number of elements in the vector", OcArg(intCl, ptRead), ptRead, VecCount));

	vectorCl->ops.push_back(OcOp(otIf, "tests if vector is not empty", OcArg(0, ptNone), ptRead, VecIf, true));
	vectorCl->ops.push_back(OcOp(otWhile, "loops while vector is not empty", OcArg(0, ptNone), ptRead, VecIf, true));

	return vectorCl;
}

// non-template members

OCF(VecCount) {
	OCP(int, ARG_RES) = OCR(OcVector, 0)->Count();
}

OCF(VecIf) {
	if (OCR(OcVector, 0)->Count() == 0)
		OCP(void *, 1) = OCP(void *, 2);
}

//
// struct
//

OcVector::OcVector() : count(0), c(0) {}

void OcVector::Clear(int s, CTOR destr) {

	if (count) {

		if (destr)
			for (int i = 0; i < count; ++i)
				destr(c + i * s);

		delete[] c;
		c = 0;
		count = 0;
	}
}

void OcVector::Add(int s, void *v, COPY copy) {

	CHUNK *n = new CHUNK[(count + 1) * s];

	if (count) {
		memcpy(n, c, count * s);
		delete[] c;
	}

	c = n;

	if (copy)
		copy(v, c + count * s);
	else
		memcpy(c + count * s, v, s);

	++count;
}

CHUNK *OcVector::At(int s, int i) {

	if (i < count)
		return c + i * s;

	return 0;	// todo: should be exception
}

int OcVector::Count() {
	return count;
}
