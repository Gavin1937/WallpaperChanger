#include "GlobTools.h"


// Functions

// check whether a directory exist
bool GlobTools::is_filedir_exist(const std::wstring& dir)
{
    std::wifstream temp(cvter.to_bytes(dir));
    if (temp) { 
        temp.close();
        return true;
    } else { 
        temp.close();
        return false;
    }
}