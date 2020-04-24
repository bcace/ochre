#include "section.h"
#include "ochre.h"
#include <sstream>



OcSection::OcSection(const SectionDesc &_desc) : desc(_desc), routine(0) {}

void OcSection::ParseInteractionArgument(Ochre *ochre, OcType *type) {

	if (desc.type & (ktSee | ktMod)) {

		// compare argument with declared this type/agent variables
		// specifically 'refs'

		OcVar *v = type->vars.Contains(desc.argument);
		if (v) {

			if (v->cl->RefsQ() == rqAgentRefs) {
				desc.other = (OcType *)v->cl->Template();
				desc.var = v;
				return;
			}
			else
				OC_ISSUE_BREAK("interactive section argument must be either type name or an agent reference collection variable", desc.argument.Pos())
		}

		// 'mod' section can only be specific (for now)

		if (desc.type & ktMod)
			OC_ISSUE_BREAK("'mod' section argument must be an agent reference collection variable", desc.argument.Pos())

		// compare section argument with type names for generic sections

		for (Ochre::iterator t = ochre->begin(); t != ochre->end(); ++t)
			if (t->Name() == desc.argument) {

				desc.other = &(*t);
				return;
			}

		// if not resolved by now, ignore the whole section

		desc.ignored = true;
	}
}

// Set the routine pointer needed during generate.
// The actual routine is owned by pool.
void OcSection::SetRoutine(OcRoutine *r) {
	routine = r;
}

// Clears the routine and generates its code.
// Also reallocates literal, local and result chunks.
void OcSection::GenerateRoutine(Ochre *ochre) {

	routine->Clear();
	AppendOffsets(routine->locScheme);
	Generate(&ochre->env, routine);
	routine->ConnectInstructions();

	// output

	ochre->out << std::endl <<
	"routine chunk sizes " << std::endl <<
	"\tres: " << routine->resScheme.Size() << std::endl <<
	"\tloc: " << routine->locScheme.Size() << std::endl <<
	"\tlit: " << routine->litScheme.Size() << std::endl <<
	"\tjmp: " << routine->jmpScheme.Size() << std::endl <<
	std::endl;

	for (OcRoutine::iterator ins = routine->begin(); ins != routine->end(); ++ins)	// todo: encapsulate in routine!
		ins->ToOutput(ochre->out, routine->jmp);
}

std::string OcSection::ToString() const {

	std::stringstream ss;
	ss << "section ";

	if (desc.type & ktSet)			ss << "set";
	else if (desc.type & ktSee)		ss << "see";
	else if (desc.type & ktMod)		ss << "mod";
	else if (desc.type & ktAct)		ss << "act";
	else if (desc.type & ktDo)		ss << "do";
	if (desc.other)					ss << " " << desc.other->Name();
	if (desc.ignored)				ss << " (ignored)";

	ss << std::endl << OcBlock::ToString("\t");

	return ss.str();
}
