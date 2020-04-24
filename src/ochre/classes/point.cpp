#include "classes/point.h"
#include "classes/vector.h"
#include "runtime/frame.h"
#include "descriptors/class.h"
#include "ochre.h"
#include <cstdlib>
#include <cmath>



OcClass *CreatePointClass() {
	return new OcClass("point", sizeof(OcPoint));
}

void InitPointClass(Ochre *ochre) {
	OcClass *pointCl = ochre->Class("point");
	OcClass *realCl = ochre->Class("real");
	OcClass *intCl = ochre->Class("int");

	OcPoint p;

	// instance vars

	pointCl->vars.Add(OcVar("x", realCl, (char *)&p.x - (char *)&p));
	pointCl->vars.Add(OcVar("y", realCl, (char *)&p.y - (char *)&p));
	pointCl->vars.Add(OcVar("z", realCl, (char *)&p.z - (char *)&p));

	// static funcs

	pointCl->sfns.push_back(OcFunc("new", "returns a point with x, y and z set to { 0.0, 0.0, 0.0 }", OcArg(pointCl, ptReadModify), ptNone, PointNew));

	pointCl->sfns.push_back(OcFunc("new", "returns a point with x, y and z set to { *x*, *y*, *z* }", OcArg(pointCl, ptReadModify), ptNone, PointNewReal));
	pointCl->sfns.back().args.push_back(OcArg("x", realCl, ptRead));
	pointCl->sfns.back().args.push_back(OcArg("y", realCl, ptRead));
	pointCl->sfns.back().args.push_back(OcArg("z", realCl, ptRead));

	pointCl->sfns.push_back(OcFunc("rand", "returns a randomly directed vector with length set to *length*", OcArg(pointCl, ptReadModify), ptNone, PointNewRand));
	pointCl->sfns.back().args.push_back(OcArg("length", realCl, ptRead));

	pointCl->sfns.push_back(OcFunc("rand2D", "returns a randomly directed 2D vector (xy plane) with length set to *length*", OcArg(pointCl, ptReadModify), ptNone, PointNewRand2D));
	pointCl->sfns.back().args.push_back(OcArg("length", realCl, ptRead));

	// instance funcs

	pointCl->fns.push_back(OcFunc("rand",
		"sets the x, y and z to random values in the [*from*, *to*] range, returns *this*",
		OcArg(pointCl, ptCaller), ptModify, PointRand, true));
	pointCl->fns.back().args.push_back(OcArg("from", realCl, ptRead));
	pointCl->fns.back().args.push_back(OcArg("to", realCl, ptRead));

	pointCl->fns.push_back(OcFunc("normalize", "scales the vector so its length is 1.0, returns *this*", OcArg(pointCl, ptCaller), ptModify, PointNormalize, true));

	pointCl->fns.push_back(OcFunc("normalizeTo", "scales the vector so its length is *length*, returns *this*", OcArg(pointCl, ptCaller), ptModify, PointNormalizeTo, true));
	pointCl->fns.back().args.push_back(OcArg("length", realCl, ptRead));

	pointCl->fns.push_back(OcFunc("clampTo", "clamps vector length to *length*, returns *this*", OcArg(pointCl, ptCaller), ptModify, PointClampTo, true));
	pointCl->fns.back().args.push_back(OcArg("length", realCl, ptRead));

	pointCl->fns.push_back(OcFunc("length", "returns the vector length", OcArg(realCl, ptRead), ptRead, PointLength));

	pointCl->fns.push_back(OcFunc("dot", "returns the dot product of *this* . *b*", OcArg(realCl, ptRead), ptRead, PointDot));
	pointCl->fns.back().args.push_back(OcArg("point", pointCl, ptRead));

	// instance ops

	pointCl->ops.push_back(OcOp(otPrefMinus, "returns *this* value with reversed signs of all its members", OcArg(pointCl, ptReadModify), ptRead, PointNegate));

	pointCl->ops.push_back(OcOp(otMultiply, "returns the product of *this* and argument", OcArg(pointCl, ptReadModify), ptRead, OcArg(realCl, ptRead), PointRealMultiply));
	pointCl->ops.push_back(OcOp(otDivide, "returns quotient of *this* and argument", OcArg(pointCl, ptReadModify), ptRead, OcArg(realCl, ptRead), PointRealDivide));
	pointCl->ops.push_back(OcOp(otPlus, "returns sum of *this* and argument", OcArg(pointCl, ptReadModify), ptRead, OcArg(realCl, ptRead), PointRealAdd));
	pointCl->ops.push_back(OcOp(otMinus, "returns difference between *this* and argument", OcArg(pointCl, ptReadModify), ptRead, OcArg(realCl, ptRead), PointRealSubtract));

	pointCl->ops.push_back(OcOp(otMultiply, "returns the cross product of *this* and argument", OcArg(pointCl, ptReadModify), ptRead, OcArg(pointCl, ptRead), PointPointMultiply));
	pointCl->ops.push_back(OcOp(otPlus, "returns sum of *this* and argument", OcArg(pointCl, ptReadModify), ptRead, OcArg(pointCl, ptRead), PointPointAdd));
	pointCl->ops.push_back(OcOp(otMinus, "returns difference between *this* and argument", OcArg(pointCl, ptReadModify), ptRead, OcArg(pointCl, ptRead), PointPointSubtract));

	pointCl->ops.push_back(OcOp(otAssign, "assigns argument value to all members of *this* and returns *this*", OcArg(pointCl, ptCaller), ptWrite, OcArg(realCl, ptRead), PointRealAssign, true));
	pointCl->ops.push_back(OcOp(otPlusAssign, "adds argument to all members of *this* and returns *this*", OcArg(pointCl, ptCaller), ptAppend, OcArg(realCl, ptRead), PointRealAddAssign, true));
	pointCl->ops.push_back(OcOp(otMinusAssign, "subtracts argument from all members of *this* and returns *this*", OcArg(pointCl, ptCaller), ptWrite, OcArg(realCl, ptRead), PointRealSubtractAssign, true));
	pointCl->ops.push_back(OcOp(otMultiplyAssign, "scales *this* by argument and returns *this*", OcArg(pointCl, ptCaller), ptWrite, OcArg(realCl, ptRead), PointRealMultiplyAssign, true));
	pointCl->ops.push_back(OcOp(otDivideAssign, "scales *this* by argument and returns *this*", OcArg(pointCl, ptCaller), ptWrite, OcArg(realCl, ptRead), PointRealDivideAssign, true));

	pointCl->ops.push_back(OcOp(otAssign, "assigns argument to *this* and returns *this*", OcArg(pointCl, ptCaller), ptWrite, OcArg(pointCl, ptRead), PointPointAssign, true));
	pointCl->ops.push_back(OcOp(otPlusAssign, "assigns sum of *this* and argument to *this* and returns *this*", OcArg(pointCl, ptCaller), ptAppend, OcArg(pointCl, ptRead), PointPointAddAssign, true));
	pointCl->ops.push_back(OcOp(otMinusAssign, "assigns difference between *this* and argument to *this* and returns *this*", OcArg(pointCl, ptCaller), ptWrite, OcArg(pointCl, ptRead), PointPointSubtractAssign, true));

	ochre->AddClass(CreateVectorClass(pointCl, intCl, PointVectorDestructor, 0, PointVectorAdd, PointVectorAt, PointVectorClear));
}

// static funcs

OCF(PointNew) {
	OCP(OcPoint, ARG_RES) = OcPoint();
}

OCF(PointNewReal) {
	OCP(OcPoint, ARG_RES) = OcPoint(OCP(double, 0), OCP(double, 1), OCP(double, 2));
}

OCF(PointNewRand) {
	OcPoint p;
	p.Rand(-1.0, 1.0);
	p.NormalizeTo(OCP(double, 0));
	OCP(OcPoint, ARG_RES) = p;
}

OCF(PointNewRand2D) {
	OcPoint p;
	p.Rand(-1.0, 1.0);
	p.z = 0.0;
	p.NormalizeTo(OCP(double, 0));
	OCP(OcPoint, ARG_RES) = p;
}

// instance funcs

OCF(PointRand) {
	OCP(OcPoint, 0).Rand(OCP(double, 2), OCP(double, 1));
}

OCF(PointNormalize) {
	OCP(OcPoint, 0).Normalize();
}

OCF(PointNormalizeTo) {
	OCP(OcPoint, 0).NormalizeTo(OCP(double, 1));
}

OCF(PointClampTo) {
	OCP(OcPoint, 0).ClampTo(OCP(double, 1));
}

OCF(PointLength) {
	OCP(double, ARG_RES) = OCP(OcPoint, 0).Length();
}

OCF(PointDot) {
	OCP(double, ARG_RES) = OCP(OcPoint, 0).Dot(OCP(OcPoint, 1));
}

// instance ops

OCF(PointNegate) {
	OCP(OcPoint, ARG_RES) = -OCP(OcPoint, 0);
}

OCF(PointRealMultiply) {
	OCP(OcPoint, ARG_RES) = OCP(OcPoint, 0) * OCP(double, 1);
}

OCF(PointRealDivide) {
	OCP(OcPoint, ARG_RES) = OCP(OcPoint, 0) / OCP(double, 1);
}

OCF(PointRealAdd) {
	OCP(OcPoint, ARG_RES) = OCP(OcPoint, 0) + OCP(double, 1);
}

OCF(PointRealSubtract) {
	OCP(OcPoint, ARG_RES) = OCP(OcPoint, 0) - OCP(double, 1);
}

OCF(PointPointMultiply) {
	OCP(OcPoint, ARG_RES) = OCP(OcPoint, 0) * OCP(OcPoint, 1);
}

OCF(PointPointAdd) {
	OCP(OcPoint, ARG_RES) = OCP(OcPoint, 0) + OCP(OcPoint, 1);
}

OCF(PointPointSubtract) {
	OCP(OcPoint, ARG_RES) = OCP(OcPoint, 0) - OCP(OcPoint, 1);
}

OCF(PointRealAssign) {
	OCP(OcPoint, 0) = OCP(double, 1);
}

OCF(PointRealAddAssign) {
	OCP(OcPoint, 0) += OCP(double, 1);
}

OCF(PointRealSubtractAssign) {
	OCP(OcPoint, 0) -= OCP(double, 1);
}

OCF(PointRealMultiplyAssign) {
	OCP(OcPoint, 0) *= OCP(double, 1);
}

OCF(PointRealDivideAssign) {
	OCP(OcPoint, 0) /= OCP(double, 1);
}

OCF(PointPointAssign) {
	OCP(OcPoint, 0) = OCP(OcPoint, 1);
}

OCF(PointPointAddAssign) {
	OCP(OcPoint, 0) += OCP(OcPoint, 1);
}

OCF(PointPointSubtractAssign) {
	OCP(OcPoint, 0) -= OCP(OcPoint, 1);
}

// vector ops

OCD(PointVectorDestructor) {
	((OcVector *)v)->Clear(sizeof(OcPoint), 0);
}

OCF(PointVectorAdd) {
	OCR(OcVector, 0)->Add(sizeof(OcPoint), OCR(OcPoint, 1), 0);
}

OCF(PointVectorAt) {
	CHUNK *at = OCR(OcVector, 0)->At(sizeof(OcPoint), OCP(int, 1));

	if (at)
		*((CHUNK **)v[2]) = at;
//	else
//		todo: exception!
}

OCF(PointVectorClear) {
	OCR(OcVector, 0)->Clear(sizeof(OcPoint), 0);
}

//
// class
//

OcPoint::OcPoint() : x(0.0), y(0.0), z(0.0) {}

OcPoint::OcPoint(double v) : x(v), y(v), z(v) {}

OcPoint::OcPoint(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

void OcPoint::Rand(double a, double b) {
	double r = b - a;
	x = a + ((double)rand() / (double)RAND_MAX) * r;
	y = a + ((double)rand() / (double)RAND_MAX) * r;
	z = a + ((double)rand() / (double)RAND_MAX) * r;
}

void OcPoint::Normalize() {
	double l = sqrt(x * x + y * y + z * z);
	x /= l;
	y /= l;
	z /= l;
}

void OcPoint::NormalizeTo(double v) {

	if (v == 0.0)
		return;	// should be exception

	double l = sqrt(x * x + y * y + z * z) / v;
	x /= l;
	y /= l;
	z /= l;
}

void OcPoint::ClampTo(double v) {

	if (v == 0.0)
		return;	// should be exception

	double l = sqrt(x * x + y * y + z * z);

	if (l > v) {

		l /= v;
		x /= l;
		y /= l;
		z /= l;
	}
}

double OcPoint::Length() const {
	return sqrt(x * x + y * y + z * z);
}

double OcPoint::Dot(const OcPoint &p) const {
	return x * p.x + y * p.y + z * p.z;
}

OcPoint OcPoint::operator-() const {
	return OcPoint(-x, -y, -z);
}

OcPoint OcPoint::operator*(double v) const {
	return OcPoint(x * v, y * v, z * v);
}

OcPoint OcPoint::operator*(const OcPoint &p) const {
	return OcPoint(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
}

OcPoint OcPoint::operator/(double v) const {
	return OcPoint(x / v, y / v, z / v);
}

OcPoint OcPoint::operator+(const OcPoint &p) const {
	return OcPoint(x + p.x, y + p.y, z + p.z);
}

OcPoint OcPoint::operator+(double v) const {
	return OcPoint(x + v, y + v, z + v);
}

OcPoint OcPoint::operator-(const OcPoint &p) const {
	return OcPoint(x - p.x, y - p.y, z - p.z);
}

OcPoint OcPoint::operator-(double v) const {
	return OcPoint(x - v, y - v, z - v);
}

void OcPoint::operator=(double s) {
	x = y = z = s;
}

void OcPoint::operator+=(double s) {
	x += s;
	y += s;
	z += s;
}

void OcPoint::operator-=(double s) {
	x -= s;
	y -= s;
	z -= s;
}

void OcPoint::operator*=(double s) {
	x *= s;
	y *= s;
	z *= s;
}

void OcPoint::operator/=(double s) {
	x /= s;
	y /= s;
	z /= s;
}

void OcPoint::operator=(const OcPoint &p) {
	x = p.x;
	y = p.y;
	z = p.z;
}

void OcPoint::operator+=(const OcPoint &p) {
	x += p.x;
	y += p.y;
	z += p.z;
}

void OcPoint::operator-=(const OcPoint &p) {
	x -= p.x;
	y -= p.y;
	z -= p.z;
}

bool OcPoint::operator!=(const OcPoint &p) const {
	return x != p.x || y != p.y || z != p.z;
}

bool OcPoint::operator<=(const OcPoint &p) const {
	return x <= p.x && y <= p.y && z <= p.z;
}

bool OcPoint::operator>=(const OcPoint &p) const {
	return x >= p.x && y >= p.y && z >= p.z;
}
