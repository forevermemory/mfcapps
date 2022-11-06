#include <Windows.h>

#pragma warning(disable:4996)

HMODULE g_hInstance = NULL;

char oldCode[5] = { 0 };
char newCode[5] = { 0xE9 };
DWORD g_hookAddr = 0;

BOOL InstallHook();
BOOL UnHook();


int WINAPI MyMessageBoxW( HWND hWnd,LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
	OutputDebugStringW(lpText);

	UnHook();
	MessageBoxW(hWnd, lpText, lpCaption, 2);
	InstallHook();

	return 1;
}

BOOL InstallHook()
{
	DWORD old = 0;
	VirtualProtect((DWORD *)g_hookAddr, 5, PAGE_EXECUTE_READWRITE, &old);
	memcpy((DWORD*)g_hookAddr, newCode, 5);
	VirtualProtect((DWORD*)g_hookAddr, 5, old, &old);
	return TRUE;
}


BOOL UnHook()
{
	DWORD old = 0;
	VirtualProtect((DWORD*)g_hookAddr, 5, PAGE_EXECUTE_READWRITE, &old);
	memcpy((DWORD*)g_hookAddr, oldCode, 5);
	VirtualProtect((DWORD*)g_hookAddr, 5, old, &old);
	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID pResverd)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		g_hInstance = hInstance;
		OutputDebugStringA("AAA==>DLL_PROCESS_ATTACH");

		// 也可以直接写 MessageBoxW , 但是有些debug版本函数是jmp xxx
		HMODULE hModule = GetModuleHandleA("user32.dll");
		g_hookAddr = (DWORD)GetProcAddress(hModule, "MessageBoxW");
		// ps: 如果是汇编中某个函数 地址则例如 0x00439345

		// 保存原函数前5个字节
		memcpy(oldCode, (char*)g_hookAddr, 5);

		// 计算偏移
		DWORD offset = (DWORD)MyMessageBoxW - g_hookAddr -5;
		memcpy(&newCode[1], &offset, 4);

		InstallHook();
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		OutputDebugStringA("AAA==>DLL_PROCESS_DETACH");
		UnHook();
	}
	return TRUE;
}