#include "ConfigManager.h"
#include "GlobTools.h"

#include <Windows.h>

namespace 
{
    std::unordered_map<std::wstring, ConfigSections> glob_wstr_ConfigSection_list = {
        {L"Empty"             , ConfigSections::Empty},
        {L"ExeRootDir"        , ConfigSections::ExeRootDir},
        {L"ConfigDir"         , ConfigSections::ConfigDir},
        {L"WallpaperCacheDir" , ConfigSections::WallpaperCacheDir},
        {L"WallpaperListDir"  , ConfigSections::WallpaperListDir},
        {L"WindowsThemeDir"   , ConfigSections::WindowsThemeDir},
        {L"SS_WallpaperID_P"  , ConfigSections::SS_WallpaperID_P},
        {L"SS_WallpaperID_L"  , ConfigSections::SS_WallpaperID_L},
        {L"MS_WallpaperID"    , ConfigSections::MS_WallpaperID}
    };
    
    std::vector<ConfigItem> glob_config_file_template = {
        ConfigItem(L"ExeRootDir="),
        ConfigItem(L"ConfigDir="),
        ConfigItem(L"WallpaperCacheDir="),
        ConfigItem(L"WallpaperListDir="),
        ConfigItem(L"WindowsThemeDir="),
        ConfigItem(L"SS_WallpaperID_P="),
        ConfigItem(L"SS_WallpaperID_L="),
        ConfigItem(L"MS_WallpaperID=")
    };
    
    // get name of ConfigItem (ConfigItemName=Value)
    std::wstring get_ConfigItem_name(const std::wstring& str)
    {
        return std::wstring(str.begin(), str.begin()+str.find(L"="));
    }
    
    std::wstring get_ConfigItem_name_by_ConfigSection(const ConfigSections& cs)
    {
        auto cur_ConfigItem_name = std::find_if(
                            glob_wstr_ConfigSection_list.begin(),
                            glob_wstr_ConfigSection_list.end(),
                            [&cs](auto p) {return p.second == cs;}
        );
        return cur_ConfigItem_name->first;
    }
};



// ====================== ConfigManager ======================

// public functions

// default constructor
ConfigManager::ConfigManager()
    : m_ConfigFile_path(L""), m_CachedConfigFile(std::vector<ConfigItem>())
{
    // create a config file current dir does not have one
    std::wstring temp_exe_path = GlobTools::getCurrExePathW();
    m_ConfigFile_path = temp_exe_path + L"config.ini";
    if (!m_CachedConfigFile.empty()) m_CachedConfigFile.clear();
    write_buff2config();
    if (!read_config2buff())
        ConfigManager::~ConfigManager();
    // get Windows Theme folder dir
    DWORD size = 32767;
    TCHAR username[32767];
    GetUserNameW(username, &size);
    std::wstring temp_WindowsTheme_path =
        L"C:\\User\\" + std::wstring(username, size-1) +
        L"\\AppData\\Roaming\\Microsoft\\Windows\\Themes\\";
    // write basic info to config.ini
    modify_config(ConfigSections::ExeRootDir, GlobTools::getCurrExePathW());
    modify_config(ConfigSections::ConfigDir, m_ConfigFile_path);
    modify_config(ConfigSections::WallpaperCacheDir, temp_exe_path+L"Wallpapers\\");
    modify_config(ConfigSections::WallpaperListDir, temp_exe_path+L"Wallpapers\\WallpaperList");
    modify_config(ConfigSections::WindowsThemeDir, temp_WindowsTheme_path);
}
// parametric constructor
ConfigManager::ConfigManager(const std::wstring& ConfigFile_path)
    : m_ConfigFile_path(ConfigFile_path), m_CachedConfigFile(std::vector<ConfigItem>())
{
    // create a config file current dir if Config_path does not exist
    std::wstring temp_exe_path = GlobTools::getCurrExePathW();
    if (!GlobTools::is_filedir_existW(ConfigFile_path)) {
        m_ConfigFile_path = GlobTools::getCurrExePathW() + L"config.ini";
        write_buff2config();
    }
    if (!m_CachedConfigFile.empty()) m_CachedConfigFile.clear();
    if (!read_config2buff())
        ConfigManager::~ConfigManager();
    // get Windows Theme folder dir
    DWORD size = 32767;
    TCHAR username[32767];
    GetUserNameW(username, &size);
    std::wstring temp_WindowsTheme_path =
        L"C:\\User\\" + std::wstring(username, size-1) +
        L"\\AppData\\Roaming\\Microsoft\\Windows\\Themes\\";
    // write basic info to config.ini
    modify_config(ConfigSections::ExeRootDir, GlobTools::getCurrExePathW());
    modify_config(ConfigSections::ConfigDir, m_ConfigFile_path);
    modify_config(ConfigSections::WallpaperCacheDir, temp_exe_path+L"Wallpapers\\");
    modify_config(ConfigSections::WallpaperListDir, temp_exe_path+L"Wallpapers\\WallpaperList");
    modify_config(ConfigSections::WindowsThemeDir, temp_WindowsTheme_path);
}

// destructor
ConfigManager::~ConfigManager()
{
    write_buff2config();
    m_ConfigFile_path.~basic_string();
    m_CachedConfigFile.~vector();
}


// write to config file, overwrite file with m_CachedConfigFile
// should call this function in destructor as well
bool ConfigManager::write_buff2config()
{
    std::wofstream output_config;
    output_config.imbue(std::locale());
    output_config.open(m_ConfigFile_path);
    if (output_config.fail()) {
        output_config.close();
        return false;
    }
    try {
        if (!m_CachedConfigFile.empty()) {
            for (auto it : m_CachedConfigFile)
                output_config << it.output_wstr() << L"\n";
        } else {
            for (auto it : glob_config_file_template)
                output_config << it.output_wstr() << L"\n";
        }
        output_config.close();
    } catch(...) { return false; }
    return true;
}


// modify a specific section of config.ini
bool ConfigManager::modify_config(
    ConfigSections section,
    const std::wstring& modify_val)
{
    auto modify_section = find_ConfigItem_by_Section(section);
    if (modify_section == m_CachedConfigFile.end()) // cannot find section
        return false;
    else // find section
        modify_section->m_Data = modify_val;
    return true;
}



// private functions

// read config file from m_ConfigFile_path to ram &
// cache to m_CachedConfigFile, call it in constructor
bool ConfigManager::read_config2buff()
{
    std::wifstream input_config;
    input_config.imbue(std::locale());
    input_config.open(m_ConfigFile_path);
    if (input_config.fail()) return false;
    std::wstring buff;
    try {
        while (std::getline(input_config, buff))
            m_CachedConfigFile.push_back(ConfigItem(buff));
        clear_empty_bad_ConfigItem();
    } catch(...) { return false; }
    input_config.close();
    return true;
}


// clear all empty & bad ConfigItem,
// this function should called inside of read_config_2_vec();
void ConfigManager::clear_empty_bad_ConfigItem()
{
    for (int i = static_cast<int>(m_CachedConfigFile.size()-1);
        i >= 0; --i) {
        bool i_is_bad_or_empty =
            m_CachedConfigFile[i].m_DataSection == ConfigSections::Empty ||
            m_CachedConfigFile[i].m_DataSection == ConfigSections::Bad;
        if (i_is_bad_or_empty)
            m_CachedConfigFile.erase(m_CachedConfigFile.begin()+i);
    }
}

// search specific ConfigItem in m_CachedConfigFile
// /w a ConfigSection param
std::vector<ConfigItem>::iterator ConfigManager::find_ConfigItem_by_Section(
    const ConfigSections& cs)
{
    auto it = m_CachedConfigFile.begin();
    while (it != m_CachedConfigFile.end()) {
        if (it->m_DataSection == cs)
            return it;
        ++it;
    }
    return it;
}


// ====================== ConfigManager end ======================




// ====================== ConfigItem ======================

// constructor
ConfigItem::ConfigItem()
    : m_DataSection(ConfigSections::Empty), m_Data(L"")
{}
// constructor /w raw wstring
ConfigItem::ConfigItem(const std::wstring& str)
    : m_DataSection(ConfigSections::Empty), m_Data(L"")
{
    size_t equal_sign_pos = str.find(L"=");
    std::wstring config_tag = std::wstring(str.begin(), str.begin()+equal_sign_pos);
    if (config_tag.empty()) m_DataSection = ConfigSections::Empty;
    else {
        try {
            m_DataSection = glob_wstr_ConfigSection_list.at(config_tag);
        } catch(...) {m_DataSection = ConfigSections::Bad;}
    }
    m_Data = std::wstring(str.begin()+equal_sign_pos+1, str.end());
}
// copy constructor
ConfigItem::ConfigItem(const ConfigItem& obj)
    : m_DataSection(obj.m_DataSection), m_Data(obj.m_Data)
{}

// destructor
ConfigItem::~ConfigItem()
{
    m_Data.~basic_string();
}

std::wstring ConfigItem::output_wstr()
{
    return get_ConfigItem_name_by_ConfigSection(m_DataSection) +
            L"=" + m_Data;
}

// ====================== ConfigItem end ======================
