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

		// Ҳ����ֱ��д MessageBoxW , ������Щdebug�汾������jmp xxx
		HMODULE hModule = GetModuleHandleA("user32.dll");
		g_hookAddr = (DWORD)GetProcAddress(hModule, "MessageBoxW");
		// ps: ����ǻ����ĳ������ ��ַ������ 0x00439345

		// ����ԭ����ǰ5���ֽ�
		memcpy(oldCode, (char*)g_hookAddr, 5);

		// ����ƫ��
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