#ifndef minrefs_h
#define minrefs_h

#include "classes/refs/genericrefs.h"
#include "descriptors/class.h"
#include "runtime/frame.h"



OcClass *CreateMinRefsClass(OcClass *, OcClass *, OcClass *);



class OcMinRefs : public OcGenericRefs
{
public:
	double min;

	OcMinRefs();

	void Clear();
	void Add(OcAgentPtr *, double);
};



OCF(MinRefsClear);
OCF(MinRefsAdd);
OCF(MinRefsCount);

#endif
