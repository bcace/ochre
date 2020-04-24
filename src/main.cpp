#include "widgets/mainwidget.h"
#include "pine/platform.h"

using namespace pine;



int main(int argc, char **argv) {

	std::string path = argv[0];
	path.erase(path.size() - 9);

	if (!CreateWindow("Ochre", 900.0, 600.0))
		return 1;

	Mainwidget mainwidget(path);
	mainwidget.Exec();

	DestroyWindow();

	return 0;
}
