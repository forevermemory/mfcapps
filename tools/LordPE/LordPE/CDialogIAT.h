#pragma once
#include "afxdialogex.h"


// CDialogIAT 对话框

class CDialogIAT : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogIAT)

public:
	CDialogIAT(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogIAT();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_IAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL ParseIAT();

public:
	virtual BOOL OnInitDialog();
	CString m_Detail;
};
