#ifndef session_h
#define session_h

#include <list>
#include <vector>
#include <string>



class CiSession : public std::vector<std::string>
{
public:
	bool operator==(const CiSession &) const;

	std::string ToString() const;
};



class CiSessions : public std::list<CiSession>
{
public:
	static CiSessions ins;

	static CiSession *Session();
	static void MakePopular(const CiSession &);
	static bool MakePopular(int);
	static void Serialize();
	static bool Deserialize();
};

#endif
