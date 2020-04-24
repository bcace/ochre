#include "ochre.h"
#include <iostream>



// Updates agent & type variable offsets so they
// can be used in routines generation.
void OcType::UpdateOffsets(Ochre *ochre) {

	svars.UpdateOffsets(0);
	vars.UpdateOffsets(0);

	// output offsets

	ochre->out << std::endl <<
	"var chunk sizes" << std::endl <<
	"\ttype: " << svars.memScheme.Size() << std::endl <<
	"\tagent: " << vars.memScheme.Size() << std::endl <<
	std::endl;
}

// Generates type routines.
void OcType::Generate(Ochre *ochre) {

	for (OcSections::iterator sect = sections.begin(); sect != sections.end(); ++sect)
		sect->GenerateRoutine(ochre);
}
