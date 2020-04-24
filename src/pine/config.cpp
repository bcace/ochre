#include "pine/config.h"
#include <sstream>
#include <fstream>
#include <iostream>



Config::Config(const std::string &path) {

	insert(CiConfigPair("openPath", path));
	insert(CiConfigPair("newPath", path));
	insert(CiConfigPair("scheme", "Monokai"));

	Deserialize();
}

void Config::Set(const std::string &k, const std::string &v) {
	iterator i = find(k);
	if (i != end())
		i->second = v;
}

void Config::Set(const std::string &k, int v) {
	iterator i = find(k);
	if (i != end()) {
		std::stringstream ss;
		ss << v;
		i->second = ss.str();
	}
}

std::string Config::GetString(const std::string &k) {
	iterator i = find(k);
	if (i != end())
		return i->second;
	else
		return "";
}

int Config::GetInt(const std::string &k) {
	iterator i = find(k);
	if (i != end()) {
		std::stringstream ss(i->second);
		int n;
		ss >> n;
		return n;
	}
	else
		return 0;
}

void Config::Serialize() {

	std::ofstream file("cinnabar.config", std::ofstream::trunc);
	if (file.is_open()) {

		for (iterator c = begin(); c != end(); ++c)
			file << c->first << ":" << c->second << "\n";

		file.close();
	}
}

void Config::Deserialize() {

	std::ifstream file("cinnabar.config");
	if (file.is_open()) {

		std::string line;
		while (std::getline(file, line)) {

			size_t d = line.find_first_of(':');
			if (d != std::string::npos) {

				std::string k = line.substr(0, d);
				std::string v = line.substr(d + 1, line.size() - d - 1);

				iterator i = find(k);
				if (i != end())
					i->second = v;
			}
		}

		file.close();
	}
}
