#pragma once
#include "afxdialogex.h"
#include "FormBase.h"


// Form1 对话框

class Form1 : public CDialogEx , public FormBase
{
	DECLARE_DYNAMIC(Form1)

public:
	Form1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Form1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORM1 };
#endif

public:
	virtual CWnd* GetWnd() { return this; };
	virtual void DoExit();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
