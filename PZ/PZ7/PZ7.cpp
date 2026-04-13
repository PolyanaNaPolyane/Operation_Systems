#include <windows.h>
#include <cwchar>

void LogMessageW(const wchar_t* message)
{
    if (!message) return;

    HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (stdoutHandle == NULL || stdoutHandle == INVALID_HANDLE_VALUE)
    {
        return;
    }

    DWORD consoleMode = 0;
    if (!GetConsoleMode(stdoutHandle, &consoleMode))
    {
        return;
    }

    DWORD written = 0;
    WriteConsoleW(stdoutHandle, message, static_cast<DWORD>(wcslen(message)), &written, NULL);
}

BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved
)
{
    UNREFERENCED_PARAMETER(hModule);
    UNREFERENCED_PARAMETER(lpReserved);

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        LogMessageW(L"\n========== DLL LOADED ==========\n");
        LogMessageW(L"PZ7 loaded\n");
        LogMessageW(L"===============================\n\n");
        break;
    case DLL_PROCESS_DETACH:
        LogMessageW(L"DLL unloaded\n");
        break;
    }

    return TRUE;
}
