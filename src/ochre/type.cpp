#include "type.h"
#include "context.h"
#include "ocstring.h"



OcType::OcType(int _id, const std::string &_path) : OcClass("", sizeof(OcAgentPtr), true),
	id(_id), path(_path) {

	ReadFile(_path);
}

int OcType::Id() const {
	return id;
}

const std::string &OcType::Path() const {
	return path;
}

const OcIssues &OcType::Issues() const {
	return issues;
}

OcSections &OcType::Sections() {
	return sections;
}

// This must remain the only point where source
// code text is modified.
void OcType::Reload(const char *pathOrCode, int source) {

	if (source == 0)
		ReadFile(path = pathOrCode);
	else
		Copy(pathOrCode);
}

//
// issues
//

void OcType::AddIssue(const std::string &txt, IssueType type, int pos) {
	issues.push_back(OcIssue(path, txt, type, Loc(pos)));
}

// If info is provided finds the matching section, or
// optionally creates a new one.
// If no info is provided, returns the last used section.
OcSection &OcType::GetSection(const SectionDesc &desc, bool newSect) {

	if (sections.size() == 0)
		sections.push_back(OcSection(SectionDesc(ktNoKey)));

	if (!newSect)
		return sections.back();

	for (OcSections::iterator section = sections.begin(); section != sections.end(); ++section)
		if (section->desc == desc) {
			sections.splice(sections.end(), sections, section);
			return sections.back();
		}

	sections.push_back(OcSection(desc));
	return sections.back();
}

//
// debug
//

const OcToken *OcType::GetTokenAt(int pos) const {

	for (OcSections::const_iterator s = sections.begin(); s != sections.end(); ++s) {
		const OcToken *t = s->GetTokenAt(pos);
		if (t)
			return t;
	}

	return 0;
}
