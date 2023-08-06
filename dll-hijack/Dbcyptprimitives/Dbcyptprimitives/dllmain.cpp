// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include <Windows.h>


extern "C" ULONG64 g_func_addr[12] = {0};

void init()
{
    HMODULE hModule = LoadLibraryA("C:\\Windows\\System32\\dinput8.dll");
    for (size_t i = 1; i < 11; i++)
    {
        g_func_addr[i] = (ULONG64)GetProcAddress(hModule, (char*)i);
    }
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        init();
        OutputDebugStringA("=======DLL_PROCESS_ATTACH=======");
    case DLL_THREAD_ATTACH:
        OutputDebugStringA("=======DLL_THREAD_ATTACH=======");
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

