#pragma once

// C++ STL
#include <string>
#include <codecvt>
#include <locale>
#include <fstream>

namespace GlobTools 
{
    // convert wstring & string
    static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvter;
    
    // check whether a directory exist
    bool is_filedir_exist(const std::wstring& dir);
};