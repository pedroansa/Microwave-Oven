#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    //ui->setupUi(this);
    QPushButton *powerbtn = new QPushButton("Power");
    QPushButton *clockbtn = new QPushButton("Clock");
    QPushButton *modebtn = new QPushButton("Mode");
    QPushButton *defrostbtn = new QPushButton("Defrost");
    QPushButton *stopbtn = new QPushButton("Stop");
    QPushButton *startbtn = new QPushButton("Start");

    QVBoxLayout *layout = new QVBoxLayout(this);

    dial = new QDial();
    dial->setRange(0,600);
    dial->setNotchesVisible(true);
    dial->setPageStep(20);
    QObject::connect(dial,SIGNAL(valueChanged(int)),this,SLOT(changeValue(int)));

    screen = new panel();
    mac = new QStateMachine();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, screen, &panel::showTime);


    //Configue States
    dft = new QState;
    stop = new QState;
    mac->addState(dft);
    mac->addState(stop);
    idle = new QState(dft);
    power = new QState(dft);
    clockh = new QState(dft);
    clockm = new QState(dft);
    mode = new QState(dft);
    defrost = new QState(dft);
    timeslc = new QState(dft);
    cooking = new QState(dft);

    idle->addTransition(clockbtn, SIGNAL(clicked()), clockh);
    idle->addTransition(powerbtn, SIGNAL(clicked()), power);
    idle->addTransition(modebtn, SIGNAL(clicked()), mode);
    idle->addTransition(defrostbtn, SIGNAL(clicked()), defrost);

    clockh->addTransition(clockbtn, SIGNAL(clicked()), clockm);
    //clockm->addTransition(clockbtn, SIGNAL(clicked()), idle);
    addTrans(clockm, idle, clockbtn, SIGNAL(clicked()), this, SLOT(clockComplete()));


    power->addTransition(powerbtn, SIGNAL(clicked()), timeslc);
    mode->addTransition(modebtn, SIGNAL(clicked()), timeslc);
    timeslc->addTransition(startbtn, SIGNAL(clicked()), cooking);
    defrost->addTransition(startbtn, SIGNAL(clicked()), cooking);
    
    cooktimer = new QTimer(this);
    connect(cooktimer, &QTimer::timeout, this, &MainWindow::changeState);

    QObject::connect (idle, SIGNAL(entered()), this, SLOT(changeState()));
    QObject::connect (power, SIGNAL(entered()), this, SLOT(changeState()));
    QObject::connect (clockh, SIGNAL(entered()), this, SLOT(changeState()));
    QObject::connect (clockm, SIGNAL(entered()), this, SLOT(changeState()));
    QObject::connect (mode, SIGNAL(entered()), this, SLOT(changeState()));
    QObject::connect (defrost, SIGNAL(entered()), this, SLOT(changeState()));
    QObject::connect (timeslc, SIGNAL(entered()), this, SLOT(changeState()));
    QObject::connect (cooking, SIGNAL(entered()), this, SLOT(changeState()));

    dft->setInitialState(idle);

    // stop state will go back to idle after seconds
    dft->addTransition(stopbtn, SIGNAL(clicked()), stop);
    dft->addTransition(this, SIGNAL(finishCooking()), stop);
    QObject::connect (stop, SIGNAL(entered()), this, SLOT(changeState()));
    stoptimer = new QTimer();
    stop->addTransition(stoptimer, SIGNAL(timeout( )), dft);

    connect(mac, SIGNAL(finished()), QApplication::instance(), SLOT(quit()));
    mac->setInitialState(dft);

    // Create Horizontal layouts
    QWidget *line1 = new QWidget;
    QHBoxLayout *layout1 = new QHBoxLayout(line1);

    QWidget *line2 = new QWidget;
    QHBoxLayout *layout2 = new QHBoxLayout(line2);

    QWidget *line3 = new QWidget;
    QHBoxLayout *layout3 = new QHBoxLayout(line3);

    QWidget *line4 = new QWidget;
    QHBoxLayout *layout4 = new QHBoxLayout(line4);

    // Add them to the main layout
    layout1->addWidget(clockbtn);
    layout1->addWidget(powerbtn);
    layout2->addWidget(modebtn);
    layout2->addWidget(defrostbtn);
    layout3->addWidget(dial);
    layout4->addWidget(stopbtn);
    layout4->addWidget(startbtn);

    layout->addWidget(screen);
    layout->addWidget(line1);
    layout->addWidget(line2);
    layout->addWidget(line3);
    layout->addWidget(line4);

    mac->start();


}

void MainWindow::changeState(){
    timer->stop();
    stoptimer->stop();
    if(mac->configuration().contains(idle)){
        this->screen->showTime();
        timer->start(100);
    }

    if(mac->configuration().contains(clockh)){
        this->screen->setClock();
    }

    else if(mac->configuration().contains(clockm)){
        this->screen->setClock();
    }

    else if(mac->configuration().contains(power)){
        this->screen->showPower(val);
    }
    else if(mac->configuration().contains(timeslc)){
        this->screen->showCron(val);
    }
    else if(mac->configuration().contains(mode)){
        this->screen->showMode(val);
    }
    else if(mac->configuration().contains(defrost)){
        this->screen->showPower(val);
        val = (int) val * 1.2;
    }

    else if(mac->configuration().contains(cooking)){
        val--;
        if(val < 0){
            emit finishCooking();
            return;
        }
        this->screen->showCron(val);
        cooktimer->start(1000);
    }
    else if(mac->configuration().contains(stop)){
        this->screen->showStop();
        stoptimer->start(1000);
    }
}

void MainWindow::changeValue(int val){
    this->val = val;
    if(mac->configuration().contains(idle)){
        dial->setRange(0,600);
    }

    else if(mac->configuration().contains(clockh)){
        dial->setRange(0,23);
        screen->setHour(val);
    }

    else if(mac->configuration().contains(clockm)){
        dial->setRange(0,59);
        screen->setMin(val);
    }

    else if(mac->configuration().contains(mode)){
        dial->setRange(0,2);
    }

    else if(mac->configuration().contains(defrost)){
        dial->setRange(1,1000);
    }

    else if(mac->configuration().contains(power)){
        dial->setRange(0,100);
    }

    else if(mac->configuration().contains(timeslc)){
        dial->setRange(0,600);
    }


    changeState();
}

void MainWindow::clockComplete(){
    screen->onManual();
    changeState();
}

MainWindow::~MainWindow()
{

}

