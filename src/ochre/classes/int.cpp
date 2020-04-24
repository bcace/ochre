#include "classes/int.h"
#include "classes/vector.h"
#include "descriptors/class.h"
#include "ochre.h"



OcClass *CreateIntClass() {
	return new OcClass("int", sizeof(int));
}

void InitIntClass(Ochre *ochre) {
	OcClass *intCl = ochre->Class("int");
	OcClass *realCl = ochre->Class("real");
	OcClass *boolCl = ochre->Class("bool");

	// instance funcs

	intCl->fns.push_back(OcFunc("modulo", "returns the modulo of *this* / *div*", OcArg(intCl, ptRead), ptRead, IntModulo));
	intCl->fns.back().args.push_back(OcArg("div", intCl, ptRead));

	// static ops

	intCl->sops.push_back(OcOp(otCast,
		"cast operator, casts a bool value to int value\n`int i = int::true`\nwhere true is cast to 1, and false to 0",
		OcArg(intCl, ptRead), ptNone, OcArg(realCl, ptRead), IntRealCast));
	intCl->sops.push_back(OcOp(otCast,
		"cast operator, casts a real value to int value\n`int b = int::4.0`",
		OcArg(intCl, ptRead), ptNone, OcArg(boolCl, ptRead), IntBoolCast));

	// instance ops

	intCl->ops.push_back(OcOp(otPrefMinus, "returns *this* with reversed sign", OcArg(intCl, ptRead), ptRead, IntNegate));

	intCl->ops.push_back(OcOp(otMultiply, "returns product of *this* and argument", OcArg(intCl, ptRead), ptRead, OcArg(intCl, ptRead), IntMultiply));
	intCl->ops.push_back(OcOp(otDivide, "returns quotient of *this* and argument", OcArg(realCl, ptRead), ptRead, OcArg(intCl, ptRead), IntDivide));

	intCl->ops.push_back(OcOp(otPlus, "returns sum of *this* and argument", OcArg(intCl, ptRead), ptRead, OcArg(intCl, ptRead), IntAdd));
	intCl->ops.push_back(OcOp(otMinus, "returns difference between *this* and argument", OcArg(intCl, ptRead), ptRead, OcArg(intCl, ptRead), IntSubtract));

	intCl->ops.push_back(OcOp(otLess, "returns true if *this* is less than argument", OcArg(boolCl, ptRead), ptRead, OcArg(intCl, ptRead), IntLess));
	intCl->ops.push_back(OcOp(otLessOrEqual, "returns true if *this* is less than or equal to argument", OcArg(boolCl, ptRead), ptRead, OcArg(intCl, ptRead), IntLessOrEqual));
	intCl->ops.push_back(OcOp(otGreater, "returns true if *this* is greater than argument", OcArg(boolCl, ptRead), ptRead, OcArg(intCl, ptRead), IntGreater));
	intCl->ops.push_back(OcOp(otGreaterOrEqual, "returns true if *this* is greater than or equal to argument", OcArg(boolCl, ptRead), ptRead, OcArg(intCl, ptRead), IntGreaterOrEqual));

	intCl->ops.push_back(OcOp(otEqual, "returns true if *this* and argument are equal", OcArg(boolCl, ptRead), ptRead, OcArg(intCl, ptRead), IntEqual));
	intCl->ops.push_back(OcOp(otNotEqual, "returns true if *this* and argument are not equal", OcArg(boolCl, ptRead), ptRead, OcArg(intCl, ptRead), IntNotEqual));

	intCl->ops.push_back(OcOp(otAssign, "assigns argument to *this* and returns *this*", OcArg(intCl, ptCaller), ptWrite, OcArg(intCl, ptRead), IntAssign, true));
	intCl->ops.push_back(OcOp(otPlusAssign, "assigns sum of *this* and argument to *this* and returns *this*", OcArg(intCl, ptCaller), ptAppend, OcArg(intCl, ptRead), IntPlusAssign, true));
	intCl->ops.push_back(OcOp(otMinusAssign, "assigns difference between *this* and argument to *this* and returns *this*", OcArg(intCl, ptCaller), ptWrite, OcArg(intCl, ptRead), IntMinusAssign, true));
	intCl->ops.push_back(OcOp(otMultiplyAssign, "assigns product of *this* and argument to *this* and returns *this*", OcArg(intCl, ptCaller), ptWrite, OcArg(intCl, ptRead), IntMultiplyAssign, true));

	intCl->ops.push_back(OcOp(otIf, "tests if *this* is not equal to 0", OcArg(0, ptNone), ptRead, IntIf, true));
	intCl->ops.push_back(OcOp(otWhile, "loops while *this* is not equal to 0", OcArg(0, ptNone), ptRead, IntIf, true));

	ochre->AddClass(CreateVectorClass(intCl, intCl, IntVectorDestructor, 0, IntVectorAdd, IntVectorAt, IntVectorClear));
}

// instance funcs

OCF(IntModulo) {
	OCP(int, ARG_RES) = OCP(int, 0) % OCP(int, 1);
}

// static ops

OCF(IntRealCast) {
	OCP(int, ARG_RES) = (int)OCP(double, 0);	// todo: round the float
}

OCF(IntBoolCast) {
	OCP(int, ARG_RES) = (int)OCP(bool, 0);
}

// instance ops

OCF(IntNegate) {
	OCP(int, ARG_RES) = -OCP(int, 0);
}

OCF(IntMultiply) {
	OCP(int, ARG_RES) = OCP(int, 0) * OCP(int, 1);
}

OCF(IntDivide) {
	OCP(int, ARG_RES) = OCP(int, 0) / OCP(int, 1);
}

OCF(IntAdd) {
	OCP(int, ARG_RES) = OCP(int, 0) + OCP(int, 1);
}

OCF(IntSubtract) {
	OCP(int, ARG_RES) = OCP(int, 0) - OCP(int, 1);
}

OCF(IntLess) {
	OCP(bool, ARG_RES) = OCP(int, 0) < OCP(int, 1);
}

OCF(IntLessOrEqual) {
	int a = OCP(int, 0);
	int b = OCP(int, 1);
	OCP(bool, ARG_RES) = OCP(int, 0) <= OCP(int, 1);
}

OCF(IntGreater) {
	OCP(bool, ARG_RES) = OCP(int, 0) > OCP(int, 1);
}

OCF(IntGreaterOrEqual) {
	OCP(bool, ARG_RES) = OCP(int, 0) >= OCP(int, 1);
}

OCF(IntEqual) {
	OCP(bool, ARG_RES) = OCP(int, 0) == OCP(int, 1);
}

OCF(IntNotEqual) {
	OCP(bool, ARG_RES) = OCP(int, 0) != OCP(int, 1);
}

OCF(IntAssign) {
	OCP(int, 0) = OCP(int, 1);
}

OCF(IntPlusAssign) {
	OCP(int, 0) += OCP(int, 1);
}

OCF(IntMinusAssign) {
	OCP(int, 0) -= OCP(int, 1);
}

OCF(IntMultiplyAssign) {
	OCP(int, 0) *= OCP(int, 1);
}

OCF(IntDivideAssign) {
	OCP(int, 0) /= OCP(int, 1);
}

OCF(IntIf) {
	if (OCP(int, 0) == 0)
		OCP(void *, 1) = OCP(void *, 2);
}

// vector ops

OCD(IntVectorDestructor) {
	((OcVector *)v)->Clear(sizeof(int), 0);
}

OCF(IntVectorAdd) {
	OCR(OcVector, 0)->Add(sizeof(int), OCR(int, 1), 0);
}

OCF(IntVectorAt) {
	CHUNK *at = OCR(OcVector, 0)->At(sizeof(int), OCP(int, 1));

	if (at)
		*((CHUNK **)v[2]) = at;
//	else
//		todo: exception!
}

OCF(IntVectorClear) {
	OCR(OcVector, 0)->Clear(sizeof(int), 0);
}
