#pragma once

// Qt Libs
#include <QWidget>
#include <QString>
#include <QProcess>

// other
#include "ConfigManager.h"
#include "GlobTools.h"


class Timer : public QWidget
{
    Q_OBJECT
    
public:
    Timer(QWidget *parent = 0);
    
public slots:
    void update_wallpapers();
    
private:
    int counter = 0;
};
