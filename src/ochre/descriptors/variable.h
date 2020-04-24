#ifndef variable_h
#define variable_h

#include "descriptors/descriptors.h"
#include "descriptors/docable.h"



class OcClass;

class OcVar : public OcDocable
{
public:
	OcString name;
	OcClass *cl;
	OwnerType owner;
	VarModifier mod;
	int off;		// chunk offset (generate)

	OcVar();
	OcVar(const std::string &, OwnerType, OcClass *);
	OcVar(const std::string &, OcClass *, int);

	UsageType Usage(KeyType, bool, UsageType) const;
	std::string ToString() const;
	std::string Description() const;
};



class OcVars : public std::vector<OcVar>
{
	friend class CopySchema;

public:
	MemScheme memScheme;

	OcVars(int);

	void Clear();
	void Add(const OcVar &);
	const char *Name(unsigned int);
	const char *Type(unsigned int);

	std::string ToString() const;
	OcVar *Contains(const std::string &);
	OcVar *Contains(const std::string &, const std::string &);
	void UpdateOffsets(MemScheme *);
};



class OcVarses : public std::vector<OcVars *>
{
public:
	OcVarses();

	OcVarses &operator<<(OcVars *);
	const OcVar *Contains(const std::string &) const;
};

#endif
