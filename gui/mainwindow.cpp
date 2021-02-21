#include <QDebug>
#include <QMessageBox>
#include <QCoreApplication>

#include "mainwindow.h"

#include <fstream>

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
  this->trayIcon->show();     // Note: without explicitly calling show(), QSystemTrayIcon::activated signal will never be emitted!

  // Interaction
  connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
//  connect(trayIcon, SIGNAL(QSystemTrayIcon::activated()),
//		this, SLOT(MainWindow::iconActivated()));
  
  
  //this->timer = new MyTimer();
  
  // handle timer
  //this->timer = new QTimer(this);
  //connect(this->timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::test));
  //timer->start(1000);
  
}

//MainWindow::~MainWindow()
//{
//  this->timer->~MyTimer();
//}

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
//  connect(quitAction, SIGNAL(QAction::triggered()), 
//		qApp, SLOT(QCoreApplication::quit()));

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
