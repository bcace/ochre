#include "pine/path.h"
#include "dirent.h"
#include <errno.h>



CiPath::CiPath() : text(""), exists(false), isDir(false) {}

CiPath::CiPath(const std::string &_text) : text(_text) {

	CleanPath();

	if (text != WIN_HD) {

		DIR *d = opendir(text.c_str());
		if (d) {
			exists = true;
			isDir = true;
			closedir(d);
		}
		else if (errno == ENOTDIR) {
			exists = true;
			isDir = false;
		}
		else {
			exists = false;
			isDir = false;
		}
	}
	else {
		exists = true;
		isDir = true;
	}
}

CiPaths CiPath::Contents(bool dirsOnly) const {

	CiPaths contents;

	if (!exists || !isDir)
		return contents;

	if (text != WIN_HD) {

		DIR *d = opendir(text.c_str());
		if (d) {

			dirent *e;
			while ((e = readdir(d)) != 0) {

				if (e->d_name[0] == '.')
					continue;

				contents.push_back(CiPath(text + ((text[text.size() - 1] == '/') ? "" : "/") + e->d_name));
			}
			
			closedir(d);
		}
	}
	else {	// windows hd, list drives

		std::string drive("a:/");
		for (unsigned char c = 'a'; c <= 'z'; ++c) {

			drive[0] = c;

			DIR *d = opendir(drive.c_str());
			if (d) {

				contents.push_back(drive);
				closedir(d);
			}
		}
	}

	return contents;
}

std::string CiPath::Location() const {

	if (text == "/")	// linux root dir
		return text;

	if (text.size() == 3 && text[1] == ':' && text[2] == '/')	// win drive
		return WIN_HD;

	for (unsigned int c = text.size() - 2; c >= 0; --c)
		if (text[c] == '/')
			return text.substr(0, c + 1);	// c + 1 to include trailing '/'

	return text;
}

std::string CiPath::Name() const {

	if (text.size() == 3 && text[1] == ':' && text[2] == '/')	// win drive
		return text;

	for (unsigned int c = text.size() - 1; c >= 0; --c)
		if (text[c] == '/')
			return text.substr(c + 1, text.size() - (c + 1));

	return text;
}

void CiPath::CleanPath() {

	// turn all back- into forward- slashes

	for (unsigned int c = 0; c < text.size(); ++c)
		if (text[c] == '\\')
			text[c] = '/';
}

//
// properties
//

const std::string &CiPath::Text() const {
	return text;
}

bool CiPath::Exists() const {
	return exists;
}

bool CiPath::IsDir() const {
	return isDir;
}

//
// static
//

std::string CiPath::PathName(const std::string &path) {

	for (unsigned int c = path.size() - 1; c >= 0; --c)
		if (path[c] == '/')
			return path.substr(c + 1, path.size() - (c + 1));

	return path;
}
