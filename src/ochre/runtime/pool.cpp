#include "runtime/pool.h"
#include "classes/box.h"
#include <cmath>



OcPool::OcPool(int _id, const std::string &_name) :
	stc(0), svars(0), vars(0), id(_id), name(_name), grid(this) {}

OcPool::OcPool(const OcPool &other) :
	stc(0), svars(0), vars(0), id(other.id), name(other.name), grid(this) {}

OcPool::~OcPool() {
	ChkDelete(&stc, svars.memScheme);
}

OcGrid &OcPool::Grid() {
	return grid;
}

int OcPool::Id() const {
	return id;
}

const std::string &OcPool::Name() const {
	return name;
}

int OcPool::Offset() const {
	return off;
}

void OcPool::SetOffset(int _off) {
	off = _off;
}

int OcPool::Index() const {
	return idx;
}

void OcPool::SetIndex(int _idx) {
	idx = _idx;
}

void OcPool::ClearRoutines() {
	routines.clear();
}

OcRoutine *OcPool::NewRoutine(int beg, int end) {
	routines.push_back(OcRoutine(beg, end));
	return &routines.back();
}

void OcPool::Clear(bool resetThreads) {

	// delete all agents

	grid.Clear(resetThreads);

	// clear pool static chunk so it doesn't appear as garbage in the next run

	ChkClear(&stc, 0, svars.memScheme.Size());
}

// Updates type's and agents' memory chunks by reallocating
// and copying old data into new memory chunks.
void OcPool::Update(OcVars &newSvars, OcVars &newVars) {

	CopySchema ss(svars, newSvars), s(vars, newVars);

	// type memory chunk

	if (ss.modified) {
		ChkReallocateAndCopy(&stc, newSvars.memScheme.Size(), 0, &ss);
	}

	// agent memory chunks

	grid.RefreshAgentChunks(newVars.memScheme.Size(), s);

	svars = newSvars;
	vars = newVars;
}

void OcPool::SetBreakInstruction(int pos) {
	for (OcRoutines::iterator r = routines.begin(); r != routines.end(); ++r)
		r->SetBreakInstruction(pos);
}
