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
	betFrame = new QFrame(this);
	activeFrame = new QFrame(this);
	QString style = "QFrame { border : 5px ridge #999999}";
	activeFrame->setFixedSize(253,203);
	activeFrame->move(0,0);
	activeFrame->setStyleSheet(style);
	betFrame->setFixedSize(752,203);
	betFrame->move(248,0);
	betFrame->setStyleSheet(style);
	winFrame->setFixedSize(303,302);
	winFrame->move(0,198);
	winFrame->setStyleSheet(style);
	houseFrame->setFixedSize(305,302);
	houseFrame->move(298,198);
	houseFrame->setStyleSheet(style);
	totalFrame->setFixedSize(402,302);
	totalFrame->move(598,198);
	totalFrame->setStyleSheet(style);

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
	activeRace = new QComboBox(this);
	activeRace->setFont(comboFont);
	activeRace->addItem("None");
	activeRace->addItems(raceList);
	activeRace->move(78,90);
	activeRace->setFixedWidth(95);

	activeRaceLabel = new QLabel("Active Race", this);
	activeRaceLabel->setFont(titleFont);
	activeRaceLabel->setFixedSize(230,40);
	activeRaceLabel->move(10,20);

	activeSubmit = new QPushButton("OK", this);
	activeSubmit->setFont(comboFont);
	activeSubmit->move(78,150);

	connect(activeSubmit, SIGNAL(clicked()), this, SLOT(changeActive()));

	raceList.push_front("Active");

	//Bet Area Objects
	betRace = new QComboBox(this);
	betRace->setFont(comboFont);
	betRace->addItems(raceList);
	betRace->move(675,40);
	betRace->setFixedWidth(95);

	betNameLabel = new QLabel("Race:",this);
	betNameLabel->setFont(comboFont);
	betNameLabel->setFixedSize(150,30);
	betNameLabel->move(505,42);
	betNameLabel->setAlignment(Qt::AlignRight);
	
	betHorse = new QComboBox(this);
	betHorse->setFont(comboFont);
	betHorse->addItems(horseList);
	betHorse->move(675,140);
	betHorse->setFixedWidth(100);

	betHorseLabel = new QLabel("Horse:",this);
	betHorseLabel->setFont(comboFont);
	betHorseLabel->setFixedSize(150,30);
	betHorseLabel->move(675,105);

	betName = new QLineEdit(this);
	betName->setFont(comboFont);
	betName->setFixedSize(350,30);
	betName->move(300,140);

	betNameLabel = new QLabel("Name:",this);
	betNameLabel->setFont(comboFont);
	betNameLabel->setFixedSize(150,30);
	betNameLabel->move(300,105);

	betAmt = new QLineEdit(this);
	betAmt->setFont(comboFont);
	betAmt->setFixedSize(100,30);
	betAmt->move(800,140);

	QStringList amts;
	for (int i=0; i< 100; i++)
		amts.push_back(QString::number(i*50));
	amts.sort();
	betCompl = new QCompleter(amts, this);
	betAmt->setCompleter(betCompl);

	betAmtLabel = new QLabel("Bet:",this);
	betAmtLabel->setFont(comboFont);
	betAmtLabel->setFixedSize(150,30);
	betAmtLabel->move(800,105);

	betTitle = new QLabel("New Bet:",this);
	betTitle->setFont(titleFont);
	betTitle->setFixedSize(200, 40);
	betTitle->move(270,20);

	betSubmit = new QPushButton("OK", this);
	betSubmit->setFont(comboFont);
	betSubmit->move(920,140);
	betSubmit->setFixedSize(50,30);
	
	connect(betSubmit,SIGNAL(clicked()),this,SLOT(newBet()));

	// Winner Area Objects
	winTitle = new QLabel("Set Winner:",this);
	winTitle->setFont(titleFont);
	winTitle->setFixedSize(300,40);
	winTitle->move(20,220);

	winRace = new QComboBox(this);
	winRace->setFont(comboFont);
	winRace->addItems(raceList);
	winRace->move(120,330);
	winRace->setFixedWidth(95);

	winRaceLabel = new QLabel("Race:",this);
	winRaceLabel->setFont(comboFont);
	winRaceLabel->setFixedSize(90,30);
	winRaceLabel->move(20,332);
	winRaceLabel->setAlignment(Qt::AlignRight);
	
	winHorse = new QComboBox(this);
	winHorse->setFont(comboFont);
	winHorse->addItem("None");
	winHorse->addItems(horseList);
	winHorse->move(120,410);
	winHorse->setFixedWidth(100);

	winHorseLabel = new QLabel("Horse:",this);
	winHorseLabel->setFont(comboFont);
	winHorseLabel->setFixedSize(90,30);
	winHorseLabel->move(20,412);
	winHorseLabel->setAlignment(Qt::AlignRight);

	winSubmit = new QPushButton("OK", this);
	winSubmit->setFont(comboFont);
	winSubmit->move(237,410);
	winSubmit->setFixedSize(50,30);

	connect(winSubmit,SIGNAL(clicked()),this,SLOT(setWinner()));
	
	//House Objects
	houseTitle = new QLabel("House Take:",this);
	houseTitle->setFont(titleFont);
	houseTitle->setFixedSize(300,40);
	houseTitle->move(320,220);

	houseRace = new QComboBox(this);
	houseRace->setFont(comboFont);
	houseRace->addItems(raceList);
	houseRace->move(420,330);
	houseRace->setFixedWidth(95);

	houseRaceLabel = new QLabel("Race:",this);
	houseRaceLabel->setFont(comboFont);
	houseRaceLabel->setFixedSize(90,30);
	houseRaceLabel->move(320,332);
	houseRaceLabel->setAlignment(Qt::AlignRight);

	housePct = new QLineEdit(this);
	housePct->setFont(comboFont);
	housePct->setFixedSize(95,30);
	housePct->move(420,410);

	housePctLabel = new QLabel("Take (%):",this);
	housePctLabel->setFont(comboFont);
	housePctLabel->setFixedSize(92,30);
	housePctLabel->move(318,412);
	housePctLabel->setAlignment(Qt::AlignRight);

	houseSubmit = new QPushButton("OK", this);
	houseSubmit->setFont(comboFont);
	houseSubmit->move(537,410);
	houseSubmit->setFixedSize(50,30);

	connect(houseSubmit,SIGNAL(clicked()),this,SLOT(setTake()));

	//Totals Objects
	totalTitle = new QLabel("House Earnings:",this);
	totalTitle->setFont(titleFont);
	totalTitle->setFixedSize(350,40);
	totalTitle->move(620,220);

	totalEarnings = new QLabel("Overall:       $",this);
	totalEarnings->setFont(comboFont);
	totalEarnings->setFixedSize(360,30);
	totalEarnings->move(620,332);
	
	totalActive = new QLabel("Active Race: $",this);
	totalActive->setFont(comboFont);
	totalActive->setFixedSize(360,30);
	totalActive->move(620,412);
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
	//killTimer(timerid);
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

	//TextBoxes
	delete betName;
	delete betAmt;
	delete housePct;

	//ComboBoxes
	delete betRace;
	delete betHorse;
	delete activeRace;
	delete winRace;
	delete winHorse;
	delete houseRace;

	delete betSubmit;
	delete activeSubmit;
	delete winSubmit;
	delete houseSubmit;
	
//	delete participCompl;
//	delete betCompl;

	delete ui;

	closesocket(sock);
	WSACleanup();

}

bool RaceClientGUI::hasValidSock(){ return (sock==INVALID_SOCKET ? false : true); }

void RaceClientGUI::timerEvent(QTimerEvent *){
	list <string> p;
	int r;
	float f;
	int wAll, w;
	int i = houseRace->currentIndex();
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

	if (!housePct->isModified())
		housePct->setText(QString::number(std::floor(f*100+0.5)));

}

void RaceClientGUI::changeActive(){
	int r = activeRace->currentIndex();
	socklock.lock();
	setActiveRace(r-1,&sock);
	socklock.unlock();
}

void RaceClientGUI::newBet(){
	QString n = betName->text();
	int h = betHorse->currentIndex();
	int r = betRace->currentIndex();
	int b = betAmt->text().toInt();
	
	socklock.lock();
	if (r == 0){
		getActiveRace(&r,&sock);
		if (r >= 0)
			addBet(r,h,b,n.toStdString(),&sock);
	}
	else
		addBet(r-1,h,b,n.toStdString(),&sock);
			
	socklock.unlock();
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