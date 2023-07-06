
// MFCDllInjectDlg.h: 头文件
//

#pragma once
#include <vector>
#include "LoadLibraryR.h"



typedef struct _PROCESS_INFO {
	DWORD pid;
	CString processName;
	DWORD arch; // 32 64
} PROCESS_INFO, * PPROCESS_INFO;

// CMFCDllInjectDlg 对话框
class CMFCDllInjectDlg : public CDialogEx
{
// 构造
public:
	CMFCDllInjectDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCDLLINJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonUninstall();
	afx_msg void OnBnClickedButtonSelectDll();
	void GetProcessList();

public:
	CString m_DllPath;

	ULONG64 m_LoadLibraryA_Addr;
	ULONG64 m_LoadLibraryW_Addr;
	
	int m_Select_Pid;
	PPROCESS_INFO m_Select_Process;
	CListCtrl m_Process_List;
	std::vector<PPROCESS_INFO> m_plistVec;
	afx_msg void OnColumnclickListProcessList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListProcessList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonRefreshProcess();
	afx_msg void OnBnClickedButtonRemoteThread();
	afx_msg void OnBnClickedButtonRtlcreateuserthread();
	afx_msg void OnBnClickedButtonNtcreatethreadex();
	afx_msg void OnBnClickedButtonApc();
	afx_msg void OnBnClickedButtonHijack();
	afx_msg void OnBnClickedButtonHook();
	afx_msg void OnBnClickedButtonReflect();
};


