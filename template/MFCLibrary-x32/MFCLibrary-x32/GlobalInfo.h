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
	// dll���������ĶԻ�����
	HWND m_hWndDlgMain;
};

