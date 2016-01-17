#ifndef RACESERVERGUI_H
#define RACESERVERGUI_H

#include <QtWidgets/QMainWindow>
#include <qtabwidget.h>
#include <qtextedit.h>
#include <qtextdocument.h>
#include <qtablewidget.h>
#include <qcombobox.h>
#include <qstringlist.h>
#include <qpushbutton.h>
#include <qflags.h>
#include <qlistwidget.h>

#include <thread>

#include "ui_raceservergui.h"
#include "qdebugstream.h"
#include "stringlistutils.h"
#include "guiserver.h"

#define NUMTABS 5
#define STATUSTAB 0
#define PEOPLETAB 1
#define RACETAB 2
#define BETTAB 3
#define PAYOUTTAB 4

class RaceServerGUI : public QMainWindow
{
	Q_OBJECT

public:
	RaceServerGUI(QWidget *parent = 0);
	~RaceServerGUI();

public slots:
	//General
	void tabChanged(int t);
	void scrollToBottom();

	//Status Tab

	//Partipicant Tab
	void updatePartTable();
	void editParticipant(QListWidgetItem * i);

	//Bet Tab
	void updateBetTable();
	void editBet(int r, int c);
	void editBet(QTableWidgetItem * i);

	//Win Tab
	void updateWinTable();

signals:
	void doScroll(int i);
	void clearList();

private:
	Ui::RaceServerGUIClass ui;

	QTextEdit * serverlogwindow;
	QTextDocument * serverlog;
	QDebugStream * errStream;
	QDebugStream * logStream;
	std::mutex loglk;

	QTabWidget * tabgroup;
	QWidget * tab [NUMTABS];

	QListWidget * partList;
	QPushButton * partRefresh;
	std::list <std::string> plist;

	QTableWidget * betTable;
	QComboBox * betRaceSelect;
	QPushButton * betRefresh;
	bool isLoadingBets;
	
	QTableWidget * winTable;
	QComboBox * winRaceSelect;
	QPushButton * winRefresh;

	std::thread * serverThread;
	bool isServerActive;
};

#endif // RACESERVERGUI_H
