#include "pch.h"
#include <Windows.h>
#include <Math.h>
//回调函数的函数指针
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
//替换的窗口回调
LRESULT CALLBACK MyWindowProc(HWND hwnd,      // handle to window
    UINT uMsg,      // message identifier
    WPARAM wParam,  // first message parameter
    LPARAM lParam   // second message parameter
);
//打开窗口句柄
HANDLE hProcess = NULL;
//自身的模块句柄
HMODULE g_hMod = NULL;
//原来的窗口过程函数
WNDPROC g_pfnOldWndProc = NULL;
void init();//初始化
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hMod = hModule;
        init();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void init()
{
    HWND hWnd = FindWindowA(NULL, "连连看 v4.1");;
    if (hWnd == NULL)
    {
        OutputDebugString(L"句柄获取失败");
        return;
    }
    DWORD dwProcid = 0;
    GetWindowThreadProcessId(hWnd, &dwProcid);
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcid);

    //替换回调函数

    g_pfnOldWndProc = (WNDPROC)SetWindowLongA(hWnd, GWL_WNDPROC, (LONG)MyWindowProc);

}
//注入dll的回调函数
LRESULT CALLBACK MyWindowProc(HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
        __asm
        {
            mov eax, eax
            mov eax, eax
        }
        break;
    case WM_COMMAND:
    {
        __asm
        {
            mov eax, eax
            mov eax, eax
            mov eax, eax
        }
        break;
    }
    }

    wchar_t buf[32] = { 0 };
    wsprintfW(buf, L"%d-%d",uMsg, 99);
    OutputDebugStringW(buf);
    return g_pfnOldWndProc(hwnd, uMsg, wParam, lParam);
}
