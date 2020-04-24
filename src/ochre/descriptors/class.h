#ifndef class_h
#define class_h

#include "descriptors/function.h"
#include "descriptors/operator.h"
#include "descriptors/variable.h"



class OcClass : public OcDocable
{
public:
	OcVars vars, svars;
	OcFuncs fns, sfns;
	OcOps ops, sops;
	int size;
	CTOR destr;
	COPY cptor;

	OcClass(const std::string &_name, int _size, bool _user = false, CTOR _destr = 0, COPY _cptor = 0);
	OcClass(const std::string &_name, int _size, OcClass *_template, CTOR _destr, COPY _cptor, RefsQualifier _refsQ = rqNotRefs);

	const OcString &Name() const;
	OcClass *Template();
	bool User() const;
	RefsQualifier RefsQ() const;
	const OcVars &Vars() const;
	const OcVars &Svars() const;
	const OcFuncs &Fns() const;
	const OcFuncs &Sfns() const;
	bool Contains(OcProto &);
	OcFuncPtrs Funcs(const OcProto &);
	OcFunc *Func(const OcProto &, const OcFuncSign &);
	OcVar *Var(const std::string &, bool);
	OcOp *Operator(OpType, OcClass *, bool);

	std::string Description() const;

protected:
	OcString name;
	OcClass *templateCl;
	bool user;			// is user-defined (agent type)
	RefsQualifier refsQ;	// is agent reference collection, and if so, who should own it
};

#endif
