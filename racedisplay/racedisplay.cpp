#include "racedisplay.h"

RaceDisplay::RaceDisplay(QString addr, QString p, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RaceDisplay)
{
    ui->setupUi(this);
	port = p;
	ip = addr;
 //   int y = this->height()-40;
	//int x = this->width()-40;
	//int yspc = NUM_HORSES_PER_RACE+1;
    //std::cout << "y:" << y;
    
	//QFont hnumFont;
	//QFont hnameFont;
	//QFont hoddsFont;
	//hnumFont.setPointSize(30);
	//hnameFont.setItalic(true);
	//hnameFont.setPointSize(24);
	//hoddsFont.setBold(true);
	//hoddsFont.setPointSize(40);
    for (int i=0 ; i< NUM_HORSES_PER_RACE; i++){
		hBack[i] = new QLabel(this);
		if (i%2)
			hBack[i]->setStyleSheet("QLabel { background-color : #CCFFFF; }");
		else
			hBack[i]->setStyleSheet("QLabel { background-color : #FFFF99; }");

        QString title = "Horse " + QString::number(i+1) + ":";
        std::cout << title.toStdString();
        hNum[i] = new QLabel (title,this);
		//hNum[i]->setFont(hnumFont);
  //      hNum[i]->move(20,20+i*y/yspc);
		//hNum[i]->setMinimumWidth(x*3/4);
		//hNum[i]->setMinimumHeight(40);
		

		title = "Horse " + QString::number(i+1) + " Name!";
		hName[i] = new QLabel (title,this);
		//hName[i]->setFont(hnameFont);
		//hName[i]->move(40,20+qRound((i+0.5)*y/yspc));
		//hName[i]->setMinimumWidth(x*3/4);

		title = QString::number(i*i*i) +":1";
		hOdds[i] = new QLabel (title,this);
		//hOdds[i]->setFont(hoddsFont);
		//hOdds[i]->move(x*3/4,20+i*y/yspc);
		//hOdds[i]->setMinimumHeight(60);
		//hOdds[i]->setMinimumWidth(x/8);
		hOdds[i]->setAlignment(Qt::AlignRight);

    }
	for (int i=0;i<2;i++){
		mov[i] = new QMovie("runninghorse.gif");
		img[i] = new QLabel(this);
		img[i]->setMovie(mov[i]);
		mov[i]->start();
		img[i]->setAlignment(Qt::AlignCenter);
	}
	img[0]->move(10,10);
	QFont rfont;
	rfont.setPointSize(56);
	rfont.setBold(true);
	race = new QLabel("RACE X", this);
	race->setAlignment(Qt::AlignCenter);
	race->setFixedSize(320,100);
	race->setFont(rfont);
	race->setStyleSheet("QLabel { background-color : #66CC66;}");
	
	//winBack = new QLabel(this);
	//winBack->setStyleSheet("QLabel { background-color : yellow; border-width : 5px; border-style: solid; border-color : red}");
	//winNum = new QLabel("WINNER: Horse X", this);
	//winName = new QLabel("Also needs a name", this);
	//winOdds = new QLabel("1000:1", this);
	//winOdds->setAlignment(Qt::AlignRight);

	//hBack[0]->setStyleSheet("QLabel { background-color : red; }");
	//hBack[1]->setStyleSheet("QLabel { background-color : orange; }");
	//hBack[2]->setStyleSheet("QLabel { background-color : yellow; }");
	//hBack[3]->setStyleSheet("QLabel { background-color : green; }");
	//hBack[4]->setStyleSheet("QLabel { background-color : blue; }");
	//hBack[5]->setStyleSheet("QLabel { background-color : violet; }");
    //connect (u)

	//Set up socket connection

#if defined(_WIN32) || defined(_WIN64)
    int socksetup = createClientSocket(ip.toStdString(),port.toStdString(), &sock, &wsaData);
#elif defined (__linux__)
    int socksetup = createClientSocket(ip.toStdString(),port.toStdString(), &sock);
#endif
	if (socksetup != 0){
		QMessageBox errmsg;
		errmsg.setText("Error: Could not connect to server!");
		errmsg.setInformativeText("Please verify the address (" + ip +
			") and port used (" + port + ")");
		errmsg.setStandardButtons(QMessageBox::Ok);
		errmsg.setIcon(QMessageBox::Critical);
		errmsg.exec();
	}
	else{
		timerid = startTimer(750);
	}
}

RaceDisplay::~RaceDisplay()
{
	killTimer(timerid);
    delete ui;
    for (int i=0; i < NUM_HORSES_PER_RACE; i++){
        delete hName[i];
		delete hNum[i];
		delete hOdds[i];
    }
	/*delete img [0];
	delete img [1];
	delete mov [0];
	delete mov [1];
	delete race;*/
	//delete winBack;
	//delete winNum;
	//delete winName;
	//delete winOdds;
    //closesocket(sock);
    //WSACleanup();
    closeClientSocket(&sock);
}

void RaceDisplay::resizeEvent(QResizeEvent * event){
    int y = this->height()-120;//-40;
	int X = this->width();
	int x = X-40;
	int yspc = NUM_HORSES_PER_RACE;

    std::cout << "y:" << y;
	
	QFont hnumFont;
	QFont hnameFont;
	QFont hoddsFont;
	
	hnumFont.setPointSize(y/(2*yspc)*.7);
	hnameFont.setItalic(true);
	hnameFont.setPointSize(y/(2*yspc)*.6);
	hoddsFont.setBold(true);
	hoddsFont.setPointSize(y/(2*yspc)*1.1);

    for (int i=0 ; i< NUM_HORSES_PER_RACE; i++){
		hBack[i]->move(20,120+i*y/yspc);
		hBack[i]->setFixedHeight(y/yspc);
		hBack[i]->setFixedWidth(x);

        hNum[i]->move(20,120+i*y/yspc);
		hNum[i]->setMinimumWidth(x*3/4);
		hNum[i]->setFixedHeight(y/(2*yspc)*.8);
		hNum[i]->setFont(hnumFont);

		hName[i]->move(40,120+qRound((i+0.4)*y/yspc));
		hName[i]->setMinimumWidth(x*3/4);
		hName[i]->setFont(hnameFont);
		hName[i]->setFixedHeight(y/(2*yspc)*.8);

		hOdds[i]->move(x*3/4,120+i*y/yspc);
		hOdds[i]->setFixedWidth(x/4);
		hOdds[i]->setFont(hoddsFont);
		hOdds[i]->setFixedHeight(y/yspc);
    }
	race->move(X/2-160,10);
	img[0]->setFixedSize(X/2-180,60);
	img[1]->move(X/2+170,10);
	img[1]->setFixedSize(X/2-180,60);
	//hnameFont.setBold(true);
	//hnumFont.setBold(true);

	//winBack->move(20,20+NUM_HORSES_PER_RACE*y/yspc);
	//winBack->setFixedHeight(y/yspc);
	//winBack->setFixedWidth(x);

	//winNum->setFont(hnumFont);
	//winNum->move(20,20+NUM_HORSES_PER_RACE*y/yspc);
	//winNum->setMinimumWidth(x*3/4);
	//winNum->setFixedHeight(y/(2*yspc)*.8);

	//winName->setFont(hnameFont);
	//winName->move(40,20+qRound((NUM_HORSES_PER_RACE+0.4)*y/yspc));
	//winName->setMinimumWidth(x*3/4);
	//winName->setFixedHeight(y/(2*yspc)*.8);

	//winOdds->setFont(hoddsFont);
	//winOdds->move(x*3/4,20+NUM_HORSES_PER_RACE*y/yspc);
	//winOdds->setFixedWidth(x/4);
	//winOdds->setFixedHeight(y/yspc);

}

void RaceDisplay::timerEvent(QTimerEvent * event){
	enum HRErrorCode err;
	list<string> names;
	vector<int> odds;
	int winner;
	int r;
	err = getAllHorseNamesActive(&names,&sock);
	if (err == HR_SUCCESS && getAllHorseOddsActive(&odds,&sock)==HR_SUCCESS){
		int i=0;
		for (i=0; i< NUM_HORSES_PER_RACE; i++){
			hOdds[i]->setText(QString::number(odds[i])+ ":1");
			hName[i]->setText("");
			if (i%2)
				hBack[i]->setStyleSheet("QLabel { background-color : #CCFFFF; }");
			else
				hBack[i]->setStyleSheet("QLabel { background-color : #FFFF99; }");
		}
		i=0;
		for (auto& x: names){
			hName[i]->setText(QString::fromStdString(x));
			i++;
		}
	}
	else if (err == HR_NO_ACTIVE_RACE){
		for (int i=0; i< NUM_HORSES_PER_RACE; i++){
			if (i%2)
				hBack[i]->setStyleSheet("QLabel { background-color : #CCFFFF; }");
			else
				hBack[i]->setStyleSheet("QLabel { background-color : #FFFF99; }");
			hName[i]->setText("");
			hOdds[i]->setText("");
		}
	}

	err = getWinningHorseActive(&winner, &sock);
	if (err == HR_SUCCESS && winner >= 0)
		hBack[winner]->setStyleSheet("QLabel { background-color : yellow;}");

	err = getActiveRace(&r,&sock);
	if (r==-1)
		race->setText("");
	else{
		r++;
        QString rs = "Race " + QString::fromStdString(std::to_string(r));
		race->setText(rs);
	}

}

void RaceDisplay::setIP(QString s){ ip = s; }
void RaceDisplay::setPort(QString s){ port = s; }
bool RaceDisplay::hasValidSock(){ return (sock==INVALID_SOCKET ? false : true); }
