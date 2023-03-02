#include "panel.h"

panel::panel(QWidget *parent) : QLCDNumber(parent)
{
    time = QTime::currentTime();
    hour = time.hour();
    min = time.minute();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &panel::updateTime);
    timer->start(60000);

    setSegmentStyle(Filled);

    showTime();

    setWindowTitle(tr("Digital Clock"));

    resize(150,60);

}

void panel::setHour(int h){
    hour = h;
}

void panel::setMin(int m){
    min = m;
}

void panel::setTime(){
    time = QTime(hour, min);
}

void panel::updateTime(){
    min++;
    if(min > 59){
        min = 0;
        hour++;
    }

    if(hour > 23){
        hour = 0;
    }
    setTime();
}


void panel::showTime()
{
    QString text = time.toString("hh:mm");
    if ((QTime::currentTime().second() % 2) == 0)
        text[2] = ' ';
    display(text);
}


//show setclock
void panel::setClock(){
    manual = true;
    //setTime();
    QString text = QTime(hour, min).toString("hh:mm");
    display(text);
    manual = false;
}

void panel::showPower(int pow)
{
    QString text = QString::number(pow) + '%';
    display(text);
}

void panel::showCron(int sec)
{
    QString text;

    if(sec > 60){
        text = QString::number((int) sec / 60) + ":" + QString::number(sec % 60);
    }
    else{
        text = "00:" + QString::number(sec);
    }
    display(text);
}


void panel::showMode(int pos)
{
    QString text;
    if(pos == 0){
        text = "A";
    }

    else if(pos == 1){
        text = "B";
    }

    else{
        text = "C";
    }
    display(text);
}

void panel::showStop(){
    display("00");
}

void panel::onManual(){
    setTime();
    manual = true;
    timer->start(60000);
}

