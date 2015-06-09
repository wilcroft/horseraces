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

#include <thread>

#include "ui_raceservergui.h"
#include "qdebugstream.h"
#include "stringlistutils.h"
#include "guiserver.h"

#define NUMTABS 5

class RaceServerGUI : public QMainWindow
{
	Q_OBJECT

public:
	RaceServerGUI(QWidget *parent = 0);
	~RaceServerGUI();

public slots:
	void updateBetTable();

private:
	Ui::RaceServerGUIClass ui;

	QTextEdit * serverlogwindow;
	QTextDocument * serverlog;
	QDebugStream * errStream;
	QDebugStream * logStream;

	QTabWidget * tabgroup;
	QWidget * tab [NUMTABS];


	QTableWidget * betTable;
	QComboBox * betRaceSelect;
	QPushButton * betRefresh;

	std::thread * serverThread;
	bool isServerActive;
};

#endif // RACESERVERGUI_H
