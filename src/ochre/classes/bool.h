#ifndef ocbool_h
#define ocbool_h

#include "descriptors/descriptors.h"



class Ochre;
class OcClass;

OcClass *CreateBoolClass();
void InitBoolClass(Ochre *);

OCF(BoolIntCast);
OCF(BoolRealCast);

OCF(BoolNegate);
OCF(BoolEqual);
OCF(BoolNotEqual);
OCF(BoolAnd);
OCF(BoolOr);
OCF(BoolAssign);
OCF(BoolIf);

OCD(BoolVectorDestructor);
OCF(BoolVectorClear);
OCF(BoolVectorAdd);
OCF(BoolVectorAt);

#endif
