#pragma once

// C++ STL
#include <string>
#include <vector>
#include <codecvt>
#include <locale>
#include <fstream>

namespace GlobTools 
{
    // global variables 
    
    // convert wstring & string
    static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvter;
    
    
    
    // global functions
    
    // check whether a file or directory exist
    bool is_filedir_existA(const std::string& dir);
    // check whether a file or directory exist
    bool is_filedir_existW(const std::wstring& dir);
    
    // change all char in string to upper
    std::string all2upperA(const std::string& src);
    // change all wchar_t in wstring to upper
    std::wstring all2upperW(const std::wstring& src);
    
    // change all char in string to lower
    std::string all2lowerA(const std::string& src);
    // change all wchar_t in wstring to lower
    std::wstring all2lowerW(const std::wstring& src);
    
    // get current .exe path, return string, end with '\\'
    std::string getCurrExePathA();
    // get current .exe path, return wstring, end with L'\\'
    std::wstring getCurrExePathW();
    
    // get file list of input directory
    void getFilesUnderDirA(const std::string& dir, std::vector<std::string>& buff);
    // get file list of input directory
    void getFilesUnderDirW(const std::wstring& dir, std::vector<std::wstring>& buff);
};