#pragma once

// C++ STL
#include <string>
#include <fstream>

#if defined(_UNICODE) || defined(UNICODE) 
#define T(x) L ## x
#define STRING std::wstring
#define OFSTREAM std::wofstream
#define IFSTREAM std::wifstream
#else // ASCII
#define T(x) x
#define STRING std::string
#define OFSTREAM std::ofstream
#define IFSTREAM std::ifstream
#endif


// write cache from char*/char** wchar_t*/wchar_t**
class Cache_Writer
{
public:
    // parametric constructor
    Cache_Writer(const STRING& str, const STRING& output_filename = T("core_cache"));
    Cache_Writer(const STRING str_arr[], const STRING& output_filename = T("core_cache"));
    
    // destructor
    ~Cache_Writer();
    
    // write cache
    void write_to(const STRING& filename);
    void write_to(const STRING filename[]);
    
private:
    STRING *p_CacheBuff;
    int m_ArrSize;
};