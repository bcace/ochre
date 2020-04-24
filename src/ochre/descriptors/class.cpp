#include "descriptors/class.h"
#include "runtime/agent.h"



OcClass::OcClass(const std::string &_name, int _size, bool _user, CTOR _destr, COPY _cptor) :
	vars(_user ? OcAgent::kernelDataSize : 0),
	svars(0),
	size(_size),
	destr(_destr),
	cptor(_cptor),
	name(_name, 0),
	templateCl(0),
	user(_user),
	refsQ(rqNotRefs) {}

OcClass::OcClass(const std::string &_name, int _size, OcClass *_templateCl, CTOR _destr, COPY _cptor, RefsQualifier _refsQ) :
	vars(0),
	svars(0),
	size(_size),
	destr(_destr),
	cptor(_cptor),
	name(_name, 0),
	templateCl(_templateCl),
	user(false),
	refsQ(_refsQ) {}

const OcString &OcClass::Name() const {
	return name;
}

OcClass *OcClass::Template() {
	return templateCl;
}

bool OcClass::User() const {
	return user;
}

RefsQualifier OcClass::RefsQ() const {
	return refsQ;
}

const OcVars &OcClass::Vars() const {
	return vars;
}

const OcVars &OcClass::Svars() const {
	return svars;
}

const OcFuncs &OcClass::Fns() const {
	return fns;
}

const OcFuncs &OcClass::Sfns() const {
	return sfns;
}

bool OcClass::Contains(OcProto &proto) {

	OcFuncs &_fns = (proto.stat) ? sfns : fns;
	for (OcFuncs::iterator fn = _fns.begin(); fn != _fns.end(); ++fn)
		if (fn->name == proto.name)
			return proto.valid = true;

	return proto.valid = false;
}

OcFuncPtrs OcClass::Funcs(const OcProto &proto) {
	OcFuncPtrs ptrs;

	OcFuncs &_fns = (proto.stat) ? sfns : fns;
	for (OcFuncs::iterator fn = _fns.begin(); fn != _fns.end(); ++fn)
		if (fn->name == proto.name)
			ptrs.push_back(&(*fn));

	return ptrs;
}

OcFunc *OcClass::Func(const OcProto &proto, const OcFuncSign &sgn) {

	OcFuncs &_fns = (proto.stat) ? sfns : fns;
	for (OcFuncs::iterator fn = _fns.begin(); fn != _fns.end(); ++fn)
		if (fn->name == proto.name && fn->Match(sgn))
			return &(*fn);

	return 0;
}

OcVar *OcClass::Var(const std::string &varName, bool stat) {

	OcVars &_vars = (stat) ? svars : vars;
	return _vars.Contains(varName);
}

OcOp *OcClass::Operator(OpType opType, OcClass *argCl, bool stat) {

	OcOps &_ops = (stat) ? sops : ops;
	for (OcOps::iterator op = _ops.begin(); op != _ops.end(); ++op)
		if (op->type == opType && op->arg.cl == argCl)
			return &(*op);

	return 0;
}
/*
MGRT OcClass::MemCopyElement(OcClass *cl) {
	OcMgrts::iterator m = mgrts.find((void *)cl);
	return (m != mgrts.end()) ? m->second : 0;
}
*/

std::string OcClass::Description() const {

	std::string s = "## class " + name + (templateCl ? ("<" + templateCl->Name() + ">  \n") : ("  \n"));

	if (vars.size()) {

		s += "  \n#### variables:  \n  \n";

		for (OcVars::const_iterator v = vars.begin(); v != vars.end(); ++v)
			s += "`" + v->Description() + "`  \n" + v->description + "  \n";
	}

	if (svars.size()) {

		s += "  \n#### static variables:  \n  \n";

		for (OcVars::const_iterator v = svars.begin(); v != svars.end(); ++v)
			s += "`" + v->Description() + "`  \n" + v->description + "  \n";
	}

	if (ops.size()) {

		s += "  \n#### operators:  \n  \n";

		for (OcOps::const_iterator o = ops.begin(); o != ops.end(); ++o)
			s += "`" + o->Description() + "`  \n" + o->description + "  \n";
	}

	if (sops.size()) {

		s += "  \n#### static operators:  \n  \n";

		for (OcOps::const_iterator o = sops.begin(); o != sops.end(); ++o)
			s += "`" + o->Description() + "`  \n" + o->description + "  \n";
	}

	if (fns.size()) {

		s += "  \n#### functions:  \n  \n";

		for (OcFuncs::const_iterator f = fns.begin(); f != fns.end(); ++f)
			s += "`" + f->Description() + "`  \n" + f->description + "  \n";
	}

	if (sfns.size()) {

		s += "  \n#### static functions:  \n  \n";

		for (OcFuncs::const_iterator f = sfns.begin(); f != sfns.end(); ++f)
			s += "`" + f->Description() + "`  \n" + f->description + "  \n";
	}

	return s + "  \n  \n";
}
