#include "raceservergui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RaceServerGUI w;
	w.show();
	return a.exec();
}
