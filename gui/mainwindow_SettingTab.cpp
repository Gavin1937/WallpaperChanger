
// Setting Tab UI implementation of class MainWindow

#include "mainwindow.h"


// ====================== mainwindow_SettingTab ======================

// private:
void MainWindow::init_SettingTab()
{
    // setup "Setting" tab
    mainwindowTab->setTabText(1, "Setting");
    // universal buttons (OK, Cancel, Apply)
    connect(tab1OK, &QPushButton::clicked, this, &MainWindow::onOKPressed);
    connect(tab1Cancel, &QPushButton::clicked, this, &MainWindow::onCancelPressed);
    connect(tab1Apply, &QPushButton::clicked, this, &MainWindow::onApplyPressed);
}


// ====================== mainwindow_SettingTab End ======================
