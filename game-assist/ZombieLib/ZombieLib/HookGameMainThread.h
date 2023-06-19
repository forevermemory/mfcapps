#pragma once

#define BaseWndHandle 0x6E9D841C
#define	WM_MYCODE_TO_MAIN_THREAD WM_USER + 1
#define MY_MSG_CODE_1 1





class HookGameMainThread
{

public:
	BOOL HookMainThread();
	void PutPlants(char* pName);
	BOOL UnHookMainThread();
};

