#include <windows.h>
#include <stdio.h>


#define LinkName L"\\??\\MyDDKDevice"
#pragma warning(disable:4996)

HANDLE hDevice = 0;

int main()
{
	HANDLE hDevice =
		CreateFile(LinkName,
			GENERIC_READ | GENERIC_WRITE,
			0,		// share mode none
			NULL,	// no security
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);		// no template

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("Failed to obtain file handle to device: "
			"%s with Win32 error code: %d\n",
			"MyWDMDevice", GetLastError());
		return 1;
	}

	printf("==============write=============\n");
	DWORD dwWriten = 0;
	char buff[32] = "r3 write to r0";
	WriteFile(hDevice, buff, sizeof(buff), &dwWriten, NULL);
	printf("WriteFile dwWriten:%d\n", dwWriten);
	
	printf("==============read=============\n");
	UCHAR buffer[10] = { 0 };
	DWORD dwRead = 0;
	BOOL bRet = ReadFile(hDevice, buffer, 10, &dwRead, NULL);
	if (bRet)
	{
		printf("Read %d bytes:", dwRead);
		for (int i = 0; i < dwRead; i++)
		{
			printf("%02X ", buffer[i]);
		}
		printf("\n");
	}

	CloseHandle(hDevice);


	printf("按任意键退出");
	getchar();

	return 0;
}