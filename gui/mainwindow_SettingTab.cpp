
// Setting Tab UI implementation of class MainWindow

#include "mainwindow.h"


// ====================== mainwindow_SettingTab ======================

// public slots: Setting Tab slots
void MainWindow::onTab1_TextEditChanged()
{
    // get int val in edit
    int edit_val = WUI_Edit->text().toInt();
    // convert val to seconds & store it
    time_converter(edit_val, DropDownState::Seconds, m_Curr_DropDownState);
    m_WallpaperUpdateInterval = edit_val;
    m_ControlChanged = true;
}
void MainWindow::onTab1_DropDownChanged()
{
    // update DropDownState
    m_Last_DropDownState = m_Curr_DropDownState;
    int converted_num = WUI_Edit->text().toInt();
    switch(WUI_DropDown->currentIndex())
    {
    case 0: // Seconds
        m_Curr_DropDownState = DropDownState::Seconds;
        time_converter(converted_num, m_Curr_DropDownState, m_Last_DropDownState); 
        WUI_Edit->setText(QString::number(converted_num));
        break;
    case 1: // Minutes
        m_Curr_DropDownState = DropDownState::Minutes;
        time_converter(converted_num, m_Curr_DropDownState, m_Last_DropDownState); 
        WUI_Edit->setText(QString::number(converted_num));
        break;
    case 2: // Hours
        m_Curr_DropDownState = DropDownState::Hours;
        time_converter(converted_num, m_Curr_DropDownState, m_Last_DropDownState); 
        WUI_Edit->setText(QString::number(converted_num));
        break;
    case 3: // Days
        m_Curr_DropDownState = DropDownState::Days;
        time_converter(converted_num, m_Curr_DropDownState, m_Last_DropDownState); 
        WUI_Edit->setText(QString::number(converted_num));
        break;
    case 4: // Weeks
        m_Curr_DropDownState = DropDownState::Weeks;
        time_converter(converted_num, m_Curr_DropDownState, m_Last_DropDownState); 
        WUI_Edit->setText(QString::number(converted_num));
        break;
    }
}
void MainWindow::onTab1_ChkBoxStatusChanged()
{
    // toggle ProgramCloseMode
    if (m_ProgramCloseMode == ProgramCloseMode::HIDE)
        m_ProgramCloseMode = ProgramCloseMode::EXIT;
    else
        m_ProgramCloseMode = ProgramCloseMode::HIDE;
    m_ControlChanged = true;
}


// private:
void MainWindow::init_SettingTab()
{
    // setup "Setting" tab
    mainwindowTab->setTabText(1, "Setting");
    // initialize m_ProgramCloseMode base on config file
    // And set tab1ChkBox
    if (m_Config.getBool(L"program", L"hide_when_closed") == true) {
        m_ProgramCloseMode = ProgramCloseMode::HIDE;
        tab1ChkBox->setCheckState(Qt::Checked);
    }
    else {
        m_ProgramCloseMode = ProgramCloseMode::EXIT;
        tab1ChkBox->setCheckState(Qt::Unchecked);
    }
    // initialize m_WallpaperUpdateInterval base on config file
    m_WallpaperUpdateInterval = m_Config.getInt(L"program", L"wallpaper_update_time");
    // And set WUI_Edit & WUI_DropDown
    WUI_Edit->setText(QString::number(m_WallpaperUpdateInterval));
    WUI_DropDown->setCurrentIndex(0);
    
    // connect controls to functions
    connect(WUI_Edit, &QLineEdit::textEdited, this, &MainWindow::onTab1_TextEditChanged);
    connect(WUI_DropDown, &QComboBox::currentIndexChanged, this, &MainWindow::onTab1_DropDownChanged);
    connect(tab1ChkBox, &QCheckBox::toggled, this, &MainWindow::onTab1_ChkBoxStatusChanged);
    // universal buttons (OK, Cancel, Apply)
    connect(tab1OK, &QPushButton::clicked, this, &MainWindow::onOKPressed);
    connect(tab1Cancel, &QPushButton::clicked, this, &MainWindow::onCancelPressed);
    connect(tab1Apply, &QPushButton::clicked, this, &MainWindow::onApplyPressed);
}
void MainWindow::save_SettingTab()
{
    if (m_ControlChanged) {
        // save ProgramCloseMode
        if (m_ProgramCloseMode == ProgramCloseMode::HIDE)
            m_Config.set(L"program", L"hide_when_closed", L"true");
        else
            m_Config.set(L"program", L"hide_when_closed", L"false");
        m_Config.set(L"program", L"wallpaper_update_time", std::to_wstring(m_WallpaperUpdateInterval));
    }
}

// convert m_WallpaperUpdateInterval between different DropDownState (sec, min, hour, day, week)
void MainWindow::time_converter(int& time, const DropDownState& curr_state, const DropDownState& last_state)
{
    int converter_list[4] = {
        60, // sec <=> min
        60, // min <=> hour
        24, // hour <=> day
        7 // day <=> week
    };
    int last_ind = static_cast<int>(last_state);
    int curr_ind = static_cast<int>(curr_state);
    if (last_state < curr_state) { // division
        for (int i = last_ind; i < curr_ind; ++i)
            time /= converter_list[i];
    } else if (last_state > curr_state) { // multiplication
        for (int i = last_ind-1; i >= curr_ind; --i)
            time *= converter_list[i];
    } else { // last_state == curr_state, do nothing
        return;
    }
}

// ====================== mainwindow_SettingTab End ======================
