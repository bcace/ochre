#ifndef agent_h
#define agent_h

#include "runtime/chunk.h"
#include <vector>
#include <limits>

#define AGENT_NULL_ID	0



typedef long long int AGENTID;

class OcAgentPtr;
class OcCell;
class OcPool;
class CopySchema;
class OcPoint;

class OcAgent
{
public:
	static int kernelDataSize;	// user-defined agent variables start at this offset
	static int cellPtrOffset, poolPtrOffset;	//

	OcAgent(AGENTID, OcPool *);
	~OcAgent();

	OcAgentPtr &Ptr();
	OcCell *Cell();
	void SetCell(OcCell *);
	OcPoint *Pos();
	void SetPos(const OcPoint &);
	OcPool *Pool();

	void Delete();
	void Clear();
	void ReallocateAndCopy(int, CopySchema &);

	CHUNK *c;

private:
};

typedef std::vector<OcAgent *>	OcAgents;



struct OcAgentPtr
{
	OcAgent *agent;
	AGENTID id;

	OcAgentPtr() :
		agent(0), id(AGENT_NULL_ID) {}
	OcAgentPtr(OcAgent *_agent, AGENTID _id) :
		agent(_agent), id(_id) {}

	/*
	never, ever, ever add a destructor to this struct!
	the refs class won't like it very much...
	*/

	bool IsNull() {

		if (agent == 0)
			return true;
		else if (agent->Ptr().id != id) {
			agent = 0;	// make testing faster next time
			return true;
		}

		return false;
	}

	bool operator==(const OcAgentPtr &other) const {
		return agent == other.agent && id == other.id;
	}
};

#endif
