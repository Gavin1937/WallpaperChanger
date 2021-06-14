#include "ConfigManager.h"


// ====================== Public Functions ======================

STRING all2upper(const STRING& str)
{
    STRING output = str;
    for (auto &it : output)
        it = std::toupper(it);
    return output;
}
STRING all2lower(const STRING& str)
{
    STRING output = str;
    for (auto &it : output)
        it = std::tolower(it);
    return output;
}


// ====================== Public Functions End ======================





// ====================== Option ======================

// default constructor
Option::Option()
    : m_Key(STRING()), m_Val(STRING())
{}
// parametric constructor
Option::Option(const STRING& key, const STRING& val)
    : m_Key(key), m_Val(val)
{
    m_Key = key;
    m_Val = val;
}
// construct obj with whole option line (key = val)
Option::Option(const STRING& whole_opt_line)
    : m_Key(STRING()), m_Val(STRING())
{
    // clear spaces in begin & end, |--->X
    size_t beg, end;
    for (beg = 0; beg < whole_opt_line.size(); ++beg) {
        if (whole_opt_line[beg] != T(' '))
            break;
    }
    // X<---|
    for (end = whole_opt_line.size()-1; end >= 0; --end) {
        if (whole_opt_line[end] != T(' ')) {
            end++;
            break;
        }
    }
    // clear spaces around equal sign
    if (is_good_opt(whole_opt_line)) {
        auto equal_sign = whole_opt_line.find(T("="));
        // to left: X<---=
        for (auto it = whole_opt_line.begin()+equal_sign-1;
            it >= whole_opt_line.begin(); it--) {
            if (*it != T(' ')) {
                m_Key.assign(whole_opt_line.begin()+beg, (it+1));
                break;
            }
        }
        // to right: =--->X
        for (auto it = whole_opt_line.begin() + equal_sign+1;
            it != whole_opt_line.end(); it++) {
            if (*it != T(' ')) {
                m_Val.assign(it, whole_opt_line.begin()+end);
                break;
            }
        }
    }
}
// copy constructor
Option::Option(const Option& obj)
{
    m_Key = obj.m_Key;
    m_Val = obj.m_Val;
}

// compare operator
bool Option::operator==(const Option& obj)
{
    return (m_Key == obj.m_Key &&
            m_Val == obj.m_Val);
}
bool Option::operator!=(const Option& obj)
{
    return (m_Key != obj.m_Key &&
            m_Val != obj.m_Val);
}

// getter 
STRING Option::getKey()
{
    return m_Key;
}
const STRING Option::getKey() const
{
    return m_Key;
}
STRING Option::getVal()
{
    return m_Val;
}
const STRING Option::getVal() const
{
    return m_Val;
}

// get a STRING output like "Key = Val"
STRING Option::getStr()
{
    if (!m_Key.empty())
        return STRING(m_Key + T(" = ") + m_Val);
    else return STRING();
}
const STRING Option::getStr() const
{
    return STRING(m_Key + T(" = ") + m_Val);
}

// setters
void Option::setKey(const STRING& key)
{
    m_Key = key;
}
void Option::setVal(const STRING& val)
{
    m_Val = val;
}

// private:

// checking a option line string
bool Option::is_good_opt(const STRING& opt_line)
{
    auto equal_sign = opt_line.find(T("="));
    // find "=" in string line
    if (equal_sign != STRING::npos) {
        // make sure there are only ONE equal sign
        if (opt_line.find(T("="), equal_sign+1) == STRING::npos)
            return true;
        else return false;
    }
    else return false;
}


// ====================== Option End ======================





// ====================== Section ======================

// default constructor
Section::Section()
    : m_Opts(std::unordered_map<STRING, Option>())
{}
// copy constructor
Section::Section(const Section& obj)
    : m_Opts(std::unordered_map<STRING, Option>())
{
    for (auto it : obj.m_Opts)
        m_Opts.insert(m_Opts.end(), it);
}

// getters

// get Option object
Option Section::getOpt(const STRING& opt_name)
{
    if (isOptExist(opt_name))
        return m_Opts[opt_name];
    return Option();
}
// get Option's value
STRING Section::getOptVal(const STRING& opt_name)
{
    if (isOptExist(opt_name))
        return m_Opts[opt_name].getVal();
    return STRING();
}

// get a copy of whole section data
std::unordered_map<STRING, Option> Section::getOptions()
{
    return m_Opts;
}

// setters
void Section::setOpt(const STRING& opt_name, const STRING& new_opt_val)
{
    if (isOptExist(opt_name))
        m_Opts[opt_name].setVal(new_opt_val);
}
void Section::setOpt(const STRING& opt_name, const Option& new_opt)
{
    if (isOptExist(opt_name))
        m_Opts[opt_name] = new_opt;
}

// modifiers
void Section::addOpt(const Option& new_opt)
{
    m_Opts.insert(m_Opts.end(), std::pair<STRING, Option>(new_opt.getKey(), new_opt));
}
void Section::removeOpt(const STRING& opt_name)
{
    if (isOptExist(opt_name))
        m_Opts.erase(opt_name);
}

// other functions
bool Section::isOptExist(const STRING& opt_name)
{
    // find option
    if (m_Opts.find(opt_name) != m_Opts.end())
        return true;
    // cannot find option
    else return false;
}


// ====================== Section End ======================





// ====================== ConfigManager ======================

// Default constructor
ConfigManager::ConfigManager(const bool& auto_write_flag)
    : m_Secs(std::unordered_map<STRING, Section>()), m_ConfigPath(STRING()),
    m_AutoWrite_flag(auto_write_flag)
{}
// parametric constructor
ConfigManager::ConfigManager(const STRING& config_path, const bool& auto_write_flag)
    : m_Secs(std::unordered_map<STRING, Section>()), m_ConfigPath(STRING()),
    m_AutoWrite_flag(auto_write_flag)
{
    // checking input config_path
    if (is_file_exist(config_path))
        this->read(config_path);
    else
        throw std::invalid_argument((
            "exception: file does not exist or is a directory. \n" + 
            getStr4Exception(config_path)
        ).c_str());
}

// destructor
ConfigManager::~ConfigManager()
{
    auto_update();
    // destruct
    m_Secs.clear();
    m_ConfigPath.~basic_string();
}

// config read & write
void ConfigManager::read(const STRING& config_path)
{
    if (is_file_exist(config_path)) {
        IFSTREAM input;
        input.imbue(std::locale("", LC_CTYPE));
        input.open(config_path);
        STRING buff;
        if (input.fail()) {
            throw std::invalid_argument((
                "exception: fail to open config file: \n" +
                getStr4Exception(config_path)
            ).c_str());
        } else {
            m_ConfigPath = config_path;
            STRING curr_sec_name;
            while (getline(input, buff)) {
                if (buff.empty()) // skip blank line
                    continue;
                else if (is_sec_line(buff)) {
                    curr_sec_name.assign(buff.begin()+buff.find(T("["))+1, buff.begin()+buff.find(T("]")));
                    addSection(curr_sec_name, Section());
                } else if (is_opt_line(buff)) {
                    addOption(curr_sec_name, Option(buff));
                } else {
                    throw std::invalid_argument((
                        "exception: bad line in config file, not a section or option. \n" +
                        getStr4Exception(buff)
                    ).c_str());
                }
            }
        }
    }
}
void ConfigManager::write(const STRING& write_path)
{
    OFSTREAM output;
    output.imbue(std::locale("", LC_CTYPE));
    output.open(write_path);
    for (auto it_sec : m_Secs) {
        output << T("[") << it_sec.first << T("]\n");
        for (auto it_opt : it_sec.second.getOptions())
            output << it_opt.second.getStr() << T("\n");
        output << T("\n");
    }
    output.close();
    if (m_ConfigPath.empty())
        m_ConfigPath = write_path;
}

// getter
STRING ConfigManager::get(const STRING& sec_name, const STRING& opt_name)
{
    try {
        return m_Secs[sec_name].getOptVal(opt_name);
    } catch(...) {
        throw std::invalid_argument((
            "exception: cannot read {" +
            getStr4Exception(sec_name) + "},[" +
            getStr4Exception(opt_name) + "]"
        ).c_str());
    }
}
int ConfigManager::getInt(const STRING& sec_name, const STRING& opt_name)
{
    int output;
    try {
        output = std::stoi(this->get(sec_name, opt_name));
    } catch(...) {
        throw std::invalid_argument((
            "exception: cannot convert {" +
            getStr4Exception(sec_name) + "},[" +
            getStr4Exception(opt_name) + "],(" +
            getStr4Exception(this->get(sec_name, opt_name)) +
            ") to integer"
        ).c_str());
    }
    return output;
}
double ConfigManager::getDouble(const STRING& sec_name, const STRING& opt_name)
{
    double output;
    try {
        output = std::stod(this->get(sec_name, opt_name));
    } catch(...) {
        throw std::invalid_argument((
            "exception: cannot convert {" +
            getStr4Exception(sec_name) + "},[" +
            getStr4Exception(opt_name) + "],(" +
            getStr4Exception(this->get(sec_name, opt_name)) +
            ") to double (floating number)"
        ).c_str());
    }
    return output;
}
bool ConfigManager::getBool(const STRING& sec_name, const STRING& opt_name)
{
    STRING val = this->get(sec_name, opt_name);
    // string "true"/"false" represent true/false
    if (all2upper(val) == T("TRUE"))
        return true;
    else if (all2upper(val) == T("FALSE"))
        return false;
    // true/false is representing by number
    else {
        // declare buffers
        int i_val = 0;
        double d_val = 0.0;
        try { // try to read current opt as integer
            i_val = this->getInt(sec_name, opt_name);
        } catch(...) { // fail
            try { // try to read current opt as double
                d_val = this->getDouble(sec_name, opt_name);
            } catch (...) { // fail, throw exception
                throw std::invalid_argument((
                    "exception: cannot convert {" +
                    getStr4Exception(sec_name) + "},[" +
                    getStr4Exception(opt_name) + "],(" +
                    getStr4Exception(this->get(sec_name, opt_name)) +
                    ") to boolean"
                ).c_str());
            }
            // able to read, convert buffer to bool
            return static_cast<bool>(d_val);
        }
        // able to read, convert buffer to bool
        return static_cast<bool>(i_val);
    }
    return false;
}

// access data
std::unordered_map<STRING, Section> ConfigManager::getSections()
{
    return m_Secs;
}
std::unordered_map<STRING, Option> ConfigManager::getOptions(const STRING& sec_name)
{
    if (isSecExist(sec_name)) {
        return m_Secs[sec_name].getOptions();
    } else {
        throw std::invalid_argument((
            "exception: input section name (" +
            getStr4Exception(sec_name) +
            ") does not exist"
        ).c_str());
    }
}
Option ConfigManager::getOptObj(const STRING& sec_name, const STRING& opt_name)
{
    if (isSecExist(sec_name)) {
        if (isOptExist(sec_name, opt_name)) {
            return m_Secs[sec_name].getOpt(opt_name);
        } else {
            throw std::invalid_argument((
                "exception: input option name (" +
                getStr4Exception(opt_name) +
                ") does not exist"
            ).c_str());
        }
    } else {
        throw std::invalid_argument((
            "exception: input section name (" +
            getStr4Exception(sec_name) +
            ") does not exist"
        ).c_str());
    }
}

// setter
void ConfigManager::set(const STRING& sec_name, const STRING& opt_name, const STRING& val)
{
    if (isSecExist(sec_name)) {
        if (isOptExist(sec_name, opt_name))
            m_Secs[sec_name].setOpt(opt_name, val);
        else {
            throw std::invalid_argument((
                "exception: input option name (" +
                getStr4Exception(opt_name) +
                ") does not exist"
            ).c_str());
        }
    } else {
        throw std::invalid_argument((
            "exception: input section name (" +
            getStr4Exception(sec_name) +
            ") does not exist"
        ).c_str());
    }
    auto_update();
}
void ConfigManager::setSection(const STRING& sec_name, const Section& new_sec)
{
    if (isSecExist(sec_name))
        m_Secs[sec_name] = new_sec;
    else {
        throw std::invalid_argument((
            "exception: input section name (" +
            getStr4Exception(sec_name) +
            ") does not exist"
        ).c_str());
    }
    auto_update();
}
void ConfigManager::setOption(const STRING& sec_name, const STRING& opt_name, const Option& new_opt)
{
    if (isSecExist(sec_name)) {
        if (isOptExist(sec_name, opt_name))
            m_Secs[sec_name].setOpt(opt_name, new_opt);
        else {
            throw std::invalid_argument((
                "exception: input option name (" +
                getStr4Exception(opt_name) +
                ") does not exist"
            ).c_str());
        }
    } else {
        throw std::invalid_argument((
            "exception: input section name (" +
            getStr4Exception(sec_name) +
            ") does not exist"
        ).c_str());
    }
    auto_update();
}

// modifier
void ConfigManager::addSection(const STRING& sec_name, const Section& new_sec)
{
    if (!isSecExist(sec_name)) { // checking sec_name
        m_Secs.insert(m_Secs.end(), std::pair<STRING, Section>(sec_name, new_sec));
    } else {
        throw std::invalid_argument((
            "exception: input section name (" +
            getStr4Exception(sec_name) +
            ") already exist"
        ).c_str());
    }
    auto_update();
}
void ConfigManager::removeSection(const STRING& sec_name)
{
    if (isSecExist(sec_name)) { // checking sec_name
        m_Secs.erase(m_Secs.find(sec_name));
    } else {
        throw std::invalid_argument((
            "exception: input section name (" +
            getStr4Exception(sec_name) +
            ") does not exist"
        ).c_str());
    }
}
void ConfigManager::addOption(const STRING& sec_name, const Option& new_opt)
{
    if (isSecExist(sec_name)) {
        // make sure no duplicate Option
        if (!isOptExist(sec_name, new_opt.getKey())) {
            m_Secs[sec_name].addOpt(new_opt);
        } else {
            throw std::invalid_argument((
                "exception: input option name (" +
                getStr4Exception(new_opt.getKey()) +
                ") already exist"
            ).c_str());
        }
    } else {
        throw std::invalid_argument((
            "exception: input section name (" +
            getStr4Exception(sec_name) +
            ") does not exist"
        ).c_str());
    }
    auto_update();
}
void ConfigManager::removeOption(const STRING& sec_name, const STRING& opt_name)
{
    if (isSecExist(sec_name)) {
        if (isOptExist(sec_name, opt_name)) {
            m_Secs[sec_name].removeOpt(opt_name);
        } else {
            throw std::invalid_argument((
                "exception: input option name (" +
                getStr4Exception(opt_name) +
                ") does not exist"
            ).c_str());
        }
    } else {
        throw std::invalid_argument((
            "exception: input section name (" +
            getStr4Exception(sec_name) +
            ") does not exist"
        ).c_str());
    }
}
void ConfigManager::add(const STRING& sec_name, const STRING& opt_name, const STRING& val)
{
    if (isSecExist(sec_name)) {
        if (isOptExist(sec_name, opt_name)) {
            m_Secs[sec_name].setOpt(opt_name, val);
        } else {
            m_Secs[sec_name].addOpt(Option(opt_name, val));
        }
    } else {
        m_Secs.insert(m_Secs.end(), std::pair<STRING, Section>(sec_name, Section()));
        m_Secs[sec_name].addOpt(Option(opt_name, val));
    }
    auto_update();
}

// existence checking
bool ConfigManager::isSecExist(const STRING& sec_name)
{
    // find section
    if (m_Secs.find(sec_name) != m_Secs.end())
        return true;
    // cannot find section
    else return false;
}
bool ConfigManager::isOptExist(const STRING& sec_name, const STRING& opt_name)
{
    if (isSecExist(sec_name)) {
        return m_Secs[sec_name].isOptExist(opt_name);
    } else return false;
}


// private functions

// existence checking
bool ConfigManager::is_file_exist(const STRING& file_path)
{
    return !(std::filesystem::is_directory(file_path));
}

// line checking for reading file
bool ConfigManager::is_sec_line(const STRING& sec_line)
{
    size_t beg = sec_line.find(T("["));
    size_t end = sec_line.find(T("]"));
    // make sure find both beg & end
    if (beg != STRING::npos && end != STRING::npos) {
        // make sure beg is in front of end
        return (beg <= end);
    } // missing one square bracket
    else return false;
}
bool ConfigManager::is_opt_line(const STRING& opt_line)
{
    auto equal_sign = opt_line.find(T("="));
    // find "=" in string line
    if (equal_sign != STRING::npos) {
        // make sure there are only ONE equal sign
        if (opt_line.find(T("="), equal_sign+1) == STRING::npos)
            return true;
        else return false;
    }
    else return false;
}

// auto update config file if auto_write_flag set to true
void ConfigManager::auto_update()
{
    if (m_AutoWrite_flag && !m_ConfigPath.empty())
        this->write(m_ConfigPath);
}


// ====================== ConfigManager End ======================