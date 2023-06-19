#pragma once
#include "afxdialogex.h"


// CDialogExport 对话框

class CDialogExport : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogExport)

public:
	CDialogExport(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogExport();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DIRECTORY_EXPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	BOOL ParseExport();
	CListCtrl m_ListExport;
	CString m_Func_Addr;
	CString m_Func_Name;
	CString m_Func_Ord;
	CString m_DllName;
	CString m_Num_func;
	CString m_Num_funcname;
};
