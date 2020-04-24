#ifndef config_h
#define config_h

#include <string>
#include <map>



typedef std::pair<std::string, std::string> CiConfigPair;

class Config : public std::map<std::string, std::string>
{
public:
	Config(const std::string &);

	void Set(const std::string &, const std::string &);
	void Set(const std::string &, int);

	std::string GetString(const std::string &);
	int GetInt(const std::string &);

	void Serialize();
	void Deserialize();
};

#endif
