#include "raceclientgui.h"
#include "setupwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SetupWindow s;
	if (s.exec() == QDialog::Accepted){
      //  cout << "ok!";
      //  cout << "Address:" << s.getIP().toStdString();
      //  cout << "Port:" << s.getPort().toStdString();
		RaceClientGUI w(s.getIP(),s.getPort());
		if (w.hasValidSock()){
			w.show();

			return a.exec();
		}
		else return -1;
    }
    else{
      //  cout << "nope!";
        return 0;
    }
}
