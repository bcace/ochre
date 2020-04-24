#ifndef ocblock_h
#define ocblock_h

#include "statement.h"
#include "descriptors/variable.h"
#include "runtime/instruction.h"
#include <vector>
#include <list>



class Ochre;
class OcEnv;
class OcType;

enum BlockOffFlags { bofSingle = 1, bofMulti = 2, bofRoot = 4 };

class OcBlock : public std::list<OcStat>
{
	friend class OcStat;

public:
	OcBlock();

	const OcVars &Vars() const;

	void ParseExpressions(OcType *, OcVarses, KeyType, OcType *, OcVar *);
	void ParseTemplates(Ochre *, OcType *);
	void ParseDeclarations(OcType *, OcVarses, bool);
	iterator BlockOff(OcType *, int);
	void RemoveAbsentRequires(OcType *);
	void CheckIfElseStatements(OcType *);
	void AppendOffsets(MemScheme &);
	void Generate(OcEnv *, OcRoutine *);
	void ConnectIfElses(OcInses &, OcRoutine *);
	std::string ToString(const std::string &) const;
	
	const OcToken *GetTokenAt(int) const;

private:
	OcVars vars;
};

#endif
