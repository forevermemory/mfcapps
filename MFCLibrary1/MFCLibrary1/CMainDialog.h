#pragma once
#include "afxdialogex.h"


// CMainDialog 对话框

class CMainDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDialog)

public:
	CMainDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMainDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnDllExitDlg(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
	void CreateAssistItems();

public:
	CListCtrl m_Assist_items;
	virtual BOOL OnInitDialog();
};
