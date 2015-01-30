#ifndef RACECLIENTGUI_H
#define RACECLIENTGUI_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <qcombobox.h>
#include <QtGui/QFont>
#include <qmessagebox.h>
#include <QMutex>
#include <qcompleter.h>
#include <qpushbutton.h>
#include <cmath>

#include "ui_raceclientgui.h"

#include "../clientlibrary/hrclientlibrary.h"
#include "../horseraces/constants.h"

#if defined(_WIN32) || defined(_WIN64)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#pragma comment (lib, "clientlibrary.lib")
#endif

namespace Ui {
class RaceClientGUI;
}

class RaceClientGUI : public QMainWindow
{
	Q_OBJECT

	int w;
	int h;

    QString port;
    QString ip;

	QStringList participants;

	QMutex socklock;

	QCompleter * participCompl;
	QCompleter * betCompl;

	//Frames
	QFrame * activeFrame;
	QFrame * betFrame;
	QFrame * winFrame;
	QFrame * houseFrame;
	QFrame * totalFrame;
	QFrame * horseFrame;

	//Labels
	QLabel * betTitle;
	QLabel * betNameLabel;
	QLabel * betHorseLabel;
	QLabel * betAmtLabel;
	QLabel * betRaceLabel;
	QLabel * activeRaceLabel;
	QLabel * winTitle;
	QLabel * winRaceLabel;
	QLabel * winHorseLabel;
	QLabel * houseTitle;
	QLabel * houseRaceLabel;
	QLabel * housePctLabel;
	QLabel * totalTitle;
	QLabel * totalEarnings;
	QLabel * totalActive;
	QLabel * horseTitle;
	QLabel * horseNameLabel;
	QLabel * horseRaceLabel;
	QLabel * horseNumLabel;

	//TextBoxes
	QLineEdit * betName;
	QLineEdit * betAmt;
	QLineEdit * housePct;
	QLineEdit * horseName;
	

	//ComboBoxes
	QComboBox * betRace;
	QComboBox * betHorse;
	QComboBox * activeRace;
	QComboBox * winRace;
	QComboBox * winHorse;
	QComboBox * houseRace;
	QComboBox * horseNum;
	QComboBox * horseRace;

	//Buttons
	QPushButton * betSubmit;
	QPushButton * activeSubmit;
	QPushButton * winSubmit;
	QPushButton * houseSubmit;
	QPushButton * horseSubmit;

	int timerid;

	SOCKET sock;
	WSADATA wsaData;

public:
	RaceClientGUI(QString ip = "", QString port = "", QWidget *parent = 0);
	~RaceClientGUI();
	bool hasValidSock();

protected:
	void timerEvent(QTimerEvent *);

public slots:
	void changeActive();
	void newBet();
	void setWinner();
	void setTake();
	void pullTake();
	void pullName();

private:
	Ui::RaceClientGUI * ui;
};

#endif // RACECLIENTGUI_H
