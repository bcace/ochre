#include "block.h"
#include "type.h"
#include "context.h"
#include <sstream>
#include <limits>



OcBlock::OcBlock() : vars(0) {}

const OcVars &OcBlock::Vars() const {
	return vars;
}

void OcBlock::ParseExpressions(OcType *type, OcVarses varses, KeyType section, OcType *other, OcVar *refs) {

	varses << &vars;

	for (iterator stat = begin(); stat != end(); ++stat) {
		stat->ParseExpression(type, varses, section, other, refs);

		if (stat->Block())
			stat->Block()->ParseExpressions(type, varses, section, other, refs);
	}
}

void OcBlock::ParseTemplates(Ochre *ochre, OcType *type) {

	for (iterator stat = begin(); stat != end(); ++stat) {
		stat->ParseTemplates(ochre, type);

		if (stat->Block())
			stat->Block()->ParseTemplates(ochre, type);
	}
}

// Parses declaration statements.
// Receives variables declared in scope as a
// list of lists of vars.
void OcBlock::ParseDeclarations(OcType *type, OcVarses varses, bool declSectionRoot) {

	if (!declSectionRoot)
		varses << &vars;

	for (iterator stat = begin(); stat != end();) {
		DeclParseRes declRes = stat->ParseDeclaration(type, varses, declSectionRoot);

		if (declRes.isDecl) {
			stat = erase(stat);
			splice(stat, declRes.exps, declRes.exps.begin(), declRes.exps.end());
		}
		else
			++stat;
	}

	for (iterator stat = begin(); stat != end(); ++stat)
		if (stat->Block())
			stat->Block()->ParseDeclarations(type, varses, false);
}

OcBlock::iterator OcBlock::BlockOff(OcType *type, int flags) {

	int blockStartPos = 0;

	if (!(flags & (bofRoot | bofSingle))) {	// remove '{'
		blockStartPos = front().front().Pos();
		pop_front();
	}

	iterator stat = begin();

	for (; stat != ((flags & bofSingle) ? ++iterator(begin()) : end()); ++stat) {

		if (stat->RequiresBlock()) {

			iterator peek = stat;
			if (++peek == end())
				type->AddIssue("Expecting statement block", itError, stat->back().EndPos());
			else {

				OcBlock *b = stat->AddBlock();
				b->splice(b->end(), *this, peek, end());
				iterator ret = b->BlockOff(type, peek->IsBlockStart() ? bofMulti : bofSingle);
				splice(end(), *b, ret, b->end());
			}
		}
		else if (stat->IsBlockEnd()) {
			if (flags & (bofRoot | bofSingle)) {
				type->AddIssue("'}' without a previous '{'", itError, stat->front().Pos());
				stat = erase(stat);
			}
			else
				return erase(stat);	// remove '}'
		}
	}

	if (!(flags & (bofRoot | bofSingle)))
		type->AddIssue("This block is not closed", itError, blockStartPos);

	return stat;
}

// Removes absent 'requires' blocks and
// flattens present ones.
void OcBlock::RemoveAbsentRequires(OcType *type) {

	for (iterator s = begin(); s != end();)
		if (s->IsRequires()) {
			if (s->CheckRequires(type) && s->Block())
				splice(++iterator(s),
					*s->Block(),
					s->Block()->begin(),
					s->Block()->end());
			s = erase(s);
		}
		else {
			if (s->Block())
				s->Block()->RemoveAbsentRequires(type);
			++s;
		}
}

void OcBlock::CheckIfElseStatements(OcType *type) {

	OpType prev = otNoOp;
	for (iterator s = begin(); s != end(); ++s) {

		OpType curr = s->IsIfElse();
		if ((curr == otElseIf || curr == otElse) &&
			(prev != otIf && prev != otElseIf))
			type->AddIssue("Expecting 'if' before 'else'", itError, s->front().Pos());

		prev = curr;

		if (s->Block())
			s->Block()->CheckIfElseStatements(type);
	}
}

void OcBlock::AppendOffsets(MemScheme &s) {

	vars.UpdateOffsets(&s);

	for (iterator stat = begin(); stat != end(); ++stat)
		if (stat->Block())
			stat->Block()->AppendOffsets(s);
}

std::string OcBlock::ToString(const std::string &pre) const {

	std::stringstream ss;
	ss << vars.ToString();

	for (const_iterator s = begin(); s != end(); ++s)
		ss << s->ToString(pre);

	return ss.str();
}

//
// debug
//

const OcToken *OcBlock::GetTokenAt(int pos) const {

	for (const_iterator s = begin(); s != end(); ++s) {

		const OcToken *t = s->GetTokenAt(pos);
		if (t)
			return t;

		if (s->Block()) {
			t = s->Block()->GetTokenAt(pos);
			if (t)
				return t;
		}
	}

	return 0;
}