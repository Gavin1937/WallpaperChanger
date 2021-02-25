#include "CacheRW.h"


// ====================== Cache_Writer ======================

// parametric constructor
Cache_Writer::Cache_Writer(const STRING& str, const STRING& output_filename)
    : p_CacheBuff(nullptr), m_ArrSize(0)
{
    if (!str.empty()) {
        // initialize members
        m_ArrSize = 1;
        p_CacheBuff = new STRING[m_ArrSize];
        // copy
        p_CacheBuff[0] = str;
        // write cache file
        this->write_to(output_filename);
    }
}
Cache_Writer::Cache_Writer(const STRING str_arr[], const STRING& output_filename)
    : p_CacheBuff(nullptr), m_ArrSize(0)
{
    if (str_arr != nullptr) {
        // initialize members
        m_ArrSize = *(&str_arr + 1) - str_arr;
        p_CacheBuff = new STRING[m_ArrSize];
        // copy
        for (int i = 0; i < m_ArrSize; ++i)
            p_CacheBuff[i] = str_arr[i];
        // write cache file
        this->write_to(output_filename);
    }
}

// destructor
Cache_Writer::~Cache_Writer()
{
    delete[] p_CacheBuff;
    m_ArrSize = 0;
}

// write cache
void Cache_Writer::write_to(const STRING& filename)
{
    if (p_CacheBuff != nullptr) {
        OFSTREAM output(filename.c_str());
        for (int i = 0; i < m_ArrSize; ++i) {
            output << p_CacheBuff[i] << T('\n');
        }
        output.close();
    }
}


// ====================== Cache_Writer ======================
