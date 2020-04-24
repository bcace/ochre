#ifndef octype_h
#define octype_h

#include "section.h"
#include "runtime/pool.h"
#include "descriptors/class.h"
#include <list>



struct OcIssue
{
	std::string src, txt;
	IssueType type;
	OcStringLoc loc;

	OcIssue(const std::string &_src, const std::string &_txt, IssueType _type, const OcStringLoc &_loc) :
		src(_src), txt(_txt), type(_type), loc(_loc) {}
};

#define OC_ISSUE_ERR_ONCE err = true;
#define OC_ISSUE(issue_txt,issue_loc) { type->AddIssue((std::string)issue_txt, itError, issue_loc); }
#define OC_ISSUE_BREAK(issue_txt,issue_loc) { type->AddIssue((std::string)issue_txt, itError, issue_loc); return; }
#define OC_ISSUE_RETURN(issue_txt,issue_loc) { type->AddIssue((std::string)issue_txt, itError, issue_loc); return false; }
#define OC_ISSUE_ERR(issue_txt,issue_loc) { if (!err) { type->AddIssue((std::string)issue_txt, itError, issue_loc); OC_ISSUE_ERR_ONCE } }
#define OC_ISSUE_ERR_RETURN(issue_txt,issue_loc) { if (!err) { type->AddIssue((std::string)issue_txt, itError, issue_loc); OC_ISSUE_ERR_ONCE } return false; }



typedef std::vector<OcIssue>	OcIssues;
typedef std::list<OcSection>	OcSections;

class Ochre;

enum ParsePhases { pphTitle, pphDeclarations, pphExpressions };

class OcType : public OcString, public OcClass
{
	friend class Ochre;
	friend class OcStat;

public:
	OcType(int, const std::string &);
	void InitTypeClass(Ochre *);

	int Id() const;
	bool Parsed() const;
	const std::string &Path() const;
	const OcIssues &Issues() const;
	OcSections &Sections();

	void Reload(const char *, int);
	bool Parse(Ochre *, int);
	void AddIssue(const std::string &, IssueType, int);
	OcSection &GetSection(const SectionDesc &, bool);
	const OcToken *GetTokenAt(int) const;
	
	// typegenerate
	void UpdateOffsets(Ochre *);
	void Generate(Ochre *);

private:
	int id;
	std::string path;
	OcStringVector lines;
	OcIssues issues;
	OcSections sections;

	// typeparse
	void ParseTitle(const OcString &);
	void ParseUpToExpressions(Ochre *, const OcStringVector &);
	void ParseExpressions(Ochre *);
};

typedef std::list<OcType> OcTypes;
typedef std::map<int, OcType *>		OcTypeIdMap;
typedef std::pair<int, OcType *>	OcTypeIdPair;

OCF(NewAgent);
OCF(NewAgentPointPos);
OCF(NewAgentRealPos);
OCF(Begin);
OCF(Next);

OCF(DeleteAgent);
OCF(IsNull);

OCF(GetMember);
OCF(RefEqual);
OCF(AssignRef);
OCF(ReferenceIf);
/*
OCD(RefVectorDestructor);
OCF(RefVectorClear);
OCF(RefVectorAdd);
OCF(RefVectorAt);
*/
#endif
