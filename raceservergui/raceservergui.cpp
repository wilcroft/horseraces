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

	logStream = new QDebugStream(std::cout,serverlogwindow, Qt::white);
	errStream = new QDebugStream(std::cerr,serverlogwindow, Qt::red);

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
	serverThread = new std::thread(serverFunc,&isServerActive);

	while (!isServerActive);

	//Tab 0 - Server Status/Info
	tabgroup->setTabText(0,"Server Info");

	
	//Tab 3 - Server Status/Info
	tabgroup->setTabText(3,"Bets");

	betRaceSelect = new QComboBox(tab[3]);
	betRaceSelect->move(20,20);
	for (int i=0; i<NUM_RACES; i++){
		betRaceSelect->addItem("Race " + QString::number(i+1));
	}

	betRefresh = new QPushButton(tab[3]);
	betRefresh->setIcon(QIcon::fromTheme("view-refresh",QIcon("view-refresh.png")));
	betRefresh->move(40,60);

	betTable = new QTableWidget(tab[3]);
	betTable->move(100,20);
	betTable->setFixedSize(800,560);

	QStringList participants = toQStringList(hr->getParticipants());
	betTable->setRowCount(participants.size());
	betTable->setColumnCount(NUM_HORSES_PER_RACE);
	betTable->setVerticalHeaderLabels(participants);

	connect(betRaceSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(updateBetTable()));
	//QStringList

}

RaceServerGUI::~RaceServerGUI()
{
	isServerActive = false;

	delete logStream;
	delete errStream;
	delete serverlog;
	delete serverlogwindow;

	for (int i = 0; i<NUMTABS; i++) delete tab[i];
	delete tabgroup;
}

void RaceServerGUI::updateBetTable(){
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

	
}