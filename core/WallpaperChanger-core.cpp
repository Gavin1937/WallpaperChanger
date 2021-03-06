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
#include "../utilities/GlobTools.h"


int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow)
{
    // clear potentially left over ./core_cache file
    CleanCache(GlobTools::getCurrExePathW()+L"core_cache");
    
    // init argc & argv
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
        } else if (arg.hasClear()) {
            config.~ConfigManager();
            wm.~WallpaperManager();
            if (arg.getClearMode() == ClearMode::ALL) {
                GlobTools::rm_all(GlobTools::getCurrExePathW()+L"Wallpapers");
                GlobTools::rm(GlobTools::getCurrExePathW()+L"config.ini");
            } else if (arg.getClearMode() == ClearMode::CACHED_WALLPAPER)
                GlobTools::rm_all(GlobTools::getCurrExePathW()+L"Wallpapers");
            else if (arg.getClearMode() == ClearMode::USER_CONFIG)
                GlobTools::rm(GlobTools::getCurrExePathW()+L"config.ini");
            return 0;
        } else {
            if (arg.hasAdd() && arg.isAddValid()) {
                bool result = wm.copy_wallpaper_2_cacheFolder(arg.getFilePath());
                
                // output
                WallpaperInfo new_wallpaper = wm.get_last_wallpaperInfo();
                std::wstring output_buff[2] = {
                    std::to_wstring(result), 
                    new_wallpaper.getNewFilename() 
                };
                Cache_Writer cache(output_buff, 2, GlobTools::getCurrExePathW()+L"core_cache");
            }
            if (arg.hasDel() && arg.isDelValid(wm.get_wallpaperInfo_data(), wm.get_wallpaperInfo_size())) {
                WallpaperInfo wallpaper_to_del = wm.find_wallpaperInfo_via_new(arg.getFileName());
                std::wstring old_wallpaper_id = wallpaper_to_del.getNewFilename();
                bool result = wm.remove_wallpaper(wallpaper_to_del);
                
                // output
                std::wstring output_buff[2] = {
                    std::to_wstring(result),
                    old_wallpaper_id
                };
                Cache_Writer cache(output_buff, 2, GlobTools::getCurrExePathW()+L"core_cache");
            }
            if (arg.hasFind() && arg.isFindValid()) {
                // output
                WallpaperInfo temp_obj = wm.find_wallpaperInfo_via_new(arg.getFileName());
                if (!temp_obj.isEmpty()) {
                    std::wstring msg[] = {
                        (L"Source Path: " + temp_obj.getSrcPath()),
                        (L"Original FileName: " + temp_obj.getOriFileName()),
                        (L"NewFilename (FileID): " + temp_obj.getNewFilename()),
                        (L"AddTime: " + std::to_wstring(temp_obj.getAddTime()))
                    };
                    Cache_Writer cache(msg, 4, GlobTools::getCurrExePathW()+L"core_cache");
                }
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
                bool result = wm.past_wallpaper_2_targetFolder(loc_file, dest_file_dir + dest_file_name);
                
                // output
                std::wstring msg[] = {
                    std::to_wstring(result),
                    loc_file.getNewFilename(),
                    dest_file_dir + dest_file_name
                };
                Cache_Writer cache(msg, 3, GlobTools::getCurrExePathW()+L"core_cache");
            }
        }
	}
	catch (std::exception& err) {
		return -1;
	}
    
    return 0;
}