#ifndef function_h
#define function_h

#include "descriptors/argument.h"
#include "descriptors/docable.h"



class OcClass;

struct OcProto
{
	std::string name; // callee name
	UsageType use;	// caller use
	OcClass *owner;	// caller type
	bool stat, valid;

	OcProto() :
		name(""), use(ptNone), owner(0), stat(false), valid(false) {}

	OcProto(const std::string &_name, UsageType _use, OcClass *_cl, bool _stat) :
		name(_name), use(_use), owner(_cl), stat(_stat), valid(false) {}
};



typedef std::vector<OcClass *> OcFuncSign;

class OcFunc : public OcDocable
{
public:
	OcString name;
	OcArg res;
	UsageType use;	// subject (caller) usage
	OcArgs args;
	EXEC exec;
	bool returnsThis;
	int bannedFrom;	// sections this func is banned from (KeyType)

	OcFunc() :
		name("n/s", 0) {}

	OcFunc(const std::string &_name, const char *_desc, const OcArg &_res, UsageType _use, EXEC _exec, bool _returnsThis = false, int _bannedFrom = 0) :
		OcDocable(_desc), name(_name, 0), res(_res), use(_use), exec(_exec), returnsThis(_returnsThis), bannedFrom(_bannedFrom) {}

	bool Match(const OcFuncSign &) const;
	std::string Description() const;
};



class OcFuncs : public std::vector<OcFunc>
{
public:
};

typedef std::vector<OcFunc *> OcFuncPtrs;

#endif
