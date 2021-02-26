#pragma once

// C++ STL
#include <string>
#include <vector>
#include <fstream>
#include <locale>
#include <filesystem>


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


// read cache from cache file
class Cache_ReaderA
{
public:
    // constructor
    Cache_ReaderA(const std::string& cache_filename = "core_cache", const bool& enable_auto_delete = true);
    // destructor
    ~Cache_ReaderA();
    
    // access data
    std::vector<std::string>* getData();
    
    bool isCacheExist();
    
private:
    void delete_cache_file();
    
private:
    std::vector<std::string> *p_CacheBuff;
    std::string m_CacheFileName;
    bool m_EnableAutoDelete;
    bool m_FileExist;
};


// read cache from cache file
class Cache_ReaderW
{
public:
    // constructor
    Cache_ReaderW(const std::wstring& cache_filename = L"core_cache", const bool& enable_auto_delete = true);
    // destructor
    ~Cache_ReaderW();
    
    // access data
    std::vector<std::wstring>* getData();
    
    bool isCacheExist();
    
private:
    void delete_cache_file();
    
private:
    std::vector<std::wstring> *p_CacheBuff;
    std::wstring m_CacheFileName;
    bool m_EnableAutoDelete;
    bool m_FileExist;
};