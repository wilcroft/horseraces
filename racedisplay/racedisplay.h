#ifndef RACEDISPLAY_H
#define RACEDISPLAY_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtGui/QFont>
#include <qmessagebox.h>
#include <iostream>

#include "ui_racedisplay.h"
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
class RaceDisplay;
}
class RaceDisplay : public QMainWindow
{
	Q_OBJECT
    QString port;
    QString ip;
    QLabel * hName[NUM_HORSES_PER_RACE];
    QLabel * hNum[NUM_HORSES_PER_RACE];
    QLabel * hOdds[NUM_HORSES_PER_RACE];
	QLabel * hBack[NUM_HORSES_PER_RACE];
	QLabel * winName;
	QLabel * winNum;
	QLabel * winOdds;
	QLabel * winBack;

	SOCKET sock;
	WSADATA wsaData;

	int timerid;

public:
	RaceDisplay(QString addr ="", QString p = "", QWidget *parent = 0);
	~RaceDisplay();
    void setIP(QString s);
    void setPort(QString s);
	bool hasValidSock();
	
protected:
    void resizeEvent(QResizeEvent *);
	void timerEvent(QTimerEvent *);

private:
	Ui::RaceDisplay * ui;


};

#endif // RACEDISPLAY_H
