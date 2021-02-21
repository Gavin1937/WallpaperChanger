#include "timer.h"
#include <QTimer>

Timer::Timer(QWidget *parent)
    : QWidget(parent)
{

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Timer::updateProgress);
    timer->start(1000);

}

void Timer::updateProgress()
{
    std::ofstream output("test.txt", std::ios::app);
    output << "output test string. c=" << counter++ << "\n";
    output.close();
}
