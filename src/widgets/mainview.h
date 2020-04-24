#ifndef mainview_h
#define mainview_h

#include "pine/control.h"



namespace pine {

	struct CiSelection
	{
		int p, a;

		CiSelection();
		CiSelection(int, int);
	};

	struct CiAgent
	{
		int id;
		double p[3], c[3], size;

		CiAgent(int);
		CiAgent(int, double, double, double);
		CiAgent(int, double, double, double, double);
		CiAgent(int, double, double, double, double, double, double);
		CiAgent(int, double, double, double, double, double, double, double);

		void Paint();
		void PaintForSelection();
	};

	class CiPool : public std::vector<CiAgent>
	{
	public:
		int id;
		std::string name;
		int so, co;
		int sso, sco;
		double size;
		Vertex color;

		CiPool(int);
	};

	typedef std::vector<CiPool> CiPools;



	class Mainview : public Control
	{
	public:
		Mainview(Widget *);

		void Update(UpdateEvent &);
		void Paint();

		void MousePressEvent(MouseEvent &);
		void MouseReleaseEvent(MouseEvent &);
		void MouseMoveEvent(MouseEvent &);
		void MouseScrollEvent(MouseEvent &);
		void MouseHoverEvent(MouseEvent &);

	private:
		double rh, rv, ph, pv, zoom;
		int cells[6];
		double cellSize, mouseRay[6];
		Bounds bounds;
		CiPools pools;
		Point mousePos;
		int viewport[4];
		double modelview[16];
		double projection[16];

		void Setup();
		void Dismantle();
		CiSelection PickTest(const Point &);
		std::string ComposeAgentTooltipText(const CiSelection &);
	};
}

#endif
