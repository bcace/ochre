#ifndef ochre_h
#define ochre_h

#include "type.h"
#include "context.h"
#include "runtime/env.h"
#include <sstream>
#include <vector>
#include <list>



class OcClass;

class Ochre : public OcTypes
{
	friend class OcType;
	friend class OcSection;

public:
	static Ochre *ins;

	Ochre();
	~Ochre();

	const char *GetOutput();
	void ClearOutput();

	int AddType(const std::string &);
	int RemoveType();
	int ReloadType(const char *, int);
	int Parse();
	int ParseId() const;
	bool Parsed() const;
	OcType *Type(int);
	OcType *Type(const std::string &);
	OcClass *Class(const std::string &_name, OcClass *_template = 0, bool _ignoreTemplate = false);
	void AddClass(OcClass *);
	OcTypeIdMap GetTypeIdMap();

	// ochreruntime
	bool Run(int, int);
	bool Stop();
	bool Pause();
	bool Resume();
	bool Reset();
	int GetState();
	bool SetThreadCount(int);
	void Step();
	int ThreadCount();
	int BreakType();
	int BreakPosition();
	const char *PositionalInfo(int, int);
	double GetGrid(int *);
	void GetBounds(double *);
	void SetCursorRay(double *);

	// ochrecontext
	bool SelectType(int);
	int SelectFirstType();
	int SelectNextType();
	bool SelectPool(int);
	int SelectFirstPool();
	int SelectNextPool();
	const char *GetPoolName();
	int SelectFirst();
	int SelectNext();
	void *GetData(int);
	void *GetStaticData(int);
	int GetVarOffset(const char *, const char *, int);
	const char *GetVarName(int, int);
	const char *GetVarType(int, int);
	int IssueCount();
	const OcIssue *Issue(int);

private:
	int typeId, parseId;
	bool parsed;
	OcEnv env;
	std::vector<OcClass *> cls;
	std::stringstream out;
	std::string outString, infoString;

	// ochrecontext
	Ochre::iterator selType;
	OcEnv::iterator selPool;
	RuntimeContext context;
};

#endif
