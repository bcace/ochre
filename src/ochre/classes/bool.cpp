#include "classes/bool.h"
#include "classes/vector.h"
#include "descriptors/class.h"
#include "ochre.h"



OcClass *CreateBoolClass() {
	return new OcClass("bool", sizeof(char));
}

void InitBoolClass(Ochre *ochre) {
	OcClass *boolCl = ochre->Class("bool");
	OcClass *intCl = ochre->Class("int");
	OcClass *realCl = ochre->Class("real");

	// static ops

	boolCl->sops.push_back(OcOp(otCast,
		"cast operator, casts an int value to bool value\n`bool b = bool::4`\nwhere only 0 is cast to false",
		OcArg(boolCl, ptRead), ptNone, OcArg(intCl, ptRead), BoolIntCast));
	boolCl->sops.push_back(OcOp(otCast,
		"cast operator, casts a real value to bool value\n`bool b = bool::4.0`\nwhere only 0.0 is cast to false",
		OcArg(boolCl, ptRead), ptNone, OcArg(realCl, ptRead), BoolRealCast));

	// instance ops

	boolCl->ops.push_back(OcOp(otPrefMinus, "returns the inverted value", OcArg(boolCl, ptRead), ptRead, BoolNegate));

	boolCl->ops.push_back(OcOp(otEqual, "returns true if *this* and argument values are equal", OcArg(boolCl, ptRead), ptRead, OcArg(boolCl, ptRead), BoolEqual));
	boolCl->ops.push_back(OcOp(otNotEqual, "returns true if *this* and argument values are not equal", OcArg(boolCl, ptRead), ptRead, OcArg(boolCl, ptRead), BoolNotEqual));

	boolCl->ops.push_back(OcOp(otAnd, "returns true if both *this* and argument values are true", OcArg(boolCl, ptRead), ptRead, OcArg(boolCl, ptRead), BoolAnd));
	boolCl->ops.push_back(OcOp(otOr, "returns true if either *this* or argument value is true", OcArg(boolCl, ptRead), ptRead, OcArg(boolCl, ptRead), BoolOr));

	boolCl->ops.push_back(OcOp(otAssign, "assigns argument value to *this* and returns *this*", OcArg(boolCl, ptCaller), ptWrite, OcArg(boolCl, ptRead), BoolAssign, true));

	boolCl->ops.push_back(OcOp(otIf, "tests if *this* value is true", OcArg(0, ptNone), ptRead, BoolIf, true));
	boolCl->ops.push_back(OcOp(otElseIf, "tests if *this* value is true", OcArg(0, ptNone), ptRead, BoolIf, true));
	boolCl->ops.push_back(OcOp(otWhile, "loops while *this* value is true  ", OcArg(0, ptNone), ptRead, BoolIf, true));

	ochre->AddClass(CreateVectorClass(boolCl, intCl, BoolVectorDestructor, 0, BoolVectorAdd, BoolVectorAt, BoolVectorClear));
}

// static ops

OCF(BoolIntCast) {
	OCP(bool, ARG_RES) = (bool)OCP(int, 0);
}

OCF(BoolRealCast) {
	OCP(bool, ARG_RES) = (bool)OCP(double, 0);
}

// instance ops

OCF(BoolNegate) {
	OCP(bool, ARG_RES) = !OCP(bool, 0);
}

OCF(BoolEqual) {
	OCP(bool, ARG_RES) = OCP(bool, 0) == OCP(bool, 1);
}

OCF(BoolNotEqual) {
	OCP(bool, ARG_RES) = OCP(bool, 0) != OCP(bool, 1);
}

OCF(BoolAnd) {
	OCP(bool, ARG_RES) = OCP(bool, 0) && OCP(bool, 1);
}

OCF(BoolOr) {
	OCP(bool, ARG_RES) = OCP(bool, 0) || OCP(bool, 1);
}

OCF(BoolAssign) {
	OCP(bool, 0) = OCP(bool, 1);
}

OCF(BoolIf) {
	if (!OCP(bool, 0))
		OCP(void *, 1) = OCP(void *, 2);
}

// vector ops

OCD(BoolVectorDestructor) {
	((OcVector *)v)->Clear(sizeof(bool), 0);
}

OCF(BoolVectorAdd) {
	OCR(OcVector, 0)->Add(sizeof(bool), OCR(bool, 1), 0);
}

OCF(BoolVectorAt) {
	CHUNK *at = OCR(OcVector, 0)->At(sizeof(bool), OCP(int, 1));

	if (at)
		*((CHUNK **)v[2]) = at;
//	else
//		todo: exception!
}

OCF(BoolVectorClear) {
	OCR(OcVector, 0)->Clear(sizeof(bool), 0);
}
