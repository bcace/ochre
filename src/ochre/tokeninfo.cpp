#include "token.h"



const OcToken *OcToken::GetTokenAt(int pos) const {

	if (ContainsPos(pos))
		return this;

	for (OcTokens::const_iterator a = args.begin(); a != args.end(); ++a) {
		const OcToken *t = a->GetTokenAt(pos);
		if (t)
			return t;
	}

	return 0;
}



bool OcToken::GetInfo(std::string &infoText) const {

	const OcToken *info = Info();

	if (info->var) {
		infoText = info->var->Description();
		return true;
	}

	if (info->func) {
		infoText = info->func->Description();
		if (info->func->description != "")
			infoText += "\n\n" + info->func->description;
		return true;
	}

	if (info->op) {
		infoText = info->op->Description();
		if (info->op->description != "")
			infoText += "\n\n" + info->op->description;
		return true;
	}

	if (info->cl) {
		infoText = info->cl->Name();
		return true;
	}

	return false;
}