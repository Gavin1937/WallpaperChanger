
// Configuration Manager Library for C++,
// Require C++ 17 or above

#pragma once

// C++ STL
#include <string>
#include <fstream>
#include <unordered_map>
#include <filesystem>
#include <stdexcept>
#include <codecvt>
#include <locale>

#if defined(_UNICODE) || defined(UNICODE) 
#define T(x) L ## x
#define STRING std::wstring
#define FSTREAM std::wfstream
#define IFSTREAM std::wifstream
#define OFSTREAM std::wofstream
#else // ASCII
#define T(x) x
#define STRING std::string
#define FSTREAM std::fstream
#define IFSTREAM std::ifstream
#define OFSTREAM std::ofstream
#endif

// class definitions
class Option;
class Section;
class ConfigManager;

// function definitions
STRING all2upper(const STRING& str);
STRING all2lower(const STRING& str);

// glob variables
namespace {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvter;
}

// basic unit in config file
class Option
{
public:
    // default constructor
    Option();
    // parametric constructor
    Option(const STRING& key, const STRING& val);
    // construct obj with whole option line (key = val)
    Option(const STRING& whole_opt_line);
    // copy constructor
    Option(const Option& obj);
    
    // compare operator
    bool operator==(const Option& obj);
    bool operator!=(const Option& obj);
    
    // getter 
    STRING getKey();
    const STRING getKey() const;
    STRING getVal();
    const STRING getVal() const;
    
    // get a STRING output like "Key = Val"
    STRING getStr();
    const STRING getStr() const;
    
    // setters
    void setKey(const STRING& key);
    void setVal(const STRING& val);
    
private:
    // checking a option line string
    bool is_good_opt(const STRING& opt_line);
    
private:
    STRING m_Key;
    STRING m_Val;
};


// section chunk of a config file
class Section
{
public:
    // default constructor
    Section();
    // copy constructor
    Section(const Section& obj);
    
    // getters
    
    // get Option object
    Option getOpt(const STRING& opt_name);
    // get Option's value
    STRING getOptVal(const STRING& opt_name);
    
    // get a copy of whole section data
    std::unordered_map<STRING, Option> getOptions();
    
    // setters
    void setOpt(const STRING& opt_name, const STRING& new_opt_val);
    void setOpt(const STRING& opt_name, const Option& new_opt);
    
    // modifiers
    void addOpt(const Option& new_opt);
    void removeOpt(const STRING& opt_name);
    
    // other functions
    bool isOptExist(const STRING& opt_name);
    
private:
    std::unordered_map<STRING, Option> m_Opts;
};


class ConfigManager
{
public:
    // Default constructor
    ConfigManager(const bool& auto_write_flag = true);
    // parametric constructor
    ConfigManager(const STRING& config_path, const bool& auto_write_flag = true);
    // disable copy constructor
    ConfigManager(const ConfigManager& obj) = delete;
    
    // destructor
    ~ConfigManager();
    
    // config read & write
    void read(const STRING& config_path);
    void write(const STRING& write_path);
    
    // getter
    STRING get(const STRING& sec_name, const STRING& opt_name);
    int getInt(const STRING& sec_name, const STRING& opt_name);
    double getDouble(const STRING& sec_name, const STRING& opt_name);
    bool getBool(const STRING& sec_name, const STRING& opt_name);
    
    // access data
    std::unordered_map<STRING, Section> getSections();
    std::unordered_map<STRING, Option> getOptions(const STRING& sec_name);
    Option getOptObj(const STRING& sec_name, const STRING& opt_name);
    
    // setter
    void set(const STRING& sec_name, const STRING& opt_name, const STRING& val);
    void setSection(const STRING& sec_name, const Section& new_sec);
    void setOption(const STRING& sec_name, const STRING& opt_name, const Option& new_opt);
    
    // modifier
    void addSection(const STRING& sec_name, const Section& new_sec);
    void removeSection(const STRING& sec_name);
    void addOption(const STRING& sec_name, const Option& new_opt);
    void removeOption(const STRING& sec_name, const STRING& opt_name);
    void add(const STRING& sec_name, const STRING& opt_name, const STRING& val);
    
    // existence checking
    bool isSecExist(const STRING& sec_name);
    bool isOptExist(const STRING& sec_name, const STRING& opt_name);
    
private:
    // existence checking
    bool is_file_exist(const STRING& file_path);
    
    // line checking for reading file
    bool is_sec_line(const STRING& sec_line);
    bool is_opt_line(const STRING& opt_line);
    
    // auto update config file if auto_write_flag set to true
    void auto_update();
    
private:
    std::unordered_map<STRING, Section> m_Secs;
    STRING m_ConfigPath;
    bool m_AutoWrite_flag;
};
