#ifndef PANEL_H
#define PANEL_H
#include <QLCDNumber>
#include <QTimer>
#include <QTime>

class panel : public QLCDNumber
{
    Q_OBJECT
public:
    panel(QWidget *parent = nullptr);
    void setMin(int min);
    void setHour(int h);
    void setTime();

public slots:
    void updateTime();
    void showTime();
    void setClock();
    void showPower(int pow);
    void showMode(int mode);
    void showCron(int sec);
    void showStop();
    void onManual();

private:
    QTime time;
    QTimer *timer;
    int hour, min;
    bool manual = false;
};

#endif // PANEL_H
