#include "pch.h"
#include "HookGameMainThread.h"
#pragma warning(disable:4996)
HHOOK g_hHkGame;
HWND g_hGame;

FILE* fp1;



void   PutZombie2(UINT zombieid, UINT x)
{

	__asm {
		pushad

		push x // 水平位置x坐标
		push zombieid // 僵尸编号
		mov eax, ds: [0x6A9EC0]
		mov eax, ds : [eax + 0x768]  // 参数3 [ [0x6A9EC0] + 0x768]
		mov ebx, 0x0040DDC0
		call ebx

		popad
	}
}

LRESULT GameWndProc(int code, WPARAM wParam, LPARAM lParam)
{
	char* str;
	CWPSTRUCT* lpArg = (CWPSTRUCT*)lParam;


	fprintf(fp1, "GameWndProc %d \n", code);
	fflush(fp1);


	if (code > HC_ACTION)
	{
		return CallNextHookEx(g_hHkGame, code, wParam, lParam);
	}

	// 自己的业务消息
	if (lpArg->hwnd == g_hGame && lpArg->message == WM_MYCODE_TO_MAIN_THREAD)
	{
		// 消息传到主线程中
		switch (lpArg->wParam)
		{
		case  MY_MSG_CODE_1:
			// 使用物品call  (char *)lpArg->lParam 为传入的lParam
			str = (char*)(lpArg->lParam);
			fprintf(fp1, "MY_MSG_CODE_1 %d  %s\n", lpArg->message, str);
			fflush(fp1);

			for (UINT i = 0; i < 6; i++)
			{
				PutZombie2(32,i);
			}

			return 1;
		default:
			break;
		}
	}
	
}


BOOL HookGameMainThread::HookMainThread()
{

	fp1 = fopen("a.txt", "w+");
	if (NULL == fp1)
	{
		AfxMessageBox("open file error");
		return FALSE;
	}
	fprintf(fp1, "HookMainThread  %s\n", "success");
	fflush(fp1);

	g_hGame = *(HWND*)BaseWndHandle;

	// 返回某个窗口的线程id 第二个参数是获取进程的pid
	DWORD mainThreadId = GetWindowThreadProcessId(g_hGame, NULL);
	g_hHkGame = SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC)GameWndProc, NULL, mainThreadId);
	return TRUE;

}

BOOL HookGameMainThread::UnHookMainThread()
{
	fclose(fp1);

	return UnhookWindowsHookEx(g_hHkGame);
}


void HookGameMainThread::PutPlants(char* pName)
{
	// 参数2: 自定义消息
	// 参数3: wParam
	// 参数4: lParam


	fprintf(fp1, "PutPlants  %s\n" , pName);
	fflush(fp1);

	::SendMessage(g_hGame, WM_MYCODE_TO_MAIN_THREAD, MY_MSG_CODE_1, (LPARAM)pName);
}
