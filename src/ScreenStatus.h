#pragma once


enum class DisplayMode {
    Unknow = 0, 
    Portrait, Landscape
};

// record & monitor screen status (resolution)
class ScreenStatus
{
public:
    // default constructor
    ScreenStatus();
    // disable copy constructor
    ScreenStatus(const ScreenStatus& obj) = delete;
    
    // destructor
    ~ScreenStatus();
    
    // getters interfaces
    unsigned int getResHorizontal() const;
    unsigned int getResHorizontal();
    unsigned int getResVertical() const;
    unsigned int getResVertical();
    DisplayMode getCurrDisplayMode() const;
    DisplayMode getCurrDisplayMode();
    
    // monitoring screen status
    void startMonitor();
    void stopMonitor();
    
private:
    void getScreenRes();
    void determineCurrDisplayMode();
private:
    unsigned int m_ResHorizontal;
    unsigned int m_ResVertical;
    DisplayMode m_CurrDisplayMode;
    bool continue_updating;
};