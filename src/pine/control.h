#ifndef control_h
#define control_h

#include "widget.h"



namespace pine {

	class Control : public Widget
	{
	public:
		Control(Widget *);
		Control(Widget *, int, const std::string &);
		Control(Widget *, int, const std::string &, const std::string &);
		~Control();

		void Connect(Action *, int);
		void Emit(const ActionEvent &);
	};
}

#endif
