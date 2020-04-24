#ifndef ocstatement_h
#define ocstatement_h

#include "token.h"
#include "runtime/instruction.h"
#include <list>



class Ochre;
class OcType;
class OcBlock;
class OcVarses;
struct DeclParseRes;

class OcStat : public OcTokens
{
public:
	enum ParseState { psOperand, psOperator };

	OcStat(OcStat &, OcStat::iterator, OcStat::iterator);
	OcStat(Ochre *, OcType *, const OcString &);
	~OcStat();

	OcBlock *AddBlock();
	OcBlock *Block() const;
	std::string ToString(const std::string &) const;
	bool RequiresBlock() const;
	bool IsBlockStart() const;
	bool IsBlockEnd() const;
	bool IsRequires() const;
	bool CheckRequires(OcType *) const;
	OpType IsIfElse();	// todo: replace with IsOp
	OpType IsOp(int);
	OpType IsRes(int);

	// statementdecl
	DeclParseRes ParseDeclaration(OcType *, const OcVarses &, bool);
	void ParseTemplates(Ochre *, OcType *);

	// statementexpr
	void ParseExpression(OcType *, const OcVarses &, KeyType, OcType *, OcVar *);
	bool ParseOperand(OcType *, OcTokens &, OcTokens::iterator &, int);
	bool ParseInfix(OcType *, OcTokens &, OcTokens::iterator &, int);
	bool ParsePrefix(OcType *, OcTokens &, OcTokens::iterator &, int);
	bool ParseSub(OcType *, OcTokens &, OcTokens::iterator &, int);

	void Generate(OcEnv *, OcRoutine *);
	const OcToken *GetTokenAt(int) const;

private:
	OcBlock *block;
	bool err;

	bool LeftPrecedes(int, int);
};



struct DeclParseRes
{
	bool isDecl;
	std::list<OcStat> exps;

	DeclParseRes(bool _isDecl) : isDecl(_isDecl) {}
};

bool OpPrecedes(int, int);

#endif
