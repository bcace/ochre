#ifndef ocsection_h
#define ocsection_h

#include "block.h"
#include <list>



class Ochre;
class OcType;
class OcRoutine;

struct SectionDesc
{
	KeyType type;
	OcType *other;
	bool ignored;

	// see & mod sections only
	OcString argument;	// string argument
	OcVar *var;			// if interaction specific

	// debug data
	int beg, end;

	SectionDesc(KeyType _type, OcType *_other = 0, bool _ignored = false) :
		type(_type), other(_other), ignored(_ignored), var(0), beg(0), end(0) {}

	bool operator==(const SectionDesc &o) const {
		return type == o.type && other == o.other && ignored == o.ignored && argument == o.argument && var == o.var;
	}
};



class OcSection : public OcBlock
{
public:
	SectionDesc desc;

	OcSection(const SectionDesc &);

	void ParseInteractionArgument(Ochre *, OcType *);
	void SetRoutine(OcRoutine *);
	void GenerateRoutine(Ochre *);
	std::string ToString() const;

private:
	OcRoutine *routine;	// valid during generate only (routine owned by pool)
};

#endif
