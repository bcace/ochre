#include "classes/real.h"
#include "classes/vector.h"
#include "runtime/frame.h"
#include "descriptors/class.h"
#include "ochre.h"
#include <cstdlib>
#include <cmath>



OcClass *CreateRealClass() {
	return new OcClass("real", sizeof(double));
}

void InitRealClass(Ochre *ochre) {
	OcClass *realCl = ochre->Class("real");
	OcClass *boolCl = ochre->Class("bool");
	OcClass *intCl = ochre->Class("int");
	OcClass *pointCl = ochre->Class("point");

	// static funcs

	realCl->sfns.push_back(OcFunc("rand", "returns a random number within [0.0, 1.0] range", OcArg(realCl, ptRead), ptNone, NumUnitRand));
	realCl->sfns.push_back(OcFunc("rand", "returns a random number within [**from**, **to**] range", OcArg(realCl, ptRead), ptNone, NumRand));	// fahst mefod evah!
	realCl->sfns.back().args.push_back(OcArg("from", realCl, ptRead));
	realCl->sfns.back().args.push_back(OcArg("to", realCl, ptRead));
	realCl->sfns.push_back(OcFunc("sqrt", "returns square root of *num*", OcArg(realCl, ptRead), ptNone, NumSqrt));
	realCl->sfns.back().args.push_back(OcArg("arg", realCl, ptRead));
	realCl->sfns.push_back(OcFunc("sin", "returns sine of *num* [rad]", OcArg(realCl, ptRead), ptNone, NumSin));
	realCl->sfns.back().args.push_back(OcArg("arg", realCl, ptRead));
	realCl->sfns.push_back(OcFunc("cos", "return cosine of *num* [rad]", OcArg(realCl, ptRead), ptNone, NumCos));
	realCl->sfns.back().args.push_back(OcArg("arg", realCl, ptRead));

	// static ops

	realCl->sops.push_back(OcOp(otCast,
		"cast operator, casts an int value to real value\n`real b = real::4`",
		OcArg(realCl, ptRead), ptNone, OcArg(intCl, ptRead), NumIntCast));

	// instance funcs

	realCl->fns.push_back(OcFunc("pow", "returns the *p* power of *this*", OcArg(realCl, ptRead), ptRead, NumPow));
	realCl->fns.back().args.push_back(OcArg("power", realCl, ptRead));

	// instance ops

	realCl->ops.push_back(OcOp(otPrefMinus, "returns *this* with reversed sign", OcArg(realCl, ptRead), ptRead, NumNegate));

	realCl->ops.push_back(OcOp(otMultiply, "returns product of *this* and argument", OcArg(realCl, ptRead), ptRead, OcArg(realCl, ptRead), NumMultiply));
	realCl->ops.push_back(OcOp(otDivide, "returns quotient of *this* and argument", OcArg(realCl, ptRead), ptRead, OcArg(realCl, ptRead), NumDivide));
	realCl->ops.push_back(OcOp(otPlus, "returns sum of *this* and argument", OcArg(realCl, ptRead), ptRead, OcArg(realCl, ptRead), NumAdd));
	realCl->ops.push_back(OcOp(otMinus, "returns difference between *this* and argument", OcArg(realCl, ptRead), ptRead, OcArg(realCl, ptRead), NumSubtract));

	realCl->ops.push_back(OcOp(otMultiply, "", OcArg(pointCl, ptRead), ptRead, OcArg(pointCl, ptRead), NumPointMultiply));

	realCl->ops.push_back(OcOp(otLess, "returns true if *this* is less than argument", OcArg(boolCl, ptRead), ptRead, OcArg(realCl, ptRead), NumLess));
	realCl->ops.push_back(OcOp(otLessOrEqual, "returns true if *this* is less than or equal to argument", OcArg(boolCl, ptRead), ptRead, OcArg(realCl, ptRead), NumLessOrEqual));
	realCl->ops.push_back(OcOp(otGreater, "returns true if *this* is greater than argument", OcArg(boolCl, ptRead), ptRead, OcArg(realCl, ptRead), NumGreater));
	realCl->ops.push_back(OcOp(otGreaterOrEqual, "returns true if *this* is greater than or equal to argument", OcArg(boolCl, ptRead), ptRead, OcArg(realCl, ptRead), NumGreaterOrEqual));

	realCl->ops.push_back(OcOp(otEqual, "returns true if *this* and argument are equal", OcArg(boolCl, ptRead), ptRead, OcArg(realCl, ptRead), NumEqual));
	realCl->ops.push_back(OcOp(otNotEqual, "returns true if *this* and argument are not equal", OcArg(boolCl, ptRead), ptRead, OcArg(realCl, ptRead), NumNotEqual));

	realCl->ops.push_back(OcOp(otAssign, "assigns argument to *this* and returns *this*", OcArg(realCl, ptCaller), ptWrite, OcArg(realCl, ptRead), NumAssign, true));
	realCl->ops.push_back(OcOp(otPlusAssign, "assigns sum of *this* and argument to *this* and returns *this*", OcArg(realCl, ptCaller), ptAppend, OcArg(realCl, ptRead), NumPlusAssign, true));
	realCl->ops.push_back(OcOp(otMinusAssign, "assigns difference between *this* and argument to *this* and returns *this*", OcArg(realCl, ptCaller), ptWrite, OcArg(realCl, ptRead), NumMinusAssign, true));
	realCl->ops.push_back(OcOp(otMultiplyAssign, "assigns product of *this* and argument to *this* and returns *this*", OcArg(realCl, ptCaller), ptWrite, OcArg(realCl, ptRead), NumMultiplyAssign, true));
	realCl->ops.push_back(OcOp(otDivideAssign, "assigns quotient of *this* and argument to *this* and returns *this*", OcArg(realCl, ptCaller), ptWrite, OcArg(realCl, ptRead), NumDivideAssign, true));

	realCl->ops.push_back(OcOp(otIf, "tests if *this* is not equal to 0.0", OcArg(0, ptNone), ptRead, NumIf, true));
	realCl->ops.push_back(OcOp(otWhile, "loops while *this* is not equal to 0.0", OcArg(0, ptNone), ptRead, NumIf, true));

	ochre->AddClass(CreateVectorClass(realCl, intCl, NumVectorDestructor, 0, NumVectorAdd, NumVectorAt, NumVectorClear));
}

// static

OCF(NumRand) {
	OCP(double, ARG_RES) = OCP(double, 0) + ((double)rand() / (double)RAND_MAX) * (OCP(double, 1) - OCP(double, 0));
}

OCF(NumUnitRand) {
	OCP(double, ARG_RES) = (double)rand() / (double)RAND_MAX;
}

OCF(NumSqrt) {
	OCP(double, ARG_RES) = sqrt(OCP(double, 0));
}

OCF(NumSin) {
	OCP(double, ARG_RES) = sin(OCP(double, 0));
}

OCF(NumCos) {
	OCP(double, ARG_RES) = cos(OCP(double, 0));
}

OCF(NumIntCast) {
	OCP(double, ARG_RES) = (double)OCP(int, 0);
}

// method

OCF(NumPow) {
	OCP(double, ARG_RES) = pow(OCP(double, 0), OCP(double, 1));
}

// operator

OCF(NumNegate) {
	OCP(double, ARG_RES) = -OCP(double, 0);
}

OCF(NumMultiply) {
	OCP(double, ARG_RES) = OCP(double, 0) * OCP(double, 1);
}

OCF(NumPointMultiply) {
	OCP(OcPoint, ARG_RES) = OCP(OcPoint, 1) * OCP(double, 0);
}

OCF(NumDivide) {
	OCP(double, ARG_RES) = OCP(double, 0) / OCP(double, 1);
}

OCF(NumAdd) {
	OCP(double, ARG_RES) = OCP(double, 0) + OCP(double, 1);
}

OCF(NumSubtract) {
	OCP(double, ARG_RES) = OCP(double, 0) - OCP(double, 1);
}

OCF(NumLess) {
	OCP(bool, ARG_RES) = OCP(double, 0) < OCP(double, 1);
}

OCF(NumLessOrEqual) {
	OCP(bool, ARG_RES) = OCP(double, 0) <= OCP(double, 1);
}

OCF(NumGreater) {
	OCP(bool, ARG_RES) = OCP(double, 0) > OCP(double, 1);
}

OCF(NumGreaterOrEqual) {
	OCP(bool, ARG_RES) = OCP(double, 0) >= OCP(double, 1);
}

OCF(NumEqual) {
	OCP(bool, ARG_RES) = OCP(double, 0) == OCP(double, 1);
}

OCF(NumNotEqual) {
	OCP(bool, ARG_RES) = OCP(double, 0) != OCP(double, 1);
}

OCF(NumAssign) {
	OCP(double, 0) = OCP(double, 1);
}

OCF(NumPlusAssign) {
	OCP(double, 0) += OCP(double, 1);
}

OCF(NumMinusAssign) {
	OCP(double, 0) -= OCP(double, 1);
}

OCF(NumMultiplyAssign) {
	OCP(double, 0) *= OCP(double, 1);
}

OCF(NumDivideAssign) {
	OCP(double, 0) /= OCP(double, 1);
}

OCF(NumIf) {
	if (OCP(int, 0) == 0.0)
		OCP(void *, 1) = OCP(void *, 2);
}

// vector ops

OCD(NumVectorDestructor) {
	((OcVector *)v)->Clear(sizeof(double), 0);
}

OCF(NumVectorAdd) {
	OCR(OcVector, 0)->Add(sizeof(double), OCR(double, 1), 0);
}

OCF(NumVectorAt) {
	CHUNK *at = OCR(OcVector, 0)->At(sizeof(double), OCP(int, 1));

	if (at)
		*((CHUNK **)v[2]) = at;
//	else
//		todo: exception!
}

OCF(NumVectorClear) {
	OCR(OcVector, 0)->Clear(sizeof(double), 0);
}
