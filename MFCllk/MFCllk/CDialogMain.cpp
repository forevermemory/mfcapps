// CDialogMain.cpp: 实现文件
//

#include "pch.h"
#include "MFCllk.h"
#include "afxdialogex.h"
#include "CDialogMain.h"


// CDialogMain 对话框

IMPLEMENT_DYNAMIC(CDialogMain, CDialogEx)

CDialogMain::CDialogMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCLLK_DIALOG, pParent)
	, m_Life(FALSE)
{

}

CDialogMain::~CDialogMain()
{
}

void CDialogMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK3, m_Life);
}


BEGIN_MESSAGE_MAP(CDialogMain, CDialogEx)
	
END_MESSAGE_MAP()


// CDialogMain 消息处理程序


