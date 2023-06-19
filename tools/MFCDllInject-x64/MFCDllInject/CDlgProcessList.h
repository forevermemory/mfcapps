#pragma once
#include "afxdialogex.h"


// CDlgProcessList 对话框

class CDlgProcessList : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProcessList)

public:
	CDlgProcessList(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgProcessList();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROCESS_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonProcessConfirm();
	afx_msg void OnBnClickedButtonProcessRefresh();

public:
	void RefreshProcessList();
	DWORD m_Pid;

	CListCtrl m_Precsss_lists;
	CListBox m_Process;
};
