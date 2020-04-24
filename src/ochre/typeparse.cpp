#include "ochre.h"
#include "statement.h"
#include <iostream>



bool OcType::Parse(Ochre *ochre, int phase) {

	// parse title name

	if (phase == pphTitle) {

		issues.clear();
		sections.clear();
		vars.Clear();
		svars.Clear();

		Uncomment();
		lines = Split("\r\n;~{}");

		if (lines.size()) {
			ParseTitle(lines.front());
			lines.erase(lines.begin());
		}
		else
			AddIssue("Missing agent type name", itError, 0);
	}

	// block off & parse declarations

	else if (phase == pphDeclarations)
		ParseUpToExpressions(ochre, lines);

	// parse expressions

	else if (phase == pphExpressions)
		ParseExpressions(ochre);

	return issues.size() == 0;
}

void OcType::ParseUpToExpressions(Ochre *ochre, const OcStringVector &lines) {

	// create statements, distribute to sections

	for (OcStringVector::const_iterator line = lines.begin(); line != lines.end(); ++line)
		OcStat stat(ochre, this, *line);

	// erase ignored sections

	OcSection *declSect = 0;
	for (OcSections::iterator s = sections.begin(); s != sections.end(); ++s)
		if (s->desc.type == ktNoKey)
			declSect = &(*s);

	// create statement blocks

	for (OcSections::iterator sect = sections.begin(); sect != sections.end(); ++sect) {
		sect->BlockOff(this, bofRoot | bofMulti);
		sect->RemoveAbsentRequires(this);
		sect->CheckIfElseStatements(this);
	}

	for (OcSections::iterator sect = sections.begin(); sect != sections.end(); ++sect)
		sect->ParseTemplates(ochre, this);

	// add built-in variables

	vars.Add(OcVar("P", owtAgent, ochre->Class("point")));

	// parse declaration section declarations

	if (declSect)
		declSect->ParseDeclarations(this, OcVarses() << &svars << &vars, true);

	// parse interaction sections arguments

	for (OcSections::iterator s = sections.begin(); s != sections.end(); ++s)
		s->ParseInteractionArgument(ochre, this);

	// erase ignored sections

	for (OcSections::iterator s = sections.begin(); s != sections.end();)
		if (s->desc.ignored)
			s = sections.erase(s);
		else
			++s;

	// parse local declarations

	for (OcSections::iterator s = sections.begin(); s != sections.end(); ++s)
		if (s->desc.type != ktNoKey)
			s->ParseDeclarations(this, OcVarses() << &svars << &vars, false);

	// output declared vars

	ochre->out << "vars:" << std::endl << vars.ToString() << "svars: " << std::endl << svars.ToString();
}

void OcType::ParseExpressions(Ochre *ochre) {

	// parse expressions

	for (OcSections::iterator s = sections.begin(); s != sections.end(); ++s)
		s->ParseExpressions(this, OcVarses() << &svars << &vars, s->desc.type, s->desc.other, s->desc.var);

	// output parse tree

	for (OcSections::iterator section = sections.begin(); section != sections.end(); ++section)
		ochre->out << section->ToString() << std::endl;
	ochre->out << std::endl;
}

// Extracts the agent type name from the
// first non-comment line in the source.
void OcType::ParseTitle(const OcString &title) {

	int err = title.IsValidName();
	if (err == 0)
		name = title;
	else if (err == 1)
		AddIssue("Empty agent type name", itError, title.Pos());
	else if (err == 2)
		AddIssue("Invalid agent type name. Type name cannot contain a whitespace character",
			itError, title.Pos());
	else if (err == 3)
		AddIssue("Invalid agent type name. Type name cannot contain a reserved character: " + OcString::resChars,
			itError, title.Pos());
}
