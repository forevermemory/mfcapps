﻿// Taichu.h: Taichu DLL 的主标头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号


// CTaichuApp
// 有关此类实现的信息，请参阅 Taichu.cpp
//

class CTaichuApp : public CWinApp
{
public:
	CTaichuApp();

// 重写
public:
	virtual BOOL InitInstance();
	int ExitInstance();
	HANDLE m_Thread;
	DECLARE_MESSAGE_MAP()
};
