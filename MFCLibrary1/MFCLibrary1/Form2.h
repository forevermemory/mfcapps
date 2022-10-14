#pragma once
#include "afxdialogex.h"
#include "FormBase.h"

// Form2 对话框

class Form2 : public CDialogEx, public FormBase
{
	DECLARE_DYNAMIC(Form2)

public:
	Form2(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Form2();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORM2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	virtual CWnd* GetWnd() { return this; };
	virtual void DoExit();

	DECLARE_MESSAGE_MAP()
};
