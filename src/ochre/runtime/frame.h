#ifndef frame_h
#define frame_h

#include "runtime/chunk.h"

#define MAX_CHUNKS	128
#define REF_CHUNKS	48	// referenced agent chunks start at this index
#define MAX_ARGS	8	// register slots count
#define ARG_RES		7	// result slot, MAX_ARGS - 1
#define ARG_THR		6	// thread index slot, MAX_ARGS - 2
#define ARG_ENV		5	// env slot, MAX_ARGS - 3



enum FrameChunk {

	fcFrame = 0,	// ptr to frame chunks array as chunk (must be first!)

	// owned by frame

	fcThread,	// thread index
	fcNext,		// storage for the next instruction pointer
	fcRes,
	fcLocal,

	// owned by 'this', 'other' agents or 'this' type

	fcThis,
	fcOther,
	fcType,

	// owned by routine

	fcLit,
	fcJump,

	// owned by env

	fcPools,	// storage for pointers to pools
	fcStatic,	// type static chunks, fcStatic [+ index]

	// first agent chunk ptr retrieved by reference

	fcRef = REF_CHUNKS		// usage: fcRef [+ offset]
};



class OcIns;
class OcTask;

struct OcFrame
{
	CHUNK *c[MAX_CHUNKS];
	CHUNK thr[sizeof(int)], nxt[sizeof(void *)];
	void *v[MAX_ARGS];

	void MapArgs(OcIns *);
	void AllocateChunks(OcTask *, CHUNK *, CHUNK *, CHUNK *, CHUNK **, unsigned int, void *);
	void DeleteChunks(OcTask *);
};

#endif
