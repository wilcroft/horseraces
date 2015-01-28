#include "racedisplay.h"
#include "setupwindow.h"
#include <QtWidgets/QApplication>
#include <iostream>

using std::cout;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SetupWindow s;
    if (s.exec() == QDialog::Accepted){
        cout << "ok!";
        cout << "Address:" << s.getIP().toStdString();
        cout << "Port:" << s.getPort().toStdString();
        RaceDisplay w;//(s.getIP(),s.getPort());
        w.setPort(s.getPort());
        w.setIP(s.getIP());
        w.show();

        return a.exec();
    }
    else{
        cout << "nope!";
        return 0;
    }
}