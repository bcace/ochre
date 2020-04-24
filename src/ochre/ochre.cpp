#include "ochre.h"
#include "classes/real.h"
#include "classes/bool.h"
#include "classes/int.h"
#include "classes/point.h"
#include "classes/text.h"
#include "classes/box.h"
#include <iostream>
//#include <fstream>



Ochre *Ochre::ins = 0;

Ochre::Ochre() : typeId(0), parseId(0), parsed(false) {

	cls.push_back(CreateRealClass());
	cls.push_back(CreateBoolClass());
	cls.push_back(CreateIntClass());
	cls.push_back(CreatePointClass());
	cls.push_back(CreateEnvClass());
	cls.push_back(CreateTextClass());
	cls.push_back(CreateBoxClass());

	InitRealClass(this);
	InitBoolClass(this);
	InitIntClass(this);
	InitPointClass(this);
	InitEnvClass(this);
	InitTextClass(this);
	InitBoxClass(this);
/*
	std::ofstream file("classref_dump", std::ofstream::trunc);
	if (file.is_open()) {

		for (std::vector<OcClass *>::iterator c = cls.begin(); c != cls.end(); ++c)
			file << (*c)->Description();
		file.close();
	}*/
}

Ochre::~Ochre() {
	for (std::vector<OcClass *>::iterator cl = cls.begin(); cl != cls.end(); ++cl)
		delete *cl;
}

int Ochre::AddType(const std::string &path) {

	push_back(OcType(typeId++, path));
	back().InitTypeClass(this);

	parsed = false;

	return back().Id();
}

int Ochre::RemoveType() {

	if (selType != end())
		for (iterator t = begin(); t != end(); ++t)
			if (selType == t) {
				erase(t);
				break;
			}

	parsed = false;

	return 0;
}

int Ochre::ReloadType(const char *pathOrSource, int source) {

	if (selType != end()) {
		selType->Reload(pathOrSource, source);
		parsed = false;
	}

	return 0;
}

// Parses loaded types' source codes,
// builds syntax trees.
int Ochre::Parse() {

	for (iterator t = begin(); t != end(); ++t)
		t->Parse(this, pphTitle);

	for (iterator tA = begin(); tA != --iterator(end()); ++tA)
		for (iterator tB = ++iterator(tA); tB != end(); ++tB)
			if (tA->Name() == tB->Name())
				tB->AddIssue("Agent type with the same name already loaded from: " + tA->Path(), itError, 0);

	parsed = true;

	for (iterator t = begin(); t != end(); ++t)
		if (!t->Parse(this, pphDeclarations))
			parsed = false;

	for (iterator t = begin(); t != end(); ++t)
		if (!t->Parse(this, pphExpressions))
			parsed = false;

	// count & print issues to out

	int issues = 0;
	for (iterator t = begin(); t != end(); ++t)
		for (OcIssues::const_iterator issue = t->Issues().begin(); issue != t->Issues().end(); ++issue, ++issues) {
			switch (issue->type) {
				case itError:	out << "error: "; break;
				case itWarning:	out << "warning: "; break;
				case itNote:
				default:		out << "note: "; break;
			}
			out << issue->loc.ln << ":" << issue->loc.cl << "\t" << issue->txt << "\t<" << issue->src << ">" << std::endl;
		}

	++parseId;

	return issues;
}

// Parse id is updated after each parse, and used to
// determine if new instructions should be generated.
int Ochre::ParseId() const {
	return parseId;
}

// Returns true if parse-time env in general is ok
// and can be synced with run-time env (parsed correctly
// after modifying/adding/removing types/code).
bool Ochre::Parsed() const {
	return parsed;
}

OcType *Ochre::Type(int typeId) {
	for (iterator t = begin(); t != end(); ++t)
		if (t->id == typeId)
			return &(*t);
	return 0;
}

OcType *Ochre::Type(const std::string &name) {

	for (iterator t = begin(); t != end(); ++t)
		if (t->Name() == name)
			return &(*t);

	return 0;
}

OcClass *Ochre::Class(const std::string &_name, OcClass *_template, bool _ignoreTemplate) {

	for (std::vector<OcClass *>::iterator cl = cls.begin(); cl != cls.end(); ++cl)
		if ((*cl)->Name() == _name && (_ignoreTemplate || (*cl)->Template() == _template))
			return *cl;

	return 0;
}

void Ochre::AddClass(OcClass *cl) {
	cls.push_back(cl);
}

OcTypeIdMap Ochre::GetTypeIdMap() {
	OcTypeIdMap m;
	for (iterator t = begin(); t != end(); ++t)
		m.insert(OcTypeIdPair(t->id, &(*t)));
	return m;
}

//
// output
//

// Returns pointer to output buffer data.
// If the output stream is not empty, copies the
// output stream contents to output string buffer
// and clears the stream.
const char *Ochre::GetOutput() {

	if (out.rdbuf()->in_avail() > 0) {
		outString += out.str();
		out.str("");
	}

	return outString.c_str();
}

// Clears the output buffer string and the
// output buffer.
void Ochre::ClearOutput() {
	out.str(outString = "");
}
