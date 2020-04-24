#include "descriptors/class.h"
#include <sstream>



OcVar::OcVar() :
	name("n/s", 0), cl(0), owner(owtLocal), mod(vmPrivate), off(-1) {}

OcVar::OcVar(const std::string &_name, OwnerType _owner, OcClass *_cl) :	// overload used for built-in vars
	name(_name, 0), cl(_cl), owner(_owner), mod(vmPublic), off(-1) {}

OcVar::OcVar(const std::string &_name, OcClass *_cl, int _off) :	// overload used for class vars
	name(_name, 0), cl(_cl), owner(owtObject), mod(vmPublic), off(_off) {}

std::string OcVar::ToString() const {
	std::stringstream ss;
	ss << "['" << name << "' " << cl->Name() << "]";
	return ss.str();
}

std::string OcVar::Description() const {

	std::stringstream ss;
	ss << "[" << cl->Name() << "] " << name;
	switch (mod) {
		case vmPrivate:		ss << " (private, "; break;
		case vmPublic:		ss << " (public, "; break;
		case vmConst:		ss << " (const, "; break;
		default:			ss << " (unhandled modifier, "; break;
	}
	switch (owner) {
		case owtStatic:		ss << "type)"; break;
		case owtAgent:		ss << "agent)"; break;
		case owtObject:		ss << "object)"; break;
		case owtLocal:		ss << "local)"; break;
		default:			ss << "unhandled owner)"; break;
	}
	return ss.str();
}



OcVars::OcVars(int ini) : memScheme(ini) {}

void OcVars::Clear() {
	clear();
	memScheme.Clear();
}

void OcVars::Add(const OcVar &v) {
	push_back(v);
}

const char *OcVars::Name(unsigned int i) {
	return (i < size()) ? at(i).name.c_str() : 0;
}

const char *OcVars::Type(unsigned int i) {
	return (i < size()) ? at(i).cl->Name().c_str() : 0;
}

std::string OcVars::ToString() const {
	std::stringstream ss;

	for (const_iterator var = begin(); var != end(); ++var)
		ss << var->ToString() << std::endl;

	return ss.str();
}

OcVar *OcVars::Contains(const std::string &name) {
	for (iterator var = begin(); var != end(); ++var)
		if (var->name == name)
			return &(*var);
	return 0;
}

OcVar *OcVars::Contains(const std::string &name, const std::string &typeName) {
	for (iterator var = begin(); var != end(); ++var)
		if (var->name == name && var->cl->Name() == typeName)
			return &(*var);
	return 0;
}

// Updates variable offsets using a mem scheme.
// Uses local mem scheme unless an external one is specified.
void OcVars::UpdateOffsets(MemScheme *extMemScheme) {

	MemScheme *ms = extMemScheme ? extMemScheme : &memScheme;

	for (iterator v = begin(); v != end(); ++v)
		v->off = ms->Add(v->cl);
}



OcVarses::OcVarses() {}

OcVarses &OcVarses::operator<<(OcVars *vars) {
	push_back(vars);
	return *this;
}

const OcVar *OcVarses::Contains(const std::string &name) const {
	const OcVar *var;
	for (const_iterator vars = begin(); vars != end(); ++vars)
		if ((var = (*vars)->Contains(name)))
			return var;
	return 0;
}
