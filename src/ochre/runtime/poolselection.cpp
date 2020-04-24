#include "runtime/pool.h"



void *OcPool::GetStaticData(int o) {
	return (void *)(stc + o);
}

int OcPool::GetVarOffset(const char *n, const char *t, int o) {

	if (o == 0) {
		const OcVar *v = vars.Contains(n, t);
		return v ? v->off : -1;
	}
	else if (o == 1) {
		const OcVar *v = svars.Contains(n, t);
		return v ? v->off : -1;
	}

	return -1;
}

const char *OcPool::GetVarName(int i, int s) {

	if (s == 0)
		return vars.Name(i);
	else if (s == 1)
		return svars.Name(i);

	return 0;
}

const char *OcPool::GetVarType(int i, int s) {

	if (s == 0)
		return vars.Type(i);
	else if (s == 1)
		return svars.Type(i);

	return 0;
}
