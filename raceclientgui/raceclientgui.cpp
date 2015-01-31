#include "raceclientgui.h"

RaceClientGUI::RaceClientGUI(QString ip, QString port,QWidget *parent)
	: QMainWindow(parent),
    ui(new Ui::RaceClientGUI)
{
	ui->setupUi(this);
	ui->centralWidget->setStyleSheet("QWidget {background-color : skyblue}");
	this->ip = ip;
	this->port = port;
	int w = 1000;
	int h = 500;
	this->setFixedSize(w,h);
	

	//Draw Frames
	totalFrame = new QFrame(this);
	houseFrame = new QFrame(this);
	winFrame = new QFrame(this);
	horseFrame = new QFrame(this);
	betFrame = new QFrame(this);
	activeFrame = new QFrame(this);
	QString style = "QFrame { border : 5px ridge #999999}";
	activeFrame->setFixedSize(253,323);
	activeFrame->move(0,0);
	activeFrame->setStyleSheet(style);
	betFrame->setFixedSize(752,163);
	betFrame->move(248,0);
	betFrame->setStyleSheet(style);
	winFrame->setFixedSize(303,182);
	winFrame->move(0,318);
	winFrame->setStyleSheet(style);
	houseFrame->setFixedSize(305,182);
	houseFrame->move(298,318);
	houseFrame->setStyleSheet(style);
	totalFrame->setFixedSize(402,182);
	totalFrame->move(598,318);
	totalFrame->setStyleSheet(style);
	horseFrame->setFixedSize(752,165);
	horseFrame->move(248,158);
	horseFrame->setStyleSheet(style);

	// Add Combo Boxes
	QFont comboFont,titleFont;
	comboFont.setPointSize(16);
	titleFont.setPointSize(28);
	titleFont.setBold(true);
	titleFont.setItalic(true);
	QStringList raceList;
	for (int i = 1; i <= NUM_RACES; i++){
		raceList.push_back("Race " + QString::number(i));
	}
	QStringList horseList;
	for (int i = 1; i <= NUM_HORSES_PER_RACE; i++){
		horseList.push_back("Horse " + QString::number(i));
	}

	//Active Area Objects
	gif = new QMovie("horserace.gif");
	horsegif = new QLabel(this);
	horsegif->move(20,20);
	horsegif->setMovie(gif);
	horsegif->setFixedSize(208,136);
	gif->start();

	activeRaceLabel = new QLabel("Active Race", this);
	activeRaceLabel->setFont(titleFont);
	activeRaceLabel->setFixedSize(230,40);
	activeRaceLabel->move(10,170);

	activeRace = new QComboBox(this);
	activeRace->setFont(comboFont);
	activeRace->addItem("None");
	activeRace->addItems(raceList);
	activeRace->move(78,225);
	activeRace->setFixedWidth(95);

	activeSubmit = new QPushButton("OK", this);
	activeSubmit->setFont(comboFont);
	activeSubmit->move(78,270);

	connect(activeSubmit, SIGNAL(clicked()), this, SLOT(changeActive()));

	raceList.push_front("Active");

	//Bet Area Objects
	betRace = new QComboBox(this);
	betRace->setFont(comboFont);
	betRace->addItems(raceList);
	betRace->move(675,25);
	betRace->setFixedWidth(95);

	betRaceLabel = new QLabel("Race:",this);
	betRaceLabel->setFont(comboFont);
	betRaceLabel->setFixedSize(150,30);
	betRaceLabel->move(505,27);
	betRaceLabel->setAlignment(Qt::AlignRight);
	
	betHorse = new QComboBox(this);
	betHorse->setFont(comboFont);
	betHorse->addItems(horseList);
	betHorse->move(675,100);
	betHorse->setFixedWidth(100);

	betHorseLabel = new QLabel("Horse:",this);
	betHorseLabel->setFont(comboFont);
	betHorseLabel->setFixedSize(150,30);
	betHorseLabel->move(675,65);

	betName = new QLineEdit(this);
	betName->setFont(comboFont);
	betName->setFixedSize(350,30);
	betName->move(300,100);

	betNameLabel = new QLabel("Name:",this);
	betNameLabel->setFont(comboFont);
	betNameLabel->setFixedSize(150,30);
	betNameLabel->move(300,65);

	betAmt = new QLineEdit(this);
	betAmt->setFont(comboFont);
	betAmt->setFixedSize(100,30);
	betAmt->move(800,100);

	betOK = new QLabel("Bet Received!",this);
	betOK->setFont(comboFont);
	betOK->setFixedSize(150,30);
	betOK->move (820,135);
	betOK->setAlignment(Qt::AlignRight);
	betOK->setStyleSheet("QLabel { color : #339900; }");
	betOK->setVisible(false);

	QStringList amts;
	for (int i=0; i< 100; i++)
		amts.push_back(QString::number(i*50));
	amts.sort();
	betCompl = new QCompleter(amts, this);
	betAmt->setCompleter(betCompl);

	betAmtLabel = new QLabel("Bet:",this);
	betAmtLabel->setFont(comboFont);
	betAmtLabel->setFixedSize(150,30);
	betAmtLabel->move(800,65);

	betTitle = new QLabel("New Bet:",this);
	betTitle->setFont(titleFont);
	betTitle->setFixedSize(200, 40);
	betTitle->move(270,20);

	betSubmit = new QPushButton("OK", this);
	betSubmit->setFont(comboFont);
	betSubmit->move(920,100);
	betSubmit->setFixedSize(50,30);
	
	connect(betSubmit,SIGNAL(clicked()),this,SLOT(newBet()));

	//Horse Area Objects
	horseTitle = new QLabel("Name a Horse:",this);
	horseTitle->setFont(titleFont);
	horseTitle->setFixedSize(350, 40);
	horseTitle->move(270,180);

	horseName = new QLineEdit(this);
	horseName->setFont(comboFont);
	horseName->setFixedSize(350,30);
	horseName->move(300,260);

	horseNameLabel = new QLabel("Name:",this);
	horseNameLabel->setFont(comboFont);
	horseNameLabel->setFixedSize(150,30);
	horseNameLabel->move(300,225);
	
	horseNum = new QComboBox(this);
	horseNum->setFont(comboFont);
	horseNum->addItems(horseList);
	horseNum->move(675,260);
	horseNum->setFixedWidth(100);

	horseNumLabel = new QLabel("Horse:",this);
	horseNumLabel->setFont(comboFont);
	horseNumLabel->setFixedSize(150,30);
	horseNumLabel->move(675,225);

	horseRace = new QComboBox(this);
	horseRace->setFont(comboFont);
	horseRace->addItems(raceList);
	horseRace->move(800,260);
	horseRace->setFixedWidth(95);

	horseRaceLabel = new QLabel("Race:",this);
	horseRaceLabel->setFont(comboFont);
	horseRaceLabel->setFixedSize(150,30);
	horseRaceLabel->move(800,225);

	horseSubmit = new QPushButton("OK", this);
	horseSubmit->setFont(comboFont);
	horseSubmit->move(920,260);
	horseSubmit->setFixedSize(50,30);
	
	connect(horseSubmit,SIGNAL(clicked()),this,SLOT(setName()));
	connect(horseNum,SIGNAL(currentIndexChanged(int)),this,SLOT(pullName()));
	connect(horseRace,SIGNAL(currentIndexChanged(int)),this,SLOT(pullName()));

	// Winner Area Objects
	winTitle = new QLabel("Set Winner:",this);
	winTitle->setFont(titleFont);
	winTitle->setFixedSize(300,40);
	winTitle->move(20,340);

	winRace = new QComboBox(this);
	winRace->setFont(comboFont);
	winRace->addItems(raceList);
	winRace->move(120,400);
	winRace->setFixedWidth(95);

	winRaceLabel = new QLabel("Race:",this);
	winRaceLabel->setFont(comboFont);
	winRaceLabel->setFixedSize(90,30);
	winRaceLabel->move(20,402);
	winRaceLabel->setAlignment(Qt::AlignRight);
	
	winHorse = new QComboBox(this);
	winHorse->setFont(comboFont);
	winHorse->addItem("None");
	winHorse->addItems(horseList);
	winHorse->move(120,450);
	winHorse->setFixedWidth(100);

	winHorseLabel = new QLabel("Horse:",this);
	winHorseLabel->setFont(comboFont);
	winHorseLabel->setFixedSize(90,30);
	winHorseLabel->move(20,452);
	winHorseLabel->setAlignment(Qt::AlignRight);

	winSubmit = new QPushButton("OK", this);
	winSubmit->setFont(comboFont);
	winSubmit->move(237,450);
	winSubmit->setFixedSize(50,30);

	connect(winSubmit,SIGNAL(clicked()),this,SLOT(setWinner()));
	
	//House Objects
	houseTitle = new QLabel("House Take:",this);
	houseTitle->setFont(titleFont);
	houseTitle->setFixedSize(300,40);
	houseTitle->move(320,340);

	houseRace = new QComboBox(this);
	houseRace->setFont(comboFont);
	houseRace->addItems(raceList);
	houseRace->move(420,400);
	houseRace->setFixedWidth(95);

	houseRaceLabel = new QLabel("Race:",this);
	houseRaceLabel->setFont(comboFont);
	houseRaceLabel->setFixedSize(90,30);
	houseRaceLabel->move(320,402);
	houseRaceLabel->setAlignment(Qt::AlignRight);

	housePct = new QLineEdit(this);
	housePct->setFont(comboFont);
	housePct->setFixedSize(95,30);
	housePct->move(420,450);

	housePctLabel = new QLabel("Take (%):",this);
	housePctLabel->setFont(comboFont);
	housePctLabel->setFixedSize(92,30);
	housePctLabel->move(318,452);
	housePctLabel->setAlignment(Qt::AlignRight);

	houseSubmit = new QPushButton("OK", this);
	houseSubmit->setFont(comboFont);
	houseSubmit->move(537,450);
	houseSubmit->setFixedSize(50,30);

	connect(houseSubmit,SIGNAL(clicked()),this,SLOT(setTake()));
	connect(houseRace,SIGNAL(currentIndexChanged(int)), this, SLOT(pullTake()));

	//Totals Objects
	totalTitle = new QLabel("House Earnings:",this);
	totalTitle->setFont(titleFont);
	totalTitle->setFixedSize(350,40);
	totalTitle->move(620,340);

	totalEarnings = new QLabel("Overall:       $",this);
	totalEarnings->setFont(comboFont);
	totalEarnings->setFixedSize(360,30);
	totalEarnings->move(620,400);
	
	totalActive = new QLabel("Active Race: $",this);
	totalActive->setFont(comboFont);
	totalActive->setFixedSize(360,30);
	totalActive->move(620,450);
	/*participants <<  "alpha" << "beta" << "charlie"<< "delta";
	participCompl = new QCompleter(participants, this);
	participCompl->setCaseSensitivity(Qt::CaseInsensitive);
	betName->setCompleter(participCompl);
	participants ;
	delete participCompl;
	participCompl = new QCompleter(participants, this);
	participCompl->setCaseSensitivity(Qt::CaseInsensitive);
	betName->setCompleter(participCompl);*/

	int socksetup = createClientSocket(ip.toStdString(),port.toStdString(), &sock, &wsaData);
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
		list <string> p;
		socklock.lock();
		getParticipantList(&p, &sock);
		socklock.unlock();
		participants.clear();
		for (auto& x: p){
			participants.push_back(QString::fromStdString(x));
		}
		participCompl = new QCompleter(participants, this);
		participCompl->setCaseSensitivity(Qt::CaseInsensitive);
		betName->setCompleter(participCompl);
		timerid = startTimer(10000);
	}
}

RaceClientGUI::~RaceClientGUI()
{
	killTimer(timerid);
	//Frames
	delete activeFrame;
	delete betFrame;
	delete winFrame;
	delete houseFrame;
	delete totalFrame;

	//Labels
	delete betTitle;
	delete betNameLabel;
	delete betHorseLabel;
	delete betAmtLabel;
	delete betRaceLabel;
	delete activeRaceLabel;
	delete winTitle;
	delete winRaceLabel;
	delete winHorseLabel;
	delete houseTitle;
	delete houseRaceLabel;
	delete housePctLabel;
	delete totalTitle;
	delete totalEarnings;
	delete totalActive;
	delete horseTitle;
	delete horseNameLabel;
	delete horseRaceLabel;
	delete horseNumLabel;
	delete horsegif;

	//TextBoxes
	delete betName;
	delete betAmt;
	delete housePct;
	delete horseName;

	//ComboBoxes
	delete betRace;
	delete betHorse;
	delete activeRace;
	delete winRace;
	delete winHorse;
	delete houseRace;
	delete horseNum;
	delete horseRace;

	delete betSubmit;
	delete activeSubmit;
	delete winSubmit;
	delete houseSubmit;
	delete horseSubmit;
	
	delete gif;
//	delete participCompl;
//	delete betCompl;

	delete ui;

	closesocket(sock);
	WSACleanup();

}

bool RaceClientGUI::hasValidSock(){ return (sock==INVALID_SOCKET ? false : true); }

void RaceClientGUI::timerEvent(QTimerEvent *event){
	if(event->timerId()==timerid){
		list <string> p;
		int r;
		float f;
		int wAll, w;
		int i = houseRace->currentIndex();
		int j = horseRace->currentIndex();
		int h = horseNum->currentIndex();
		string n;
		socklock.lock();
		//do things
		getParticipantList(&p, &sock);
		getAllHouseWinnings(&wAll,&sock);
		getActiveRace(&r, &sock);
		if (r!=-1)
			getHouseWinningsActive(&w,&sock);
		else 
			w = 0;
		if (i==0)
			getHouseTakePctActive(&f, &sock);
		else
			getHouseTakePct(i-1,&f,&sock);
		if (j == 0)
			getHorseNameActive(h,&n,&sock);
		else
			getHorseName(j-1,h,&n,&sock);
		socklock.unlock();
		participants.clear();
		for (auto& x: p){
			participants.push_back(QString::fromStdString(x));
		}
		delete participCompl;
		participCompl = new QCompleter(participants, this);
		participCompl->setCaseSensitivity(Qt::CaseInsensitive);
		betName->setCompleter(participCompl);

		activeRace->setCurrentIndex(r+1);
		totalEarnings->setText("Overall:       $" + QString::number(wAll));
		totalActive->setText("Active Race: $" + QString::number(w));
	
		if (!horseName->isModified())
			horseName->setText(QString::fromStdString(n));

		if (!housePct->isModified())
			housePct->setText(QString::number(std::floor(f*100+0.5)));
	}
	else if (event->timerId() == oktimer){
		betOK->setVisible(false);
		killTimer(oktimer);
	}

}

void RaceClientGUI::changeActive(){
	int r = activeRace->currentIndex();
	socklock.lock();
	if (r == 0)
		setNoActiveRace(&sock);
	else
		setActiveRace(r-1,&sock);
	socklock.unlock();
}

void RaceClientGUI::newBet(){
	QString n = betName->text();
	int h = betHorse->currentIndex();
	int r = betRace->currentIndex();
	enum HRErrorCode err;
	int b = betAmt->text().toInt();
	
	socklock.lock();
	if (r == 0){
		getActiveRace(&r,&sock);
		if (r >= 0)
			err = addBet(r,h,b,n.toStdString(),&sock);
		else
			err = HR_NO_ACTIVE_RACE;
	}
	else
		err = addBet(r-1,h,b,n.toStdString(),&sock);
			
	socklock.unlock();
	if (err==HR_SUCCESS){
		betOK->setVisible(true);
		oktimer = startTimer(1500);
	}
}

void RaceClientGUI::setWinner(){
	int r = winRace->currentIndex();
	int h = winHorse->currentIndex();
	
	socklock.lock();
	if (r==0)
		setWinningHorseActive(h-1,&sock);
	else
		setWinningHorse	(r-1,h-1,&sock);
	socklock.unlock();
}
void RaceClientGUI::setTake(){
	int f = housePct->text().toInt();
	int r = houseRace->currentIndex();
	
	socklock.lock();
	if (r==0)
		setHouseTakeActive((f/100.0),&sock);
	else
		setHouseTake (r-1,(f/100.0),&sock);
	socklock.unlock();
}
void RaceClientGUI::setName(){
	QString n = horseName->text();
	int r = horseRace->currentIndex();
	int h = horseNum->currentIndex();

	socklock.lock();
	if (r == 0)
		setHorseNameActive(h,n.toStdString(),&sock);
	else
		setHorseName(r-1,h,n.toStdString(),&sock);
	socklock.unlock();
	horseName->setModified(false);
}
void RaceClientGUI::pullTake(){
	float f;
	int i = houseRace->currentIndex();

	socklock.lock();
	if (i==0)
		getHouseTakePctActive(&f, &sock);
	else
		getHouseTakePct(i-1,&f,&sock);
	socklock.unlock();
	housePct->setText(QString::number(std::floor(f*100+0.5)));
	housePct->setModified(false);
}
void RaceClientGUI::pullName(){
	if (!horseName->isModified()){
		int r = horseRace->currentIndex();
		int h = horseNum->currentIndex();
		string n;
		socklock.lock();
		if (r == 0)
			getHorseNameActive(h,&n,&sock);
		else
			getHorseName(r-1,h,&n,&sock);
		socklock.unlock();
		horseName->setText(QString::fromStdString(n));
		horseName->setModified(false);
	}
}