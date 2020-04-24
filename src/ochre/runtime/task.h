#ifndef task_h
#define task_h

#include "enums.h"
#include "runtime/chunk.h"
#include <list>



class OcEnv;
class OcPool;
class OcRoutine;

class OcTask
{
public:
	OcEnv *env;
	IterPhase phase;
	OcPool *p0, *p1;
	int o;	// specific interactions only, reference collection offset in agent chunk
	OcRoutine *r0;

	OcTask(OcEnv *_env, IterPhase _phase, OcPool *_p0) :
		env(_env), phase(_phase), p0(_p0), o(-1), r0(0) {}

	OcTask(OcEnv *_env, IterPhase _phase, OcPool *_p0, OcRoutine *_r0) :
		env(_env), phase(_phase), p0(_p0), o(-1), r0(_r0) {}

	OcTask(OcEnv *_env, IterPhase _phase, OcPool *_p0, OcPool *_p1, OcRoutine *_r0) :
		env(_env), phase(_phase), p0(_p0), p1(_p1), o(-1), r0(_r0) {}

	OcTask(OcEnv *_env, IterPhase _phase, OcPool *_p0, OcPool *_p1, int _o, OcRoutine *_r0) :
		env(_env), phase(_phase), p0(_p0), p1(_p1), o(_o), r0(_r0) {}

	bool operator<(const OcTask &other) {
		return phase < other.phase;
	}
};

typedef std::list<OcTask> OcSchedule;

#endif
