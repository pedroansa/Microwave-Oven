#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "panel.h"
#include "Transitions.h"
#include <QWidget>
#include <QDial>
#include <QStateMachine>
#include <QTimer>
#include <QTime>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void changeState();
    void changeValue(int val);
    void clockComplete();

private:
    QDial *dial;
    panel *screen;
    QStateMachine *mac;
    QTimer *timer;
    QTimer *stoptimer;
    QTimer *cooktimer;
    int val;

    QState *dft;
    QState *stop;
    QState *idle;
    QState *power;
    QState *clockh;
    QState *clockm;
    QState *mode;
    QState *defrost;
    QState *timeslc;
    QState *cooking;

signals:
    void finishCooking();
};
#endif // MAINWINDOW_H
