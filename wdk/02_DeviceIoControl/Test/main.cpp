#include <windows.h>
#include <stdio.h>
#include <winioctl.h>


#define LinkName L"\\??\\MyDDKDevice"
#pragma warning(disable:4996)


#define IOCTL_TEST_1 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
HANDLE hDevice = 0;

typedef struct  _MY_STRUCT {
	ULONG64 Size;
	ULONG64 TYPE;
	char buf[32];
	wchar_t wbuf[64];
	char data[1024];
} MY_STRUCT, *PMY_STRUCT;



int main()
{
    hDevice = CreateFileW(LinkName,
		GENERIC_READ | GENERIC_WRITE, // 读写
		FILE_SHARE_READ | FILE_SHARE_WRITE, // 共享读写
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	// TODO: 在此添加控件通知处理程序代码
	MY_STRUCT st = { 0 };

	strcpy(st.buf, "hello wdk==");
	StrCpyW(st.wbuf, L"hello 2 wdk==");

	MY_STRUCT stout = { 0 };
	DWORD ret = 0;
	DeviceIoControl(g_FileHandle, IOCTL_TEST_1,
		&st, sizeof(st),
		&stout,sizeof(stout),
		&ret, NULL);
	
	OutputDebugStringA("$$$r3===================");
	OutputDebugStringW((wchar_t*)stout.data);
	CString ostr;
	ostr.Format("$$$r3out:size:%llX, type:%llX", stout.Size, stout.TYPE);
	OutputDebugStringA(ostr);
	OutputDebugStringA("$$$r3===================");

	CloseHandle(hDevice);
	return 0;
}