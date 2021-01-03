#include "ScreenStatus.h"


// ====================== ScreenStatus ======================

// default constructor
ScreenStatus::ScreenStatus()
    : m_ResHorizontal(0), m_ResVertical(0),
    m_CurrDisplayMode(DisplayMode::Unknow),
    m_Continue_Updating(true)
{}

// destructor
ScreenStatus::~ScreenStatus()
{
    m_ResVertical = 0;
    m_ResVertical = 0;
    m_CurrDisplayMode = DisplayMode::Unknow;
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

// monitoring screen status
void ScreenStatus::startMonitor()
{
    if (m_Continue_Updating != true) 
        m_Continue_Updating = true;
    std::thread t(&ScreenStatus::thread_monitoring_func, this);
    t.join();
}
void ScreenStatus::stopMonitor()
{
    m_Continue_Updating = false;
}


// private functions

void ScreenStatus::getScreenRes()
{
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    m_ResHorizontal = desktop.right;
    m_ResVertical = desktop.bottom;
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
void ScreenStatus::thread_monitoring_func()
{
    while (m_Continue_Updating) {
        std::lock_guard<std::mutex> lck(m_Mtx);
        getScreenRes();
        determineCurrDisplayMode();
    }
}

// ====================== ScreenStatus end ======================