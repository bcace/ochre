#include "ochre.h"



DeclParseRes OcStat::ParseDeclaration(OcType *type, const OcVarses &varses, bool declSectRoot) {

	DeclParseRes declRes(true);

	OcVars *vars = varses.back(), *svars = varses.front();
	OcStat::iterator token = begin(), peek;
	bool typeKey = false;

	// optional 'type' keyword (static vars)

	if (token->IsKey(ktType)) {

		typeKey = true;
		peek = token;

		if (++peek == end()) {
			type->AddIssue("Expecting declaration statement following the 'type' keyword", itError, token->EndPos());
			return declRes;
		}
		else if (!declSectRoot) {
			type->AddIssue("'type' keyword can only be used within declaration section", itError, token->EndPos());
			return declRes;
		}

		++token;
	}

	OcVar var;

	if (declSectRoot)
		var.owner = typeKey ? owtStatic : owtAgent;
	else
		var.owner = owtLocal;

	// type or class name

	if (token->Tt() == ttClass || token->Tt() == ttType)
		var.cl = token->Cl();								// datatype is class
	else if (typeKey) {
		type->AddIssue("Expecting type or class name", itError, token->Pos());
		return declRes;
	}
	else
		return DeclParseRes(false);							// not a declaration

	peek = token;
	if (++peek != end() && peek->IsOp(otReference))
		return DeclParseRes(false);							// not a declaration, static type/class member reference

	// variables

	if (token->Tt() == ttType && declSectRoot)				// prevent type/agent owned references
		type->AddIssue("Cannot declare non-local agent references", itError, token->Pos());
	else if (token->Tt() == ttClass && !declSectRoot && token->Cl()->RefsQ() == rqAgentRefs)
		type->AddIssue("Cannot declare non-local agent reference collection as local", itError, token->Pos());
	else if (token->Tt() == ttClass && declSectRoot && token->Cl()->RefsQ() == rqLocalRefs)
		type->AddIssue("Cannot declare local agent reference collection as non-local", itError, token->Pos());
	else if (token->Tt() == ttClass && declSectRoot && typeKey && token->Cl()->RefsQ() != rqNotRefs)
		type->AddIssue("Agent reference collections cannot belong to a type", itError, token->Pos());
	else if (++token == end())
		type->AddIssue("Missing variable name", itError, back().EndPos());
	else
		while (token != end()) {
			if (token->Tt() == ttUnknown) {

				// check name

				const OcVar *mVar = varses.Contains(*token);
				if (mVar) {
					type->AddIssue("Variable name '" + *token + "' already declared", itError, token->Pos());
					type->AddIssue("at this point", itError, mVar->name.Pos());
				}
				else {
					var.name = *token;
					var.mod = token->GetModifier();
					if (typeKey)
						svars->Add(var);
					else
						vars->Add(var);
				}

				// look for comma or end

				int braceStack = 0;
				OcStat::iterator fin = ++OcStat::iterator(token);
				while (fin != end() && ((braceStack > 0) || !fin->IsOp(otComma))) {
					braceStack += (int)fin->IsStruct(stOpenBrace) - (int)fin->IsStruct(stCloseBrace);
					++fin;
				}

				// if expression found

				if (std::distance(token, fin) > 1)
					declRes.exps.push_back(OcStat(*this, token, fin));

				token = fin;

				// skip comma

				if (token != end())
					++token;
			}
			else {
				type->AddIssue("Illegal variable name", itError, token->Pos());
				++token;
			}
		}

	return declRes;
}

void OcStat::ParseTemplates(Ochre *ochre, OcType *type) {

	for (OcStat::iterator token = begin(); token != end(); ++token) {

		if (token->Tt() == ttClass) {

			if (token->Cl()->Template()) {

				OcStat::iterator peek = token;
				if (++peek != end() && peek->IsOp(otIndex)) {

					peek = erase(peek);

					if (peek == end())
						OC_ISSUE("Missing template argument", back().EndPos())
					else {

						if (peek->Tt() != ttClass && peek->Tt() != ttType)
							OC_ISSUE("Illegal template argument", peek->Pos())
						else {

							OcClass *cl = ochre->Class(*token, peek->Cl());
							if (cl)
								token->SetTemplateInstance(cl);
							else
								OC_ISSUE("No matching template instance found for " + *token +  ":" + peek->Cl()->Name(), token->Pos())
						}

						erase(peek);
					}
				}
				else
					OC_ISSUE("template class " + *token +  " requires template argument", token->Pos())
			}
		}
	}
}
