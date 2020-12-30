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

// change all char in string to upper
std::string GlobTools::all2upperA(const std::string& src)
{
    std::string output;
    for (auto it : src)
        output += std::toupper(it);
    return output;
}
// change all wchar_t in wstring to upper
std::wstring GlobTools::all2upperW(const std::wstring& src)
{
    std::wstring output;
    for (auto it : src)
        output += std::toupper(it);
    return output;
}

// change all char in string to lower
std::string GlobTools::all2lowerA(const std::string& src)
{
    std::string output;
    for (auto it : src)
        output += std::tolower(it);
    return output;
}
// change all wchar_t in wstring to lower
std::wstring GlobTools::all2lowerW(const std::wstring& src)
{
    std::wstring output;
    for (auto it : src)
        output += std::tolower(it);
    return output;
}
