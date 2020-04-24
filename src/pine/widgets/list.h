#ifndef list_h
#define list_h

#include "pine/widgets/scroll.h"

#define LIST_ITEM_PADDING	3.0



namespace pine {

	enum ListItemState { litNormal = 0, litHover, litPressed };

	struct ListItem
	{
		int icon;
		std::string text;
		Tag tag;
		int group;
		ListItemState state;

		ListItem(int _icon, const std::string &_text, const Tag &_tag, bool _selected = false, int _group = 1) :
			icon(_icon), text(_text), tag(_tag), group(_group), state(_selected ? litPressed : litNormal) {}

		bool operator>(const ListItem &) const;
		bool TextIsGreaterThan(const std::string &) const;
	};

	typedef std::vector<ListItem> ListItems;



	class List : public Control
	{
	public:
		enum ListActions { Choose = 0 };

		enum ItemStyle {
			Text = 0x1,
			Icon = 0x2,
			IconAndText = 0x3,
			Group = 0x10,
			Alphabetize = 0x20
		};

		List(Widget *, bool, ItemStyle);
		List(Widget *, bool, bool, ItemStyle);

		void Clear();
		void Add(const ListItem &);
		void RemoveSelected();
		bool SelectNext();
		bool SelectPrevious();
		Tags SelectedItems(int group = 0xFFFF) const;
		int SelectedIndex() const;
		void SelectByIndex(int);
		unsigned int Count() const;
		double ItemHeight() const;

		void Resize(const Rect &);
		void Paint();

		void KeyPressEvent(KeyEvent &);

		void MousePressEvent(MouseEvent &);
		void MouseMoveEvent(MouseEvent &);
		void MouseReleaseEvent(MouseEvent &);
		void MouseScrollEvent(MouseEvent &);

	private:
		bool multiselect, permaselect, chooseOnClick;
		ItemStyle style;
		ListItems items;
		CiVScroll scroll;
		double itemHeight;

		void AutoScroll();
		unsigned int ItemFromPos(const Point &);
		bool UpdateHover(const Point &);
		bool ClearHover();
		bool ClearPressed();
	};
}

#endif
