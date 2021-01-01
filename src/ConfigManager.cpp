#include "ConfigManager.h"
#include "GlobTools.h"


namespace 
{
    std::vector<std::wstring> glob_config_file_template = {
        L"Config.ini"
    };
};



// ====================== ConfigManager ======================

// public functions

// default constructor
ConfigManager::ConfigManager()
    : m_ConfigFile_path(L""), m_CachedConfigFile(std::vector<std::wstring>())
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
    : m_ConfigFile_path(ConfigFile_path), m_CachedConfigFile(std::vector<std::wstring>())
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
                output_config << it << L"\n";
        } else {
            for (auto it : glob_config_file_template)
                output_config << it << L"\n";
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
            m_CachedConfigFile.push_back(buff);
    } catch(...) { return false; }
    input_config.close();
    return true;
}


// ====================== ConfigManager end ======================
