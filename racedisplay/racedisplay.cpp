#include "racedisplay.h"

RaceDisplay::RaceDisplay(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RaceDisplay)
{
    ui->setupUi(this);
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
}

RaceDisplay::~RaceDisplay()
{
    delete ui;
    for (int i=0; i < NUM_HORSES_PER_RACE; i++){
        delete hName[i];
		delete hNum[i];
		delete hOdds[i];
    }
	//delete winBack;
	//delete winNum;
	//delete winName;
	//delete winOdds;
}

void RaceDisplay::resizeEvent(QResizeEvent * event){
    int y = this->height()-40;
	int x = this->width()-40;
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
		hBack[i]->move(20,20+i*y/yspc);
		hBack[i]->setFixedHeight(y/yspc);
		hBack[i]->setFixedWidth(x);

        hNum[i]->move(20,20+i*y/yspc);
		hNum[i]->setMinimumWidth(x*3/4);
		hNum[i]->setFixedHeight(y/(2*yspc)*.8);
		hNum[i]->setFont(hnumFont);

		hName[i]->move(40,20+qRound((i+0.4)*y/yspc));
		hName[i]->setMinimumWidth(x*3/4);
		hName[i]->setFont(hnameFont);
		hName[i]->setFixedHeight(y/(2*yspc)*.8);

		hOdds[i]->move(x*3/4,20+i*y/yspc);
		hOdds[i]->setFixedWidth(x/4);
		hOdds[i]->setFont(hoddsFont);
		hOdds[i]->setFixedHeight(y/yspc);
    }
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

void RaceDisplay::setIP(QString s){ ip = s; }
void RaceDisplay::setPort(QString s){ port = s; }
