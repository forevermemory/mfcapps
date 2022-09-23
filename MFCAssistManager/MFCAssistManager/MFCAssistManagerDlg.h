
// MFCAssistManagerDlg.h: 头文件
//

#pragma once

#include <vector>
#include <map>
#include <set>
#include <tlhelp32.h>
#include "CTrayIcon.h"

//#include <functional>


typedef struct ProcessItemData
{
	bool openAssist; // 是否打开辅助
	HWND game_hwnd; // 游戏的窗口句柄

	int listIndex; // 所有
	DWORD pid; // 游戏进程id
	char username[64]; // 用户名
	char processName[64]; // 进程名称

	HWND assist_hwnd; // 辅助的窗口句柄


}ProcessItemData;


typedef struct ProcessAssistData
{

	HWND assist_hwnd; // 辅助的窗口句柄
	DWORD pid; 
	char username[64]; // 用户名


}ProcessAssistData;

typedef  std::vector<ProcessAssistData> ASSIST_VECTOR;


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

	CString m_strGamePath;

	// 右键菜单
	CMenu m_Rmenu;

public:
	void GetGameProcessList(CString processName);
	void UpdateGameProgressList();
	int FindProcess(DWORD pid);
	CString GetAppPath();
	void ListAssistWnd(ASSIST_VECTOR & data);
	HWND GetGameWnd(DWORD pid);
	int FindPidFromCur(ASSIST_VECTOR& data, DWORD pid);

protected:
	afx_msg LRESULT OnMsgIconNotify(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonBrowser();
	afx_msg void OnMenuExit();
	afx_msg void OnMenuHide();
	afx_msg void OnMenuShow();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonRunGame();
	afx_msg void OnBnClickedButtonRunAssist();
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonExitAssist();
	afx_msg void OnBnClickedButtonShowHideGame();
	afx_msg void OnBnClickedButtonShowHideAssist();
};
