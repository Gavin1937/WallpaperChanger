#include "CacheRW.h"


// ====================== Public Functions ======================

bool CleanCache(const std::string& cache_filepath)
{
    if (std::filesystem::exists(cache_filepath))
        return std::filesystem::remove(cache_filepath);
    else return false;
}
bool CleanCache(const std::wstring& cache_filepath)
{
    if (std::filesystem::exists(cache_filepath))
        return std::filesystem::remove(cache_filepath);
    else return false;
}


// ====================== Public Functions End ======================





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


// ====================== Cache_Writer End ======================





// ====================== Cache_ReaderA ======================

// constructor
Cache_ReaderA::Cache_ReaderA(const std::string& cache_filename, const bool& enable_auto_delete)
    : p_CacheBuff(nullptr), m_CacheFileName(cache_filename), m_EnableAutoDelete(enable_auto_delete), m_FileExist(false)
{
    if (std::filesystem::exists(m_CacheFileName)) {
        m_FileExist = true;
        std::ifstream input;
        input.imbue(std::locale("", LC_CTYPE));
        input.open(m_CacheFileName);
        std::string buff;
        p_CacheBuff = new std::vector<std::string>();
        while(std::getline(input, buff)) {
            p_CacheBuff->push_back(buff);
            buff.clear();
        }
        // delete cache file after finish reading
        input.close();
        CleanCache(m_CacheFileName);
    }
}
// destructor
Cache_ReaderA::~Cache_ReaderA()
{
    if (m_EnableAutoDelete && m_FileExist)
        CleanCache(m_CacheFileName);
    delete p_CacheBuff;
    m_CacheFileName.~basic_string();
    m_EnableAutoDelete = NULL;
    m_FileExist = NULL;
}

// access data
const std::vector<std::string>* Cache_ReaderA::getData() const
{
    return p_CacheBuff;
}

bool Cache_ReaderA::isCacheExist()
{
    return (p_CacheBuff != nullptr);
}


// ====================== Cache_ReaderA End ======================





// ====================== Cache_ReaderW ======================

// constructor
Cache_ReaderW::Cache_ReaderW(const std::wstring& cache_filename, const bool& enable_auto_delete)
    : p_CacheBuff(nullptr), m_CacheFileName(cache_filename), m_EnableAutoDelete(enable_auto_delete), m_FileExist(false)
{
    if (std::filesystem::exists(m_CacheFileName)) {
        m_FileExist = true;
        std::wifstream input;
        input.imbue(std::locale("", LC_CTYPE));
        input.open(m_CacheFileName);
        std::wstring buff;
        p_CacheBuff = new std::vector<std::wstring>();
        while(std::getline(input, buff)) {
            p_CacheBuff->push_back(buff);
            buff.clear();
        }
        // delete cache file after finish reading
        input.close();
        CleanCache(m_CacheFileName);
    }
}
// destructor
Cache_ReaderW::~Cache_ReaderW()
{
    if (m_EnableAutoDelete && m_FileExist)
        CleanCache(m_CacheFileName);
    delete p_CacheBuff;
    m_CacheFileName.~basic_string();
    m_EnableAutoDelete = NULL;
    m_FileExist = NULL;
}

// access data
const std::vector<std::wstring>* Cache_ReaderW::getData() const
{
    return p_CacheBuff;
}

bool Cache_ReaderW::isCacheExist()
{
    return (p_CacheBuff != nullptr);
}


// ====================== Cache_ReaderW End ======================