#include "GlobTools.h"

// C++ STL
#include <Windows.h>
#include <algorithm>

// Functions

// check whether a file or directory exist
bool GlobTools::is_filedir_existA(const std::string& path)
{
    return std::filesystem::exists(path);
}
// check whether a file or directory exist
bool GlobTools::is_filedir_existW(const std::wstring& path)
{
    return std::filesystem::exists(path);
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

// get current .exe path, return string, end with '\\'
std::string GlobTools::getCurrExePathA()
{
    return (std::filesystem::current_path().string()+"\\");
}
// get current .exe path, return wstring, end with L'\\'
std::wstring GlobTools::getCurrExePathW()
{
    return (std::filesystem::current_path().wstring()+L"\\");
}

// get file list of input directory
bool GlobTools::getFilesUnderDirA(const std::string& dir, std::vector<std::string>& buff)
{
    if (GlobTools::is_filedir_existA(dir)) {
        for (auto file : std::filesystem::directory_iterator(dir))
            buff.push_back(file.path().filename().string());
    } else return false;
    return true;
}
// get file list of input directory
bool GlobTools::getFilesUnderDirW(const std::wstring& dir, std::vector<std::wstring>& buff)
{
    if (GlobTools::is_filedir_existW(dir)) {
        for (auto file : std::filesystem::directory_iterator(dir))
            buff.push_back(file.path().filename().wstring());
    } else return false;
    return true;
}


// ifstream /w utf8 locale set
GlobTools::utf8_ifstream::utf8_ifstream()
{
    imbue(std::locale());
}
GlobTools::utf8_ifstream::utf8_ifstream(
    const std::string& path, std::ios_base::openmode mode)
{
    imbue(std::locale());
    open(path, mode);
}
GlobTools::utf8_ifstream::utf8_ifstream(
    const std::wstring& path, std::ios_base::openmode mode)
{
    imbue(std::locale());
    open(path, mode);
}

// wifstream /w utf8 locale set
GlobTools::utf8_wifstream::utf8_wifstream()
{
    imbue(std::locale());
}
GlobTools::utf8_wifstream::utf8_wifstream(
    const std::string& path, std::ios_base::openmode mode)
{
    imbue(std::locale());
    open(path, mode);
}
GlobTools::utf8_wifstream::utf8_wifstream(
    const std::wstring& path, std::ios_base::openmode mode)
{
    imbue(std::locale());
    open(path, mode);
}

// ofstream /w utf8 locale set
GlobTools::utf8_ofstream::utf8_ofstream()
{
    imbue(std::locale());
}
GlobTools::utf8_ofstream::utf8_ofstream(
    const std::string& path, std::ios_base::openmode mode)
{
    imbue(std::locale());
    open(path, mode);
}
GlobTools::utf8_ofstream::utf8_ofstream(
    const std::wstring& path, std::ios_base::openmode mode)
{
    imbue(std::locale());
    open(path, mode);
}

// wofstream /w utf8 locale set
GlobTools::utf8_wofstream::utf8_wofstream()
{
    imbue(std::locale());
}
GlobTools::utf8_wofstream::utf8_wofstream(
    const std::string& path, std::ios_base::openmode mode)
{
    imbue(std::locale());
    open(path, mode);
}
GlobTools::utf8_wofstream::utf8_wofstream(
    const std::wstring& path, std::ios_base::openmode mode)
{
    imbue(std::locale());
    open(path, mode);
}
