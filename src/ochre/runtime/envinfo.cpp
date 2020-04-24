#include "runtime/env.h"
#include "ochre.h"



unsigned OcEnv::ThreadCount() {
	return threadPool.size();
}

int OcEnv::BreakType() {
	return (state == envSuspended) ? breakType : -1;
}

int OcEnv::BreakPosition() {
	return (state == envSuspended) ? breakPos : -1;
}
/*
std::string OcEnv::GetVarInfo(const OcVar *var) const {

	if (state != envSuspended)
		return "";

	return "";
}
*/