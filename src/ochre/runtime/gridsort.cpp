#include "runtime/grid.h"
#include "classes/point.h"
#include <math.h>



void OcGrid::SortPrologue() {
	for (iterator c = begin(); c != end(); ++c) {
		c->RemoveDead(dead, threads);
		c->UpdateRanges(threads);
	}
}

void OcGrid::Sort(int thread) {

	if (volume == 1) {

		OcBucket &bucket = born[thread];
		for (OcBucket::iterator a = bucket.begin(); a != bucket.end(); ++a) {

			OcPoint p(*(*a)->Pos());

			at(0).AcceptMoved(*a, thread);
		}
	}
	else {

		// sort born

		OcBucket &bucket = born[thread];
		for (OcBucket::iterator a = bucket.begin(); a != bucket.end(); ++a) {

			OcPoint p(*(*a)->Pos());

			Cell(	(int)floor(p.x / cellSize),
					(int)floor(p.y / cellSize),
					(int)floor(p.z / cellSize)).AcceptMoved(*a, thread);
		}

		// sort moved
		// todo: add check for stationary types

		for (iterator c = begin(); c != end(); ++c) {
			OcCell &cell = *c;

			OcRange &range = cell.Range(thread);
			for (int r = range.beg; r < range.end; ++r) {

				OcAgent *a = cell[r];
				OcPoint p(*(OcPoint *)(a->c + OcAgent::kernelDataSize));

				// check if agent is still within the same cell

				int i = (int)floor(p.x / cellSize);
				int j = (int)floor(p.y / cellSize);
				int k = (int)floor(p.z / cellSize);

				OcClamp(i, 0, n.i);
				OcClamp(j, 0, n.j);
				OcClamp(k, 0, n.k);

				if (i != c->i || j != c->j || k != c->k) {
					at(k * area + j * length + i).AcceptMoved(a, thread);
					cell.anyMovedOut = true;
				}
			}
		}
	}
}

// Updates non-empty cells list.
void OcGrid::SortEpilogue() {

	born.Clear(false);	// assuming SortBorn() took over all the agents

	nonEmptyCells.clear();
	for (iterator c = begin(); c != end(); ++c) {

		c->RemoveMovedOut();
		c->IntegrateMovedIn();

		if (c->size()) {
			c->UpdateRanges(threads);
			nonEmptyCells.push_back(&(*c));
		}
	}
}
