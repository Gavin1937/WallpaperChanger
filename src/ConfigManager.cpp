#include "ConfigManager.h"
#include "GlobTools.h"


namespace 
{
    std::vector<ConfigItem> glob_config_file_template = {
        ConfigItem(L"Config.ini")
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



// ====================== ConfigItem end ======================
