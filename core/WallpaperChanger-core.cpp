// C++ STL
#include <iostream>
#include <string>
#include <stdexcept>
#include <windows.h>
#include <shellapi.h>

// others
#include "ArgumentHandler.h"
#include "WallpaperManager.h"
#include "ConfigManager.h"


int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow)
{
    int argc = 0;
    wchar_t** argv = (wchar_t**)CommandLineToArgvW(GetCommandLineW(), &argc);
    ArgumentHandlerW arg(argc, argv);
    WallpaperManager wm;
    ConfigManager config(false);
    try { // try to open ./config.ini
        config.read(L"./config.ini");
    } catch(...) { // cannot open ./config.ini, exit
        return -1;
    }
    
    if (arg.hasHelp()) {
        arg.helpMsgBox();
        return 0;
    } else {
        if (arg.hasAdd()) {
            wm.copy_wallpaper_2_cacheFolder(arg.getFilePath());
        }
        if (arg.hasDel() && !arg.getFileName().empty()) {
            wm.remove_wallpaper(wm.find_wallpaperInfo_via_new(arg.getFileName()));
        }
        if (arg.hasFind() && !arg.getFileName().empty()) {
            std::wstring msg;
            auto temp_obj = wm.find_wallpaperInfo_via_new(arg.getFileName());
            msg += L"Original FileName: " + temp_obj.getOriFileName() + L"\n";
            msg += L"NewFilename (FileID): " + temp_obj.getNewFilename() + L"\n";
            msg += L"AddTime: " + std::to_wstring(temp_obj.getAddTime());
            MessageBoxW(0, msg.c_str(), L"Find Information", 0);
        }
        if (arg.hasPaste() && !arg.getFileName().empty()) {
            // get loc_file & dest_file_dir
            WallpaperInfo loc_file = wm.find_wallpaperInfo_via_new(arg.getFileName());
            std::wstring dest_file_dir = config.get(L"system", L"windows_theme_dir");
            if (dest_file_dir[dest_file_dir.size()-1] != L'\\')
                dest_file_dir += L'\\';
            dest_file_dir += L"CachedFiles\\";
            // process dest_file_name
            std::wstring dest_file_name = L"CachedImage_";
            if (arg.getWallpaperMode() == WallpaperMode::LANDSCAPE)
                dest_file_name += config.get(L"system", L"horz_res") + L"_" + config.get(L"system", L"vert_res");
            else if (arg.getWallpaperMode() == WallpaperMode::PORTRAIT)
                dest_file_name += config.get(L"system", L"vert_res") + L"_" + config.get(L"system", L"horz_res");
            else return -1;
            dest_file_name += L"_POS4.jpg";
            // pasting file to Windows System Theme folder
            wm.past_wallpaper_2_targetFolder(loc_file, dest_file_dir + dest_file_name);
        }
    }
    
    
    
    
    return 0;
}