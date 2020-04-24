#ifndef docable_h
#define docable_h

#include <string>



/*
Interface for classes that are expected to be
self-documenting (descriptors mainly).
*/

class OcDocable
{
public:
	// additional description text provided by the author
	std::string description;

	OcDocable() {}
	OcDocable(const char *_description) : description(_description) {}

	// generates basic description from available data
	virtual std::string Description() const { return ""; }
};

#endif