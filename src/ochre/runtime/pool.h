#ifndef pool_h
#define pool_h

#include "descriptors/variable.h"
#include "runtime/instruction.h"
#include "runtime/agent.h"
#include "runtime/grid.h"
#include <map>



struct OcBox;

class OcPool
{
	friend class OcEnv;

public:
	CHUNK *stc;	// type (static) chunk
	OcVars svars, vars;

	OcPool(int, const std::string &);
	OcPool(const OcPool &);
	~OcPool();

	OcGrid &Grid();

	int Id() const;
	const std::string &Name() const;
	int Offset() const;
	void SetOffset(int);
	int Index() const;
	void SetIndex(int);

	void ClearRoutines();
	OcRoutine *NewRoutine(int, int);

	void Clear(bool);
	void Update(OcVars &, OcVars &);

	void SetBreakInstruction(int);

	// poolselection
	void *GetStaticData(int);
	int GetVarOffset(const char *, const char *, int);
	const char *GetVarName(int, int);
	const char *GetVarType(int, int);

private:
	int id;				// must be equal to type id
	std::string name;	// must be equal to type name
	OcRoutines routines;
	OcGrid grid;
	int off;	// offset of pool pointer in the pool chunk (owned by env)
	int idx;	// index of pool static chunk in frame (after fcStatic)
};

typedef std::list<OcPool> OcPools;

#endif
