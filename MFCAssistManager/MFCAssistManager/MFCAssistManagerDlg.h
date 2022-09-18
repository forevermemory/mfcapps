
// MFCAssistManagerDlg.h: 头文件
//

#pragma once

#include <map>
#include <set>
#include <tlhelp32.h>
#include "CTrayIcon.h"

//#include <functional>


typedef struct ProcessItemData
{
	bool openAssist;
	int listIndex;
	DWORD pid;
	char username[64];
	char processName[64];


}ProcessItemData;


// CMFCAssistManagerDlg 对话框
class CMFCAssistManagerDlg : public CDialogEx
{
// 构造
public:
	CMFCAssistManagerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCASSISTMANAGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);


	DECLARE_MESSAGE_MAP()


public:
	CListCtrl m_GameProcess;
	std::set<DWORD> m_TmpPids;
	std::set<DWORD> m_CurPids;
	std::set<DWORD> m_FindPids;

	CTrayIcon m_TrayIcon;

public:
	void GetGameProcessList(CString processName);
	void UpdateGameProgressList();
	int FindProcess(DWORD pid);
protected:
	afx_msg LRESULT OnMsgIconNotify(WPARAM wParam, LPARAM lParam);
};
