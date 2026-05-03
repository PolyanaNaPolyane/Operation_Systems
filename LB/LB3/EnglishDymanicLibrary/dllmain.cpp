#include "pch.h"
#include <cstdio>
#include <cstdint>

static BOOL CheckIntegrity(HMODULE hModule)
{
    char path[MAX_PATH];
    GetModuleFileNameA(hModule, path, MAX_PATH);

    FILE* f = nullptr;
    if (fopen_s(&f, path, "rb") != 0 || !f)
        return FALSE;

    fseek(f, 0, SEEK_END);
    long fileSize = ftell(f);
    rewind(f);

    if (fileSize < static_cast<long>(sizeof(uint32_t)))
    {
        fclose(f);
        return TRUE;
    }

    long dataSize = fileSize - static_cast<long>(sizeof(uint32_t));

    uint32_t computed = 0;
    fseek(f, 0, SEEK_SET);
    for (long i = 0; i < dataSize; ++i)
    {
        int c = fgetc(f);
        if (c == EOF) { fclose(f); return FALSE; }
        computed += static_cast<uint8_t>(c);
    }

    uint32_t stored = 0;
    fseek(f, dataSize, SEEK_SET);
    fread(&stored, sizeof(stored), 1, f);
    fclose(f);

    return (computed == stored) ? TRUE : FALSE;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        char msg[64];
        sprintf_s(msg, "lpReserved = %p", lpReserved);
        MessageBoxA(NULL, msg, "English DLL", MB_OK);

        if (!CheckIntegrity(hModule))
        {
            MessageBoxA(NULL,
                "Integrity check FAILED!\nThe DLL file may be corrupted.",
                "English DLL", MB_ICONERROR | MB_OK);
            return FALSE;
        }
    }
    return TRUE;
}

