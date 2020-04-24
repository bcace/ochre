#ifndef path_h
#define path_h

#include <vector>
#include <string>

#define WIN_HD	"win_hd"



class CiPath
{
public:
	CiPath();
	CiPath(const std::string &);

	std::vector<CiPath> Contents(bool dirsOnly = false) const;
	std::string Location() const;
	std::string Name() const;

	const std::string &Text() const;
	bool Exists() const;
	bool IsDir() const;

	static std::string PathName(const std::string &);

private:
	std::string text;
	bool exists, isDir;

	void CleanPath();
};

typedef std::vector<CiPath> CiPaths;

#endif