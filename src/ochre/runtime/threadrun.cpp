#include "runtime/env.h"
#include "runtime/instruction.h"
#include "classes/refs/genericrefs.h"
#include <stdlib.h>


void RunTask(OcThread *);
void DebugTask(OcThread *);

void *ThreadFunc(void *arg) {

	OcThread *t = (OcThread *)arg;

	srand((unsigned int)t->seed);

	while (!t->stop) {	// run until stopped

		pthread_mutex_lock(&t->awaitTask);
		pthread_mutex_unlock(&t->awaitTask);

		if (t->stop)
			break;

		// sort task only

		if (t->task->phase & itpSort)
			t->task->p0->Grid().Sort(t->index);
		else if (t->task->r0 && t->task->r0->breakIns)
			DebugTask(t);
		else
			RunTask(t);

		pthread_mutex_lock(&t->awaitTask);
		pthread_mutex_unlock(&t->blockWorker);
	}

	return 0;
}



#define DO_INSTRUCTIONS i = iFirst; \
while (i) { \
(*next) = i->next; \
i->MapArgs(frame); \
i->exec(v); \
i = (*next); \
}

void RunTask(OcThread *t) {

	OcEnv *env = t->task->env;
	OcRoutine *r0 = t->task->r0;
	OcFrame &frame = t->frame;
	OcIns **next = (OcIns **)frame.c[fcNext];
	OcIns *i, *iFirst = &r0->front();
	void **v = frame.v;

	*((int *)frame.c[fcThread]) = t->index;
	v[ARG_THR] = (void *)frame.c[fcThread];	// todo: see if it really needs a chunk

	frame.c[fcType] = t->task->p0->stc;
	frame.c[fcFrame] = (CHUNK *)frame.c;

	// execute task

	if (t->task->phase & (itpSet | itpDo)) {
		DO_INSTRUCTIONS
	}

	else if (t->task->phase & itpDecl) {

		OcBucket &born = t->task->p0->Grid().Born(t->index);
		for (OcBucket::iterator a = born.begin(); a != born.end(); ++a) {

			frame.c[fcThis] = (*a)->c;
			DO_INSTRUCTIONS
		}
	}

	else if (t->task->phase & itpImplSee) {

		OcNeighbors neighbors;
		OcGrid &otherGrid = t->task->p1->Grid();

		OcCellPtrs &thisCells = t->task->p0->Grid().NonEmptyCells();
		for (OcCellPtrs::iterator cptr = thisCells.begin(); cptr != thisCells.end(); ++cptr) {

			OcCell &cell = *(*cptr);
			otherGrid.GetNonEmptyNeighbors(cell.i, cell.j, cell.k, neighbors);

			OcRange &range = cell.Range(t->index);
			for (int a = range.beg; a < range.end; ++a) {	// 'this' agents

				frame.c[fcThis] = cell[a]->c;

				for (int n = 0; n < neighbors.count; ++n) {

					OcCell &otherCell = otherGrid.Cell(neighbors.n[n]);

					// 'other' agents

					if (&cell == &otherCell) {

						for (unsigned int b = 0; b < otherCell.size(); ++b) {
							if (cell[a] != otherCell[b]) {
								frame.c[fcOther] = otherCell[b]->c;
								DO_INSTRUCTIONS
							}
						}
					}
					else {

						for (unsigned int b = 0; b < otherCell.size(); ++b) {
							frame.c[fcOther] = otherCell[b]->c;
							DO_INSTRUCTIONS
						}
					}
				}
			}
		}
	}

	else if (t->task->phase & itpSpecSee) {

		int refsOff = t->task->o;

		OcCellPtrs &cells = t->task->p0->Grid().NonEmptyCells();
		for (OcCellPtrs::iterator c = cells.begin(); c != cells.end(); ++c) {

			OcCell &cell = *(*c);

			OcRange &range = cell.Range(t->index);
			for (int a = range.beg; a < range.end; ++a) {	// 'this' agents

				frame.c[fcThis] = cell[a]->c;

				OcGenericRefs *refs = (OcGenericRefs *)(cell[a]->c + refsOff);
				for (int r = 0; r < refs->count; ++r) {		// 'other' agents

					if (!refs->c[r].IsNull()) {

						frame.c[fcOther] = refs->c[r].agent->c;
						DO_INSTRUCTIONS
					}
				}
			}
		}
	}

	else if (t->task->phase & itpSpecMod) {

		int refsOff = t->task->o;

		OcCellPtrs &cells = t->task->p0->Grid().NonEmptyCells();
		for (OcCellPtrs::iterator c = cells.begin(); c != cells.end(); ++c) {

			for (OcCell::iterator a = (*c)->begin(); a != (*c)->end(); ++a) {

				frame.c[fcThis] = (*a)->c;

				OcGenericRefs *refs = (OcGenericRefs *)((*a)->c + refsOff);
				for (int r = 0; r < refs->count; ++r) {

					if (!refs->c[r].IsNull()) {

						frame.c[fcOther] = refs->c[r].agent->c;
						DO_INSTRUCTIONS
					}
				}
			}
		}
	}

	else if (t->task->phase & itpAct) {

		OcCellPtrs &cells = t->task->p0->Grid().NonEmptyCells();
		for (OcCellPtrs::iterator c = cells.begin(); c != cells.end(); ++c) {

			OcCell &cell = *(*c);
			OcRange &range = cell.Range(t->index);
			for (int a = range.beg; a < range.end; ++a) {

				frame.c[fcThis] = cell[a]->c;
				DO_INSTRUCTIONS
			}
		}
	}
}



#define DO_INSTRUCTIONS i = iFirst; \
while (i) { \
	if (r0->breakIns == i && env->run && !env->stop) { \
		if (pthread_mutex_trylock(&env->breakLock) == 0) { \
			env->state = envSuspended; \
			env->breakType = t->task->p0->Id(); \
			env->breakPos = i->pos; \
		} \
		pthread_mutex_lock(&env->breakLock); \
		pthread_mutex_unlock(&env->breakLock); \
	} \
	(*next) = i->next; \
	i->MapArgs(frame); \
	i->exec(v); \
	i = (*next); \
}

void DebugTask(OcThread *t) {

	OcEnv *env = t->task->env;
	OcRoutine *r0 = t->task->r0;
	OcFrame &frame = t->frame;
	OcIns **next = (OcIns **)frame.c[fcNext];
	OcIns *i, *iFirst = &r0->front();
	void **v = frame.v;

	*((int *)frame.c[fcThread]) = t->index;
	v[ARG_THR] = (void *)frame.c[fcThread];	// todo: see if it really needs a chunk

	frame.c[fcType] = t->task->p0->stc;
	frame.c[fcFrame] = (CHUNK *)frame.c;

	// execute task

	if (t->task->phase & (itpSet | itpDo)) {
		DO_INSTRUCTIONS
	}

	else if (t->task->phase & itpDecl) {

		OcBucket &born = t->task->p0->Grid().Born(t->index);
		for (OcBucket::iterator a = born.begin(); a != born.end(); ++a) {

			frame.c[fcThis] = (*a)->c;
			DO_INSTRUCTIONS
		}
	}

	else if (t->task->phase & itpImplSee) {

		OcNeighbors neighbors;
		OcGrid &otherGrid = t->task->p1->Grid();

		OcCellPtrs &thisCells = t->task->p0->Grid().NonEmptyCells();
		for (OcCellPtrs::iterator cptr = thisCells.begin(); cptr != thisCells.end(); ++cptr) {

			OcCell &cell = *(*cptr);
			otherGrid.GetNonEmptyNeighbors(cell.i, cell.j, cell.k, neighbors);

			OcRange &range = cell.Range(t->index);
			for (int a = range.beg; a < range.end; ++a) {	// 'this' agents

				frame.c[fcThis] = cell[a]->c;

				for (int n = 0; n < neighbors.count; ++n) {

					OcCell &otherCell = otherGrid.Cell(neighbors.n[n]);

					// 'other' agents

					if (&cell == &otherCell) {

						for (unsigned int b = 0; b < otherCell.size(); ++b) {
							if (cell[a] != otherCell[b]) {
								frame.c[fcOther] = otherCell[b]->c;
								DO_INSTRUCTIONS
							}
						}
					}
					else {

						for (unsigned int b = 0; b < otherCell.size(); ++b) {
							frame.c[fcOther] = otherCell[b]->c;
							DO_INSTRUCTIONS
						}
					}
				}
			}
		}
	}

	else if (t->task->phase & itpSpecSee) {

		int refsOff = t->task->o;

		OcCellPtrs &cells = t->task->p0->Grid().NonEmptyCells();
		for (OcCellPtrs::iterator c = cells.begin(); c != cells.end(); ++c) {

			OcCell &cell = *(*c);

			OcRange &range = cell.Range(t->index);
			for (int a = range.beg; a < range.end; ++a) {	// 'this' agents

				frame.c[fcThis] = cell[a]->c;

				OcGenericRefs *refs = (OcGenericRefs *)(cell[a]->c + refsOff);
				for (int r = 0; r < refs->count; ++r) {		// 'other' agents

					if (!refs->c[r].IsNull()) {

						frame.c[fcOther] = refs->c[r].agent->c;
						DO_INSTRUCTIONS
					}
				}
			}
		}
	}

	else if (t->task->phase & itpSpecMod) {

		int refsOff = t->task->o;

		OcCellPtrs &cells = t->task->p0->Grid().NonEmptyCells();
		for (OcCellPtrs::iterator c = cells.begin(); c != cells.end(); ++c) {

			for (OcCell::iterator a = (*c)->begin(); a != (*c)->end(); ++a) {

				frame.c[fcThis] = (*a)->c;

				OcGenericRefs *refs = (OcGenericRefs *)((*a)->c + refsOff);
				for (int r = 0; r < refs->count; ++r) {

					if (!refs->c[r].IsNull()) {

						frame.c[fcOther] = refs->c[r].agent->c;
						DO_INSTRUCTIONS
					}
				}
			}
		}
	}

	else if (t->task->phase & itpAct) {

		OcCellPtrs &cells = t->task->p0->Grid().NonEmptyCells();
		for (OcCellPtrs::iterator c = cells.begin(); c != cells.end(); ++c) {

			OcCell &cell = *(*c);
			OcRange &range = cell.Range(t->index);
			for (int a = range.beg; a < range.end; ++a) {

				frame.c[fcThis] = cell[a]->c;
				DO_INSTRUCTIONS
			}
		}
	}
}
