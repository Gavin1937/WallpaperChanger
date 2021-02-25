#include "CacheRW.h"


// ====================== Cache_Writer ======================

// parametric constructor
Cache_Writer::Cache_Writer(const std::string& str, const std::string& output_filename)
    : p_CacheBuffA(nullptr), p_CacheBuffW(nullptr), m_ArrSize(0)
{
    if (!str.empty()) {
        // initialize members
        m_ArrSize = 1;
        p_CacheBuffA = new std::string[m_ArrSize];
        // copy
        *p_CacheBuffA = str;
        // write cache file
        this->write_to(output_filename);
    }
}
Cache_Writer::Cache_Writer(const std::string str_arr[], const int& arr_size, const std::string& output_filename)
	: p_CacheBuffA(nullptr), p_CacheBuffW(nullptr), m_ArrSize(0)
{
    if (str_arr != nullptr) {
        // initialize members
        m_ArrSize = arr_size;
        p_CacheBuffA = new std::string[m_ArrSize];
        // copy
        for (int i = 0; i < m_ArrSize; ++i)
            p_CacheBuffA[i] = str_arr[i];
        // write cache file
        this->write_to(output_filename);
    }
}
Cache_Writer::Cache_Writer(const std::wstring& str, const std::wstring& output_filename)
	: p_CacheBuffA(nullptr), p_CacheBuffW(nullptr), m_ArrSize(0)
{
    if (!str.empty()) {
        // initialize members
        m_ArrSize = 1;
        p_CacheBuffW = new std::wstring[m_ArrSize];
        // copy
        *p_CacheBuffW = str;
        // write cache file
        this->write_to(output_filename);
    }
}
Cache_Writer::Cache_Writer(const std::wstring str_arr[], const int& arr_size, const std::wstring& output_filename)
	: p_CacheBuffA(nullptr), p_CacheBuffW(nullptr), m_ArrSize(0)
{
    if (str_arr != nullptr) {
        // initialize members
        m_ArrSize = arr_size;
        p_CacheBuffW = new std::wstring[m_ArrSize];
        // copy
        for (int i = 0; i < m_ArrSize; ++i)
            p_CacheBuffW[i] = str_arr[i];
        // write cache file
        this->write_to(output_filename);
    }
}

// destructor
Cache_Writer::~Cache_Writer()
{
    delete[] p_CacheBuffA;
    delete[] p_CacheBuffW;
    m_ArrSize = 0;
}

// write cache
void Cache_Writer::write_to(const std::string& filename)
{
    if (p_CacheBuffA != nullptr) {
        std::ofstream output;
        output.imbue(std::locale("", LC_CTYPE));
        output.open(filename.c_str());
        for (int i = 0; i < m_ArrSize; ++i)
            output << p_CacheBuffA[i] << '\n';
        output.close();
    }
}
void Cache_Writer::write_to(const std::wstring& filename)
{
    if (p_CacheBuffW != nullptr) {
        std::wofstream output;
        output.imbue(std::locale("", LC_CTYPE));
        output.open(filename.c_str());
        for (int i = 0; i < m_ArrSize; ++i)
            output << p_CacheBuffW[i] << '\n';
        output.close();
    }
}


// ====================== Cache_Writer ======================
