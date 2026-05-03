#include "pch.h"
#include <cstdio>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        char msg[64];
        sprintf_s(msg, "lpReserved = %p", lpReserved);
        MessageBoxA(NULL, msg, "RSA DLL", MB_OK);
    }
    return TRUE;
}

