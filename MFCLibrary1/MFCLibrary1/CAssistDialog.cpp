// CAssistDialog.cpp: 实现文件
//

#include "pch.h"
#include "MFCLibrary1.h"
#include "afxdialogex.h"
#include "CAssistDialog.h"


// CAssistDialog 对话框

IMPLEMENT_DYNAMIC(CAssistDialog, CDialogEx)

CAssistDialog::CAssistDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CAssistDialog, pParent)
{



}

CAssistDialog::~CAssistDialog()
{
}

void CAssistDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAssistDialog, CDialogEx)
END_MESSAGE_MAP()


// CAssistDialog 消息处理程序


BOOL CAssistDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CTime tm = CTime::GetCurrentTime();

	CString str;
	str.Format("用户名:%s-完美辅助", tm.Format("%H:%M:%S"));
	SetWindowText(str);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CAssistDialog::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}
