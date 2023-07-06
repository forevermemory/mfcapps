
#include <Windows.h>
#include <stdio.h>
#pragma warning(disable:4996)

BOOL hasLoaded = FALSE;
char buf[32] = { 0 };
HMODULE g_hModule = 0;

extern "C" LRESULT __declspec(dllexport) CALLBACK myHookStartFunc(int nCode, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK myHookStartFunc(int nCode, WPARAM wParam, LPARAM lParam)
{
    sprintf(buf, "MyProc======== %d %x %x \n", nCode, wParam, lParam);
    OutputDebugStringA(buf);

    // home按键触发
    // lParam The previous key state.
    // 30	The value is 1 if the key is down before the message is sent; it is 0 if the key is up
    if (nCode == HC_ACTION && wParam == VK_HOME && (lParam & 0xC0000000) )
    {
        OutputDebugStringA("=========VK_HOME==DWON===========");
        MessageBoxA(0, "hook-content", "title", 0);
        // todo ...

        hasLoaded = TRUE;
    }
    //继续调用钩子过程
    return CallNextHookEx(0, nCode, wParam, lParam);  
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        OutputDebugStringA("DllMain DLL_PROCESS_ATTACH=================================");
        g_hModule = hModule;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

