#include "pch.h"
#include <Windows.h>
#include <process.h>
#include "mydciman.h"

#pragma comment(linker, "/EXPORT:DCIBeginAccess=_hacker_DCIBeginAccess,@1")
#pragma comment(linker, "/EXPORT:DCICloseProvider=_hacker_DCICloseProvider,@2")
#pragma comment(linker, "/EXPORT:DCICreateOffscreen=_hacker_DCICreateOffscreen,@3")
#pragma comment(linker, "/EXPORT:DCICreateOverlay=_hacker_DCICreateOverlay,@4")
#pragma comment(linker, "/EXPORT:DCICreatePrimary=_hacker_DCICreatePrimary,@5")
#pragma comment(linker, "/EXPORT:DCIDestroy=_hacker_DCIDestroy,@6")
#pragma comment(linker, "/EXPORT:DCIDraw=_hacker_DCIDraw,@7")
#pragma comment(linker, "/EXPORT:DCIEndAccess=_hacker_DCIEndAccess,@8")
#pragma comment(linker, "/EXPORT:DCIEnum=_hacker_DCIEnum,@9")
#pragma comment(linker, "/EXPORT:DCIOpenProvider=_hacker_DCIOpenProvider,@10")
#pragma comment(linker, "/EXPORT:DCISetClipList=_hacker_DCISetClipList,@11")
#pragma comment(linker, "/EXPORT:DCISetDestination=_hacker_DCISetDestination,@12")
#pragma comment(linker, "/EXPORT:DCISetSrcDestClip=_hacker_DCISetSrcDestClip,@13")
#pragma comment(linker, "/EXPORT:GetDCRegionData=_hacker_GetDCRegionData,@14")
#pragma comment(linker, "/EXPORT:GetWindowRegionData=_hacker_GetWindowRegionData,@15")
#pragma comment(linker, "/EXPORT:WinWatchClose=_hacker_WinWatchClose,@16")
#pragma comment(linker, "/EXPORT:WinWatchDidStatusChange=_hacker_WinWatchDidStatusChange,@17")
#pragma comment(linker, "/EXPORT:WinWatchGetClipList=_hacker_WinWatchGetClipList,@18")
#pragma comment(linker, "/EXPORT:WinWatchNotify=_hacker_WinWatchNotify,@19")
#pragma comment(linker, "/EXPORT:WinWatchOpen=_hacker_WinWatchOpen,@20")

// 以上是导出表的函数 可以使用Dependency工具查看 


HMODULE m_hModule = NULL;

 BOOL WINAPI Load()
{
	char tzPath[MAX_PATH];
	GetSystemDirectoryA(tzPath, MAX_PATH);
	lstrcatA(tzPath, "\\dciman.dll");
	m_hModule = LoadLibraryA(tzPath);

	return m_hModule == NULL;
}

 BOOL WINAPI Free()
{
	return FreeLibrary(m_hModule);
}

FARPROC WINAPI GetAddress(PCSTR pszProcName)
{
	FARPROC fpAddress;

	fpAddress = GetProcAddress(m_hModule, pszProcName);
	return fpAddress;
}

void WINAPIV Init(LPVOID pParam)
{
	LoadLibraryA("ZombieLib.dll");
	// MessageBoxA(NULL, "a", "title-注冊成功", MB_OK);
	return;
}


// ★
// 导出函数转发
// ★



ALCDECL hacker_DCIBeginAccess(void)
{
	GetAddress("DCIBeginAccess");
	__asm jmp eax;
}

ALCDECL hacker_DCICloseProvider(void)
{
	GetAddress("DCICloseProvider");
	__asm jmp eax;
}

ALCDECL hacker_DCICreateOffscreen(void)
{
	GetAddress("DCICreateOffscreen");
	__asm jmp eax;
}

ALCDECL hacker_DCICreateOverlay(void)
{
	GetAddress("DCICreateOverlay");
	__asm jmp eax;
}
ALCDECL hacker_DCICreatePrimary(void)
{
	GetAddress("DCICreatePrimary");
	__asm jmp eax;
}
ALCDECL hacker_DCIDestroy(void)
{
	GetAddress("DCIDestroy");
	__asm jmp eax;
}
ALCDECL hacker_DCIDraw(void)
{
	GetAddress("DCIDraw");
	__asm jmp eax;
}
ALCDECL hacker_DCIEndAccess(void)
{
	GetAddress("DCIEndAccess");
	__asm jmp eax;
}
ALCDECL hacker_DCIEnum(void)
{
	GetAddress("DCIEnum");
	__asm jmp eax;
}
ALCDECL hacker_DCIOpenProvider(void)
{
	GetAddress("DCIOpenProvider");
	__asm jmp eax;
}
ALCDECL hacker_DCISetClipList(void)
{
	GetAddress("DCISetClipList");
	__asm jmp eax;
}
ALCDECL hacker_DCISetDestination(void)
{
	GetAddress("DCISetDestination");
	__asm jmp eax;
}
ALCDECL hacker_DCISetSrcDestClip(void)
{
	GetAddress("DCISetSrcDestClip");
	__asm jmp eax;
}
ALCDECL hacker_GetDCRegionData(void)
{
	GetAddress("GetDCRegionData");
	__asm jmp eax;
}
ALCDECL hacker_GetWindowRegionData(void)
{
	GetAddress("GetWindowRegionData");
	__asm jmp eax;
}
ALCDECL hacker_WinWatchClose(void)
{
	GetAddress("WinWatchClose");
	__asm jmp eax;
}
ALCDECL hacker_WinWatchDidStatusChange(void)
{
	GetAddress("WinWatchDidStatusChange");
	__asm jmp eax;
}
ALCDECL hacker_WinWatchGetClipList(void)
{
	GetAddress("WinWatchGetClipList");
	__asm jmp eax;
}
ALCDECL hacker_WinWatchNotify(void)
{
	GetAddress("WinWatchNotify");
	__asm jmp eax;
}
ALCDECL hacker_WinWatchOpen(void)
{
	GetAddress("WinWatchOpen");
	__asm jmp eax;
}