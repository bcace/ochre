#include "runtime/grid.h"
#include "runtime/pool.h"
#include "runtime/agent.h"



OcCell::OcCell(OcGrid *_grid, int _i, int _j, int _k, int threads) :
	grid(_grid), i(_i), j(_j), k(_k), movedIn(threads),
	anyDead(false), anyMovedOut(false) {}

// Updates the number of vectors used for receiving
// agents removed fom their cells during threaded sort.
void OcCell::UpdateThreadCount(int threads) {
	movedIn.resize(threads);
	UpdateRanges(threads);
}

MemScheme &OcCell::AgentMemScheme() {
	return grid->pool->vars.memScheme;
}

void OcCell::SetIndices(int _i, int _j, int _k) {
	i = _i; j = _j; k = _k;
}

void OcCell::MarkAsHavingDead() {
	anyDead = true;
}

void OcCell::AcceptMoved(OcAgent *a, int thread) {
	movedIn[thread].push_back(a);
	a->SetCell(this);
}

// Moves sorted agents from thread-specific vectors
// into the main vector.
void OcCell::IntegrateMovedIn() {

	for (OcBuckets::iterator m = movedIn.begin(); m != movedIn.end(); ++m) {

		for (OcAgents::iterator a = m->begin(); a != m->end(); ++a)
			push_back(*a);

		m->clear();
	}
}

void OcCell::RemoveMovedOut() {

	if (!anyMovedOut)
		return;

	for (iterator a = begin(); a != end();)
		if ((*a)->Cell() != this) {

			*a = back();
			pop_back();
		}
		else
			++a;

	anyMovedOut = false;
}

// Moves agents marked as dead (cell == 0) from
// the cell into the dead population.
void OcCell::RemoveDead(OcBuckets &dead, int threads) {

	if (!anyDead)
		return;

	int rr = 0;	// just to distribute the dead evenly among threads
	for (iterator a = begin(); a != end();)
		if ((*a)->Cell() == 0) {

			(*a)->Ptr().id = ++grid->id;
			dead[rr].push_back(*a);	// add to dead

			*a = back();
			pop_back();

			if (++rr >= threads)
				rr = 0;
		}
		else
			++a;

	UpdateRanges(threads);

	anyDead = false;
}
