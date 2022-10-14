// Form2.cpp: 实现文件
//

#include "pch.h"
#include "MFCLibrary1.h"
#include "afxdialogex.h"
#include "Form2.h"


// Form2 对话框

IMPLEMENT_DYNAMIC(Form2, CDialogEx)

Form2::Form2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FORM2, pParent)
{

}

Form2::~Form2()
{
}

void Form2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Form2, CDialogEx)
END_MESSAGE_MAP()


// Form2 消息处理程序


void Form2::DoExit()
{}