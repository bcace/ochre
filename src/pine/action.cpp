#include "pine/action.h"



namespace pine {

	// actions

	Actions::~Actions() {
		for (std::map<std::string, Action *>::iterator i = actions.begin(); i != actions.end(); ++i)
			delete i->second;
	}

	Action *Actions::operator[](const std::string &actionName) const {

		std::map<std::string, Action *>::const_iterator i = actions.find(actionName);
		if (i != actions.end())
			return i->second;

		return 0;
	}

	void Actions::insert(const std::string &name, Action *action) {
		actions.insert(std::pair<std::string, Action *>(name, action));
	}

	bool Actions::exec_key(int k, bool ctrl, bool shift) {

		for (std::map<std::string, Action *>::iterator i = actions.begin(); i != actions.end(); ++i)
			if (i->second->shortcut.type == KeyShortcut::Key && i->second->shortcut.key == k && i->second->shortcut.ctrl == ctrl && i->second->shortcut.shift == shift) {
				i->second->Exec();
				return true;
			}

		return false;
	}

	bool Actions::exec_char(unsigned char c, bool ctrl, bool shift) {

		for (std::map<std::string, Action *>::iterator i = actions.begin(); i != actions.end(); ++i)
			if (i->second->shortcut.type == KeyShortcut::Char && i->second->shortcut.key == c && i->second->shortcut.ctrl == ctrl && i->second->shortcut.shift == shift) {
				i->second->Exec();
				return true;
			}

		return false;
	}

	// subscribed actions

	SubscribedActions::SubscribedActions(Face **_face) :
		face(_face) {}

	void SubscribedActions::insert(int event, Action *action) {

		std::map<int, std::vector<Action *> >::iterator i = actions.find(event);
		if (i != actions.end())
			i->second.push_back(action);
		else {
			actions.insert(std::pair<int, std::vector<Action *> >(event, std::vector<Action *>(1, action)));
			if (event == PI_FACE_EVENT)
				*face = action;
		}
	}

	SubscribedActions::iterator SubscribedActions::begin(int event) {

		std::map<int, std::vector<Action *> >::iterator i = actions.find(event);
		if (i != actions.end())
			selected = &i->second;
		else
			selected = &dummy;

		return selected->begin();
	}

	SubscribedActions::iterator SubscribedActions::end() {
		return selected->end();
	}
}
