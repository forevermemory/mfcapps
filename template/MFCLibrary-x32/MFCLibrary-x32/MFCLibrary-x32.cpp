// MFCLibrary-x32.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "MFCLibrary-x32.h"
#include "CMainDialog.h"  // CMainDialog 是创建的对话框, 并关联了类
#include "GlobalInfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CMFCLibraryx32App

BEGIN_MESSAGE_MAP(CMFCLibraryx32App, CWinApp)
END_MESSAGE_MAP()


// CMFCLibraryx32App 构造

CMFCLibraryx32App::CMFCLibraryx32App()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CMFCLibraryx32App 对象

CMFCLibraryx32App theApp;

DWORD WINAPI ShowMainDlg(LPVOID pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CMainDialog m_Dlg;
	m_Dlg.DoModal();

	return 0;
}

// CMFCLibraryx32App 初始化



BOOL CMFCLibraryx32App::InitInstance()
{
	CWinApp::InitInstance();

	m_Thread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ShowMainDlg,
		NULL, NULL, NULL);


	return TRUE;
}



int CMFCLibraryx32App::ExitInstance()
{
	// 发送退出消息给对话框
	::SendMessage(GlobalInfo::GetInstance()->m_hWndDlgMain, WM_CLOSE, 0, 0);

	// 退出线程
	::TerminateThread(m_Thread, 0);
	::WaitForSingleObject(m_Thread, INFINITE);

	return CWinApp::ExitInstance();
}

