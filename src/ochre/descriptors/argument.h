#ifndef argument_h
#define argument_h

#include "descriptors/descriptors.h"



class OcClass;

struct OcArg
{
	std::string name;
	OcClass *cl;
	UsageType use;

	OcArg() : name("n/s"), cl(0), use(ptNone) {}

	OcArg(OcClass *_cl, UsageType _use) :
		name("n/s"), cl(_cl), use(_use) {}

	OcArg(const std::string &_name, OcClass *_cl, UsageType _use) :
		name(_name), cl(_cl), use(_use) {}
};

typedef std::vector<OcArg> OcArgs;

#endif