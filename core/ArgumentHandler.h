#pragma once
#include <string>
#include <iostream>
#include <windows.h>

#include "GlobTools.h"

/*
 * Allowed Arguments
 * 
 *    --add {file_path} => adding new wallpaper to ./Wallpapers/ folder
 *    --del {file_name} => deleting wallpaper by its name (hex # in ./Wallpapers/ folder)
 *    --find {file_name} => find a wallpaper's info base on its name (hex # in ./Wallpapers/ folder)
 *    --paste {file_name} + {PORTRAIT/LANDSCAPE} => paste inputted wallpaper to Windows System Theme folder with specific mode
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
        L"\t--help, -h => print help menu\n"
    };
    std::string helpArrA[] = {
        "Allowed Arguments\n",
        "\n",
        "\t--add {file_path} => adding new wallpaper to ./Wallpapers/ folder\n",
        "\t--del {file_name} => deleting wallpaper by its name (hex # in ./Wallpapers/ folder)\n",
        "\t--find {file_name} => find a wallpaper's info base on its name (hex # in ./Wallpapers/ folder)\n",
        "\t--paste {file_name} + {PORTRAIT/LANDSCAPE} => paste inputted wallpaper to Windows System Theme folder with specific mode\n",
        "\t--help, -h => print help menu\n"
    };
};


// Mode of Wallpaper, PORTRAIT or LANDSCAPE
enum class WallpaperMode {
    UNKNOW = 0,
    PORTRAIT, LANDSCAPE
};

// handling arguments in wmain() function
// produce commands with input arguments
class ArgumentHandlerW
{
public:
    // constructor
    ArgumentHandlerW(int argc, wchar_t** argv);
    
    void helpMsgBox();
    
    // getters
    std::wstring getFileName();
    std::wstring getFilePath();
    WallpaperMode getWallpaperMode();
    
    bool hasAdd();
    const bool hasAdd() const;
    bool hasDel();
    const bool hasDel() const;
    bool hasFind();
    const bool hasFind() const;
    bool hasPaste();
    const bool hasPaste() const;
    bool hasHelp();
    const bool hasHelp() const;
    
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
    bool m_HasHelp;
};


// handling arguments in main() function
// produce commands with input arguments
class ArgumentHandlerA
{
public:
    // constructor
    ArgumentHandlerA(int argc, char** argv);
    
    void helpMsgBox();
    
    // getters
    std::string getFileName();
    std::string getFilePath();
    WallpaperMode getWallpaperMode();
    
    bool hasAdd();
    const bool hasAdd() const;
    bool hasDel();
    const bool hasDel() const;
    bool hasFind();
    const bool hasFind() const;
    bool hasPaste();
    const bool hasPaste() const;
    bool hasHelp();
    const bool hasHelp() const;
    
private:
    // argument buffer
    std::string m_FileName;
    std::string m_FilePath;
    // flags
    bool m_HasAdd;
    bool m_HasDel;
    bool m_HasFind;
    bool m_HasPaste;
    WallpaperMode m_WallpaperMode;
    bool m_HasHelp;
};