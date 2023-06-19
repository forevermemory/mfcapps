// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "mydciman.h"
#include <process.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        if (Load())
        {
            _beginthread(Init, NULL, NULL); // 注入代码线程
        }
        else
            return FALSE;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        Free();
        break;
    }
    return TRUE;
}

