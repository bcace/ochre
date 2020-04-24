#include "widgets/mainview.h"
#include "pine/color.h"
#include "ochreapi.h"

using namespace pine;



void Mainview::Update(UpdateEvent &) {

	pools.clear();

	cells[0] = cells[1] = cells[2] = cells[3] = cells[4] = cells[5] = 0;
	cellSize = OcGetGrid(cells);

	double _bounds[6];
	OcGetBounds(_bounds);
	bounds = Bounds(_bounds);

	int id = OcSelectFirstPool();
	while (id != -1) {

		pools.push_back(CiPool(id));

		const char *poolName = OcGetPoolName();
		pools.back().name = poolName ? poolName : "name_not_available";

		int po = OcGetVarOffset("P", "point", 0);
		if (po != -1) {

			int so = OcGetVarOffset("Size", "real", 0);
			int sso = OcGetVarOffset("Size", "real", 1);

			int co = OcGetVarOffset("Color", "point", 0);
			int sco = OcGetVarOffset("Color", "point", 1);

			double ss = (sso != -1) ? *(double *)OcGetStaticData(sso) : 1.0;

			Color &color = Schema::GetColor(scoFrgr);
			Vertex sc = (sco != -1) ? *(Vertex *)OcGetStaticData(sco) : Vertex(color.RedD(), color.GreenD(), color.BlueD());

			pools.back().so = so;
			pools.back().sso = sso;
			pools.back().co = co;
			pools.back().sco = sco;
			pools.back().size = ss;
			pools.back().color = sc;

			int i = OcSelectFirst();

			if (so == -1) {

				if (co == -1) {

					while (i) {
						Vertex *p = (Vertex *)OcGetData(po);
						pools.back().push_back(CiAgent(i, p->x, p->y, p->z, sc.x, sc.y, sc.z, ss));
						i = OcSelectNext();
					}
				}
				else {

					while (i) {
						Vertex *p = (Vertex *)OcGetData(po);
						Vertex *c = (Vertex *)OcGetData(co);
						pools.back().push_back(CiAgent(i, p->x, p->y, p->z, c->x, c->y, c->z, ss));
						i = OcSelectNext();
					}
				}
			}
			else {

				if (co == -1) {

					while (i) {
						double *s = (double *)OcGetData(so);
						Vertex *p = (Vertex *)OcGetData(po);
						pools.back().push_back(CiAgent(i, p->x, p->y, p->z, sc.x, sc.y, sc.z, *s));
						i = OcSelectNext();
					}
				}
				else {

					while (i) {
						double *s = (double *)OcGetData(so);
						Vertex *p = (Vertex *)OcGetData(po);
						Vertex *c = (Vertex *)OcGetData(co);
						pools.back().push_back(CiAgent(i, p->x, p->y, p->z, c->x, c->y, c->z, *s));
						i = OcSelectNext();
					}
				}
			}
		}

		id = OcSelectNextPool();
	}

//	std::cerr << mouseRay[0] << " " << mouseRay[1] << " " << mouseRay[2] << " - " << mouseRay[3] << " " << mouseRay[4] << " " << mouseRay[5] << std::endl;

	OcSetCursorRay(mouseRay);

	Widget::paint = true;
}
