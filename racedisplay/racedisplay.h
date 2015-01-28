#ifndef RACEDISPLAY_H
#define RACEDISPLAY_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtGui/QFont>
#include <iostream>
#define NUM_HORSES_PER_RACE 6

#include "ui_racedisplay.h"

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

public:
	RaceDisplay(QWidget *parent = 0);
	~RaceDisplay();
    void setIP(QString s);
    void setPort(QString s);
	
protected:
    void resizeEvent(QResizeEvent *);

private:
	Ui::RaceDisplay * ui;
};

#endif // RACEDISPLAY_H
