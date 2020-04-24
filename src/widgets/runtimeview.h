#ifndef runtimeview_h
#define runtimeview_h

#include "pine/widgets/container.h"
#include "widgets/mainview.h"



namespace pine {

	class RuntimeView : public CiContainer
	{
	public:
		RuntimeView(Widget *);

		void Resize(const Rect &);

	private:
		Mainview mainview;
	};
}

#endif
