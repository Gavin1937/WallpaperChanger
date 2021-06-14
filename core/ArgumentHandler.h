#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <windows.h>

#include "WallpaperManager.h"
#include "../utilities/GlobTools.h"

/*
 * Allowed Arguments
 * 
 *    --add {file_path} => adding new wallpaper to ./Wallpapers/ folder
 *    --del {file_name} => deleting wallpaper by its name (hex # in ./Wallpapers/ folder)
 *    --find {file_name} => find a wallpaper's info base on its name (hex # in ./Wallpapers/ folder)
 *    --paste {file_name} + {PORTRAIT/LANDSCAPE} => paste inputted wallpaper to Windows System Theme folder with specific mode
 *    --clear {MODE} => clear cached Wallpapers or config.ini. {MODE: ALL, CACHED_WALLPAPER, USER_CONFIG}
 *    --help, -h => print help menu
 * 
 */

namespace {
    std::wstring helpArrW[] = {
        L"Allowed Arguments\n",
        L"\n",
        L"\t--add {file_path} => adding new wallpaper to ./Wallpapers/ folder\n",
        L"\t--del {file_name} => deleting wallpaper by its name (hex # in ./Wallpapers/ folder)\n",
        L"\t--find {file_name} => find a wallpaper's info base on its name (hex # in ./Wallpapers/ folder)\n",
        L"\t--paste {file_name} + {PORTRAIT/LANDSCAPE} => paste inputted wallpaper to Windows System Theme folder with specific mode\n",
        L"\t--clear {MODE} => clear cached Wallpapers or config.ini. {MODE: ALL, CACHED_WALLPAPER, USER_CONFIG}\n",
        L"\t--help, -h => print help menu\n"
    };
};


// Mode of Wallpaper, PORTRAIT or LANDSCAPE
enum class WallpaperMode {
    UNKNOW = 0,
    DEFAULT, PORTRAIT, LANDSCAPE
};
enum class ClearMode {
    ALL = 0,
    CACHED_WALLPAPER, USER_CONFIG
};

// handling arguments in wWinMain() function
// produce commands with input arguments
class ArgumentHandler
{
public:
    // constructor
    ArgumentHandler(int argc, wchar_t** argv);
    
    void helpMsgBox();
    std::wstring helpMsgStr();
    
    // getters
    std::wstring getFileName();
    std::wstring getFilePath();
    WallpaperMode getWallpaperMode();
    ClearMode getClearMode();
    
    // argument flags
    bool hasAdd();
    const bool hasAdd() const;
    bool hasDel();
    const bool hasDel() const;
    bool hasFind();
    const bool hasFind() const;
    bool hasPaste();
    const bool hasPaste() const;
    bool hasClear();
    const bool hasClear() const;
    bool hasHelp();
    const bool hasHelp() const;
    
    // argument validation
    bool isAddValid();
    const bool isAddValid() const;
    bool isDelValid(const WallpaperInfo* wallpaperInfo_arr, const int& arr_size);
    const bool isDelValid(const WallpaperInfo* wallpaperInfo_arr, const int& arr_size) const;
    bool isFindValid();
    const bool isFindValid() const;
    bool isPasteValid(const WallpaperInfo* wallpaperInfo_arr, const int& arr_size);
    const bool isPasteValid(const WallpaperInfo* wallpaperInfo_arr, const int& arr_size) const;
    
private:
    // argument buffer
    std::wstring m_FileName;
    std::wstring m_FilePath;
    // flags
    bool m_HasAdd;
    bool m_HasDel;
    bool m_HasFind;
    bool m_HasPaste;
    WallpaperMode m_WallpaperMode;
    bool m_HasClear;
    ClearMode m_ClearMode;
    bool m_HasHelp;
};
