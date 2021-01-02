#pragma once

// C++ STL
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <codecvt>
#include <locale>

// class, struct, & function declaration
class ConfigManager;
class ConfigItem;

// enum class to specify different sections in config.ini
enum class ConfigSections{
    Bad = -1, Empty = 0,
    ExeRootDir,        // Root Dir of .exe file
    ConfigDir,         // Dir to config.ini file
    WallpaperCacheDir, // Dir to /Wallpapers/ directory
    WallpaperListDir,  // Dir to WallaperList file
    WindowsThemeDir,   // Dir to C:\Users\${USERNAME}\AppData\Roaming\Microsoft\Windows\Themes
    SS_WallpaperID_P,  // Single Screen Wallpaper Portrait
    SS_WallpaperID_L,  // Single Screen Wallpaper Landscape
    MS_WallpaperID     // Multi Screen Wallpaper
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
    bool write_buff2config();
    
    // modify a specific section of config.ini
    // @Param: section    => specify a section to
    // @Param: modify_val => value to be modify
    // return true(1) if function success
    // return fail(0) if function fail or cannot find section
    bool modify_config(ConfigSections section, const std::wstring& modify_val);
    
private:
    // read config file from m_ConfigFile_path to ram &
    // cache to m_CachedConfigFile, call it in constructor
    // return true(1) if function success
    // return fail(0) if function fail
    bool read_config2buff();
    
    // clear all empty & bad ConfigItem,
    // this function should called inside of read_config_2_vec();
    void clear_empty_bad_ConfigItem();
    
    // search specific ConfigItem in m_CachedConfigFile
    // w/ a ConfigSection param
    std::vector<ConfigItem>::iterator find_ConfigItem_by_Section(const ConfigSections& cs);
    
    // write basic info w/ modify_config() to config.ini
    // This function should be called at the end of constructor
    void write_basic_info_2_config();
    
    
private:
    std::wstring m_ConfigFile_path; // path to config.ini
    std::vector<ConfigItem> m_CachedConfigFile; // cached config.ini in ram
};


// a single item in config.ini
class ConfigItem
{
public:
    ConfigSections m_DataSection;
    std::wstring m_Data;
public:
    // constructor
    ConfigItem();
    // constructor w/ raw wstring
    ConfigItem(const std::wstring& str);
    // copy constructor
    ConfigItem(const ConfigItem& obj);
    
    // destructor
    ~ConfigItem();
    
    std::wstring output_wstr();
    
};