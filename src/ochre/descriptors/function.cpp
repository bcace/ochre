#include "descriptors/class.h"
#include <sstream>



bool OcFunc::Match(const OcFuncSign &sgn) const {

	if (sgn.size() != args.size())
		return false;

	for (int i = 0; i < (int)args.size(); ++i)
		if (args[i].cl != sgn[i])
			return false;

	return true;
}

std::string OcFunc::Description() const {

	std::stringstream ss;

	if (res.cl)
		ss << (returnsThis ? "[this, " : "[") << res.cl->Name().c_str();
	else
		ss << "[void";

	ss << "] " << name << "(";

	for (int i = 0; i < (int)args.size(); ++i)
		ss << args[i].cl->Name() << " " << args[i].name <<
		((i < (int)args.size() - 1) ? ", " : "");

	ss << ")";

	return ss.str();
}
