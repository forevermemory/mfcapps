#pragma once
#include "afxdialogex.h"


// CDialogRelocation 对话框

class CDialogRelocation : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogRelocation)

public:
	CDialogRelocation(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogRelocation();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DIRECTORY_RELOCATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL ParseRelocation();

public:
	CListCtrl m_Sec;
	CListCtrl m_Item;
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListDllList(NMHDR* pNMHDR, LRESULT* pResult);
};
