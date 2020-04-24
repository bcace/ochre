#ifndef action_h
#define action_h

#include <string>
#include <vector>
#include <map>

#define PI_FACE_EVENT	0



namespace pine {

	struct KeyShortcut
	{
		enum Type { Invalid = 0, Key, Char };

		unsigned int key;
		bool ctrl, shift;
		Type type;

		KeyShortcut() :
			key(13021980), type(Invalid), ctrl(false), shift(false) {}

		explicit KeyShortcut(int k, bool _ctrl = false, bool _shift = false) :
			key(k), type(Key), ctrl(_ctrl), shift(_shift) {}

		explicit KeyShortcut(char c, bool _ctrl = false, bool _shift = false) :
			key(c), type(Char), ctrl(_ctrl), shift(_shift) {

			int h = 0;
		}

		operator bool() const {
			return type != Invalid;
		}
	};



	class Face
	{
	public:
		std::string text, desc;
		int icon;
		bool enabled;

		Face(const std::string &_text, const std::string &_desc, int _icon) :
			text(_text), desc(_desc), icon(_icon), enabled(true) {}
	};



	class Widget;

	class Action : public Face
	{
	public:
		Widget *widget;
		std::string name;
		KeyShortcut shortcut;

		Action(Widget *_widget, const std::string &_name, const std::string &_desc, int _icon, const KeyShortcut &_shortcut) :
			Face(_name, _desc, _icon),
			widget(_widget), name(_name), shortcut(_shortcut) {}

		virtual void Exec() {}
	};



	template<class T>
	class _Action : public Action
	{
	public:
		void (T::*exec)();

		_Action(Widget *_widget, void (T::*_exec)(), const std::string &_name, const std::string &_desc, int _icon, const KeyShortcut &_shortcut) :
			Action(_widget, _name, _desc, _icon, _shortcut),
			exec(_exec) {}

		void Exec() {
			if (enabled) ((T *)widget->*exec)();
		}
	};



	class Actions
	{
	public:
		~Actions();

		Action *operator[](const std::string &) const;

		void insert(const std::string &, Action *);
		bool exec_key(int, bool, bool);
		bool exec_char(unsigned char, bool, bool);

	private:
		std::map<std::string, Action *> actions;
	};



	class SubscribedActions
	{
	public:
		SubscribedActions(Face **);

		void insert(int, Action *);

		class iterator {
		public:
			iterator(const std::vector<Action *>::iterator &_i) : i(_i) {}

			bool operator!=(const iterator &o) const {
				return i != o.i;
			}

			iterator &operator++() {
				++i;
				return *this;
			}

			Action *operator->() {
				return *i;
			}

		private:
			std::vector<Action *>::iterator i;
		};

		iterator begin(int);
		iterator end();

	private:
		Face **face;
		std::map<int, std::vector<Action *> > actions;
		std::vector<Action *> dummy, *selected;
	};
}

#endif
