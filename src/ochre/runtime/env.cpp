#include "runtime/env.h"
#include <windows.h>



OcEnv::OcEnv() :
	run(false), stop(false), setDone(false),
	pls(0), state(envIdle), generateId(0), threadsCount(32) {

	pthread_mutex_init(&breakLock, 0);
}

OcEnv::~OcEnv() {
	Stop();
	ChkDelete(&pls);
	env.Clear();
	pthread_mutex_destroy(&breakLock);
}

OcSchedule &OcEnv::Schedule() {
	return iterSchedule;
}

OcSchedule &OcEnv::SetSchedule() {
	return setSchedule;
}

OcSchedule &OcEnv::DeclSchedule() {
	return declSchedule;
}

OcSchedule &OcEnv::SortSchedule() {
	return sortSchedule;
}

OcThreadPool &OcEnv::Worker() {
	return threadPool;
}

OcPool *OcEnv::Pool(int id) {
	for (iterator p = begin(); p != end(); ++p)
		if (id == p->id)
			return &(*p);
	return 0;
}

bool OcEnv::Run(Ochre *ochre, int typeId, int pos) {

	if (state == envSuspended) {	// continue

		SetBreakInstruction(typeId, pos);
		state = envRunning;
		pthread_mutex_unlock(&breakLock);

		return true;
	}

	if (state == envIdle) {			// run

		Refresh(ochre);
		SetBreakInstruction(typeId, pos);
		state = envRunning;
		stop = false;
		run = true;
		threadPool.Create(threadsCount);
		pthread_create(&ptr, 0, EnvRun, this);
		pthread_mutex_unlock(&breakLock);

		return true;
	}

	return false;
}

bool OcEnv::Stop() {

	if (state == envRunning || state == envPaused) {

		stop = true;
		pthread_join(ptr, 0);
		threadPool.Destroy();
		state = envIdle;

		return true;
	}

	return false;
}

bool OcEnv::Pause() {

	if (state == envRunning) {

		asleep = false;
		run = false;
		while (!asleep)
			Sleep(1);	// wait until all tasks finish their tasks
		state = envPaused;

		return true;
	}

	return false;
}

bool OcEnv::Resume(Ochre *ochre) {

	Refresh(ochre);

	if (state == envPaused) {

		state = envRunning;
		run = true;

		return true;
	}

	return false;
}

bool OcEnv::Reset() {

	if (state == envIdle) {

		for (iterator p = begin(); p != end(); ++p)
			p->Clear(true);
		setDone = false;

		return true;
	}

	return false;
}

// Function only called from the simulation thread,
// at the end of iteration if the flag has been set.
void OcEnv::Restart() {

	if (env.restart) {

		for (iterator p = begin(); p != end(); ++p)
			p->Clear(false);

		setDone = false;
	}
}

bool OcEnv::SetThreadCount(int threads) {

	if (state == envIdle) {
		threadsCount = threads;
		return true;
	}

	return false;
}

void OcEnv::ResetEnvironment() {
	env.Clear();
}

void OcEnv::UpdateGrids() {
	for (iterator p = begin(); p != end(); ++p)
		p->Grid().UpdateCells(env);
}

double OcEnv::GetGrid(int *indices) {

	indices[0] = env.o.i;
	indices[1] = env.o.j;
	indices[2] = env.o.k;
	indices[3] = env.n.i;
	indices[4] = env.n.j;
	indices[5] = env.n.k;

	return env.cellSize;
}

void OcEnv::GetBounds(double *b) {

	b[0] = env.bounds.min.x;
	b[1] = env.bounds.min.y;
	b[2] = env.bounds.min.z;
	b[3] = env.bounds.max.x;
	b[4] = env.bounds.max.y;
	b[5] = env.bounds.max.z;
}

void OcEnv::SetCursorRay(double *r) {
	env.mouseRay = OcBox(OcPoint(r[0], r[1], r[2]), OcPoint(r[3], r[4], r[5]));
}

void OcEnv::SetBreakInstruction(int typeId, int pos) {
	if (state == envSuspended || state == envIdle)
		for (OcEnv::iterator p = begin(); p != end(); ++p)
			p->SetBreakInstruction((p->Id() == typeId) ? pos : -1);
}
