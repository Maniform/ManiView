#include "MainWindow.h"
#include "ViewerApplication.h"

int main(int argc, char* argv[])
{
	ViewerApplication a(argc, argv);
	MainWindow w;
	a.setMainWindow(&w);
	w.show();
	return a.exec();
}
