#include <stdio.h>
#include <Windows.h>


DWORD getHashCode(const char* str)
{
	DWORD digest = 0;
	while (*str)
	{
		digest = (digest << 25 | digest >> 7);
		digest = digest + *str;
		str++;
	}
	return digest;
}
const char* str = "helloworld";
void __declspec(naked) asmHashCode()
{

	// 函数使用 
	// push str 首地址
	// call xxx

	__asm
	{
		push ebp
		mov ebp, esp

		sub esp,0x4 // 分配四字节空间
		mov dword ptr [ebp-0x4], 0

		push ebx
		push ecx
		push edx
		push esi

		mov esi, [ebp + 0x08] // 参数的地址, 即字符串地址
		// 用[ebp-0x4]存放digest  ecx 存放循环index 
		xor ecx, ecx
		xor edx, edx
		xor ebx, ebx
	tag_loop_start: // eax 的al存放字节  ebx edx存放中间变量
		xor eax, eax
		mov al, [esi + ecx]
		test al,al
		jz end    // 比较是否到字符串结尾
		mov ebx, dword ptr[ebp - 0x4]
		mov edx, dword ptr[ebp - 0x4]
		shl edx, 25
		shr ebx, 7
		or edx, ebx
		add edx, eax
		inc ecx
		mov dword ptr[ebp - 0x4], edx
		jmp tag_loop_start

	end:
		pop esi
		pop edx
		pop ecx
		pop ebx

		mov eax, [ebp - 0x4]
		mov esp, ebp
		pop ebp
		retn
	}
}

DWORD getHashCodeWchar(const wchar_t* str)
{
	DWORD digest = 0;
	while (*str)
	{
		digest = (digest << 25 | digest >> 7);
		digest = digest + *str;
		str++;
	}
	return digest;
}


int mainC233()
{

	//const wchar_t* str = TEXT("KERNELBASE.dll"); // 84871AB2
	//DWORD result = getHashCodeWchar(str);
	//printf("hashcode:%X \n", result); 
	


	const char* arr[30] = {
		"LoadLibraryA",
		"LoadLibraryExA",
		"MessageBoxA",
		"ExitProcess",
		"LoadLibraryExA",
		"WSAStartup",
		"WSASocketA",
		"bind",
		"listen",
		"accept",
		"CreateProcessA",
		"CloseHandle",
		
		0
	};
	
	for (size_t i = 0; i < 30; i++)
	{
		if (arr[i] == 0)break;
		DWORD result = getHashCode(arr[i]);
		printf("%s %X \n", arr[i], result);
	}
	return 0;
}

/*
wchar_t
KERNEL32.DLL  228C4218
KERNELBASE.dll 84871AB2

char
LoadLibraryA C917432
LoadLibraryExA C0D83287
MessageBoxA 1E380A6A
ExitProcess 4FD18963
LoadLibraryExA C0D83287
WSAStartup 80B46A3D
WSASocketA DE78322D
bind DDA71064
listen 4BD39F0C
accept 1971EB1
CreateProcessA 6BA6BCC9
CloseHandle FF0D6657
*/

/*

*/