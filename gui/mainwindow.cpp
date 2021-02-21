// Qt Libs
#include <QDebug>
#include <QMessageBox>
#include <QCoreApplication>

// C++ STL
#include <fstream>

// others
#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , trayIcon(new QSystemTrayIcon(this)),
    timer()
{
    // Tray icon menu
    auto menu = this->createMenu();
    this->trayIcon->setContextMenu(menu);
    
    // App icon
    auto appIcon = QIcon(":/res/icon.png");
    this->trayIcon->setIcon(appIcon);
    this->setWindowIcon(appIcon);
    
    // Displaying the tray icon
    this->trayIcon->show();
    
    // Interaction
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    
}

void test()
{
    std::ofstream output("test.txt", std::ios::app);
    output << "output test string\n";
    output.close();
}

QMenu* MainWindow::createMenu()
{
    // App can exit via Quit menu
    auto quitAction = new QAction("&Quit", this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    
    // add menu
    auto menu = new QMenu(this);
    menu->addAction(quitAction);
    
    menu->addAction("test", test);
    
    return menu;
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason_)
{
    switch (reason_) {
    case QSystemTrayIcon::Trigger:
        this->trayIcon->showMessage("Hello", "You clicked me!");
        break;
    default:
        ;
    }
}
