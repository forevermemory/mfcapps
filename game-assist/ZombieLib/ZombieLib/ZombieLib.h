// ZombieLib.h: ZombieLib DLL 的主标头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号


// CZombieLibApp
// 有关此类实现的信息，请参阅 ZombieLib.cpp
//

class CZombieLibApp : public CWinApp
{
public:
	CZombieLibApp();

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()


public:
	HANDLE m_Thread;
	HWND m_Hwnd;
};
