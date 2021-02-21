#pragma once

// Qt Libs
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QtPlugin>

// others
#include "timer.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    
public slots:
    void iconActivated(QSystemTrayIcon::ActivationReason);
    
private:
    QMenu* createMenu();
    
private:
    QSystemTrayIcon* trayIcon;
    QMenu* trayIconMenu;
    Timer timer;
    
};
