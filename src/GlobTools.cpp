#include "GlobTools.h"

// C++ STL
#include <Windows.h>
#include <algorithm>
#include <filesystem>

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
    char temp_buff[MAX_PATH];
    HMODULE hModule = GetModuleHandleA(NULL);
    GetModuleFileNameA(hModule, temp_buff, sizeof(temp_buff));
    auto it = std::find(std::begin(temp_buff), std::end(temp_buff), '\\');
    auto old_it = it;
    while (it != std::end(temp_buff)) {
        old_it = it;
        it = std::find(it+1, std::end(temp_buff), '\\');
    }
    return std::string(temp_buff, old_it+1);
}
// get current .exe path, return wstring, end with L'\\'
std::wstring GlobTools::getCurrExePathW()
{
    wchar_t temp_buff[MAX_PATH];
    HMODULE hModule = GetModuleHandleW(NULL);
    GetModuleFileNameW(hModule, temp_buff, sizeof(temp_buff));
    auto it = std::find(std::begin(temp_buff), std::end(temp_buff), L'\\');
    auto old_it = it;
    while (it != std::end(temp_buff)) {
        old_it = it;
        it = std::find(it+1, std::end(temp_buff), L'\\');
    }
    return std::wstring(temp_buff, old_it+1);
}

// get file list of input directory
void GlobTools::getFilesUnderDirA(const std::string& dir, std::vector<std::string>& buff)
{
    if (GlobTools::is_filedir_existA(dir)) {
        
    }
}
// get file list of input directory
void GlobTools::getFilesUnderDirW(const std::wstring& dir, std::vector<std::wstring>& buff);
