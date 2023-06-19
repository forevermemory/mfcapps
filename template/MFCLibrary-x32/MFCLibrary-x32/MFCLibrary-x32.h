// MFCLibrary-x32.h: MFCLibrary-x32 DLL 的主标头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号


// CMFCLibraryx32App
// 有关此类实现的信息，请参阅 MFCLibrary-x32.cpp
//

class CMFCLibraryx32App : public CWinApp
{
public:
	CMFCLibraryx32App();

// 重写
public:
	virtual BOOL InitInstance();

	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()

public:
	HANDLE m_Thread; // 记录线程句柄

};
