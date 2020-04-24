#ifndef octoken_h
#define octoken_h

#include "enums.h"
#include "ocstring.h"
#include "descriptors/class.h"
#include "runtime/instruction.h"
#include <list>



class OcEnv;
class OcType;
class OcToken;

typedef std::list<OcToken> OcTokens;

class OcToken : public OcString
{
	friend class OcStat;

public:
	OcToken();
	OcToken(const OcString &);
	OcToken(const OcString &, int);
	OcToken(const OcString &, int, int);
	OcToken(const OcString &, int, OcClass *);
	OcToken(const OcString &, int, OcClass *, int);
	OcToken(const OcString &, int, OcClass *, bool);
	OcToken(const OcString &, int, OcClass *, double);
	OcToken(const OcString &, int, OcClass *, const std::string &);

	std::string ToString(const std::string &) const;
	int Tt() const;
	int Stt() const;
	OcClass *Cl() const;
	bool IsStruct(StructType) const;
	bool IsKey(KeyType) const;
	bool IsOp(OpType) const;
	bool IsRes(OpType) const;
	bool IsOperand() const;
	bool IsPrefix() const;
	bool IsPrefixRes() const;
	bool IsInfix() const;
	bool IsInfixRes() const;
	bool IsVoid() const;
	const OcToken *Info() const;

	void CommitElse();
	void CommitPrefix(OcTokens &, OcTokens::iterator &);
	void CommitInfix(OcTokens &, OcTokens::iterator &, OcTokens::iterator &);
	void CommitFunc(OcTokens &, OcTokens::iterator &);
	void CommitArgs(OcTokens &, OcTokens::iterator &);
	void FlattenCommas(OcTokens &);
	void SetTemplateInstance(OcClass *);

	// tokenperms
	bool ResolveIdentifiers(OcType *, const OcVarses &, KeyType, OcType *, OcVar *, bool &);
	bool VerifyUse(OcType *, UsageType, KeyType, OcVar *);

	// tokengenerate
	void Generate(OcEnv *, OcRoutine *, int &);

	// tokeninfo
	const OcToken *GetTokenAt(int) const;
	bool GetInfo(std::string &) const;

private:
	int tt, stt, itt;
	OcClass *cl;
	UsageType use;	// if var token, has context and var info encoded at higher bits
	OcProto proto;
	const OcFunc *func;
	const OcVar *var;
	const OcOp *op;
	const OcToken *infoToken; // token info is taken from, initially set to 'this'

	// literals
	int intL;
	bool boolL;
	double realL;
	std::string textL;

	OcTokens args;
	OcIns *ins; // used by generate
	int memberOff, memberOwner; // struct member offset (p.x), and owner

	void SetInstructionArg(OcEnv *, OcRoutine *, OcIns *);
};


#endif
