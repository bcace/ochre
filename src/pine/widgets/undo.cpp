#include "pine/widgets/undo.h"

using namespace pine;



UndoManager::UndoManager() : cur(end()) {}

UndoOp &UndoManager::Back() {
	return back();
}

void UndoManager::Add(unsigned int b, unsigned int e, unsigned char c) {

	erase(cur, end());


	OpConcat concat = occNone;

	// todo: figure the concat type out

	push_back(UndoOp(b, e, concat));

	if (size() > 1000)
		pop_front();

	cur = end();
}

void UndoManager::Pop() {
	pop_back();
	cur = end();
}

UndoOp *UndoManager::Undo() {
	UndoOp *op = 0;

	if (cur != begin()) {
		--cur;
		op = &(*cur);
	}

	return op;
}

UndoOp *UndoManager::Redo() {
	UndoOp *op = 0;

	if (cur != end()) {
		op = &(*cur);
		++cur;
	}

	return op;
}
