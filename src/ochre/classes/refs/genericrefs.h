#ifndef genericrefs_h
#define genericrefs_h



struct OcAgentPtr;

class OcGenericRefs
{
public:
	int count;
	OcAgentPtr *c;

	OcGenericRefs() : count(0), c(0) {}

	int Count() { return count; }
};

#endif