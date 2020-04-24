#ifndef widget_h
#define widget_h

#include "geometry.h"
#include "exception.h"
#include "pine/action.h"
#include <string>
#include <vector>
#include <list>

#define MAIN_MENU_H			64.0
#define MAIN_MENU_W			54.0
#define MAIN_MENU_PADDING	16.0
#define TOOLBAR_H			24.0
#define SHADOW_W			4.0

#define RETURN_IF_INVISIBLE				if (!visible) return;
#define RETURN_IF_INVISIBLE_OR_ACCEPTED	if (!visible || !face->enabled || e.accepted) return;
#define RETURN_IF_ACCEPTED				if (e.accepted) return;
#define BREAK_IF_ACCEPTED				if (e.accepted) break;



namespace pine {

	struct Tag
	{
		int i, j;
		bool b;
		std::string s;

		Tag() : i(13021980), j(13021980), b(false), s("") {}
		Tag(int _i) : i(_i), j(13021980), b(false), s("") {}
		Tag(const std::string &_s) : i(13021980), j(13021980), b(false), s(_s) {}
		Tag(bool _b, const std::string &_s) : i(13021980), j(13021980), b(_b), s(_s) {}
		Tag(int _i, const std::string &_s) : i(_i), j(13021980), b(false), s(_s) {}
		Tag(int _i, const std::string &_s, int _j) : i(_i), j(_j), b(false), s(_s) {}
		Tag(int _i, int _j) : i(_i), j(_j), b(false), s("") {}
	};

	typedef std::vector<Tag> Tags;

	/*
	widgets own other widgets, widgets are destroyed exclusively by sending
	an appropriate notification to parent.
	*/

	class Event;
	class KeyEvent;
	class CharEvent;
	class MouseEvent;
	class UpdateEvent;
	class ActionEvent;
	class Window;
	class Control;

	class Widget
	{
	public:
		static int mouseButtons, modifiers, hoverTime;
		static bool paint;
		static Point windowSize, mousePos;
		static std::vector<Window *> windowStack;

		Widget * const parent;

		virtual ~Widget() {}

		const Rect &Frame() const { return frame; }
		const Tag &GetTag() const { return tag; }
		bool Visible() const { return visible; }
		virtual bool SetVisible(bool);
		const Point &Size() const;
		void SetSize(double, double);

		/*
		keyboard events
		*/

		virtual void KeyPressEvent(KeyEvent &);
		virtual void CharPressEvent(CharEvent &);

		/*
		mouse events
		*/

		virtual void MousePressEvent(MouseEvent &);
		virtual void MouseReleaseEvent(MouseEvent &);
		virtual void MouseMoveEvent(MouseEvent &);
		virtual void MouseScrollEvent(MouseEvent &);
		virtual void MouseHoverEvent(MouseEvent &);

		/*
		events caused by the event loop
		*/

		virtual void Update(UpdateEvent &);
		virtual void Resize(const Rect &);
		virtual void Paint();

		void EnableScissor();
		void DisableScissor();
		void ApplyViewport();
		void DrawRect(double, double, double, double);
		void FocusWidget();
		void UnfocusWidget();

		/*
		actions
		*/

		template<class T> void AddAction(const std::string &actionName, const std::string &actionDesc, int actionIcon, void (T::*actionExec)(), const KeyShortcut &actionShortcut = KeyShortcut()) {
			actions.insert(actionName, new _Action<T>(this, actionExec, actionName, actionDesc, actionIcon, actionShortcut));
		}

		void RegisterControl(Control *);
		void UnregisterControl(Control *);
		bool IsParent(Control *);

		/*
		representation
		*/

		std::string &Text();
		std::string &Desc();
		int &Icon();
		bool &Enabled();

	protected:
		bool visible;
		Face *face;	// text, description, icon, enabledness...
		Rect frame;
		Tag tag;
		Point size;	// suggested size for layout purposes
		Actions actions;
		SubscribedActions subscribedActions;

		Widget(Widget *);
		Widget(Widget *, int, const std::string &);
		Widget(Widget *, int, const std::string &, const std::string &);

	private:
		std::list<Control *> controls;
		Face defFace;

		Widget(const Widget &) :
			parent(0),
			subscribedActions(&face),
			defFace("", "", -1) {}
	};
}

#endif
