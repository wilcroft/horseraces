#ifndef SETUPWINDOW_H
#define SETUPWINDOW_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QFont>

class SetupWindow : public QDialog
{
    Q_OBJECT
    QString ip;
    QString port;
    QPushButton * okbut;
    QLineEdit * ipline;
    QLineEdit * portline;
    QLabel * iplabel;
    QLabel * portlabel;
public:
    explicit SetupWindow(QWidget *parent = 0);
    ~SetupWindow();
    QString getIP();
    QString getPort();

signals:

public slots:
        void updateIP();
        void updatePort();

};

#endif // SETUPWINDOW_H
