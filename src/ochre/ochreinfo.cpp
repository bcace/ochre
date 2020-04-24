#include "ochre.h"



const char *Ochre::PositionalInfo(int typeId, int pos) {

	infoString = "";

	OcType *type = Type(typeId);
	if (type == 0)
		return 0;

	const OcToken *token = type->GetTokenAt(pos);
	if (token == 0)
		return 0;

	return token->GetInfo(infoString) ? infoString.data() : 0;
}
