#include "ArgumentHandler.h"

// ====================== ArgumentHandlerW ======================

// constructor
ArgumentHandlerW::ArgumentHandlerW(int argc, wchar_t* argv[])
    : m_HasAdd(false), m_HasDel(false),
    m_HasFind(false), m_HasHelp(false)
{
    std::wstring buff;
    for (int i = 1; i < argc; ++i) {
        buff.assign(argv[i]);
        if (buff == L"--add") {
            m_HasAdd = true;
            m_FilePath = argv[i];
        } 
        if (buff == L"--del") {
            m_HasDel = true;
            if (isFileName(argv[i]))
                m_FileName = argv[i];
            else
                m_FileId = argv[i];
        } 
        if (buff == L"--find") {
            m_HasFind = true;
            if (isFileName(argv[i]))
                m_FileName = argv[i];
            else
                m_FileId = argv[i];
        } 
        if (buff == L"--help" || buff == L"-h") {
            
            
            m_HasHelp = true;
        }
    }
}

void ArgumentHandlerW::printHelp()
{
    for (auto it : helpArrW)
        std::wcout << it;
}

// getters
std::wstring ArgumentHandlerW::getFileName()
{
    return m_FileName;
}
std::wstring ArgumentHandlerW::getFilePath()
{
    return m_FilePath;
}
std::wstring ArgumentHandlerW::getFileId()
{
    return m_FileId;
}

bool ArgumentHandlerW::hasAdd()
{
    return m_HasAdd;
}
const bool ArgumentHandlerW::hasAdd() const
{
    return m_HasAdd;
}
bool ArgumentHandlerW::hasDel()
{
    return m_HasDel;
}
const bool ArgumentHandlerW::hasDel() const
{
    return m_HasDel;
}
bool ArgumentHandlerW::hasFind()
{
    return m_HasFind;
}
const bool ArgumentHandlerW::hasFind() const
{
    return m_HasFind;
}
bool ArgumentHandlerW::hasHelp()
{
    return m_HasHelp;
}
const bool ArgumentHandlerW::hasHelp() const
{
    return m_HasHelp;
}

// private function

// check whether a wstring is filename
bool ArgumentHandlerW::isFileName(wchar_t* str)
{
    std::wstring buff(str);
    // found '.' in file
    if (buff.find(L'.') != std::wstring::npos) 
        return true;
    return false;
}


// ====================== ArgumentHandlerW End ======================




// ====================== ArgumentHandlerA ======================

// constructor
ArgumentHandlerA::ArgumentHandlerA(int argc, char* argv[])
    : m_HasAdd(false), m_HasDel(false),
    m_HasFind(false), m_HasHelp(false)
{
    std::string buff;
    for (int i = 1; i < argc; ++i) {
        buff.assign(argv[i]);
        if (argv[i] == "--add") {
            m_HasAdd = true;
            m_FilePath = argv[i];
        } else if (argv[i] == "--del") {
            m_HasDel = true;
            if (isFileName(argv[i]))
                m_FileName = argv[i];
            else
                m_FileId = argv[i];
        } else if (argv[i] == "--find") {
            m_HasFind = true;
            if (isFileName(argv[i]))
                m_FileName = argv[i];
            else
                m_FileId = argv[i];
        } else if (argv[i] == "--help" || argv[i] == "-h") {
            m_HasHelp = true;
        }
    }
}

void ArgumentHandlerA::printHelp()
{
    for (auto it : helpArrA)
        std::cout << it;
}

// getters
std::string ArgumentHandlerA::getFileName()
{
    return m_FileName;
}
std::string ArgumentHandlerA::getFilePath()
{
    return m_FilePath;
}
std::string ArgumentHandlerA::getFileId()
{
    return m_FileId;
}

bool ArgumentHandlerA::hasAdd()
{
    return m_HasAdd;
}
const bool ArgumentHandlerA::hasAdd() const
{
    return m_HasAdd;
}
bool ArgumentHandlerA::hasDel()
{
    return m_HasDel;
}
const bool ArgumentHandlerA::hasDel() const
{
    return m_HasDel;
}
bool ArgumentHandlerA::hasFind()
{
    return m_HasFind;
}
const bool ArgumentHandlerA::hasFind() const
{
    return m_HasFind;
}
bool ArgumentHandlerA::hasHelp()
{
    return m_HasHelp;
}
const bool ArgumentHandlerA::hasHelp() const
{
    return m_HasHelp;
}

// private function

// check whether a string is filename
bool ArgumentHandlerA::isFileName(char* str)
{
    std::string buff(str);
    // found '.' in file
    if (buff.find('.') != std::string::npos) 
        return true;
    return false;
}


// ====================== ArgumentHandlerA End ======================
