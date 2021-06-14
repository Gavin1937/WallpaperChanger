#include "ArgumentHandler.h"

// ====================== ArgumentHandler ======================

// constructor
ArgumentHandler::ArgumentHandler(int argc, wchar_t** argv)
    : m_HasAdd(false), m_HasDel(false),
    m_HasFind(false), m_HasPaste(false),
    m_HasClear(false), m_HasHelp(false), 
    m_WallpaperMode(WallpaperMode::UNKNOW)
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
            if (buff == L"--clear") {
                if (GlobTools::all2upperW(argv[i+1]) == L"ALL")
                    m_ClearMode = ClearMode::ALL;
                else if (GlobTools::all2upperW(argv[i+1]) == L"CACHED_WALLPAPER")
                    m_ClearMode = ClearMode::CACHED_WALLPAPER;
                else if (GlobTools::all2upperW(argv[i+1]) == L"USER_CONFIG")
                    m_ClearMode = ClearMode::USER_CONFIG;
                m_HasClear = true;
            }
            if (buff == L"--help" || buff == L"-h") {
                m_HasHelp = true;
            }
        }
    } catch (...) {
        throw std::invalid_argument("Argument Invalid or Incomplete");
    }
}

void ArgumentHandler::helpMsgBox()
{
    std::wstring msg;
    for (auto it : helpArrW)
        msg += it + L"\n";
    MessageBoxW(0, msg.c_str(), L"Help Information", 0);
}
std::wstring ArgumentHandler::helpMsgStr()
{
	std::wstring msg;
	for (auto it : helpArrW)
		msg += it + L"\n";
    return msg;
}

// getters
std::wstring ArgumentHandler::getFileName()
{
    return m_FileName;
}
std::wstring ArgumentHandler::getFilePath()
{
    return m_FilePath;
}
WallpaperMode ArgumentHandler::getWallpaperMode()
{
    return m_WallpaperMode;
}
ClearMode ArgumentHandler::getClearMode()
{
    return m_ClearMode;
}


// argument flags
bool ArgumentHandler::hasAdd()
{
    return m_HasAdd;
}
const bool ArgumentHandler::hasAdd() const
{
    return m_HasAdd;
}
bool ArgumentHandler::hasDel()
{
    return m_HasDel;
}
const bool ArgumentHandler::hasDel() const
{
    return m_HasDel;
}
bool ArgumentHandler::hasFind()
{
    return m_HasFind;
}
const bool ArgumentHandler::hasFind() const
{
    return m_HasFind;
}
bool ArgumentHandler::hasPaste()
{
    return m_HasPaste;
}
const bool ArgumentHandler::hasPaste() const
{
    return m_HasPaste;
}
bool ArgumentHandler::hasClear()
{
    return m_HasClear;
}
const bool ArgumentHandler::hasClear() const
{
    return m_HasClear;
}
bool ArgumentHandler::hasHelp()
{
    return m_HasHelp;
}
const bool ArgumentHandler::hasHelp() const
{
    return m_HasHelp;
}

// argument validation
bool ArgumentHandler::isAddValid()
{
    return (!m_FilePath.empty() && // has FilePath argument
            GlobTools::is_filedir_existW(m_FilePath) // FilePath exist
    );
}
const bool ArgumentHandler::isAddValid() const
{
    return (!m_FilePath.empty() && // has FilePath argument
            GlobTools::is_filedir_existW(m_FilePath) // FilePath exist
    );
}
bool ArgumentHandler::isDelValid(const WallpaperInfo* wallpaperInfo_arr, const int& arr_size) 
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
const bool ArgumentHandler::isDelValid(const WallpaperInfo* wallpaperInfo_arr, const int& arr_size) const
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
bool ArgumentHandler::isFindValid()
{
    return (!m_FileName.empty()); // has FileName argument
}
const bool ArgumentHandler::isFindValid() const
{
    return (!m_FileName.empty()); // has FileName argument
}
bool ArgumentHandler::isPasteValid(const WallpaperInfo* wallpaperInfo_arr, const int& arr_size)
{
    if (!m_FileName.empty() && wallpaperInfo_arr != nullptr && arr_size > 0) {
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
const bool ArgumentHandler::isPasteValid(const WallpaperInfo* wallpaperInfo_arr, const int& arr_size) const
{
    if (!m_FileName.empty() && wallpaperInfo_arr != nullptr && arr_size > 0) {
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


// ====================== ArgumentHandler End ======================
