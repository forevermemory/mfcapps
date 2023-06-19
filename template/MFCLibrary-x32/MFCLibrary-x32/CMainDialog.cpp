// CMainDialog.cpp: 实现文件
//

#include "pch.h"
#include "MFCLibrary-x32.h"
#include "afxdialogex.h"
#include "CMainDialog.h"
#include "GlobalInfo.h"


// CMainDialog 对话框

IMPLEMENT_DYNAMIC(CMainDialog, CDialogEx)

CMainDialog::CMainDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MAIN, pParent)
{

}

CMainDialog::~CMainDialog()
{
}

void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MSGBOX, &CMainDialog::OnBnClickedButtonMsgbox)
END_MESSAGE_MAP()





BOOL CMainDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
		// 保存窗口句柄
	GlobalInfo::GetInstance()->m_hWndDlgMain = m_hWnd;

	return TRUE;
}


void CMainDialog::OnBnClickedButtonMsgbox()
{
	// TODO: 在此添加控件通知处理程序代码

	MessageBoxA("helloworld", 0 , 2);
}
