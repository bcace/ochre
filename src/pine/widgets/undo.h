#ifndef undo_h
#define undo_h

#include <string>
#include <list>



namespace pine {

	enum OpConcat {
		occNone = 0,
		occAlpha = 1,
		occNumeric = 2,
		occWhitespace = 3,
		occSymbol = 4
	};

	struct UndoCur
	{
		unsigned int beg, end;

		UndoCur() :
			beg(0), end(0) {}

		UndoCur(unsigned int _beg, unsigned int _end) :
			beg(_beg), end(_end) {}
	};

	struct UndoOp
	{
		UndoCur c1, c2, c3, c4;
		std::string s1, s2;
		OpConcat concat;

		UndoOp(unsigned int beg, unsigned int end, OpConcat _concat) :
			c1(beg, end), concat(_concat) {}
	};



	class UndoManager : std::list<UndoOp>
	{
	public:
		UndoManager();

		UndoOp &Back();

		void Add(unsigned int b, unsigned int e, unsigned char c = 0);
		void Pop();
		UndoOp *Undo();
		UndoOp *Redo();

	private:
		UndoManager::iterator cur;
	};
}

#endif
