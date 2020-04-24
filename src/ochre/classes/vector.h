#ifndef ocvector_h
#define ocvector_h

#include "descriptors/class.h"
#include "runtime/frame.h"



OcClass *CreateVectorClass(OcClass *, OcClass *, CTOR, COPY, EXEC, EXEC, EXEC);



struct OcVector
{
	int count, size;
	CHUNK *c;

	OcVector();

	void Clear(int, CTOR);
	void Add(int, void *, COPY);
	CHUNK *At(int, int);

	int Count();
};



OCF(VecCount);
OCF(VecIf);

#endif
