#include "pine/widgets/edit.h"
#include "pine/platform.h"

#define MUST_BE_EDITABLE	if (!editable) return;
#define MUST_BE_MULTILINE	if (!multiline) return;

using namespace pine;



void Edit::Insert(const estring &s, bool r) {

	if (beg > end)
		std::swap<unsigned int>(beg, end);

	if (r) {
		undo.Back().c2 = UndoCur(beg, end);
		undo.Back().s1 = text.substr(beg, end - beg);
	}

	text.replace(beg, end - beg, s);
	end = beg += s.size();

	if (r) {
		undo.Back().c3 = UndoCur(beg - s.size(), end);
		undo.Back().c4 = UndoCur(beg, end);
		undo.Back().s2 = s;
	}

	if (r)
		++rev;

	UpdateTextStats();
	AutoScroll();
}

void Edit::Undo() {
	UndoOp *op = undo.Undo();
	if (op) {

		beg = op->c3.beg;
		end = op->c3.end;

		Insert(op->s1, false);

		beg = op->c1.beg;
		end = op->c1.end;

		--rev;
	}
}

void Edit::Redo() {
	UndoOp *op = undo.Redo();
	if (op) {

		beg = op->c2.beg;
		end = op->c2.end;

		Insert(op->s2, false);

		beg = op->c4.beg;
		end = op->c4.end;

		++rev;
	}
}

//
// derived
//

void Edit::Type(echar c) {
	MUST_BE_EDITABLE

	undo.Add(beg, end, c);

	Insert(estring(1, c));
}

void Edit::Delete(bool ctrl) {
	MUST_BE_EDITABLE

	undo.Add(beg, end);

	if (ctrl && beg == end)
		MoveToEow(end);

	if (beg == end) {
		if (beg == text.size()) {
			undo.Pop();
			return;
		}

		++end;
	}

	Insert(estring());
}

void Edit::Back(bool ctrl) {
	MUST_BE_EDITABLE

	undo.Add(beg, end);

	if (ctrl && beg == end && beg > 0)
		MoveToBow(end);

	if (beg == end) {
		if (beg == 0) {
			undo.Pop();
			return;
		}

		--beg;
	}

	Insert(estring());
}

void Edit::Cut() {
	MUST_BE_EDITABLE

	if (beg != end) {
		Copy();

		undo.Add(beg, end);
		Insert(estring());
	}
}

void Edit::Copy() {
	if (beg != end) {
		if (beg < end)
			SetClipboardString(text.substr(beg, end - beg).c_str());
		else
			SetClipboardString(text.substr(end, beg - end).c_str());
	}
}

void Edit::Paste() {
	MUST_BE_EDITABLE

	undo.Add(beg, end);
	Insert(GetClipboardString());
}

void Edit::Tab(bool shft) {
	MUST_BE_EDITABLE

	undo.Add(beg, end);
	Insert("\t");
}

void Edit::Return(bool ctrl) {
	MUST_BE_EDITABLE
	MUST_BE_MULTILINE

	undo.Add(beg, end);
	estring nl = "\n";

	if (beg > end)
		std::swap<unsigned int>(beg, end);

	unsigned int ind = IndentLevel(beg);

	for (unsigned int t = 0; t < ind; ++t)
		nl += "\t";

	Insert(nl);
}

void Edit::Block() {
	MUST_BE_EDITABLE

	undo.Add(beg, end);
	estring bl;

	if (beg == end) {

		int t = TabsOnlyToBol(end - 1);
		if (t != -1) {

			for (unsigned int i = 1; i < (unsigned int)t; ++i)
				bl += "\t";

			beg -= t;
		}
	}

	Insert(bl + "}");
}

//
// move
//

void Edit::ArrowUp(bool ctrl, bool shft) {
	MUST_BE_MULTILINE

	if (ctrl) {
		ScrollUp();
		return;
	}

	unsigned int s = SpacesToBol(end);
	if (MoveToBol(end)) {
		--end;
		if (SpacesToBol(end) > s) {
			MoveToBol(end);
			MoveRight(end, s);
		}
	}

	if (!shft)
		beg = end;

	AutoScroll();
}

void Edit::ArrowDown(bool ctrl, bool shft) {
	MUST_BE_MULTILINE

	if (ctrl) {
		ScrollDown();
		return;
	}

	unsigned int s = SpacesToBol(end);
	if (MoveToEol(end)) {
		++end;
		if (SpacesToEol(end) >= s)
			MoveRight(end, s);
		else
			MoveToEol(end);
	}

	if (!shft)
		beg = end;

	AutoScroll();
}

void Edit::ArrowLeft(bool ctrl, bool shft) {

	if (end > 0) {
		if (ctrl)
			MoveToBow(end);
		else
			--end;
	}

	if (!shft)
		beg = end;

	AutoScroll();
}

void Edit::ArrowRight(bool ctrl, bool shft) {

	if (end < text.size()) {
		if (ctrl)
			MoveToEow(end);
		else
			++end;
	}

	if (!shft)
		beg = end;

	AutoScroll();
}

void Edit::PgUp(bool shft) {
	MUST_BE_MULTILINE

	unsigned int c = PgSize() - 1, s = SpacesToBol(end);

	for (unsigned int i = 0; i < c; ++i) {
		if (MoveToBol(end))
			--end;
		else
			break;
	}

	if (MoveToBol(end)) {
		--end;
		if (SpacesToBol(end) > s) {
			MoveToBol(end);
			MoveRight(end, s);
		}
	}

	if (!shft)
		beg = end;

	AutoScroll();
}

void Edit::PgDown(bool shft) {
	MUST_BE_MULTILINE

	unsigned int c = PgSize() - 1, s = SpacesToBol(end);

	for (unsigned int i = 0; i < c; ++i) {
		if (MoveToEol(end))
			++end;
		else
			break;
	}

	if (MoveToEol(end)) {
		++end;
		if (SpacesToEol(end) >= s)
			MoveRight(end, s);
		else
			MoveToEol(end);
	}

	if (!shft)
		beg = end;

	AutoScroll();
}

void Edit::Home(bool ctrl, bool shft) {

	if (ctrl) {
		beg = end = 0;
		return;
	}

	MoveToBol(end);

	if (!shft)
		beg = end;

	AutoScroll();
}
void Edit::End(bool ctrl, bool shft) {

	if (ctrl) {
		beg = end = text.size();
		return;
	}

	MoveToEol(end);

	if (!shft)
		beg = end;

	AutoScroll();
}
