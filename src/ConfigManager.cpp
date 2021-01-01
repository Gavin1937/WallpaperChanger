#include "ConfigManager.h"
#include "GlobTools.h"

// ====================== ConfigManager ======================

// default constructor
ConfigManager::ConfigManager()
    : m_ConfigFile_path(L""), m_CachedConfigFile(std::vector<std::wstring>())
{
    // create a config file current dir does not have one
    m_ConfigFile_path = GlobTools::getCurrExePathW();
    
}
// parametric constructor
ConfigManager::ConfigManager(const std::wstring& ConfigFile_path)
    : m_ConfigFile_path(ConfigFile_path), m_CachedConfigFile(std::vector<std::wstring>())
{
    
}


// ====================== ConfigManager end ======================
