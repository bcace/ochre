#ifndef hilight_h
#define hilight_h

#include "pine/color.h"



namespace pine {

	typedef std::string estring;

	class CiHilight
	{
	public:
		SchemeColor color;

		CiHilight(estring &);

		void Reset();
		bool Update(estring::iterator &);

	private:
		estring &text;
		estring::iterator end;

		estring::iterator Lookahead(estring::iterator);
		bool Matches(estring::iterator, estring::iterator, unsigned int, const estring &);
	};
}

#endif
