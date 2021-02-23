#pragma once

// Qt Libs
#include <QWidget>
#include <QString>
#include <QProcess>
#include <QTimer>

// other
#include "ConfigManager.h"
#include "GlobTools.h"


class WallpaperUpdater : public QWidget
{
    Q_OBJECT
    
public:
    WallpaperUpdater(QWidget *parent = 0);
    
public slots:
    void update_wallpapers();
    
private:
    QTimer* m_Timer;
};
