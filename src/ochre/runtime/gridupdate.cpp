#include "runtime/pool.h"
#include "classes/environment.h"
#include <math.h>



void OcGrid::UpdateThreads(int _threads) {

	if (threads != _threads) {

		threads = _threads;

		// rebuild born & dead threads

		OcAgents bb;
		for (OcBuckets::iterator b = born.begin(); b != born.end(); ++b)
			for (OcBucket::iterator a = b->begin(); a != b->end(); ++a)
				bb.push_back(*a);

		OcAgents db;
		for (OcBuckets::iterator b = dead.begin(); b != dead.end(); ++b)
			for (OcBucket::iterator a = b->begin(); a != b->end(); ++a)
				db.push_back(*a);

		born.Clear(false);
		dead.Clear(false);

		born.resize(threads);
		dead.resize(threads);

		int rr = 0;

		for (OcBucket::iterator a = bb.begin(); a != bb.end(); ++a) {
			born[rr].push_back(*a);
			if (++rr >= threads) rr = 0;
		}

		for (OcBucket::iterator a = db.begin(); a != db.end(); ++a) {
			dead[rr].push_back(*a);
			if (++rr >= threads) rr = 0;
		}

		// rebuild cell threads

		for (iterator c = begin(); c != end(); ++c)
			c->UpdateThreadCount(threads);
	}
}

void OcGrid::UpdateCells(const OcEnvironment &env) {

	cellSize = env.cellSize;

	if (n != env.n) {

		n = env.n;

		length = n.i;
		area = length * n.j;
		volume = area * n.k;

		// clean out all dead agents first

		for (iterator c = begin(); c != end(); ++c)
			c->RemoveDead(dead, threads);

		// move all agents to a vector before clearing cells

		OcAgents agents;
		for (iterator c = begin(); c != end(); ++c) {
			for (OcCell::iterator a = c->begin(); a != c->end(); ++a)
				agents.push_back(*a);
			c->Clear(false);	// so the OcBucket destructor doesn't delete agents
		}

		// create empty cells

		clear();
		reserve(volume);

		for (int k = 0; k < n.k; ++k)
			for (int j = 0; j < n.j; ++j)
				for (int i = 0; i < n.i; ++i)
					push_back(OcCell(this, i, j, k, threads));

		// populate cells

		for (OcBucket::iterator a = agents.begin(); a != agents.end(); ++a) {

			OcPoint p(*(*a)->Pos());

			if (volume == 1)
				at(0).AcceptMoved(*a, 0);
			else
				Cell(	(int)floor(p.x / cellSize),
						(int)floor(p.y / cellSize),
						(int)floor(p.z / cellSize)).AcceptMoved(*a, 0);
		}

		// update non-empty cells list

		nonEmptyCells.clear();
		for (iterator c = begin(); c != end(); ++c) {
			c->UpdateRanges(threads);
			if (c->size())
				nonEmptyCells.push_back(&(*c));
		}
	}
}

// Gets an array of non-empty cell indices neighboring the
// one specified with ci, cj and ck.
void OcGrid::GetNonEmptyNeighbors(int ci, int cj, int ck, OcNeighbors &neighbors) {

	neighbors.count = 0;

	int i0 = OcMax(ci - 1, 0), i1 = OcMin(ci + 2, n.i);
	int j0 = OcMax(cj - 1, 0), j1 = OcMin(cj + 2, n.j);
	int k0 = OcMax(ck - 1, 0), k1 = OcMin(ck + 2, n.k);

	// look for non-empty cells within range

	for (int i = i0; i < i1; ++i)
		for (int j = j0; j < j1; ++j)
			for (int k = k0; k < k1; ++k)
				if (CellUnsafe(i, j, k).size())
					neighbors.n[neighbors.count++].Set(i, j, k);
}
