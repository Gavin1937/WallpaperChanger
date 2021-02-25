#pragma once

// C++ STL
#include <windows.h>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <memory>
#include <fstream>
#include <ctime>
#include <codecvt>
#include <locale>


// class, struct, function declaration
class WallpaperManager;
class WallpaperInfo;


// manager wallpapers. (Copy, Past, Tracking stored files)
class WallpaperManager
{
public:
    // default constructor
    WallpaperManager();
    // parametric constructor
    WallpaperManager(const std::wstring& WallpaperList_path);
    // disable copy constructor
    WallpaperManager(const WallpaperManager& obj) = delete;
    
    // destructor, also write & save all data in m_CachedWallpaperInfo to WallpaperList
    ~WallpaperManager();
    
    // write cached WallpaperInfo to WallpaperList (OVERWRITE)
    void update_WallpaperList();
    
    // copy file from src_path to ./Wallpapers/
    // @Param: src_path => path to source directory of a wallpaper
    // return true(1) if function success
    // return false(0) if function fail
    bool copy_wallpaper_2_cacheFolder(const std::wstring& src_path);
    
    // past file from ./Wallpapers/ to tar_path 
    // @Param: wallpaper_ind => index to a wallpaper in m_CachedWallpaperInfo
    // @Param: tar_path      => path (with filename) to target directory for pasting
    // return true(1) if function success
    // return false(0) if function fail
    bool past_wallpaper_2_targetFolder(const int& wallpaper_ind,
                                    const std::wstring& tar_path);
    // past file from ./Wallpapers/ to tar_path 
    // @Param: wallpaper_info => WallpapInfo object of a wallpaper
    // @Param: tar_path       => path (with filename) to target directory for pasting
    // return true(1) if function success
    // return false(0) if function fail
    bool past_wallpaper_2_targetFolder(const WallpaperInfo& wallpaper_info,
                                    const std::wstring& tar_path);
    
    // remove a cached wallpaper
    // @Param: wallpaper_info => WallpaperInfo object of target wallpaper
    // return true(1) if function success
    // return false(0) if function fail
    bool remove_wallpaper(const WallpaperInfo& wallpaper_info);
    
    // find wallpaper base on its basic info, return its index in m_CachedWallpaperInfo
    const int find_wallpaperIndex_via_wallInfo(const WallpaperInfo& wallpaper_info);
    WallpaperInfo& find_wallpaperInfo_via_src(const std::wstring& src_path);
    WallpaperInfo& find_wallpaperInfo_via_ori(const std::wstring& original_filename);
    WallpaperInfo& find_wallpaperInfo_via_new(const std::wstring& new_filename_hexID);
    WallpaperInfo& find_wallpaperInfo_via_time(const int& add_time);
    
    // CachedWallpaperInfo data accessing
    WallpaperInfo& get_first_wallpaperInfo();
    WallpaperInfo& get_last_wallpaperInfo();
    WallpaperInfo* get_wallpaperInfo_data();
    
    // CachedWallpaperInfo basic info
    size_t get_wallpaperInfo_size();
    const size_t get_wallpaperInfo_size() const;
    bool is_wallpaperInfo_empty();
    const bool is_wallpaperInfo_empty() const;
    
private:
    // appent & update to WallpaperList file under ./Wallpapers/
    void append_WallpaperList(const std::wstring& src_path);
    
    // remove & update to WallpaperList file under ./Wallpapers/
    void remove_WallpaperList(const WallpaperInfo& info);
    
private:
    std::wstring m_WallpaperList_path;               // path to WallpaperList file
    // WallpaperList cached in RAM
    // Data syntax in WallpaperList file
    // 1st line:    L"\"src_path\", \"hex_id\", \"add_time\"\n"
    // other lines: L"\"wstring\", \"wstring\", \"int\""
    // WallpaperList file is written in form of a CVS file
    std::vector<WallpaperInfo> m_CachedWallpaperInfo; 
};


// basic infomations of a wallpaper in WallpaperList
class WallpaperInfo
{
public:
    // default constructor
    WallpaperInfo();
    // parametric constructor
    WallpaperInfo(const std::wstring& src_path, const std::wstring& new_filename, const int& add_time = 0);
    // copy constructor
    WallpaperInfo(const WallpaperInfo& obj);
    
    // compare operators
    
    bool operator==(const WallpaperInfo& obj);
    bool operator!=(const WallpaperInfo& obj);
    
    // getter & setter
    
    const std::wstring getSrcPath() const;
    void setSrcPath(const std::wstring& src_path);
    const std::wstring getOriFileName() const;
    const std::wstring getNewFilename() const;
    void setNewFilename(const std::wstring& new_filename);
    const int getAddTime() const;
    
    bool isEmpty() const;
    
private:
    std::wstring m_SrcPath; // source path of wallpaper
    std::wstring m_OriFileName; // filename on source path
    std::wstring m_NewFileName; // new filename or HEX id of wallpaper under ./Wallpapers/. (6 digit hex #)
    int m_AddTime; // add time of wallpaper (unix timestamp)
};
