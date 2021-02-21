#pragma once

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>

#include <QtPlugin>
//#include <QTimer>

#include "timer.h"


class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  
//  ~MainWindow();
  
public slots:
  void iconActivated(QSystemTrayIcon::ActivationReason);
  //void test();

private:
  QSystemTrayIcon* trayIcon;
  QMenu* trayIconMenu;
  //QTimer* timer;
  Timer timer;

  QMenu* createMenu();
};
