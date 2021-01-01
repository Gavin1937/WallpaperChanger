#pragma once

// C++ STL
#include <string>
#include <vector>
#include <fstream>
#include <codecvt>
#include <locale>

// class, struct, & function declaration
class ConfigManager;
class ConfigItem;

// enum class to specify different sections in config.ini
enum class ConfigSections{
    Bad = -1, Empty = 0
};

// manage (R & W) config.ini file
class ConfigManager
{
public:
    // default constructor
    ConfigManager();
    // parametric constructor
    ConfigManager(const std::wstring& ConfigFile_path);
    // disable copy constructor
    ConfigManager(const ConfigManager& obj) = delete;
    
    // destructor
    ~ConfigManager();
    
    // write to config file, overwrite file with m_CachedConfigFile
    // should call this function in destructor as well
    // return true(1) if function success
    // return fail(0) if function fail
    bool write_vec_2_config();
    
    // modify a specific section of config.ini
    // @Param: modify_val => value to be modify
    // @Param: section    => specify a section to
    // return true(1) if function success
    // return fail(0) if function fail
    bool modify_config(const std::wstring& modify_val, ConfigSections section);
    
private:
    // read config file from m_ConfigFile_path to ram &
    // cache to m_CachedConfigFile, call it in constructor
    // return true(1) if function success
    // return fail(0) if function fail
    bool read_config_2_vec();
    
    // clear all empty & bad ConfigItem,
    // this function should called inside of read_config_2_vec();
    void clear_empty_bad_ConfigItem();
    
private:
    std::wstring m_ConfigFile_path; // path to config.ini
    std::vector<ConfigItem> m_CachedConfigFile; // cached config.ini in ram
};


// a single item in config.ini
class ConfigItem
{
public:
    std::wstring m_Data;
    ConfigSections m_DataSection;
public:
    // constructor
    ConfigItem();
    // constructor /w raw wstring
    ConfigItem(const std::wstring& str);
    // copy constructor
    ConfigItem(const ConfigItem& obj);
    
    // destructor
    ~ConfigItem();
    
    std::wstring output_wstr();
    
};