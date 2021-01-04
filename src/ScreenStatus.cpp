#include "ScreenStatus.h"
#include "GlobTools.h"


// ====================== ScreenStatus ======================

// default constructor
ScreenStatus::ScreenStatus()
    : m_ResHorizontal(0), m_ResVertical(0),
    m_CurrDisplayMode(DisplayMode::Unknow),
    m_IsSingleScreen(false),
    m_Continue_Updating(true)
{}

// destructor
ScreenStatus::~ScreenStatus()
{
    m_ResVertical = 0;
    m_ResHorizontal = 0;
    m_CurrDisplayMode = DisplayMode::Unknow;
    m_IsSingleScreen = false;
    m_Continue_Updating = false;
}

// getters interfaces
unsigned int ScreenStatus::getResHorizontal() const
{
    return m_ResHorizontal;
}
unsigned int ScreenStatus::getResHorizontal()
{
    return m_ResHorizontal;
}
unsigned int ScreenStatus::getResVertical() const
{
    return m_ResVertical;
}
unsigned int ScreenStatus::getResVertical()
{
    return m_ResVertical;
}
DisplayMode ScreenStatus::getCurrDisplayMode() const
{
    return m_CurrDisplayMode;
}
DisplayMode ScreenStatus::getCurrDisplayMode()
{
    return m_CurrDisplayMode;
}
bool ScreenStatus::WhetherIsSingleScreen() const
{
    return m_IsSingleScreen;
}
bool ScreenStatus::WhetherIsSingleScreen()
{
    return m_IsSingleScreen;
}


// monitoring screen status
void ScreenStatus::stopMonitoring()
{
    m_Continue_Updating = false;
}
// function use to monitor Screen Status in a separate thread
void ScreenStatus::thread_monitoring_func(const int& time_in_ms)
{
    while (m_Continue_Updating) {
        std::lock_guard<std::mutex> lck(GlobTools::glob_mtx);
        getScreenRes();
        determineCurrDisplayMode();
        if (time_in_ms >= 0)
            Sleep(time_in_ms);
    }
}


// private functions

void ScreenStatus::getScreenRes()
{
    //determine whether the system is multi-monitor system
    //get width & height of the virtual screen (add up all the screen)
    int virX = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int virY = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    //get width & height of the main screen
    int X = GetSystemMetrics(SM_CXSCREEN);
    int Y = GetSystemMetrics(SM_CYSCREEN);
    if ((virX != X) || (virY != Y)) //total resolution != mainscreen resolution => multi-monitor
        m_IsSingleScreen = false;
    else //single monitor
        m_IsSingleScreen = true;
    // set X & Y to class members
    m_ResHorizontal = X;
    m_ResVertical = Y;
}
void ScreenStatus::determineCurrDisplayMode()
{
    if (m_ResHorizontal != m_ResVertical) {
        if (m_ResHorizontal > m_ResVertical)
            m_CurrDisplayMode = DisplayMode::Landscape;
        else // m_ResHorizontal < m_ResVertical
            m_CurrDisplayMode = DisplayMode::Portrait;
    } else { // m_ResHorizontal == m_ResVertical
        if (m_CurrDisplayMode != DisplayMode::Unknow) {
            m_CurrDisplayMode = 
                (m_CurrDisplayMode == DisplayMode::Landscape) ?
                (DisplayMode::Portrait) : (DisplayMode::Landscape);
        } 
    }
}


// ====================== ScreenStatus end ======================