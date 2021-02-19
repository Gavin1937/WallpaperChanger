#include <iostream>

#include "ArgumentHandler.h"
#include "WallpaperManager.h"

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
    }
    
    
    
    
    return 0;
}