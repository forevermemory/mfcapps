#pragma once

#include "afxdialogex.h"

#define WM_DLL_EXIT_DLG WM_USER + 3000

class GlobalInfo
{

public:
	GlobalInfo(void);
	~GlobalInfo();
	static GlobalInfo* GetInstance();

private:
	static GlobalInfo* m_pInstance;


public:
	// dll创建出来的对话框句柄
	HWND m_hWndDlgMain;
};

