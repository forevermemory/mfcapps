// Taichu.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "Taichu.h"
#include "CDialogMain.h"
#include "GlobalInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CTaichuApp, CWinApp)
END_MESSAGE_MAP()


// CTaichuApp 构造

CTaichuApp::CTaichuApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CTaichuApp 对象

CTaichuApp theApp;


DWORD WINAPI ShowMainDlg(LPVOID pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDialogMain  m_Dlg; // CMainDialog  是一个对话框 
	m_Dlg.DoModal();

	return 0;
}

// CTaichuApp 初始化

BOOL CTaichuApp::InitInstance()
{
	CWinApp::InitInstance();

	m_Thread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ShowMainDlg,
		NULL, NULL, NULL);
	return TRUE;
}

int CTaichuApp::ExitInstance()
{
	// 发送退出消息给对话框  SendMessage同步执行 WM_DLL_EXIT_DLG 自定义消息
	::SendMessage(GlobalInfo::GetInstance()->m_hWndDlgMain, WM_CLOSE, 0, 0);
	// 退出线程
	::TerminateThread(m_Thread, 0);
	::WaitForSingleObject(m_Thread, INFINITE);
	// 方式2：

	return CWinApp::ExitInstance();
}