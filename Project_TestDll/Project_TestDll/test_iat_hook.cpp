#include <Windows.h>
#include <stdio.h>

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
	BYTE code[13] = {0};
	int codeLength = 12;
	// 找特征码
	BYTE* dataBuffer = NULL;
	BOOL readReturn = 0;

	// 获取 PageSize 和地址粒度
	SYSTEM_INFO sysInfo = { 0 };
	GetSystemInfo(&sysInfo);
	char* p = (char*)sysInfo.lpMinimumApplicationAddress;
	MEMORY_BASIC_INFORMATION  memInfo = { 0 };

	char buf[1024] = { 0 };

	while (p < sysInfo.lpMaximumApplicationAddress) {
		size_t size = VirtualQuery(
			p,										// 要查询内存块的基地址指针
			&memInfo,								// 接收内存块信息的 MEMORY_BASIC_INFORMATION 对象
			sizeof(MEMORY_BASIC_INFORMATION)		// 缓冲区大小
		);
		if (size != sizeof(MEMORY_BASIC_INFORMATION)) { break; }

		if (memInfo.Type == MEM_PRIVATE && !(memInfo.Protect & PAGE_NOACCESS))
		{
			//str.Format("find addr: %p, length:%x", p, memInfo.RegionSize);
			//OutputDebugStringA(str);

			//// 查找 memInfo.RegionSize
			dataBuffer = (BYTE*)malloc(memInfo.RegionSize);
			memset(dataBuffer, 0, memInfo.RegionSize);

			//DWORD old;
			//VirtualProtect((LPVOID)p, memInfo.RegionSize, PAGE_EXECUTE_READWRITE, &old);
			SIZE_T rd;
			ReadProcessMemory(GetCurrentProcess(), p, dataBuffer, memInfo.RegionSize, &rd);
			//VirtualProtect((LPVOID)p, memInfo.RegionSize, old, &old);

			size_t length = memInfo.RegionSize;
			//str.Format("find addr: %p, length:%x, rd:%X", p, length, rd);
			//OutputDebugStringA(str);


			for (int i = 0; i < length; i++)
			{
				BOOLEAN found = TRUE;
				for (int j = 0; j < codeLength; j++)
				{
					if (code[j] != dataBuffer[i + j])
					{
						found = FALSE;
						break;
					}

				}

				if (found)
				{
					// 地址为
					DWORD addr = (DWORD)(p)+i;
					sprintf(buf, "find addr: %X", addr);
					OutputDebugStringA(buf);
					break;
					
				}
			}

			free(dataBuffer);
		}

		p += memInfo.RegionSize;
	}

	OutputDebugStringA("find finished");




	DWORD old = 0;
	VirtualProtect((DWORD *)g_hookAddr, 5, PAGE_EXECUTE_READWRITE, &old);
	memcpy((DWORD*)g_hookAddr, newCode, 5);
	VirtualProtect((DWORD*)g_hookAddr, 5, old, &old);
	return TRUE;
}



BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID pResverd)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		g_hInstance = hInstance;
		OutputDebugStringA("AAA==>DLL_PROCESS_ATTACH");

		InstallHook();
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		OutputDebugStringA("AAA==>DLL_PROCESS_DETACH");
	}
	return TRUE;
}