// Form1.cpp: 实现文件
//

#include "pch.h"
#include "MFCLibrary1.h"
#include "afxdialogex.h"
#include "Form1.h"


// Form1 对话框

IMPLEMENT_DYNAMIC(Form1, CDialogEx)

Form1::Form1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FORM1, pParent)
{

}

Form1::~Form1()
{
}

void Form1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Form1, CDialogEx)
END_MESSAGE_MAP()


// Form1 消息处理程序
void Form1::DoExit()
{}