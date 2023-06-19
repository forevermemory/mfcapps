#pragma once
#include "afxdialogex.h"


// CDialogImport 对话框

class CDialogImport : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogImport)

public:
	CDialogImport(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogImport();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DIRECTORY_IMPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_DllList;
	CListCtrl m_dll_items;

public:
	BOOL ParseImport();
	afx_msg void OnClickListDllList(NMHDR* pNMHDR, LRESULT* pResult);
};
