#ifndef env_h
#define env_h

#include "runtime/thread.h"
#include "runtime/task.h"
#include "runtime/pool.h"
#include "classes/environment.h"
#include <pthread.h>



class Ochre;

class OcEnv : public OcPools
{
public:
	bool run, stop, asleep, setDone;
	CHUNK *pls, *sts[100];		// pool pointers chunk, pool static chunks array
	OcEnvironment env;			// part of the environment that is exposed to the language
	EnvState state;

	// suspended
	int breakType, breakPos;	// current type & position in code
	pthread_mutex_t breakLock;	// used to suspend a thread mid-task

	OcEnv();
	~OcEnv();

	OcSchedule &Schedule();
	OcSchedule &SetSchedule();
	OcSchedule &DeclSchedule();
	OcSchedule &SortSchedule();
	OcThreadPool &Worker();
	OcPool *Pool(int);

	void Refresh(Ochre *);
	void SortPrologue();
	void SortEpilogue();

	bool Run(Ochre *, int, int);
	bool Stop();
	bool Pause();
	bool Resume(Ochre *);
	bool Reset();
	void Restart();

	// envinfo
	unsigned ThreadCount();
	int BreakType();
	int BreakPosition();
//	std::string GetVarInfo(const OcVar *) const;

	bool SetThreadCount(int);
	void ResetEnvironment();
	void UpdateGrids();
	double GetGrid(int *);
	void GetBounds(double *);
	void SetCursorRay(double *);

private:
	int generateId, threadsCount;
	OcSchedule setSchedule, declSchedule, iterSchedule, sortSchedule;
	OcThreadPool threadPool;
	pthread_t ptr;

	void SetBreakInstruction(int, int);
};

void *EnvRun(void *);

#endif
