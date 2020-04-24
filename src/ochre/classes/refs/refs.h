#ifndef refs_h
#define refs_h

#include "classes/refs/genericrefs.h"
#include "descriptors/class.h"
#include "runtime/frame.h"



OcClass *CreateRefsClass(OcClass *, OcClass *, bool);



class OcRefs : public OcGenericRefs
{
public:
	void Clear();
	void Add(OcAgentPtr *);
	void Assign(OcRefs *);
	OcAgentPtr *At(unsigned int);
};



OCF(RefsClear);
OCF(RefsAdd);
OCF(RefsCount);
OCF(RefsAt);
OCF(RefsAssign);

#endif
