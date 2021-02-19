#pragma once
#include <string>
#include <iostream>

/*
 * Allowed Arguments
 * 
 *    --add {file_path} => adding new wallpaper to ./Wallpapers/ folder
 *    --del {file_name}/{file_id} => deleting wallpaper by its name/id
 *    --find {file_name}/{file_id} => find a wallpaper's info base on its name/id
 *    --help => print help menu
 * 
 */

namespace {
    std::wstring helpArrW[] = {
        L"Allowed Arguments\n",
        L"\n",
        L"\b--add {file_path} => adding new wallpaper to ./Wallpapers/ folder\n",
        L"\b--del {file_name}/{file_id} => deleting wallpaper by its name/id\n",
        L"\b--find {file_name}/{file_id} => find a wallpaper's info base on its name/id\n",
        L"\b--help, -h => print help menu\n"
    };
    std::string helpArrA[] = {
        "Allowed Arguments\n",
        "\n",
        "\b--add {file_path} => adding new wallpaper to ./Wallpapers/ folder\n",
        "\b--del {file_name}/{file_id} => deleting wallpaper by its name/id\n",
        "\b--find {file_name}/{file_id} => find a wallpaper's info base on its name/id\n",
        "\b--help, -h => print help menu\n"
    };
};


// handling arguments in wmain() function
// produce commands with input arguments
class ArgumentHandlerW
{
public:
    // constructor
    ArgumentHandlerW(int argc, wchar_t* argv[]);
    
    void printHelp();
    
    // getters
    std::wstring getFileName();
    std::wstring getFilePath();
    std::wstring getFileId();
    
    bool hasAdd();
    const bool hasAdd() const;
    bool hasDel();
    const bool hasDel() const;
    bool hasFind();
    const bool hasFind() const;
    bool hasHelp();
    const bool hasHelp() const;
    
private: // private functions
    // check whether a wstring is filename
    bool isFileName(wchar_t* str);
private:
    // argument buffer
    std::wstring m_FileName;
    std::wstring m_FilePath;
    std::wstring m_FileId;
    // flags
    bool m_HasAdd;
    bool m_HasDel;
    bool m_HasFind;
    bool m_HasHelp;
};


// handling arguments in main() function
// produce commands with input arguments
class ArgumentHandlerA
{
public:
    // constructor
    ArgumentHandlerA(int argc, char* argv[]);
    
    void printHelp();
    
    // getters
    std::string getFileName();
    std::string getFilePath();
    std::string getFileId();
    
    bool hasAdd();
    const bool hasAdd() const;
    bool hasDel();
    const bool hasDel() const;
    bool hasFind();
    const bool hasFind() const;
    bool hasHelp();
    const bool hasHelp() const;
    
private: // private functions
    // check whether a string is filename
    bool isFileName(char* str);
private:
    // argument buffer
    std::string m_FileName;
    std::string m_FilePath;
    std::string m_FileId;
    // flags
    bool m_HasAdd;
    bool m_HasDel;
    bool m_HasFind;
    bool m_HasHelp;
};