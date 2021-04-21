
// Application Auto Launch, Restart, and Recovery Handler
#pragma once

// Windows API
#include <Windows.h>
#include <shlobj.h>
#include <winnls.h>
#include <shobjidl.h>
#include <objbase.h>
#include <objidl.h>
#include <shlguid.h>

// C++ STL
#include <string>
#include <algorithm>
#include <stdexcept>


// default Application Recovery Callback Function
DWORD WINAPI ApplicationRecoveryCallback(PVOID pvParameter);


// Handling Application launch at windows startup
void setupAppStartupA(
    const bool& whether_launch_at_startup,
    const std::string& program_path, // full program path
    const std::string& system_startup_dir // system startup folder
);
// Handling Application launch at windows startup
void setupAppStartupW(
    const bool& whether_launch_at_startup,
    const std::wstring& program_path, // full program path
    const std::wstring& system_startup_dir // system startup folder
);

// Handling Application re-launch & Recovery after crash
template <typename RECOVERY_FUNC>
void setupAppRestartRecover(
    RECOVERY_FUNC recovery_func,
    PVOID recovery_func_param)
{
    // create function pointer properly
    RECOVERY_FUNC func = nullptr;
    if (recovery_func != nullptr)
        func = recovery_func;
    else
        func = ApplicationRecoveryCallback;
    
    HRESULT hres = S_OK;
    
    hres = RegisterApplicationRestart(nullptr, RESTART_NO_PATCH | RESTART_NO_REBOOT);
    if (FAILED(hres))
        throw std::invalid_argument("Fail to RegisterApplicationRestart.\n");
    
    hres = RegisterApplicationRecoveryCallback(
        func, recovery_func_param,
        RECOVERY_DEFAULT_PING_INTERVAL, 0);
    if (FAILED(hres))
        throw std::invalid_argument("Fail to RegisterApplicationRecoveryCallback.\n");
}

