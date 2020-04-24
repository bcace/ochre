#ifndef menu_h
#define menu_h

#include "pine/popup.h"

#define MENU_ITEM_S	20.0



namespace pine {

	struct MenuItem
	{
		int id;
		std::string name;
		bool enabled, hovered, separator;

		MenuItem(int _id, const std::string &_name, bool _separator = false) :
			id(_id), name(_name), separator(_separator) {}
	};

	typedef std::vector<MenuItem> MenuItems;



	class Menu : public Popup
	{
	public:
		Menu(Widget *);

		int SelectedItemId() const;

		int Show(const Point &);
		void AddItem(const std::string &_n, int _i, bool _s = false);
		void SetItemEnabled(int, bool);

		void Paint();

		void MouseMoveEvent(MouseEvent &);
		void MouseReleaseEvent(MouseEvent &);

	private:
		MenuItems items;
		int selItem;
		double width, height;

		void UpdateSize();
	};
}

#endif
