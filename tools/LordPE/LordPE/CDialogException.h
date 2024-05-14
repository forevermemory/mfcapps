#pragma once
#include "afxdialogex.h"


// CDialogException 对话框

class CDialogException : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogException)

public:
	CDialogException(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogException();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DIRECTORY_EXCEPTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_ExceptionList;
	BOOL ParseException();
//	CString m_ExceptionRVA;
//	CString m_Exception_RVA;
//	CString m_Exception_RVA;
	CString m_Exception_RVA;
	CString m_Exception_Count;
};
