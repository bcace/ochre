#ifndef ocpoint_h
#define ocpoint_h

#include "descriptors/descriptors.h"



class Ochre;
class OcClass;

OcClass *CreatePointClass();
void InitPointClass(Ochre *);

OCF(PointNew);
OCF(PointNewReal);
OCF(PointNewRand);
OCF(PointNewRand2D);

OCF(PointRand);
OCF(PointNormalize);
OCF(PointNormalizeTo);
OCF(PointClampTo);
OCF(PointLength);
OCF(PointDot);

OCF(PointNegate);
OCF(PointRealMultiply);
OCF(PointRealDivide);
OCF(PointRealAdd);
OCF(PointRealSubtract);
OCF(PointPointMultiply);
OCF(PointPointAdd);
OCF(PointPointSubtract);
OCF(PointRealAssign);
OCF(PointRealAddAssign);
OCF(PointRealSubtractAssign);
OCF(PointRealMultiplyAssign);
OCF(PointRealDivideAssign);
OCF(PointPointAssign);
OCF(PointPointAddAssign);
OCF(PointPointSubtractAssign);

OCD(PointVectorDestructor);
OCF(PointVectorClear);
OCF(PointVectorAdd);
OCF(PointVectorAt);



struct OcPoint
{
	double x, y, z;

	OcPoint();
	OcPoint(double);
	OcPoint(double, double, double);

	void Rand(double, double);
	void Normalize();
	void NormalizeTo(double);
	void ClampTo(double);
	double Length() const;
	double Dot(const OcPoint &) const;

	OcPoint operator-() const;
	OcPoint operator*(double) const;
	OcPoint operator/(double) const;
	OcPoint operator+(double) const;
	OcPoint operator-(double) const;
	OcPoint operator*(const OcPoint &) const;
	OcPoint operator+(const OcPoint &) const;
	OcPoint operator-(const OcPoint &) const;
	void operator=(double);
	void operator+=(double);
	void operator-=(double);
	void operator*=(double);
	void operator/=(double);
	void operator=(const OcPoint &);
	void operator+=(const OcPoint &);
	void operator-=(const OcPoint &);

	bool operator!=(const OcPoint &) const;
	bool operator<=(const OcPoint &) const;
	bool operator>=(const OcPoint &) const;
};

#endif
