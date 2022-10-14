// Form3.cpp: 实现文件
//

#include "pch.h"
#include "MFCLibrary1.h"
#include "afxdialogex.h"
#include "Form3.h"


// Form3 对话框

IMPLEMENT_DYNAMIC(Form3, CDialogEx)

Form3::Form3(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FORM3, pParent)
{

}

Form3::~Form3()
{
}

void Form3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Form3, CDialogEx)
END_MESSAGE_MAP()


// Form3 消息处理程序

void Form3::DoExit()
{}