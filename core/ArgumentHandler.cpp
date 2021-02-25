#include "ArgumentHandler.h"

// ====================== ArgumentHandlerW ======================

// constructor
ArgumentHandlerW::ArgumentHandlerW(int argc, wchar_t** argv)
    : m_HasAdd(false), m_HasDel(false),
    m_HasFind(false), m_HasPaste(false),
    m_HasHelp(false), m_WallpaperMode(WallpaperMode::UNKNOW)
{
    try {
        std::wstring buff;
        for (int i = 1; i < argc; ++i) {
            buff.assign(argv[i]);
            if (buff == L"--add") {
                m_FilePath = argv[i + 1];
                m_HasAdd = true;
            }
            if (buff == L"--del") {
                m_FileName = argv[i + 1];
                m_HasDel = true;
            }
            if (buff == L"--find") {
                m_FileName = argv[i + 1];
                m_HasFind = true;
            }
            if (buff == L"--paste") {
                m_FileName = argv[i + 1];
                if (GlobTools::all2upperW(argv[i+2]) == L"PORTRAIT")
                    m_WallpaperMode = WallpaperMode::PORTRAIT;
                else if (GlobTools::all2upperW(argv[i+2]) == L"LANDSCAPE")
                    m_WallpaperMode = WallpaperMode::LANDSCAPE;
                else if (GlobTools::all2upperW(argv[i+2]) == L"DEFAULT")
                    m_WallpaperMode = WallpaperMode::DEFAULT;
                else
                    m_WallpaperMode = WallpaperMode::UNKNOW;
                m_HasPaste = true;
            }
            if (buff == L"--help" || buff == L"-h") {
                m_HasHelp = true;
            }
        }
    } catch (...) {
        throw std::invalid_argument("Argument Invalid or Incomplete");
    }
}

void ArgumentHandlerW::helpMsgBox()
{
    std::wstring msg;
    for (auto it : helpArrW)
        msg += it + L"\n";
    MessageBoxW(0, msg.c_str(), L"Help Information", 0);
}
std::wstring ArgumentHandlerW::helpMsgStr()
{
	std::wstring msg;
	for (auto it : helpArrW)
		msg += it + L"\n";
    return msg;
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


// argument flags
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

// argument validation
bool ArgumentHandlerW::isAddValid()
{
    return (!m_FilePath.empty() && // has FilePath argument
            GlobTools::is_filedir_existW(m_FilePath) // FilePath exist
    );
}
const bool ArgumentHandlerW::isAddValid() const
{
    return (!m_FilePath.empty() && // has FilePath argument
            GlobTools::is_filedir_existW(m_FilePath) // FilePath exist
    );
}
bool ArgumentHandlerW::isDelValid(const WallpaperInfo* wallpaperInfo_arr, const int& arr_size) 
{
    if (!m_FileName.empty() && wallpaperInfo_arr != nullptr) {
        bool is_filename_exist = false;
        for (int i = 0; i < arr_size; ++i) {
            if (wallpaperInfo_arr[i].getNewFilename() == m_FileName) {
                is_filename_exist = true;
                break;
            }
        }
        return is_filename_exist; // FileName exist, valid
    } else return false;
}
const bool ArgumentHandlerW::isDelValid(const WallpaperInfo* wallpaperInfo_arr, const int& arr_size) const
{
    if (!m_FileName.empty() && wallpaperInfo_arr != nullptr) {
        bool is_filename_exist = false;
        for (int i = 0; i < arr_size; ++i) {
            if (wallpaperInfo_arr[i].getNewFilename() == m_FileName) {
                is_filename_exist = true;
                break;
            }
        }
        return is_filename_exist; // FileName exist, valid
    } else return false;
}
bool ArgumentHandlerW::isFindValid()
{
    return (!m_FileName.empty()); // has FileName argument
}
const bool ArgumentHandlerW::isFindValid() const
{
    return (!m_FileName.empty()); // has FileName argument
}
bool ArgumentHandlerW::isPasteValid(const WallpaperInfo* wallpaperInfo_arr, const int& arr_size)
{
    if (!m_FileName.empty() && wallpaperInfo_arr != nullptr) {
        bool is_filename_exist = false;
        for (int i = 0; i < arr_size; ++i) {
            if (wallpaperInfo_arr[i].getNewFilename() == m_FileName) {
                is_filename_exist = true;
                break;
            }
        }
        return (is_filename_exist && // FileName exist
                m_WallpaperMode != WallpaperMode::UNKNOW // WallpaperMode exist
        );
    } else return false;
}
const bool ArgumentHandlerW::isPasteValid(const WallpaperInfo* wallpaperInfo_arr, const int& arr_size) const
{
    if (!m_FileName.empty() && wallpaperInfo_arr != nullptr) {
        bool is_filename_exist = false;
        for (int i = 0; i < arr_size; ++i) {
            if (wallpaperInfo_arr[i].getNewFilename() == m_FileName) {
                is_filename_exist = true;
                break;
            }
        }
        return (is_filename_exist && // FileName exist
                m_WallpaperMode != WallpaperMode::UNKNOW // WallpaperMode exist
        );
    } else return false;
}


// ====================== ArgumentHandlerW End ======================




// ====================== ArgumentHandlerA ======================

// constructor
ArgumentHandlerA::ArgumentHandlerA(int argc, char** argv)
    : m_HasAdd(false), m_HasDel(false),
    m_HasFind(false), m_HasPaste(false),
    m_HasHelp(false), m_WallpaperMode(WallpaperMode::UNKNOW)
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

void ArgumentHandlerA::helpMsgBox()
{
    std::string msg;
    for (auto it : helpArrA)
        msg += it + "\n";
    MessageBoxA(0, msg.c_str(), "Help Information", 0);
}
std::string ArgumentHandlerA::helpMsgStr()
{
    std::string msg;
    for (auto it : helpArrA)
        msg += it + "\n";
    return msg;
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
