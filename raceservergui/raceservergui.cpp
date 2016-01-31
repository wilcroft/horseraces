#include "raceservergui.h"

RaceServerGUI::RaceServerGUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	this->setFixedSize(1000,800);

	serverlogwindow = new QTextEdit(this);
	serverlogwindow->move(0,600);
	serverlogwindow->setFixedSize(1000,200);
	serverlogwindow->setReadOnly(true);
	serverlogwindow->setStyleSheet("QTextEdit { background-color : black; }");

	serverlog = new QTextDocument(serverlogwindow);
	serverlogwindow->setDocument(serverlog);

	logStream = new QDebugStream(logstream,serverlogwindow, Qt::white);
	errStream = new QDebugStream(errstream,serverlogwindow, Qt::red);

	connect (serverlogwindow, SIGNAL(textChanged()), this, SLOT(scrollToBottom()));
	connect (this, SIGNAL(doScroll(int)), serverlogwindow->verticalScrollBar(), SLOT(setValue(int)));

//	std::cout << "Test cout!" << std::endl;
//	std::cerr << "Test cerr!" << std::endl;
//	std::cout << "Test cout!" << std::endl;

	tabgroup = new QTabWidget(this);
	tabgroup->setFixedSize(1000,600);
	tabgroup->setTabPosition(QTabWidget::East);

	for (int i = 0; i<NUMTABS; i++){
		tab[i] = new QWidget ();
		//tab[i]->setStyleSheet("QWidget {background-color : #" + QString::number(i).repeated(6) + " }");
		tabgroup->addTab(tab[i],"Tab "+ QString::number(i));
	}

	isServerActive = false;
	serverThread = new std::thread(serverFunc,&(bool)isServerActive);

	while (!isServerActive);

	connect (tabgroup,SIGNAL(currentChanged(int)),this,SLOT(tabChanged(int)));

	//Tab 0 - Server Status/Info
	tabgroup->setTabText(STATUSTAB,"Server Info");
	statusIP = new QLabel("IP Address:",tab[STATUSTAB]);
	statusIPValue = new QLabel("-                        ",tab[STATUSTAB]);
	statusPort = new QLabel("Port Num:",tab[STATUSTAB]);
	statusPortValue = new QLabel("-                      ",tab[STATUSTAB]);
	statusTake = new QLabel("House Take:",tab[STATUSTAB]);
	statusTakeValue = new QLabel("-                      ",tab[STATUSTAB]);
	statusActive = new QLabel("Active Race:",tab[STATUSTAB]);
	statusActiveValue = new QLabel("-                    ",tab[STATUSTAB]);
	statusIP->move(80,80);
	statusIPValue->move(200,80);
	statusPort->move(80,100);
	statusPortValue->move(200,100);
	statusTake->move(80,140);
	statusTakeValue->move(200,140);
	statusActive->move(80,120);
	statusActiveValue->move(200,120);
	statusRefresh = new QPushButton(tab[STATUSTAB]);
	statusRefresh->setIcon(QIcon::fromTheme("view-refresh",QIcon("view-refresh.png")));
	statusRefresh->move(40,60);
	
	connect(statusRefresh,SIGNAL(clicked()), this, SLOT(updateStatus()));

	// Tab 1 - Participant List
	tabgroup->setTabText(PEOPLETAB,"Participants");

	partRefresh = new QPushButton(tab[PEOPLETAB]);
	partRefresh->setIcon(QIcon::fromTheme("view-refresh",QIcon("view-refresh.png")));
	partRefresh->move(40,60);

	partList = new QListWidget(tab[PEOPLETAB]);
	partList->setFixedSize(400,560);
	partList->move(100,20);
	
	connect(partRefresh,SIGNAL(clicked()), this, SLOT(updatePartTable()));
	connect(partList,SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(editParticipant(QListWidgetItem *)));

	//Tab 3 - Bets
	tabgroup->setTabText(BETTAB,"Bets");

	betRaceSelect = new QComboBox(tab[BETTAB]);
	betRaceSelect->move(20,20);
	for (int i=0; i<NUM_RACES; i++){
		betRaceSelect->addItem("Race " + QString::number(i+1));
	}

	betRefresh = new QPushButton(tab[BETTAB]);
	betRefresh->setIcon(QIcon::fromTheme("view-refresh",QIcon("view-refresh.png")));
	betRefresh->move(40,60);

	betTable = new QTableWidget(tab[BETTAB]);
	betTable->move(100,20);
	betTable->setFixedSize(800,560);

	QStringList participants = toQStringList(hr->getParticipants());
	betTable->setRowCount(participants.size());
	betTable->setColumnCount(NUM_HORSES_PER_RACE);
	betTable->setVerticalHeaderLabels(participants);
	isLoadingBets = false;
	
	connect(betRaceSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(updateBetTable()));
	connect(betRefresh, SIGNAL(clicked()), this, SLOT(updateBetTable()));
	connect(betTable, SIGNAL(cellChanged(int,int)), this, SLOT(editBet(int,int)));
	
	//Tab 4 - Payouts
	tabgroup->setTabText(PAYOUTTAB,"Payouts");

	winRaceSelect = new QComboBox(tab[PAYOUTTAB]);
	winRaceSelect->move(20,20);
	for (int i=0; i<NUM_RACES; i++){
		winRaceSelect->addItem("Race " + QString::number(i+1));
	}

	winRefresh = new QPushButton(tab[PAYOUTTAB]);
	winRefresh->setIcon(QIcon::fromTheme("view-refresh",QIcon("view-refresh.png")));
	winRefresh->move(40,60);

	winTable = new QTableWidget(tab[PAYOUTTAB]);
	winTable->move(100,20);
	winTable->setFixedSize(800,560);
	winTable->setColumnCount(1);

	connect(winRaceSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(updateWinTable()));
	connect(winRefresh, SIGNAL(clicked()), this, SLOT(updateWinTable()));


}

RaceServerGUI::~RaceServerGUI()
{
	isServerActive = false;

	delete logStream;
	delete errStream;
	delete serverlog;
	delete serverlogwindow;

	for (int i = NUMTABS-1; i>=0; i--) delete tab[i];
	delete tabgroup;
}

void RaceServerGUI::tabChanged(int t){
	switch (t){
	case STATUSTAB:
		updateStatus();
		break;
	case PEOPLETAB:
		updatePartTable();
		break;
	case RACETAB:
		break;
	case BETTAB:
		updateBetTable();
		break;
	case PAYOUTTAB:
		updateWinTable();
		break;
	default:
		;
	}

}

void RaceServerGUI::scrollToBottom(){
	//serverlogwindow->verticalScrollBar()->triggerAction(QAbstractSlider::SliderToMaximum);
	loglk.lock();
	emit doScroll(serverlogwindow->verticalScrollBar()->maximum());
	loglk.unlock();
}

void RaceServerGUI::updateStatus(){
	statusTakeValue->setText("$"+QString::number(hr->getHouseWinningsAll()));
	statusIPValue->setText(QString::fromStdString(serverIP));
	statusPortValue->setText(QString::fromStdString(PORT));
	statusActiveValue->setText("Race " + QString::number(hr->getActiveRace()));
}

void RaceServerGUI::updatePartTable(){
	plist = hr->getParticipants();
	partList->clear();

	QListWidgetItem * item  = new QListWidgetItem ("<New-Participant>");
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	partList->addItem(item);
	for (auto& x:plist){
		item = new QListWidgetItem (QString::fromStdString(x));
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		partList->addItem(item);
	}
	
}

void RaceServerGUI::editParticipant(QListWidgetItem * i){
	int r = partList->row(i);
	if (r == 0){
		hr->addParticipant(i->text().toStdString());
	}
	else{
		int j=1;
		for (auto& x: plist){
			if (j==r)
				hr->editParticipant(x,i->text().toStdString());
			j++;
		}
	}	
	updatePartTable();
}

void RaceServerGUI::updateBetTable(){
	isLoadingBets = true;
	int r = betRaceSelect->currentIndex();

	list <Better> betters = hr->getBetterList(r);
	betTable->setRowCount(betters.size());
	betTable->setVerticalHeaderLabels(toQStringList(betters));

	QStringList horsenames;
	for (int i=0; i<NUM_HORSES_PER_RACE; i++){
		horsenames.push_back(QString::fromStdString(hr->getHorseName(r,i)));
		//for (int j=0; j<betters.size(); j++){
		int j=0;
		for (auto& bet: betters){
			QTableWidgetItem * cell = new QTableWidgetItem(QString::number(bet.getBet(i)));
			betTable->setItem(j,i,cell);
			j++;
		}
	}
	betTable->setHorizontalHeaderLabels(horsenames);

	isLoadingBets = false;
}

void RaceServerGUI::editBet(int r, int c){
	if (!isLoadingBets){
		std::string p = betTable->verticalHeaderItem(r)->text().toStdString();

		int bet = betTable->item(r,c)->text().toInt();

		//std::cout << p.toStdString() << " bets $" << bet << " on Horse " << c+1 << std::endl;
		logstream << "Changed bet for "  << p << " on Horse " << c+1 << " to $" << bet << std::endl;
		if (hr->setBet(betRaceSelect->currentIndex(),p,c,bet) == HR_SUCCESS){
			writeBetListToFile(hr,betRaceSelect->currentIndex());
		}
	}
}

void RaceServerGUI::editBet(QTableWidgetItem * i){
	if (!isLoadingBets){
		int r = betTable->row(i);
		int c = betTable->column(i);
		QString p = betTable->verticalHeaderItem(r)->text();

		int bet = i->text().toInt();

		logstream << p.toStdString() << " bets $" << bet << " on Horse " << c+1 << std::endl;
	}
}

void RaceServerGUI::updateWinTable(){
	int r = winRaceSelect->currentIndex();

	std::list<Better> betters = hr->getBetterList(r);
	std::list<Better> winners;
	for (auto& x: betters)
		if (x.getPayout() > 0) winners.push_back(x);

	winTable->setRowCount(winners.size());
	QStringList ql;
	int j=0;
	for (auto& x:winners){
		ql.push_back(QString::fromStdString(x.getName()));
		QTableWidgetItem * cell = new QTableWidgetItem(QString::number(x.getPayout()));
		cell->setFlags(cell->flags() &  ~Qt::ItemIsEditable);
		winTable->setItem(j,0,cell);
		j++;
	}
	winTable->setHorizontalHeaderLabels(QStringList("Winnings"));
	winTable->setVerticalHeaderLabels(ql);

}

