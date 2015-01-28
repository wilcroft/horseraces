#include "setupwindow.h"

SetupWindow::SetupWindow(QWidget *parent) :
    QDialog(parent)
{

    this->resize(300,200);
    this->setWindowTitle("Enter Server Properties...");

    //Make submit button
    okbut = new QPushButton("OK",this);
    okbut->move(120,160);

    //connect button to accept signal
    connect(okbut, SIGNAL(clicked()), this, SLOT(accept()));

    //Add IP field
    QFont font;
    iplabel = new QLabel("Address",this);
    iplabel->move(20,22);
    iplabel->setAlignment(Qt::AlignRight);
    font = iplabel->font();
    font.setPointSize(font.pointSize()+3);
    iplabel->setFont(font);

    ipline = new QLineEdit("127.0.0.1",this);
    ipline->move(80,20);
    font = ipline->font();
    font.setPointSize(font.pointSize()+3);
    ipline->setFont(font);

    connect (ipline,SIGNAL(textEdited(QString)),this,SLOT(updateIP()));

    //Add port field
    portlabel = new QLabel("     Port",this);
    portlabel ->setAlignment(Qt::AlignRight);
    portlabel->move(20,102);
    font = portlabel->font();
    font.setPointSize(font.pointSize()+3);
    portlabel->setFont(font);

    portline = new QLineEdit("23456",this);
    portline->move(80,100);
    font = portline->font();
    font.setPointSize(font.pointSize()+3);
    portline->setFont(font);

    port = portline->text();
    ip = ipline->text();

    connect (portline,SIGNAL(textEdited(QString)),this,SLOT(updatePort()));
}

SetupWindow::~SetupWindow()
{
    delete okbut;
    delete ipline;
    delete iplabel;
    delete portline;
    delete portlabel;
}

QString SetupWindow::getPort(){    return port;   }
QString SetupWindow::getIP(){    return ip;   }
void SetupWindow::updateIP(){ ip = ipline->text(); }
void SetupWindow::updatePort(){ port = portline->text(); }
