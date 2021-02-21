#ifndef TIMER_H
#define TIMER_H

#include <QWidget>

#include <fstream>


class Timer : public QWidget
{
    Q_OBJECT

public:
    Timer(QWidget *parent = 0);

public slots:
    void updateProgress();

private:
    int counter = 0;
};

#endif
