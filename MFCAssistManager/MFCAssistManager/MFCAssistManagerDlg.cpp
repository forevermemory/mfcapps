
// MFCAssistManagerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCAssistManager.h"
#include "MFCAssistManagerDlg.h"
#include "afxdialogex.h"
#include "data.h"
#include <stdio.h>


#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCAssistManagerDlg 对话框



CMFCAssistManagerDlg::CMFCAssistManagerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCASSISTMANAGER_DIALOG, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CMFCAssistManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_GAME_PROCESS, m_GameProcess);
}

BEGIN_MESSAGE_MAP(CMFCAssistManagerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_MESSAGE(MSG_ICON_NOTIFY, &CMFCAssistManagerDlg::OnMsgIconNotify)
END_MESSAGE_MAP()


// CMFCAssistManagerDlg 消息处理程序

BOOL CMFCAssistManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE | SW_SHOWNA);

	// TODO: 在此添加额外的初始化代码
	SetWindowText(FZ_WINDOW_TITLE);

	m_GameProcess.ModifyStyle(LVS_ICON | LVS_SMALLICON | LVS_LIST, LVS_REPORT);
	m_GameProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT  | LVS_EX_GRIDLINES);

	m_GameProcess.InsertColumn(0, "进程ID", LVCFMT_LEFT, 60);
	m_GameProcess.InsertColumn(1, "进程名称", LVCFMT_LEFT, 110);
	m_GameProcess.InsertColumn(2, "角色名称", LVCFMT_LEFT, 148);

	// 开启扫描游戏进程的定时器
	SetTimer(1, 1000, NULL);

	// 注册托盘
	m_TrayIcon.InitTrayIcon(this);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCAssistManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCAssistManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCAssistManagerDlg::GetGameProcessList(CString processName)
{
	// 实现方式是获取进程快照
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	BOOL ret = Process32First(hSnapShot, &pe32);

	// 遍历每个进程名称和目标进程比较
	while (ret)
	{
		CString tmp = pe32.szExeFile;
		tmp.MakeLower();
		if (tmp == processName)
		{
			m_FindPids.insert(pe32.th32ProcessID);
		}
		ret = Process32Next(hSnapShot, &pe32);
	}

	CloseHandle(hSnapShot);
}

void CMFCAssistManagerDlg::UpdateGameProgressList()
{

}

void CMFCAssistManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetGameProcessList(FZ_GAME_PROCESS_NAME);

	CString str;

	

	// 动态增加和删除  游戏进程关闭了 这里也需要动态减少
	// 新旧全放到 m_TmpPids     
	for (std::set<DWORD>::iterator it = m_CurPids.begin(); it != m_CurPids.end(); it++)
	{
		m_TmpPids.insert((*it));
	}
	for (std::set<DWORD>::iterator it = m_FindPids.begin(); it != m_FindPids.end(); it++)
	{
		m_TmpPids.insert((*it));
	}


	//for (std::set<DWORD>::iterator it = m_TmpPids.begin(); it != m_TmpPids.end(); it++)
	//{
	//	CString a;
	//	a.Format(" %d ", (*it));
	//	str += a;
	//}

	//AfxMessageBox(str);
	//return;

	// 遍历 m_TmpPids 再分别判断 m_CurPids m_FindPids
	for (std::set<DWORD>::iterator it = m_TmpPids.begin(); it != m_TmpPids.end(); it++)
	{
		DWORD pid = *it;

		/*
		tmp: 1 2 3 4
		cur: 1 2 3
		find: 1 2 4
		*/

		// 1. 到cur看看有没有 如果没有 添加进去
		if (m_CurPids.find(pid) == m_CurPids.end())
		{
			int index = m_GameProcess.GetItemCount();
	   
			ProcessItemData* pData = (ProcessItemData*)malloc(sizeof(ProcessItemData));
			memset(pData, 0, sizeof(ProcessItemData));

			pData->pid = pid;
			pData->openAssist = false;
			sprintf(pData->username, "%s", "暂未开启辅助");
			sprintf(pData->processName, "%s", FZ_GAME_PROCESS_NAME);

			str.Format("%d", pid);
			m_GameProcess.InsertItem(index, str);
			m_GameProcess.SetItemText(index, 1, FZ_GAME_PROCESS_NAME);
			m_GameProcess.SetItemText(index, 2, pData->username);
			m_GameProcess.SetItemData(index,(DWORD_PTR)pData);

			m_CurPids.insert(pid);
			continue;
		}

		// 2. 如果cur存在

		// 3. 到find查找 有没有 
		
		// 4. 有pass

		// 5. 没有删除这个选项
		if (m_FindPids.find(pid) == m_FindPids.end())
		{
			int nIndex = FindProcess(pid);
			if (nIndex != -1)
			{
				ProcessItemData* pData = (ProcessItemData*)m_GameProcess.GetItemData(nIndex);
				free(pData);
				pData = NULL;
				m_GameProcess.DeleteItem(nIndex);

				m_CurPids.erase(pid);
			}
		}
	}

	m_TmpPids.clear();
	m_FindPids.clear();
	CDialogEx::OnTimer(nIDEvent);
}



// 根据进程id到当前表格项里面寻找是否存在 存在返回index 否则返回-1
int CMFCAssistManagerDlg::FindProcess(DWORD inPid)
{
	int curTotal = m_GameProcess.GetItemCount();
	for (int i = 0; i < curTotal; i++)
	{
		ProcessItemData* pData = (ProcessItemData*)m_GameProcess.GetItemData(i);
		if (pData->pid == inPid)
			return i;
	}

	return -1;
}

afx_msg LRESULT CMFCAssistManagerDlg::OnMsgIconNotify(WPARAM wParam, LPARAM lParam)
{

	if (lParam == WM_LBUTTONDOWN) 
	{
		ShowWindow(IsWindowVisible() ? SW_HIDE : SW_SHOWNA);
	}

	if (lParam == WM_RBUTTONDOWN)
	{
		AfxMessageBox("鼠标右键点击"); // 可以创建一个菜单并显示
	}
	return 0;
}
