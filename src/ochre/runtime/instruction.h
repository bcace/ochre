#ifndef instruction_h
#define instruction_h

#include "descriptors/descriptors.h"
#include "runtime/frame.h"
#include <sstream>
#include <list>



OCF(NullOp);

struct OcIns
{
	EXEC exec;
	OcIns *next;
	int argReg[MAX_ARGS], argChk[MAX_ARGS], argOff[MAX_ARGS], argNo, resNo, pos;
	unsigned int id;

	OcIns(int _id, int _pos) : exec(NullOp), next(0), argNo(0), resNo(-1), pos(_pos), id(_id) {}

	void SetNextArg(int, int);
	void SetRes(int);
	void SetRef(int, int);
	void SetSelfRes();
	void SetJumpArgs(int);
	void MapArgs(OcFrame &);
	int ResChk();
	int ResOff();
	int ResPtrOff();
	void ToOutput(std::stringstream &, CHUNK *);
};

typedef std::list<OcIns *> OcInses;



class OcRoutine : public std::list<OcIns>
{
public:
	CHUNK *lit, *jmp;
	MemScheme litScheme, jmpScheme, locScheme, resScheme;
	int begPos, endPos;	// debug data, copied from the source section
	OcIns *breakIns;

	OcRoutine(int, int);
	~OcRoutine();

	void Clear();
	OcIns *NewInstruction(int);
	OcIns *FirstInstruction();
	OcIns *LastInstruction();
	void ConnectInstructions();

	int AddLiteral(OcClass *, void *);
	int AddJump(OcIns *);

	void SetBreakInstruction(int);
};

typedef std::list<OcRoutine> OcRoutines;



OCF(Jump);

#endif
