#include "runtime/agent.h"
#include "runtime/cell.h"
#include "runtime/pool.h"
#include "runtime/memmanagement.h"



int OcAgent::kernelDataSize = sizeof(OcAgentPtr) + sizeof(OcCell *) + sizeof(OcPool *);
int OcAgent::cellPtrOffset = sizeof(OcAgentPtr);
int OcAgent::poolPtrOffset = sizeof(OcAgentPtr) + sizeof(OcCell *);



OcAgent::OcAgent(AGENTID _id, OcPool *pool) : c(0) {

	ChkAllocate(&c, OcAgent::kernelDataSize + pool->vars.memScheme.Size());	// todo: cache mem size?

	*((OcAgentPtr *)c) = OcAgentPtr(this, _id);
	*((OcCell **)(c + OcAgent::cellPtrOffset)) = 0;
	*((OcPool **)(c + OcAgent::poolPtrOffset)) = pool;
}

OcAgent::~OcAgent() {
	ChkDelete(&c, Pool()->vars.memScheme);
}

OcAgentPtr &OcAgent::Ptr() {
	return *(OcAgentPtr *)c;
}

OcCell *OcAgent::Cell() {
	return *((OcCell **)(c + OcAgent::cellPtrOffset));
}

void OcAgent::SetCell(OcCell *cell) {
	*((OcCell **)(c + OcAgent::cellPtrOffset)) = cell;
}

OcPoint *OcAgent::Pos() {
	return (OcPoint *)(c + OcAgent::kernelDataSize);
}

void OcAgent::SetPos(const OcPoint &pos) {
	*((OcPoint *)(c + OcAgent::kernelDataSize)) = pos;
}

OcPool *OcAgent::Pool() {
	return *((OcPool **)(c + OcAgent::poolPtrOffset));
}

// Notifies owner cell and sets cell pointer to null.
void OcAgent::Delete() {

	OcCell **cell = (OcCell **)(c + OcAgent::cellPtrOffset);
	if (*cell)
		(*cell)->MarkAsHavingDead();

	*cell = 0;	// so there are no double deletes
}

// Clears user data, keeps kernel data.
void OcAgent::Clear() {
	ChkClear(&c, OcAgent::kernelDataSize, Pool()->vars.memScheme.Size());	// todo: cache mem size?
}

// Reallocates chunk and copies all data wrt copy cshema.
void OcAgent::ReallocateAndCopy(int userDataSize, CopySchema &s) {
	ChkReallocateAndCopy(&c, OcAgent::kernelDataSize + userDataSize, OcAgent::kernelDataSize, &s);
}
