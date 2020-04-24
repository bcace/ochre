#ifndef operator_h
#define operator_h

#include "descriptors/argument.h"
#include "descriptors/docable.h"



class OcOp : public OcDocable
{
public:
	OpType type;
	OcArg res, arg;
	UsageType use;	// subject (caller) usage
	EXEC exec;
	bool returnsThis;

	OcOp() : type(otNoOp) {}

	OcOp(OpType _type, const char *_desc, const OcArg &_res, UsageType _use, EXEC _exec, bool _returnsThis = false) :
		OcDocable(_desc), type(_type), res(_res), use(_use), exec(_exec), returnsThis(_returnsThis) {}

	OcOp(OpType _type, const char *_desc, const OcArg &_res, UsageType _use, const OcArg &_arg, EXEC _exec, bool _returnsThis = false) :
		OcDocable(_desc), type(_type), res(_res), arg(_arg), use(_use), exec(_exec), returnsThis(_returnsThis) {}

	std::string Description() const;
};

typedef std::vector<OcOp> OcOps;

#endif
