#ifndef cell_h
#define cell_h

#include "runtime/bucket.h"
#include "classes/point.h"



class OcGrid;

struct OcCell : public OcBucket
{
	OcGrid *grid;
	int i, j, k;
	OcBuckets movedIn;
	bool anyDead;		// flag set if there are agents waiting to be deleted
	bool anyMovedOut;	// flag set if any agets moved out during sort

	OcCell(OcGrid *, int, int, int, int);

	MemScheme &AgentMemScheme();

	void SetIndices(int, int, int);
	void MarkAsHavingDead();

	void UpdateThreadCount(int);
	void AcceptMoved(OcAgent *, int);
	void IntegrateMovedIn();
	void RemoveMovedOut();
	void RemoveDead(OcBuckets &, int);
};

typedef std::vector<OcCell>		OcCells;
typedef std::vector<OcCell *>	OcCellPtrs;



struct OcNeighbor
{
	int i, j, k;

	void Set(int _i, int _j, int _k) {
		i = _i; j = _j; k = _k;
	}
};

struct OcNeighbors
{
	OcNeighbor n[27];
	int count;
};

#endif
