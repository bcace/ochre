#include "runtime/pool.h"
#include <cstdlib>



OcGrid::OcGrid(OcPool *_pool) : pool(_pool) {
	Clear(true);
}

// Deletes all agents, including any born and dead agents.
// Resets the available agent id counter.
void OcGrid::Clear(bool resetThreads) {

	if (resetThreads)
		threads = 0;

	n = OcIndices();

	// delete agents, keep buckets

	for (iterator c = begin(); c != end(); ++c)
		c->Clear(true);

	born.Clear(true);
	dead.Clear(true);
	nonEmptyCells.clear();

	id = 0;
}

// Adds a new agent (alocated or recycled from dead)
// to the born collection.
OcAgentPtr &OcGrid::New(int thread) {

	OcAgent *a;

	if (dead[thread].size()) {
		a = dead[thread].Pop();
		a->Clear();
	}
	else
		a = new OcAgent(++id, pool);

	born[thread].push_back(a);

	return a->Ptr();
}

// Adds a new agent (alocated or recycled from dead)
// to the born collection.
OcAgentPtr &OcGrid::New(int thread, const OcPoint &pos) {

	OcAgent *a;

	if (dead[thread].size()) {
		a = dead[thread].Pop();
		a->Clear();
	}
	else
		a = new OcAgent(++id, pool);

	a->SetPos(pos);

	born[thread].push_back(a);

	return a->Ptr();
}

// Reallocates and/or copies live and/or dead agents chunks.
void OcGrid::RefreshAgentChunks(int newSize, CopySchema &s) {

	if (s.modified) {

		for (iterator c = begin(); c != end(); ++c)
			for (OcCell::iterator a = c->begin(); a != c->end(); ++a)
				(*a)->ReallocateAndCopy(newSize, s);

		for (OcBuckets::iterator b = born.begin(); b != born.end(); ++b)
			for (OcBucket::iterator a = b->begin(); a != b->end(); ++a)
				(*a)->ReallocateAndCopy(newSize, s);

		for (OcBuckets::iterator b = dead.begin(); b != dead.end(); ++b)
			for (OcBucket::iterator a = b->begin(); a != b->end(); ++a)
				(*a)->ReallocateAndCopy(newSize, s);
	}
}

// Gets cell at [i, j, k].
OcCell &OcGrid::Cell(int i, int j, int k) {

	OcClamp(i, 0, n.i);
	OcClamp(j, 0, n.j);
	OcClamp(k, 0, n.k);

	return at(k * area + j * length + i);
}

// Gets cell at [i, j, k]. Use only where indices
// are guaranteed to be correct.
OcCell &OcGrid::CellUnsafe(int i, int j, int k) {
	return at(k * area + j * length + i);
}

// Gets cell at [i, j, k]. Use only where indices
// are guaranteed to be correct.
OcCell &OcGrid::Cell(const OcNeighbor &g) {
	return at(g.k * area + g.j * length + g.i);
}

//
// interface bit, maybe should go to another file
//

OcBucket &OcGrid::Born(int thread) {
	return born[thread];
}

OcCellPtrs &OcGrid::NonEmptyCells() {
	return nonEmptyCells;
}

int OcGrid::SelectFirst() {
	iter = GridIterator();
	return nonEmptyCells.size() ? 1 : 0;
}

int OcGrid::SelectNext() {

	if (iter.cell >= nonEmptyCells.size())
		return 0;

	if (++iter.agent >= nonEmptyCells[iter.cell]->size()) {

		if (++iter.cell >= nonEmptyCells.size())
			return 0;

		iter.agent = 0;
	}

	return 1;
}

void *OcGrid::GetData(int off) {
	return (void *)(nonEmptyCells[iter.cell]->at(iter.agent)->c + off);
}

OcAgentPtr OcGrid::Begin() {
	return SelectFirst() ? at(iter.cell)[iter.agent]->Ptr() : OcAgentPtr();
}

OcAgentPtr OcGrid::Next() {
	return SelectNext() ? at(iter.cell)[iter.agent]->Ptr() : OcAgentPtr();
}
