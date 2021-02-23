
// others
#include "timer.h"

WallpaperUpdater::WallpaperUpdater(QWidget *parent)
    : QWidget(parent)
{
    // set up QTimer obj
    m_Timer = new QTimer(this);
    connect(m_Timer, &QTimer::timeout, this, &WallpaperUpdater::update_wallpapers);
    // get sec from config.ini
    ConfigManager loc_config(GlobTools::getCurrExePathW()+L"config.ini", false);
    m_Timer->start(loc_config.getInt(L"program", L"wallpaper_update_time")*1000);
}

void WallpaperUpdater::update_wallpapers()
{
    ConfigManager loc_config(GlobTools::getCurrExePathW()+L"config.ini", false);
	QObject* parent1 = new QObject();
	QString program1 = QString::fromWCharArray(loc_config.get(L"program", L"core_program").c_str());
	QStringList arguments1;
	arguments1
		// update landscape wallpaper
		<< QString::fromWCharArray(L"--paste")
		<< QString::fromWCharArray(loc_config.get(L"wallpaper", L"landscape_wallpaper").c_str())
		<< QString::fromWCharArray(L"LANDSCAPE")
		;
	QProcess* myProcess1 = new QProcess(parent1);
	myProcess1->start(program1, arguments1);
	
	QObject* parent2 = new QObject();
	QString program2 = QString::fromWCharArray(loc_config.get(L"program", L"core_program").c_str());
	QStringList arguments2;
	arguments2
		// update portrait wallpaper
		<< QString::fromWCharArray(L"--paste")
		<< QString::fromWCharArray(loc_config.get(L"wallpaper", L"portrait_wallpaper").c_str())
		<< QString::fromWCharArray(L"PORTRAIT")
		;
	QProcess* myProcess2 = new QProcess(parent2);
	myProcess2->start(program2, arguments2);
}
