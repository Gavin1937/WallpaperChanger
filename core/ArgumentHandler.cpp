#include "ArgumentHandler.h"

// ====================== ArgumentHandlerW ======================

// constructor
ArgumentHandlerW::ArgumentHandlerW(int argc, wchar_t* argv[])
    : m_HasAdd(false), m_HasDel(false),
    m_HasFind(false), m_HasPaste(false),
    m_HasHelp(false)
{
    std::wstring buff;
    for (int i = 1; i < argc; ++i) {
        buff.assign(argv[i]);
        if (buff == L"--add") {
            m_HasAdd = true;
            m_FilePath = argv[i+1];
        } 
        if (buff == L"--del") {
            m_HasDel = true;
            m_FileName = argv[i+1];
        } 
        if (buff == L"--find") {
            m_HasFind = true;
            m_FileName = argv[i+1];
        } 
        if (buff == L"--paste") {
            m_HasPaste = true;
            m_FileName = argv[i+1];
            if (GlobTools::all2upperW(argv[i+2]) == L"PORTRAIT")
                m_WallpaperMode = WallpaperMode::PORTRAIT;
            else if (GlobTools::all2upperW(argv[i+2]) == L"LANDSCAPE")
                m_WallpaperMode = WallpaperMode::LANDSCAPE;
            else
                m_WallpaperMode = WallpaperMode::UNKNOW;
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
WallpaperMode ArgumentHandlerW::getWallpaperMode()
{
    return m_WallpaperMode;
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
bool ArgumentHandlerW::hasPaste()
{
    return m_HasPaste;
}
const bool ArgumentHandlerW::hasPaste() const
{
    return m_HasPaste;
}
bool ArgumentHandlerW::hasHelp()
{
    return m_HasHelp;
}
const bool ArgumentHandlerW::hasHelp() const
{
    return m_HasHelp;
}


// ====================== ArgumentHandlerW End ======================




// ====================== ArgumentHandlerA ======================

// constructor
ArgumentHandlerA::ArgumentHandlerA(int argc, char* argv[])
    : m_HasAdd(false), m_HasDel(false),
    m_HasFind(false), m_HasPaste(false),
    m_HasHelp(false)
{
    std::string buff;
    for (int i = 1; i < argc; ++i) {
        buff.assign(argv[i]);
        if (buff == "--add") {
            m_HasAdd = true;
            m_FilePath = argv[i+1];
        } 
        if (buff == "--del") {
            m_HasDel = true;
            m_FileName = argv[i+1];
        } 
        if (buff == "--find") {
            m_HasFind = true;
            m_FileName = argv[i+1];
        } 
        if (buff == "--paste") {
            m_HasPaste = true;
            m_FileName = argv[i+1];
        }
        if (buff == "--help" || buff == "-h") {
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
WallpaperMode ArgumentHandlerA::getWallpaperMode()
{
    return m_WallpaperMode;
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
bool ArgumentHandlerA::hasPaste()
{
    return m_HasPaste;
}
const bool ArgumentHandlerA::hasPaste() const
{
    return m_HasPaste;
}
bool ArgumentHandlerA::hasHelp()
{
    return m_HasHelp;
}
const bool ArgumentHandlerA::hasHelp() const
{
    return m_HasHelp;
}


// ====================== ArgumentHandlerA End ======================
