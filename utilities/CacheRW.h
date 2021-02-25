#pragma once

// C++ STL
#include <string>
#include <fstream>
#include <locale>


// write cache from char*/char** wchar_t*/wchar_t**
class Cache_Writer
{
public:
    // parametric constructor
    Cache_Writer(const std::string& str, const std::string& output_filename = "core_cache");
    Cache_Writer(const std::string str_arr[], const int& arr_size, const std::string& output_filename = "core_cache");
    Cache_Writer(const std::wstring& str, const std::wstring& output_filename = L"core_cache");
    Cache_Writer(const std::wstring str_arr[], const int& arr_size, const std::wstring& output_filename = L"core_cache");
    
    // destructor
    ~Cache_Writer();
    
    // write cache
    void write_to(const std::string& filename);
    void write_to(const std::wstring& filename);
    
private:
    std::string *p_CacheBuffA;
    std::wstring *p_CacheBuffW;
    int m_ArrSize;
};