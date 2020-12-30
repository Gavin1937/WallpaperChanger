#include "WallpaperManager.h"
#include "GlobTools.h"

namespace {
    std::vector<std::wstring> SupportedImageFileExtension = {
        L"bmp",
        L"jpg", L"jpeg",
        L"png"
    };
};



// ====================== WallpaperManager ======================

// default constructor
WallpaperManager::WallpaperManager()
    : m_WallpaperList_path(L""), m_CachedWallpaperInfo(std::vector<WallpaperInfo>())
{
    // get current directory
    wchar_t temp_buff[MAX_PATH];
    HMODULE hModule = GetModuleHandleW(NULL);
    if (hModule != NULL) {
        // assign current directory to m_WallpaperList_path
        GetModuleFileNameW(hModule, temp_buff, (sizeof(temp_buff)));
        m_WallpaperList_path.assign(temp_buff);
        reverse(m_WallpaperList_path.begin(), m_WallpaperList_path.end());
        size_t offset = m_WallpaperList_path.find(L'\\');
        m_WallpaperList_path.assign(m_WallpaperList_path.begin()+offset, m_WallpaperList_path.end());
        reverse(m_WallpaperList_path.begin(), m_WallpaperList_path.end());
        m_WallpaperList_path += L"Wallpapers\\";
        if (CreateDirectoryW(m_WallpaperList_path.c_str(), NULL) != ERROR_PATH_NOT_FOUND) {
            if (!GlobTools::is_filedir_existW(std::wstring(m_WallpaperList_path+L"WallpaperList"))) {
                // create ./Wallpapers/ folder
                // create WallpaperList file app
                std::wofstream WallpaperList(GlobTools::cvter.to_bytes(m_WallpaperList_path+L"WallpaperList").c_str(), std::ios::app);
                WallpaperList.close();
            } else {
                // load data from WallpaperList to m_CachedWallpaperInfo
                std::wifstream input(GlobTools::cvter.to_bytes(m_WallpaperList_path+L"WallpaperList").c_str());
                std::wstring buff;
                int counter = 0;
                std::wstring loc_src_path, loc_new_filename, loc_add_time;
                size_t pos1 = 0;
                size_t pos2 = 0;
                while (std::getline(input, buff)) {
                    pos1 = 0; pos2 = 0;
                    if (counter > 0) { // skip 1st line in file (title line for cvs file)
                        // find src_path
                        pos1 = buff.find(L"\"");
                        pos2 = buff.find(L"\"", pos1+1);
                        if (pos1 != std::wstring::npos && pos2 != std::wstring::npos)
                            loc_src_path.assign(buff.begin()+pos1+1, buff.begin()+pos2);
                        // find new_filename
                        pos1 = buff.find(L"\"", pos2+1);
                        pos2 = buff.find(L"\"", pos1+1);
                        if (pos1 != std::wstring::npos && pos2 != std::wstring::npos)
                            loc_new_filename.assign(buff.begin()+pos1+1, buff.begin()+pos2);
                        // find add_time
                        pos1 = buff.find(L"\"", pos2+1);
                        pos2 = buff.find(L"\"", pos1+1);
                        if (pos1 != std::wstring::npos && pos2 != std::wstring::npos)
                            loc_add_time.assign(buff.begin()+pos1+1, buff.begin()+pos2);
                        if (pos1 != std::wstring::npos && pos2 != std::wstring::npos)
                            m_CachedWallpaperInfo.push_back(WallpaperInfo(loc_src_path, loc_new_filename, std::stoi(loc_add_time)));
                    } else counter++;
                }
            }
        } else MessageBoxW(0, L"CreateDirectory Error, Path Not Found.", L"Info", 0);
    } else {
        return;
    }
}
// parametric constructor
WallpaperManager::WallpaperManager(const std::wstring& WallpaperList_path)
    : m_WallpaperList_path(WallpaperList_path), m_CachedWallpaperInfo(std::vector<WallpaperInfo>())
{
    // add \\ to m_WallpaperList_path if need
    if (m_WallpaperList_path[m_WallpaperList_path.size()-1] != L'\\')
        m_WallpaperList_path += L'\\';
    // load data from WallpaperList to m_CachedWallpaperInfo
    std::wifstream input(GlobTools::cvter.to_bytes(m_WallpaperList_path+L"WallpaperList").c_str());
    std::wstring buff;
    int counter = 0;
    std::wstring loc_src_path, loc_new_filename, loc_add_time;
    size_t pos1 = 0;
    size_t pos2 = 0;
    while (std::getline(input, buff)) {
        pos1 = 0; pos2 = 0;
        if (counter > 0) { // skip 1st line in file (title line for cvs file)
            // find src_path
            pos1 = buff.find(L"\"");
            pos2 = buff.find(L"\"", pos1+1);
            if (pos1 != std::wstring::npos && pos2 != std::wstring::npos)
                loc_src_path.assign(buff.begin()+pos1+1, buff.begin()+pos2);
            // find new_filename
            pos1 = buff.find(L"\"", pos2+1);
            pos2 = buff.find(L"\"", pos1+1);
            if (pos1 != std::wstring::npos && pos2 != std::wstring::npos)
                loc_new_filename.assign(buff.begin()+pos1+1, buff.begin()+pos2);
            // find add_time
            pos1 = buff.find(L"\"", pos2+1);
            pos2 = buff.find(L"\"", pos1+1);
            if (pos1 != std::wstring::npos && pos2 != std::wstring::npos)
                loc_add_time.assign(buff.begin()+pos1+1, buff.begin()+pos2);
            if (pos1 != std::wstring::npos && pos2 != std::wstring::npos)
                m_CachedWallpaperInfo.push_back(WallpaperInfo(loc_src_path, loc_new_filename, std::stoi(loc_add_time)));
        } else counter++;
    }
}

// destructor, also write & save all data in m_CachedWallpaperInfo to WallpaperList
WallpaperManager::~WallpaperManager()
{
    // writing data to WallpaperList file (overwrite)
    std::wofstream WallpaperList(GlobTools::cvter.to_bytes(m_WallpaperList_path+L"WallpaperList").c_str());
    WallpaperList << L"\"src_path\",\"hex_id\",\"add_time\"\n"; // initialize 1st line
    for (auto it : m_CachedWallpaperInfo) {
        WallpaperList 
                << L"\"" << it.getSrcPath() << L"\","
                << L"\"" << it.getNewFilename() << L"\","
                << L"\"" << it.getAddTime() << L"\"\n";
    }
    WallpaperList.close();
    // destruct members
    m_WallpaperList_path.~basic_string();
    m_CachedWallpaperInfo.~vector();
}

// copy file from src_path to ./Wallpapers/
bool WallpaperManager::copy_wallpaper_2_cacheFolder(const std::wstring& src_path)
{
    if (GlobTools::is_filedir_existW(src_path)) {
        append_WallpaperList(src_path);
        if (!CopyFileW(src_path.c_str(), 
                (m_WallpaperList_path+m_CachedWallpaperInfo[m_CachedWallpaperInfo.size()-1].getNewFilename()).c_str(), 
                FALSE))
            return false;
    } else {
        return false;
    } 
    return true;
}

// appent & update to WallpaperList file cached in m_CachedWallpaperInfo
void WallpaperManager::append_WallpaperList(const std::wstring& src_path)
{
    std::wstring last_filename;
    if (!m_CachedWallpaperInfo.empty())
        last_filename = m_CachedWallpaperInfo[m_CachedWallpaperInfo.size()-1].getNewFilename();
    else last_filename = L"000000";
    std::wstringstream wss;
    wss << std::hex << stoi(last_filename, nullptr, 16)+1;
    std::wstring temp = wss.str();
    while (temp.size() < 6) temp = L"0" + temp;
    m_CachedWallpaperInfo.push_back(WallpaperInfo(src_path, temp));
}

// past file from ./Wallpapers/ to tar_path
bool WallpaperManager::past_wallpaper_2_targetFolder(
                        const int& wallpaper_ind,
                        const std::wstring& tar_path)
{
    if (wallpaper_ind >= 0 && wallpaper_ind < m_CachedWallpaperInfo.size()) {
        std::wstring loc_filename =
        m_WallpaperList_path+m_CachedWallpaperInfo[wallpaper_ind].getNewFilename();
        if (!CopyFileW(loc_filename.c_str(), tar_path.c_str(), FALSE))
            return false;
    } else return false;
    return true;
}
// past file from ./Wallpapers/ to tar_path
bool WallpaperManager::past_wallpaper_2_targetFolder(
                        const WallpaperInfo& wallpaper_info,
                        const std::wstring& tar_path)
{
    if (!wallpaper_info.isEmpty()) {
        std::wstring loc_filename =
        m_WallpaperList_path+wallpaper_info.getNewFilename();
        if (!CopyFileW(loc_filename.c_str(), tar_path.c_str(), FALSE))
            return false;
    } else return false;
    return true;
}

// find wallpaper base on its basic info, return its index in m_CachedWallpaperInfo
const int WallpaperManager::find_wallpaperIndex_via_wallInfo(const WallpaperInfo& wallpaper_info)
{
    auto it = find(m_CachedWallpaperInfo.begin(), m_CachedWallpaperInfo.end(), wallpaper_info);
    if (it != m_CachedWallpaperInfo.end())
        return (it - m_CachedWallpaperInfo.begin());
    else return -1;
}
WallpaperInfo& WallpaperManager::find_wallpaperInfo_via_src(const std::wstring& src_path)
{
    for (auto it = m_CachedWallpaperInfo.begin();
        it != m_CachedWallpaperInfo.end(); ++it) {
        if (it->getSrcPath() == src_path)
            return *it;
    } return *(new WallpaperInfo());
}
WallpaperInfo& WallpaperManager::find_wallpaperInfo_via_ori(const std::wstring& original_filename)
{
    for (auto it = m_CachedWallpaperInfo.begin();
        it != m_CachedWallpaperInfo.end(); ++it) {
        if (it->getOriFileName() == original_filename)
            return *it;
    } return *(new WallpaperInfo());
}
WallpaperInfo& WallpaperManager::find_wallpaperInfo_via_new(const std::wstring& new_filename_hexID)
{
    for (auto it = m_CachedWallpaperInfo.begin();
        it != m_CachedWallpaperInfo.end(); ++it) {
        if (it->getNewFilename() == new_filename_hexID)
            return *it;
    } return *(new WallpaperInfo());
}
WallpaperInfo& WallpaperManager::find_wallpaperInfo_via_time(const int& add_time)
{
    for (auto it = m_CachedWallpaperInfo.begin();
        it != m_CachedWallpaperInfo.end(); ++it) {
        if (it->getAddTime() == add_time)
            return *it;
    } return *(new WallpaperInfo());
}

// ====================== WallpaperManager End ======================




// ====================== WallpaperInfo ======================

// default constructor
WallpaperInfo::WallpaperInfo()
    : m_SrcPath(L""), m_OriFileName(L""), m_NewFileName(L""), m_AddTime(0)
{}
// parametric constructor
WallpaperInfo::WallpaperInfo(const std::wstring& src_path, const std::wstring& new_filename, const int& add_time)
    : m_SrcPath(src_path), m_OriFileName(L""), m_NewFileName(new_filename), m_AddTime(add_time)
{
    // checking whether src_path exist
    if (!GlobTools::is_filedir_existW(src_path)) { 
        m_SrcPath = L"";
        m_NewFileName = L"";
        return;
    } else {
        // checking whether src_path is a supported file
        bool whether_support = false;
        std::wstring temp_src_path(src_path);
        std::wstring curr_fileExtension;
        reverse(temp_src_path.begin(), temp_src_path.end());
        curr_fileExtension.assign(temp_src_path.begin(), temp_src_path.begin()+temp_src_path.find(L'.'));
        reverse(curr_fileExtension.begin(), curr_fileExtension.end());
        for (auto it : SupportedImageFileExtension) {
            if (it == curr_fileExtension);
                whether_support = true;
        }
        
        if (whether_support) { // support file, assign m_OriFileName & m_AddTime
            m_OriFileName.assign(temp_src_path.begin(), temp_src_path.begin()+temp_src_path.find(L'\\'));
            reverse(m_OriFileName.begin(), m_OriFileName.end());
            if (m_AddTime == 0)
                m_AddTime = time(0);
        } else { // not support file
            return;
        }
    }
}
// copy constructor
WallpaperInfo::WallpaperInfo(const WallpaperInfo& obj)
    : m_SrcPath(obj.m_SrcPath), m_OriFileName(obj.m_OriFileName),
    m_NewFileName(obj.m_NewFileName), m_AddTime(obj.m_AddTime)
{}

// compare operators

bool WallpaperInfo::operator==(const WallpaperInfo& obj)
{
    return (
        obj.m_SrcPath == m_SrcPath &&
        obj.m_OriFileName == m_OriFileName &&
        obj.m_NewFileName == m_NewFileName &&
        obj.m_AddTime == m_AddTime
    );
}
bool WallpaperInfo::operator!=(const WallpaperInfo& obj)
{
    return (
        obj.m_SrcPath != m_SrcPath &&
        obj.m_OriFileName != m_OriFileName &&
        obj.m_NewFileName != m_NewFileName &&
        obj.m_AddTime != m_AddTime
    );
}


// getter & setter

const std::wstring WallpaperInfo::getSrcPath() const
{
    return m_SrcPath;
}
void WallpaperInfo::setSrcPath(const std::wstring& src_path)
{
    if (GlobTools::is_filedir_existW(src_path))
        m_SrcPath = src_path;
}
const std::wstring WallpaperInfo::getOriFileName() const
{
    return m_OriFileName;
}
const std::wstring WallpaperInfo::getNewFilename() const
{
    return m_NewFileName;
}
void WallpaperInfo::setNewFilename(const std::wstring& new_filename)
{
    m_NewFileName = new_filename;
}
const int WallpaperInfo::getAddTime() const
{
    return m_AddTime;
}

bool WallpaperInfo::isEmpty() const
{
    return (m_SrcPath.empty() && 
            m_OriFileName.empty() && 
            m_NewFileName.empty() && 
            m_AddTime == 0);
}

// ====================== WallpaperInfo End ======================