
// Setting Tab UI implementation of class MainWindow

#include "mainwindow.h"


// ====================== mainwindow_SettingTab ======================

// public slots: Setting Tab slots
void MainWindow::onTab1_TextEditChanged()
{
    // get int val in edit
    bool ok = true;
    int edit_val = WUI_Edit->text().toInt(&ok);
    if (!ok) { // User did not enter int
        MessageBoxW(0, L"Invalid Input, Please Enter Integers", L"Warning", 0);
        WUI_Edit->setText(QString::number(m_WallpaperUpdateInterval));
        return;
    }
    // convert val to seconds & update m_WallpaperUpdateInterval 
    time_converter(edit_val, DropDownState::Seconds, m_DropDownState);
    m_WallpaperUpdateInterval = edit_val;
    m_ControlChanged = true;
}
void MainWindow::onTab1_DropDownChanged()
{
    // update DropDownState
    int converted_num = m_WallpaperUpdateInterval;
    switch(WUI_DropDown->currentIndex())
    {
    case 0: // Seconds
        m_DropDownState = DropDownState::Seconds;
        time_converter(converted_num, m_DropDownState, DropDownState::Seconds); 
        WUI_Edit->setText(QString::number(converted_num));
        break;
    case 1: // Minutes
        m_DropDownState = DropDownState::Minutes;
        time_converter(converted_num, m_DropDownState, DropDownState::Seconds); 
        WUI_Edit->setText(QString::number(converted_num));
        break;
    case 2: // Hours
        m_DropDownState = DropDownState::Hours;
        time_converter(converted_num, m_DropDownState, DropDownState::Seconds); 
        WUI_Edit->setText(QString::number(converted_num));
        break;
    case 3: // Days
        m_DropDownState = DropDownState::Days;
        time_converter(converted_num, m_DropDownState, DropDownState::Seconds); 
        WUI_Edit->setText(QString::number(converted_num));
        break;
    case 4: // Weeks
        m_DropDownState = DropDownState::Weeks;
        time_converter(converted_num, m_DropDownState, DropDownState::Seconds); 
        WUI_Edit->setText(QString::number(converted_num));
        break;
    }
}
void MainWindow::onTab1_ChkBoxStatusChanged_HideWhenClosed()
{
    m_ControlChanged = true;
}
void MainWindow::onTab1_ChkBoxStatusChanged_LaunchAtStartup()
{
    // LaunchAtStartup is Enable
    if (tab1ChkBox_LaunchAtStartup->isChecked()) {
        m_Config.set(L"system", L"launch_at_startup", L"true");
        setupAppStartupW(
            true,
            m_Config.get(L"program", L"gui_program"),
            m_Config.get(L"system", L"windows_startup_dir")
        );
    }
    // LaunchAtStartup is Disable
    else {
        m_Config.set(L"system", L"launch_at_startup", L"false");
        setupAppStartupW(
            false,
            m_Config.get(L"program", L"gui_program"),
            m_Config.get(L"system", L"windows_startup_dir")
        );
    }
    m_ControlChanged = true;
}
void MainWindow::onTab1_ChkBoxStatusChanged_RestartAfterCrash()
{
    // RestartAfterCrash Enable
    if (tab1ChkBox_RestartAfterCrash->isChecked()) {
        ::pub_WhetherRestartAfterCrash = true;
        m_Config.set(L"system", L"restart_after_crash", L"true");
        setupAppRestartRecover(RecoveryFunc, nullptr);
    }
    // RestartAfterCrash Disable
    else {
        ::pub_WhetherRestartAfterCrash = false;
        m_Config.set(L"system", L"restart_after_crash", L"false");
    }
    m_ControlChanged = true;
}



// private:
void MainWindow::init_SettingTab()
{
    SettingTab_resetCtrls();
    SettingTab_makeConnections();
}
void MainWindow::SettingTab_resetCtrls()
{
    // keep m_ControlChanged not be modify by setText()
    bool stat_buff = m_ControlChanged;
    
    // setup "Setting" tab
    mainwindowTab->setTabText(1, "Setting");
    
    // initialize m_ProgramCloseMode base on config file
    // And set tab1ChkBox_HideWhenClosed
    if (m_Config.getBool(L"program", L"hide_when_closed") == true) {
        tab1ChkBox_HideWhenClosed->setCheckState(Qt::Checked);
        m_ProgramCloseMode = ProgramCloseMode::HIDE;
    } else {
        tab1ChkBox_HideWhenClosed->setCheckState(Qt::Unchecked);
        m_ProgramCloseMode = ProgramCloseMode::EXIT;
    }
    
    // initialize RestartAfterCrash base on config file
    if (m_Config.getBool(L"system", L"restart_after_crash") == true)
        tab1ChkBox_RestartAfterCrash->setCheckState(Qt::Checked);
    else
        tab1ChkBox_RestartAfterCrash->setCheckState(Qt::Unchecked);
    {
        // RestartAfterCrash Enable
        if (tab1ChkBox_RestartAfterCrash->isChecked()) {
            ::pub_WhetherRestartAfterCrash = true;
            m_Config.set(L"system", L"restart_after_crash", L"true");
            setupAppRestartRecover(RecoveryFunc, nullptr);
        }
        // RestartAfterCrash Disable
        else {
            ::pub_WhetherRestartAfterCrash = false;
            m_Config.set(L"system", L"restart_after_crash", L"false");
        }
    }
    
    // initialize LaunchAtStartup base on config file
    if (m_Config.getBool(L"system", L"launch_at_startup") == true)
        tab1ChkBox_LaunchAtStartup->setCheckState(Qt::Checked);
    else
        tab1ChkBox_LaunchAtStartup->setCheckState(Qt::Unchecked);
    {
        // LaunchAtStartup is Enable
        if (tab1ChkBox_LaunchAtStartup->isChecked()) {
            m_Config.set(L"system", L"launch_at_startup", L"true");
            if (!GlobTools::is_filedir_existW(
                (m_Config.get(L"system", L"windows_startup_dir")+L"WallpaperChanger.lnk").c_str()))
            {
                setupAppStartupW(
                    true,
                    m_Config.get(L"program", L"gui_program"),
                    m_Config.get(L"system", L"windows_startup_dir")
                );
            }
        }
        // LaunchAtStartup is Disable
        else {
            m_Config.set(L"system", L"launch_at_startup", L"false");
                setupAppStartupW(
                    false,
                    m_Config.get(L"program", L"gui_program"),
                    m_Config.get(L"system", L"windows_startup_dir")
                );
        }
    }
    
    // initialize m_WallpaperUpdateInterval base on config file
    m_WallpaperUpdateInterval = m_Config.getInt(L"program", L"wallpaper_update_time");
    
    // And set WUI_Edit & WUI_DropDown
    WUI_Edit->setText(QString::number(m_WallpaperUpdateInterval));
    WUI_DropDown->setCurrentIndex(0);
    
    m_ControlChanged = stat_buff;
}
void MainWindow::SettingTab_makeConnections()
{
    // connect controls to functions
    connect(WUI_Edit, &QLineEdit::textEdited, this, &MainWindow::onTab1_TextEditChanged);
    connect(WUI_DropDown, &QComboBox::currentIndexChanged, this, &MainWindow::onTab1_DropDownChanged);
    connect(tab1ChkBox_HideWhenClosed, &QCheckBox::stateChanged, this, &MainWindow::onTab1_ChkBoxStatusChanged_HideWhenClosed);
    connect(tab1ChkBox_LaunchAtStartup, &QCheckBox::stateChanged, this, &MainWindow::onTab1_ChkBoxStatusChanged_LaunchAtStartup);
    connect(tab1ChkBox_RestartAfterCrash, &QCheckBox::stateChanged, this, &MainWindow::onTab1_ChkBoxStatusChanged_RestartAfterCrash);
    connect(tab1Bnt_LaunchCacheBrowser, &QPushButton::clicked, this, &MainWindow::onBrowseCacheBntPressed);
    connect(Bnt_AboutProgram, &QPushButton::clicked, this, &MainWindow::AboutProgramDlg);
    connect(tab1Bnt_ClearCachedWallpapers, &QPushButton::clicked, this, &MainWindow::onClearCachedWallpapers);
    // universal buttons (OK, Cancel, Apply)
    connect(tab1OK, &QPushButton::clicked, this, &MainWindow::onOKPressed);
    connect(tab1Cancel, &QPushButton::clicked, this, &MainWindow::onCancelPressed);
    connect(tab1Apply, &QPushButton::clicked, this, &MainWindow::onApplyPressed);
}
void MainWindow::save_SettingTab()
{
    if (m_ControlChanged) {
        // save ProgramCloseMode
        if (tab1ChkBox_HideWhenClosed->isChecked()) {
            m_ProgramCloseMode = ProgramCloseMode::HIDE;
            m_Config.set(L"program", L"hide_when_closed", L"true");
        } else {
            m_ProgramCloseMode = ProgramCloseMode::EXIT;
            m_Config.set(L"program", L"hide_when_closed", L"false");
        }
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
        7   // day <=> week
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
