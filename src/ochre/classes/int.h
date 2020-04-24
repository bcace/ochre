#ifndef ocint_h
#define ocint_h

#include "descriptors/descriptors.h"



class Ochre;
class OcClass;

OcClass *CreateIntClass();
void InitIntClass(Ochre *);

OCF(IntModulo);

OCF(IntRealCast);
OCF(IntBoolCast);

OCF(IntNegate);
OCF(IntMultiply);
OCF(IntDivide);
OCF(IntAdd);
OCF(IntSubtract);
OCF(IntLess);
OCF(IntLessOrEqual);
OCF(IntGreater);
OCF(IntGreaterOrEqual);
OCF(IntEqual);
OCF(IntNotEqual);
OCF(IntAssign);
OCF(IntPlusAssign);
OCF(IntMinusAssign);
OCF(IntMultiplyAssign);
OCF(IntDivideAssign);
OCF(IntIf);

OCD(IntVectorDestructor);
OCF(IntVectorClear);
OCF(IntVectorAdd);
OCF(IntVectorAt);

#endif
