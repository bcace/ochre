#include "session.h"
#include "pine/path.h"
#include <algorithm>
#include <fstream>

#define MAX_SESSIONS	20



bool CiSession::operator==(const CiSession &o) const {

	if (size() != o.size())
		return false;

	for (const_iterator s = begin(); s != end(); ++s) {

		const_iterator m = std::find(o.begin(), o.end(), *s);
		if (m == o.end())
			return false;
	}

	return true;
}

std::string CiSession::ToString() const {

	std::string s;

	for (unsigned int i = 0; i < size(); ++i)
		s += CiPath::PathName(at(i)) + ((i + 1 < size()) ? ", " : "");

	return s;
}

//
// manager
//

CiSessions CiSessions::ins;

CiSession *CiSessions::Session() {
	return ins.size() ? &ins.front() : 0;	// todo: consider dummy session
}

void CiSessions::MakePopular(const CiSession &s) {

	ins.remove(s);
	ins.insert(ins.begin(), s);
	while (ins.size() > MAX_SESSIONS)
		ins.pop_back();

	Serialize();
}

bool CiSessions::MakePopular(int i) {

	if (i <= 0 || i >= ins.size())
		return false;

	CiSessions::iterator s = ins.begin();
	for (unsigned int j = 0; j < i; ++j) ++s;
	ins.splice(ins.begin(), ins, s);

	Serialize();

	return true;
}

void CiSessions::Serialize() {

	std::ofstream file("cinnabar.sessions", std::ofstream::trunc);
	if (file.is_open()) {

		for (iterator s = ins.begin(); s != ins.end(); ++s) {

			file << "session\n";
			
			for (CiSession::iterator t = s->begin(); t != s->end(); ++t)
				file << *t << "\n";
		}

		file.close();
	}
}

bool CiSessions::Deserialize() {

	std::ifstream file("cinnabar.sessions");
	if (file.is_open()) {

		std::string line;
		while (std::getline(file, line)) {

			if (line == "session")
				ins.push_back(CiSession());
			else if (ins.size())
				ins.back().push_back(line);
		}

		file.close();
	}

	return ins.size() > 0;
}
