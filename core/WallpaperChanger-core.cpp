#include <iostream>
#include <string>
#include <stdexcept>

#include "ArgumentHandler.h"
#include "WallpaperManager.h"
#include "ConfigManager.h"

std::wostream& operator<<(std::wostream& output, const WallpaperInfo& obj)
{
    output << obj.getSrcPath() << std::endl
            << L"Original FileName: " << obj.getOriFileName() << std::endl
            << L"NewFilename (FileID): " << obj.getNewFilename() << std::endl
            << L"AddTime: " << obj.getAddTime();
    return output;
}

int wmain(int argc, wchar_t* argv[])
{
    ArgumentHandlerW arg(argc, argv);
    WallpaperManager wm;
    ConfigManager config(false);
    try { // try to open ./config.ini
        config.read(L"./config.ini");
    } catch(...) { // cannot open ./config.ini, exit
        return -1;
    }
    
    if (arg.hasHelp()) {
        arg.printHelp();
        return 0;
    } else {
        if (arg.hasAdd()) {
            wm.copy_wallpaper_2_cacheFolder(arg.getFilePath());
        }
        if (arg.hasDel() && !arg.getFileName().empty()) {
            wm.remove_wallpaper(wm.find_wallpaperInfo_via_new(arg.getFileName()));
        }
        if (arg.hasFind() && !arg.getFileName().empty()) {
            std::wcout << wm.find_wallpaperInfo_via_new(arg.getFileName());
        }
        if (arg.hasPaste() && !arg.getFileName().empty()) {
            // get loc_file & dest_file_dir
            WallpaperInfo loc_file = wm.find_wallpaperInfo_via_new(arg.getFileName());
            std::wstring dest_file_dir = config.get(L"system", L"windows_theme_dir");
            if (dest_file_dir[dest_file_dir.size()-1] != L'\\')
                dest_file_dir += L'\\';
            // process dest_file_name
            std::wstring dest_file_name = L"CachedImage_";
            if (arg.getWallpaperMode() == WallpaperMode::LANDSCAPE)
                dest_file_name += config.get(L"system", L"hori_res") + L"_" + config.get(L"system", L"verti_res");
            else if (arg.getWallpaperMode() == WallpaperMode::PORTRAIT)
                dest_file_name += config.get(L"system", L"verti_res") + L"_" + config.get(L"system", L"hori_res");
            else throw std::invalid_argument("Please Enter A WallpaperMode As Well: PORTRAIT/LANDSCAPE");
            dest_file_name += L"_POS4.jpg";
            // pasting file to Windows System Theme folder
            std::wcout << loc_file << std::endl << dest_file_dir << std::endl;
            wm.past_wallpaper_2_targetFolder(loc_file, dest_file_dir + dest_file_name);
        }
    }
    
    
    
    
    return 0;
}