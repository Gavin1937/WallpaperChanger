#include <iostream>


#include "GlobTools.h"

int main()
{
    std::cout << GlobTools::all2upperA("stringa") << std::endl;
    std::wcout << GlobTools::all2upperW(L"stringa") << std::endl;
}