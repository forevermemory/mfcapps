#include<windows.h>
#include <iostream>

int mainF33()
{
	int nRet = 0;
	HINSTANCE h;

	h = GetModuleHandleA("ntdll.dll");
	if (h == NULL)
	{
		h = LoadLibraryA("ntdll.dll");
	}
	BYTE* ptr = (BYTE*)h;
	bool done = false;
	DWORD Range = DWORD(ptr) + 0x1A0000;

	while (DWORD (ptr) <= Range)
	{
		if (*ptr == 0xFF && *(ptr+1) == 0xE4)
		{
			//int pos = (int)ptr + y;
			printf("jmp esp at 0X%X\n", ptr);
		}
		ptr++;
	}

	printf("please press [Enter] exit program\n");
	getchar();
	FreeLibrary(h);
	return nRet;
}