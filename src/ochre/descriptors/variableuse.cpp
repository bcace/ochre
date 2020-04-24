#include "descriptors/variable.h"



// Returns the variable usage in a given context.
// Encodes the context and variable data in the result.
UsageType OcVar::Usage(KeyType context, bool own, UsageType caleeUse) const {

	int usage = (own ? 0 : ptReferenced);

	switch (context) {
		case ktNoKey:
			switch (owner) {
				case owtLocal:		usage |= ptAll; break;
				case owtStatic:		usage |= ptRead; break;
				case owtAgent:		usage |= ptAll; break;
				case owtObject:		usage = caleeUse; break;
				default:			usage |= ptNone; break;
			}
			break;
		case ktSet:
			switch (owner) {
				case owtLocal:		usage |= ptAll; break;
				case owtStatic:		usage |= ptAll; break;
				case owtAgent:		usage |= (own ? ptNone : ptAll); break;
				case owtObject:		usage = caleeUse; break;
				default:			usage |= ptNone; break;
			}
			break;
		case ktSee:
			switch (owner) {
				case owtLocal:		usage |= ptAll; break;
				case owtStatic:		usage |= ptRead; break;
				case owtAgent:
					switch (mod) {
						case vmPrivate: usage |= (own ? ptAppend : ptNone); break;
						case vmPublic:	usage |= ptRead; break;
						case vmConst:	usage |= ptRead; break;
						default:		usage |= ptNone; break;
					}
					break;
				case owtObject:		usage = caleeUse; break;
				default:			usage |= ptNone; break;
			}
			break;
		case ktMod:
			switch (owner) {
				case owtLocal:		usage |= ptAll; break;
				case owtStatic:		usage |= ptRead; break;
				case owtAgent:
					switch (mod) {
						case vmPrivate: usage |= (own ? ptRead : ptNone); break;
						case vmPublic:	usage |= ptAppend; break;
						case vmConst:	usage |= ptRead; break;
						default:		usage |= ptNone; break;
					}
					break;
				case owtObject:		usage = caleeUse; break;
				default:			usage |= ptNone; break;
			}
			break;
		case ktAct:
			switch (owner) {
				case owtLocal:		usage |= ptAll; break;
				case owtStatic:		usage |= ptRead; break;
				case owtAgent:		usage |= (own ? ptAll : ptNone); break;
				case owtObject:		usage = caleeUse; break;
				default:			usage |= ptNone; break;
			}
			break;
		case ktDo:
			switch (owner) {
				case owtLocal:		usage |= ptAll; break;
				case owtStatic:		usage |= ptAll; break;
				case owtAgent:		usage |= (own ? ptNone : ptRead); break;
				case owtObject:		usage = caleeUse; break;
				default:			usage |= ptNone; break;
			}
			break;
		default: usage |= ptNone; break;
	}

	return (UsageType)usage;
}
