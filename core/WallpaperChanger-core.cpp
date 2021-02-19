#include <iostream>

#include "ArgumentHandler.h"
#include "WallpaperManager.h"

int wmain(int argc, wchar_t* argv[])
{
    ArgumentHandlerW arg(argc, argv);
    WallpaperManager wm;
    
    std::cout << "\n\n\n\n";
    
    if (arg.hasHelp()) {
        arg.printHelp();
        return 0;
    } else {
        if (arg.hasAdd()) {
            wm.copy_wallpaper_2_cacheFolder(arg.getFilePath());
        }
    }
    
    
    
    
    
    std::cout << "\n\n\n" << "Hello World!\n";
    return 0;
}