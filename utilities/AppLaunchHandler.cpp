
// Implementation of Application Auto Launch, Restart, and Recovery Handler
#include "AppLaunchHandler.h"


// ============================== AppLaunchHandler Function ==============================

// default Application Recovery Callback Function
DWORD WINAPI ApplicationRecoveryCallback(PVOID pvParameter)
{
    ApplicationRecoveryFinished(TRUE);
    return 0;
}


// Handling Application launch at windows startup
void setupAppStartupA(
    const bool& whether_launch_at_startup,
    const std::string& key_val_name,
    const std::string& app_path)
{
    // open windows registry
    HKEY hKey;
    LSTATUS lstat = ERROR_SUCCESS;
    lstat = RegOpenKeyExA(
        HKEY_CURRENT_USER,
        "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        0, KEY_WRITE, &hKey
    );
    if (lstat != ERROR_SUCCESS)
        throw std::invalid_argument("Fail to Open Windows Registry.\n");
    
    std::string app_val_name = key_val_name;
    std::string app_name = app_path;
    if (*(app_name.begin()) != '\"')
        app_name = '\"' + app_name;
    if (*(app_name.end()-1) != '\"')
        app_name = app_name + '\"';
    
    // write launch at startup setting to registry
    if (whether_launch_at_startup) {
        lstat = RegSetValueExA(
            hKey,
            app_val_name.c_str(),
            0, REG_SZ,
            (BYTE*)app_name.c_str(),
            app_name.size()*sizeof(char)
        );
        if (lstat != ERROR_SUCCESS)
            throw std::invalid_argument("Fail to Set Windows Registry Value.\n");
    }
    // remove launch at startup setting in registry
    else {
        lstat = RegDeleteValueA(
            hKey,
            app_val_name.c_str()
        );
        if (lstat != ERROR_SUCCESS)
            throw std::invalid_argument("Fail to Delete Windows Registry Value.\n");
    }
    
    // close windows registry
    lstat = RegCloseKey(hKey);
    if (lstat != ERROR_SUCCESS)
        throw std::invalid_argument("Fail to Close Windows Registry.\n");
}
// Handling Application launch at windows startup
void setupAppStartupW(
    const bool& whether_launch_at_startup,
    const std::wstring& key_val_name,
    const std::wstring& app_path)
{
    // open windows registry
    HKEY hKey;
    LSTATUS lstat = ERROR_SUCCESS;
    lstat = RegOpenKeyExW(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        0, KEY_WRITE, &hKey
    );
    if (lstat != ERROR_SUCCESS)
        throw std::invalid_argument("Fail to Open Windows Registry.\n");
    
    std::wstring app_val_name = key_val_name;
    std::wstring app_name = app_path;
    if (*(app_name.begin()) != L'\"')
        app_name = L'\"' + app_name;
    if (*(app_name.end()-1) != L'\"')
        app_name = app_name + L'\"';
    
    // write launch at startup setting to registry
    if (whether_launch_at_startup) {
        lstat = RegSetValueExW(
            hKey,
            app_val_name.c_str(),
            0, REG_SZ,
            (BYTE*)app_name.c_str(),
            app_name.size()*sizeof(wchar_t)
        );
        if (lstat != ERROR_SUCCESS)
            throw std::invalid_argument("Fail to Set Windows Registry Value.\n");
    }
    // remove launch at startup setting in registry
    else {
        lstat = RegDeleteValueW(
            hKey,
            app_val_name.c_str()
        );
        bool delete_success = (
            lstat == ERROR_SUCCESS ||
            lstat == ERROR_FILE_NOT_FOUND
        );
        if (!delete_success)
            throw std::invalid_argument("Fail to Delete Windows Registry Value.\n");
    }
    
    // close windows registry
    lstat = RegCloseKey(hKey);
    if (lstat != ERROR_SUCCESS)
        throw std::invalid_argument("Fail to Close Windows Registry.\n");
}


// ============================== AppLaunchHandler Function End ==============================