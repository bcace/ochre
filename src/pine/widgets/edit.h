#ifndef edit_h
#define edit_h

#include "pine/control.h"
#include "pine/widgets/hilight.h"
#include "pine/widgets/undo.h"
#include "pine/widgets/scroll.h"
#include "pine/widgets/menu.h"
#include <set>

#define TOP_MARGIN	0.0



namespace pine {

	struct Gutter
	{
		double ln, br, tx;	// ln: line no. right alignment, br: gutter width, tx: edit text left alignment
	};

	struct Loc
	{
		int x, y;	// char coordinates

		Loc() : x(0), y(0) {}
	};



	typedef unsigned char echar;

	class Edit : public Control
	{
	public:
		enum CiEditActions { CutActn = 0, CopyActn, PasteActn, UndoActn, RedoActn };

		static unsigned int tabW;
		static unsigned int TabMod(unsigned int);

		Edit(Widget *, const estring &, bool);
		Edit(Widget *, const Tag &);
		~Edit();

		void Initialize();

		const estring &Text() const;
		void SetEditable(bool);
		bool Unsaved() const;
		void LoadFromFile(const estring &);
		void SaveToFile(const estring &);
		bool BuildEventPrologue();
		void FocusLocation(unsigned int);
		unsigned CaretPos() const;

		void Insert(const estring &s, bool r = true);

		// editops
		void Type(echar);
		void Delete(bool);
		void Back(bool);
		void Cut();
		void Copy();
		void Paste();
		void Tab(bool);
		void Return(bool);
		void Block();
		void Undo();
		void Redo();
		void ArrowUp(bool, bool);
		void ArrowDown(bool, bool);
		void ArrowLeft(bool, bool);
		void ArrowRight(bool, bool);
		void PgUp(bool);
		void PgDown(bool);
		void Home(bool, bool);
		void End(bool, bool);

		void Resize(const Rect &);
		void Paint();

		void KeyPressEvent(KeyEvent &);
		void CharPressEvent(CharEvent &);

		void MousePressEvent(MouseEvent &);
		void MouseMoveEvent(MouseEvent &);
		void MouseScrollEvent(MouseEvent &);
		void MouseHoverEvent(MouseEvent &);

	protected:
		bool multiline, editable, lineNumbers, fileBased;
		double fontW, fontH, spacing, lineH;
		int lines, columns, travel;
		unsigned int beg, end, rev, sav, par;
		Gutter gutter;
		CiVScroll vScroll;
		CiHScroll hScroll;
		Menu menu;
		estring text, clearText;
		CiHilight hilight;
		UndoManager undo;

		// editprivate
		unsigned int SpacesToBol(unsigned int);
		unsigned int SpacesToEol(unsigned int);
		bool MoveToEol(unsigned int &);
		bool MoveToBol(unsigned int &);
		void MoveRight(unsigned int &, unsigned int);
		void MoveToEow(unsigned int &);
		void MoveToBow(unsigned int &);
		int CharType(unsigned char);
		unsigned int PgSize();
		unsigned int IndentLevel(unsigned int);
		int TabsOnlyToBol(int);
		unsigned int WhitespacesAtBol(unsigned int);

		void UpdateTextStats();
		void AutoScroll();
		void ScrollUp();
		void ScrollDown();
		Loc Location(unsigned int);
		Point Position(unsigned int);
		unsigned int FromPosition(const Point &);
		int ConservativeFromPosition(const Point &);
	};

	typedef std::vector<Edit *> CiEdits;
}

#endif
