// C++ STL
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <windows.h>
#include <shellapi.h>
#include <locale>

// others
#include "ArgumentHandler.h"
#include "WallpaperManager.h"
#include "../utilities/ConfigManager.h"
#include "../utilities/CacheRW.h"


int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow)
{
    int argc = 0;
    wchar_t** argv = (wchar_t**)CommandLineToArgvW(GetCommandLineW(), &argc);
    try {
        ArgumentHandler arg(argc, argv);
        WallpaperManager wm;
        ConfigManager config(false);
        try { // try to open ./config.ini
            config.read(GlobTools::getCurrExePathW()+L"config.ini");
        } catch(...) { // cannot open ./config.ini, exit
            return -1;
        }
        
        if (arg.hasHelp()) {
            arg.helpMsgBox();
            return 0;
        } else {
            if (arg.hasAdd() && arg.isAddValid()) {
                wm.copy_wallpaper_2_cacheFolder(arg.getFilePath());
                
                // output
                auto new_wallpaper = wm.get_last_wallpaperInfo();
                Cache_Writer cache(new_wallpaper.getNewFilename());
            }
            if (arg.hasDel() && arg.isDelValid(wm.get_wallpaperInfo_data(), wm.get_wallpaperInfo_size())) {
                auto wallpaper_to_del = wm.find_wallpaperInfo_via_new(arg.getFileName());
                std::wstring old_wallpaper_id = wallpaper_to_del.getNewFilename();
                wm.remove_wallpaper(wallpaper_to_del);
                
                // output
                Cache_Writer cache(old_wallpaper_id);
            }
            if (arg.hasFind() && arg.isFindValid()) {
                // output
                auto temp_obj = wm.find_wallpaperInfo_via_new(arg.getFileName());
                if (temp_obj.isEmpty()) throw std::exception();
                std::wstring msg[] = {
                    (L"Source Path: " + temp_obj.getSrcPath()),
                    (L"Original FileName: " + temp_obj.getOriFileName()),
                    (L"NewFilename (FileID): " + temp_obj.getNewFilename()),
                    (L"AddTime: " + std::to_wstring(temp_obj.getAddTime()))
                };
                Cache_Writer cache(msg, 4);
            }
            if (arg.hasPaste() && arg.isPasteValid(wm.get_wallpaperInfo_data(), wm.get_wallpaperInfo_size())) {
                // get loc_file & dest_file_dir
                WallpaperInfo loc_file = wm.find_wallpaperInfo_via_new(arg.getFileName());
                std::wstring dest_file_dir = config.get(L"system", L"windows_theme_dir");
                if (dest_file_dir[dest_file_dir.size()-1] != L'\\')
                    dest_file_dir += L'\\';
                // process dest_file_name
                std::wstring dest_file_name = L"CachedImage_";
                if (arg.getWallpaperMode() == WallpaperMode::LANDSCAPE)
                    dest_file_name += config.get(L"system", L"horz_res") + L"_" + config.get(L"system", L"vert_res");
                else if (arg.getWallpaperMode() == WallpaperMode::PORTRAIT)
                    dest_file_name += config.get(L"system", L"vert_res") + L"_" + config.get(L"system", L"horz_res");
                else if (arg.getWallpaperMode() == WallpaperMode::UNKNOW)
                    return -1;
                if (arg.getWallpaperMode() == WallpaperMode::DEFAULT) {
                    dest_file_name = L"TranscodedWallpaper";
                } else {
                    dest_file_dir += L"CachedFiles\\";
                    dest_file_name += L"_POS4.jpg";
                }
                // pasting file to Windows System Theme folder
                wm.past_wallpaper_2_targetFolder(loc_file, dest_file_dir + dest_file_name);
                
                // output
                std::wstring msg[] = {
                    loc_file.getNewFilename(),
                    dest_file_dir + dest_file_name
                };
                Cache_Writer cache(msg, 2);
            }
        }
	}
	catch (std::exception& err) {
		return -1;
	}
    
    return 0;
}