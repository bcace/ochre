#ifndef ocreal_h
#define ocreal_h

#include "descriptors/descriptors.h"



class Ochre;
class OcClass;

OcClass *CreateRealClass();
void InitRealClass(Ochre *);

OCF(NumRand);
OCF(NumUnitRand);
OCF(NumIntCast);

OCF(NumPow);
OCF(NumSqrt);
OCF(NumSin);
OCF(NumCos);

OCF(NumNegate);
OCF(NumMultiply);
OCF(NumDivide);
OCF(NumAdd);
OCF(NumSubtract);
OCF(NumPointMultiply);
OCF(NumLess);
OCF(NumLessOrEqual);
OCF(NumGreater);
OCF(NumGreaterOrEqual);
OCF(NumEqual);
OCF(NumNotEqual);
OCF(NumAssign);
OCF(NumPlusAssign);
OCF(NumMinusAssign);
OCF(NumMultiplyAssign);
OCF(NumDivideAssign);
OCF(NumIf);

OCD(NumVectorDestructor);
OCF(NumVectorClear);
OCF(NumVectorAdd);
OCF(NumVectorAt);

#endif
