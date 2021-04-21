
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
    const std::string& program_path, // full program path
    const std::string& system_startup_dir // system startup folder
)
{
    // buffers
    std::string program_name; // program name with its extension
    std::string pwd; // present working directory without program name
    std::string shortcut_name; // windows application shortcut name with .lnk as extension (only file name)
    bool has_all_param = (
        !program_path.empty() &&
        !system_startup_dir.empty()
    );
    if (has_all_param) { // setup all string buffers if has_all_param
        // extract pwd & program_name from program_path
        pwd = program_path;
        std::reverse(pwd.begin(), pwd.end());
        size_t pos = pwd.find('\\');
        program_name.assign(pwd.begin(), pwd.begin()+pos);
        std::reverse(program_name.begin(), program_name.end());
        pwd.assign(pwd.begin()+pos, pwd.end());
        std::reverse(pwd.begin(), pwd.end());
        pos = program_name.find('.');
        // create shortcut_name from program_name
        shortcut_name.assign(program_name.begin(), program_name.begin()+pos);
        shortcut_name += ".lnk";
    } else {
        // do not have all parameters, exit function
        return;
    }
    if (whether_launch_at_startup && has_all_param) // create program shortcut & copy it to   
    {
        // setup buffers
        HRESULT hres;
        IShellLinkA *shlnk;
        IPersistFile *pf;
        
        // initialize COM library
        CoInitialize(NULL);
        
        // create COM instance
        hres = CoCreateInstance(
            CLSID_ShellLink,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_IShellLink,
            ((LPVOID*)&shlnk)
        );
        
        if (SUCCEEDED(hres)) {
            // set shortcut path, description, & pwd
            hres = shlnk->SetPath((pwd+program_name).c_str());
            hres = shlnk->SetDescription((program_name + " Shortcut").c_str());
            hres = shlnk->SetWorkingDirectory(pwd.c_str());
        }
        
        // initialize IPersistFile obj
        hres = shlnk->QueryInterface(
            IID_IPersistFile,
            (LPVOID*)&pf
        );
        
        if (SUCCEEDED(hres)) {
            // use IPersistFile obj to save application shortcut file
            WCHAR buff[MAX_PATH];
            // cast from string to wstring
            int buff_size = MultiByteToWideChar(CP_ACP, 0, 
                                    (system_startup_dir+shortcut_name).c_str(),
                                    -1, buff, MAX_PATH);
            hres = pf->Save(buff, TRUE);
            pf->Release();
        }
        shlnk->Release();
        
        // uninitialize COM library
        CoUninitialize();
    }
    else if (!whether_launch_at_startup) // remove application shortcut from system startup dir
    {
        DeleteFileA((system_startup_dir + shortcut_name).c_str());
    }
}
// Handling Application launch at windows startup
void setupAppStartupW(
    const bool& whether_launch_at_startup,
    const std::wstring& program_path, // full program path
    const std::wstring& system_startup_dir // system startup folder
)
{
    // buffers
    std::wstring program_name; // program name with its extension
    std::wstring pwd; // present working directory without program name
    std::wstring shortcut_name; // windows application shortcut name with .lnk as extension (only file name)
    bool has_all_param = (
        !program_path.empty() &&
        !system_startup_dir.empty()
    );
    if (has_all_param) { // setup all string buffers if has_all_param
        // extract pwd & program_name from program_path
        pwd = program_path;
        std::reverse(pwd.begin(), pwd.end());
        size_t pos = pwd.find(L'\\');
        program_name.assign(pwd.begin(), pwd.begin()+pos);
        std::reverse(program_name.begin(), program_name.end());
        pwd.assign(pwd.begin()+pos, pwd.end());
        std::reverse(pwd.begin(), pwd.end());
        pos = program_name.find(L'.');
        // create shortcut_name from program_name
        shortcut_name.assign(program_name.begin(), program_name.begin()+pos);
        shortcut_name += L".lnk";
    } else {
        // do not have all parameters, exit function
        return;
    }
    if (whether_launch_at_startup && has_all_param) // create program shortcut & copy it to   
    {
        // setup buffers
        HRESULT hres;
        IShellLinkW *shlnk;
        IPersistFile *pf;
        
        // initialize COM library
        CoInitialize(NULL);
        
        // create COM instance
        hres = CoCreateInstance(
            CLSID_ShellLink,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_IShellLink,
            ((LPVOID*)&shlnk)
        );
        
        if (SUCCEEDED(hres)) {
            // set shortcut path, description, & pwd
            hres = shlnk->SetPath((pwd+program_name).c_str());
            hres = shlnk->SetDescription((program_name + L" Shortcut").c_str());
            hres = shlnk->SetWorkingDirectory(pwd.c_str());
        }
        
        // initialize IPersistFile obj
        hres = shlnk->QueryInterface(
            IID_IPersistFile,
            (LPVOID*)&pf
        );
        
        if (SUCCEEDED(hres)) {
            // use IPersistFile obj to save application shortcut file
            hres = pf->Save((system_startup_dir+shortcut_name).c_str(), TRUE);
            pf->Release();
        }
        shlnk->Release();
        
        // uninitialize COM library
        CoUninitialize();
    }
    else if (!whether_launch_at_startup) // remove application shortcut from system startup dir
    {
        DeleteFileW((system_startup_dir + shortcut_name).c_str());
    }
}


// ============================== AppLaunchHandler Function End ==============================