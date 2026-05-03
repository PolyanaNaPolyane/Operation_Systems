#include <iostream>
#include <string>
#include <windows.h>
#include <io.h>
#include <fcntl.h>

#define IDS_SURNAME        101
#define IDS_FACULTY        102
#define IDS_GROUP          103
#define IDS_DISCIPLINE     104
#define IDS_LBL_SURNAME    105
#define IDS_LBL_FACULTY    106
#define IDS_LBL_GROUP      107
#define IDS_LBL_DISCIPLINE 108

static std::wstring LoadDllString(HMODULE hLib, UINT id)
{
    wchar_t buf[512] = {};
    LoadStringW(hLib, id, buf, static_cast<int>(sizeof(buf) / sizeof(buf[0])));
    return buf;
}

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    std::wcout << L"Select language / Оберіть мову:\n"
               << L"  1 - Ukrainian (Українська)\n"
               << L"  2 - English\n"
               << L"Choice: ";

    int choice = 0;
    std::cin >> choice;

    LPCWSTR dllName = nullptr;
    switch (choice)
    {
    case 1: dllName = L"UkrainianDynamicLibrary.dll"; break;
    case 2: dllName = L"EnglishDymanicLibrary.dll";   break;
    default:
        std::wcerr << L"Invalid choice.\n";
        return 1;
    }

    HMODULE hLib = LoadLibraryW(dllName);
    if (!hLib)
    {
        std::wcerr << L"LoadLibraryEx failed for \"" << dllName
                   << L"\", error: " << GetLastError() << L"\n";
        return 1;
    }

    std::wstring surname    = LoadDllString(hLib, IDS_SURNAME);
    std::wstring faculty    = LoadDllString(hLib, IDS_FACULTY);
    std::wstring group      = LoadDllString(hLib, IDS_GROUP);
    std::wstring discipline = LoadDllString(hLib, IDS_DISCIPLINE);

    std::wstring lblSurname    = LoadDllString(hLib, IDS_LBL_SURNAME);
    std::wstring lblFaculty    = LoadDllString(hLib, IDS_LBL_FACULTY);
    std::wstring lblGroup      = LoadDllString(hLib, IDS_LBL_GROUP);
    std::wstring lblDiscipline = LoadDllString(hLib, IDS_LBL_DISCIPLINE);

    FreeLibrary(hLib);

    std::wcout << L"\n"
               << lblSurname    << L": " << surname    << L"\n"
               << lblFaculty    << L": " << faculty    << L"\n"
               << lblGroup      << L": " << group      << L"\n"
               << lblDiscipline << L": " << discipline << L"\n";

    return 0;
}
