#ifndef grid_h
#define grid_h

#include "runtime/cell.h"
#include "runtime/agent.h"
#include "classes/box.h"



struct GridIterator
{
	unsigned int cell, agent;

	GridIterator() : cell(0), agent(0) {}
};



class OcPool;
struct OcEnvironment;

class OcGrid : public OcCells
{
public:
	OcPool *pool;
	AGENTID id;

	OcGrid(OcPool *);

	void Clear(bool);
	OcAgentPtr &New(int);
	OcAgentPtr &New(int, const OcPoint &);
	void RefreshAgentChunks(int, CopySchema &);
	OcCell &Cell(int, int, int);
	OcCell &CellUnsafe(int, int, int);
	OcCell &Cell(const OcNeighbor &);

	// interface
	OcBucket &Born(int);
	OcCellPtrs &NonEmptyCells();
	int SelectFirst();
	int SelectNext();
	void *GetData(int);
	OcAgentPtr Begin();
	OcAgentPtr Next();

	// gridupdate
	void UpdateThreads(int);
	void UpdateCells(const OcEnvironment &);
	void GetNonEmptyNeighbors(int, int, int, OcNeighbors &);

	// gridsort
	void SortPrologue();
	void Sort(int);
	void SortEpilogue();

private:
	// given (non-derived) grid update params
	int threads;
	double cellSize;
	OcIndices n;

	// derived properties
	OcBuckets born, dead;
	OcCellPtrs nonEmptyCells;	// pointers to non-empty cells
	GridIterator iter;
	int volume, area, length;	// cached index products
};



template<typename T>
T OcMax(T a, T b) {
	return (a > b) ? a : b;
}

template<typename T>
T OcMin(T a, T b) {
	return (a < b) ? a : b;
}

template<typename T>
void OcClamp(T &v, T min, T max) {
	if (v < min) v = min;
	else if (v >= max) v = max - 1;
}

#endif
