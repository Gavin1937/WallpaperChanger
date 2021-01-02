#include "ConfigManager.h"
#include "GlobTools.h"


namespace 
{
    std::vector<ConfigItem> glob_config_file_template = {
        ConfigItem(L"Config.ini")
    };
    
    std::unordered_map<std::wstring, ConfigSections> glob_wstr_ConfigSection_list = {
        {L"Empty", ConfigSections::Empty},
        {L"ExeRootDir", ConfigSections::ExeRootDir},
        {L"ConfigDir", ConfigSections::ConfigDir},
        {L"WallpaperCacheDir", ConfigSections::WallpaperCacheDir},
        {L"WallpaperListDir", ConfigSections::WallpaperListDir},
        {L"SS_WallpaperID_P", ConfigSections::SS_WallpaperID_P},
        {L"SS_WallpaperID_L", ConfigSections::SS_WallpaperID_L},
        {L"MS_WallpaperID", ConfigSections::MS_WallpaperID}
    };
};



// ====================== ConfigManager ======================

// public functions

// default constructor
ConfigManager::ConfigManager()
    : m_ConfigFile_path(L""), m_CachedConfigFile(std::vector<ConfigItem>())
{
    // create a config file current dir does not have one
    m_ConfigFile_path = GlobTools::getCurrExePathW() + L"config.ini";
    if (!m_CachedConfigFile.empty()) m_CachedConfigFile.clear();
    write_vec_2_config();
    if (!read_config_2_vec())
        ConfigManager::~ConfigManager();
}
// parametric constructor
ConfigManager::ConfigManager(const std::wstring& ConfigFile_path)
    : m_ConfigFile_path(ConfigFile_path), m_CachedConfigFile(std::vector<ConfigItem>())
{
    // create a config file current dir if Config_path does not exist
    if (!GlobTools::is_filedir_existW(ConfigFile_path)) {
        m_ConfigFile_path = GlobTools::getCurrExePathW() + L"config.ini";
        write_vec_2_config();
    }
    if (!m_CachedConfigFile.empty()) m_CachedConfigFile.clear();
    if (!read_config_2_vec())
        ConfigManager::~ConfigManager();
}

// destructor
ConfigManager::~ConfigManager()
{
    m_ConfigFile_path.~basic_string();
    m_CachedConfigFile.~vector();
}


// write to config file, overwrite file with m_CachedConfigFile
// should call this function in destructor as well
bool ConfigManager::write_vec_2_config()
{
    std::wofstream output_config(m_ConfigFile_path);
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



// private functions

// read config file from m_ConfigFile_path to ram &
// cache to m_CachedConfigFile, call it in constructor
bool ConfigManager::read_config_2_vec()
{
    std::wifstream input_config(m_ConfigFile_path);
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
    for (size_t i = m_CachedConfigFile.size()-1; i >= 0; --i) {
        bool i_is_bad_or_empty =
            m_CachedConfigFile[i].m_DataSection == ConfigSections::Empty &&
            m_CachedConfigFile[i].m_DataSection == ConfigSections::Bad;
        if (i_is_bad_or_empty)
            m_CachedConfigFile.erase(m_CachedConfigFile.begin()+i);
    }
}

// ====================== ConfigManager end ======================




// ====================== ConfigItem ======================

// constructor
ConfigItem::ConfigItem()
    : m_Data(L""), m_DataSection(ConfigSections::Empty)
{}
// constructor /w raw wstring
ConfigItem::ConfigItem(const std::wstring& str)
    : m_Data(L""), m_DataSection(ConfigSections::Empty)
{
    size_t equal_sign_pos = str.find(L"=");
    std::wstring config_tag = std::wstring(str.begin(), str.begin()+equal_sign_pos);
    m_DataSection = glob_wstr_ConfigSection_list[config_tag];
    m_Data = std::wstring(str.begin()+equal_sign_pos+1, str.end());
}
// copy constructor
ConfigItem::ConfigItem(const ConfigItem& obj)
    : m_Data(obj.m_Data), m_DataSection(obj.m_DataSection)
{}

// destructor
ConfigItem::~ConfigItem()
{
    m_Data.~basic_string();
}

std::wstring ConfigItem::output_wstr()
{
    auto cur_ConfigItem_name = std::find_if(
                            glob_wstr_ConfigSection_list.begin(),
                            glob_wstr_ConfigSection_list.end(),
                            [](auto&& p) {return p->second == m_DataSection;});
    return cur_ConfigItem_name->first + L"=" + m_Data;
}

// ====================== ConfigItem end ======================
