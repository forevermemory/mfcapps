
// MFCAssistManagerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCAssistManager.h"
#include "MFCAssistManagerDlg.h"
#include "afxdialogex.h"
#include "CrashDump.h"
#include "data.h"
#include <stdio.h>


#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCAssistManagerDlg 对话框



CMFCAssistManagerDlg::CMFCAssistManagerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCASSISTMANAGER_DIALOG, pParent)
	, m_strGamePath(_T(""))
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CMFCAssistManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_GAME_PROCESS, m_GameProcess);
	DDX_Text(pDX, IDC_EDIT_GAME_PATH, m_strGamePath);
}

BEGIN_MESSAGE_MAP(CMFCAssistManagerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_MESSAGE(MSG_ICON_NOTIFY, &CMFCAssistManagerDlg::OnMsgIconNotify)
	ON_BN_CLICKED(IDC_BUTTON_BROWSER, &CMFCAssistManagerDlg::OnBnClickedButtonBrowser)
	ON_COMMAND(ID_MENU_EXIT, &CMFCAssistManagerDlg::OnMenuExit)
	ON_COMMAND(ID_MENU_HIDE, &CMFCAssistManagerDlg::OnMenuHide)
	ON_COMMAND(ID_MENU_SHOW, &CMFCAssistManagerDlg::OnMenuShow)
	ON_WM_KILLFOCUS()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_RUN_GAME, &CMFCAssistManagerDlg::OnBnClickedButtonRunGame)
	ON_BN_CLICKED(IDC_BUTTON_RUN_ASSIST, &CMFCAssistManagerDlg::OnBnClickedButtonRunAssist)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CMFCAssistManagerDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_EXIT_ASSIST, &CMFCAssistManagerDlg::OnBnClickedButtonExitAssist)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_HIDE_GAME, &CMFCAssistManagerDlg::OnBnClickedButtonShowHideGame)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_HIDE_ASSIST, &CMFCAssistManagerDlg::OnBnClickedButtonShowHideAssist)
	ON_BN_CLICKED(IDC_BUTTON8, &CMFCAssistManagerDlg::OnBnClickedButton8)
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

	// 加载右键托盘菜单图标
	m_Rmenu.LoadMenu(IDR_MENU1);

	//SetUnhandledExceptionFilter(exceptionHandler);
	SetUnhandledExceptionFilter(unhandled_handler);


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
	HANDLE hSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	BOOL ret = ::Process32First(hSnapShot, &pe32);

	// 遍历每个进程名称和目标进程比较
	while (ret)
	{
		CString tmp = pe32.szExeFile;
		tmp.MakeLower();
		if (tmp == processName)
		{
			m_FindPids.insert(pe32.th32ProcessID);
		}
		ret = ::Process32Next(hSnapShot, &pe32);
	}

	::CloseHandle(hSnapShot);
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
			//pData->game_hwnd = NULL; 
			pData->game_hwnd =  GetGameWnd(pData->pid); // 游戏自身句柄
			sprintf(pData->processName, "%s", FZ_GAME_PROCESS_NAME);

			str.Format("%d", pid);
			m_GameProcess.InsertItem(index, str);
			m_GameProcess.SetItemText(index, 1, FZ_GAME_PROCESS_NAME);
			//m_GameProcess.SetItemText(index, 2, pData->username);
			m_GameProcess.SetItemText(index, 2, "暂未运行");
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
			continue;
		}

		

	}

	// 更新角色名称数据
	ASSIST_VECTOR data;
	ListAssistWnd(data);
	bool find = false;


	// 到cur找对应的pid
	for (std::set<DWORD>::iterator it = m_CurPids.begin(); it != m_CurPids.end(); it++)
	{
		DWORD tmpPid = *it;
		int nIndex = FindProcess(tmpPid);
		ProcessItemData* pData = (ProcessItemData*)m_GameProcess.GetItemData(nIndex);

		int isFind = FindPidFromCur(data, tmpPid);
		if (isFind >=0 )
		{
			m_GameProcess.SetItemText(nIndex, 2, data[isFind].username);
			pData->assist_hwnd = data[isFind].assist_hwnd;
		}
		else
		{
			m_GameProcess.SetItemText(nIndex, 2, "");
		}
	}


	m_TmpPids.clear();
	m_FindPids.clear();
	CDialogEx::OnTimer(nIDEvent);
}

int CMFCAssistManagerDlg::FindPidFromCur(ASSIST_VECTOR & data, DWORD pid)
{

	for (int i = 0; i < data.size(); i++)
	{
		if (pid == data[i].pid)
		{
			return i;
		}
	}
	return -1;
}

CString CMFCAssistManagerDlg::GetAppPath()
{
	char buf[MAX_PATH];
	GetModuleFileName(NULL, buf, MAX_PATH);
	CString tmp = buf;
	tmp = tmp.Left(tmp.ReverseFind('\\') + 1);
	return tmp;
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

	switch (lParam)
	{
	case WM_LBUTTONDOWN:
	{
		if (!IsWindowVisible())
			ShowWindow(SW_SHOWNA);
		break;
	}
	case WM_RBUTTONDOWN:
	{

		CMenu* pPopup = m_Rmenu.GetSubMenu(0);
		CPoint point;
		GetCursorPos(&point);

		// 先调用 SetForegroundWindow 点击其它地方才会隐藏菜单
		// 怎么点击其它地方 隐藏 TPM_RETURNCMD 这个参数只能点击app窗口之后才能消失
		SetForegroundWindow();
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

		break;
	}
	default:
		break;
	}

	return 0;
}


void CMFCAssistManagerDlg::OnBnClickedButtonBrowser()
{
	// TODO: 在此添加控件通知处理程序代码

	CFileDialog dlg(TRUE, NULL, NULL, 4 | 2, "应用程序(*.exe)|*.exe||");
	if (dlg.DoModal() == IDOK)
	{
		m_strGamePath = dlg.GetPathName();
	}

	UpdateData(FALSE);
}


void CMFCAssistManagerDlg::OnMenuExit()
{
	// TODO: 在此添加命令处理程序代码
	OnOK();
}


void CMFCAssistManagerDlg::OnMenuHide()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_HIDE );
}


void CMFCAssistManagerDlg::OnMenuShow()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_SHOWNA);
}


void CMFCAssistManagerDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}





void CMFCAssistManagerDlg::OnClose()
{

	//ShowWindow(SW_HIDE);
	//return;
	CDialogEx::OnClose();
}


void CMFCAssistManagerDlg::OnBnClickedButtonRunGame()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_strGamePath.IsEmpty())
	{
		MessageBox("路径不能为空");
		return;
	}

	CString strPath = m_strGamePath;
	::SetCurrentDirectory(strPath.Left(strPath.ReverseFind('\\')));
	::ShellExecute(NULL, NULL, m_strGamePath, NULL, NULL, SW_SHOW);
}


void CMFCAssistManagerDlg::OnBnClickedButtonRunAssist()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	POSITION pos = m_GameProcess.GetFirstSelectedItemPosition();
	int nPos = m_GameProcess.GetNextSelectedItem(pos);
	if (-1 == nPos)
	{
		AfxMessageBox("请选择一个游戏进程");
		return;
	}

	// 根据进程id获取进程句柄
	ProcessItemData * data = (ProcessItemData *)m_GameProcess.GetItemData(nPos);
	if(data->openAssist)
	{
		AfxMessageBox("当前进程已经加载dll, 请勿重复加载");
		return;
	}
	//CString str2;
	//str2.Format("%d-%s", data->pid, data->username);
	//MessageBox(str2);

	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, data->pid);
	if (NULL == hProcess)
	{
		AfxMessageBox("打开进程失败");
		return;
	}
	
	// 分配参数空间
	CString param(FZ_DLL_PATH);
	LPVOID pRemoteParam = ::VirtualAllocEx(hProcess, NULL, param.GetLength()+1,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (pRemoteParam == NULL)
	{
		AfxMessageBox("分配参数空间失败");
		return;
	}

	// 写入参数到目标进程
	DWORD dwWriten = 0;
	BOOL res = ::WriteProcessMemory(hProcess, pRemoteParam, param.GetBuffer(0),
		param.GetLength(), &dwWriten);
	if (!res)
	{
		AfxMessageBox("写入参数到目标进程失败");
		goto END;
	}

	// kernel32是内核函数 每个进程都会加载 在操作系统分配的地址都是一致的
	HMODULE hModule = ::GetModuleHandle("Kernel32.dll");
	PTHREAD_START_ROUTINE pRemoteFunc = (PTHREAD_START_ROUTINE)::GetProcAddress(hModule, "LoadLibraryA");
	if(NULL == pRemoteFunc)
	{
		AfxMessageBox("获取LoadLibraryA地址失败");
		goto END;
	}

	HANDLE pThread = CreateRemoteThread(hProcess, NULL, 0, pRemoteFunc,
		pRemoteParam, 0, NULL);
	if (NULL == pThread)
	{
		AfxMessageBox("创建远程线程失败");
		goto END;
	}

	// waitfor 
	 WaitForSingleObject(pThread, INFINITE);
	 //DWORD dwVal = 0;
	 //GetExitCodeThread(pThread, &dwVal);
	 //DWORD err = GetLastError();
	 //str.Format("%d-%d", dwVal, err);
	 //AfxMessageBox(str);

	 data->openAssist = true;


END:
	::VirtualFreeEx(hProcess, pRemoteParam, param.GetLength(), MEM_DECOMMIT);

}

void CMFCAssistManagerDlg::ListAssistWnd(ASSIST_VECTOR & data)
{
	//Sleep(1000);


	HWND hwnd = ::FindWindow(NULL, GW_HWNDFIRST);

	int pos = 0;
	while (NULL != hwnd)
	{
		char fz[256] = { 0 };
		::GetWindowTextA(hwnd, fz, sizeof(fz));
		CString tmp(fz);
	/*	AfxMessageBox(tmp);*/
		pos = tmp.Find("-完美辅助");
		if (pos >= 0)
		{
			ProcessAssistData ad;
			ad.assist_hwnd = hwnd;
			sprintf(ad.username, "%s", tmp.Left(pos).GetBuffer(0));
			::GetWindowThreadProcessId(hwnd, &ad.pid);
			data.push_back(ad);
		}
		hwnd = ::GetNextWindow(hwnd, GW_HWNDNEXT);
	}
}


HWND CMFCAssistManagerDlg::GetGameWnd(DWORD pid)
{

	HWND hwnd = ::FindWindow(NULL, GW_HWNDFIRST);
	int pos = 0;
	while (NULL != hwnd)
	{
		char fz[256] = { 0 };
		::GetWindowTextA(hwnd, fz, sizeof(fz));
		CString tmp(fz);
		pos = tmp.Find("完美世界v1.0");
		if (pos >= 0)
		{
			DWORD tmppid = 0;
			::GetWindowThreadProcessId(hwnd, &tmppid);
			if (tmppid == pid)
			{
				return hwnd;
			}
		}
		hwnd = ::GetNextWindow(hwnd, GW_HWNDNEXT);
	}
}

void CMFCAssistManagerDlg::OnBnClickedButtonExit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


void CMFCAssistManagerDlg::OnBnClickedButtonExitAssist()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	POSITION pos = m_GameProcess.GetFirstSelectedItemPosition();
	int nPos = m_GameProcess.GetNextSelectedItem(pos);
	if (-1 == nPos)
	{
		AfxMessageBox("请选择一个游戏进程");
		return;
	}

	// 根据进程id获取进程句柄
	ProcessItemData* data = (ProcessItemData*)m_GameProcess.GetItemData(nPos);
	//CString str2;
	//str2.Format("%d-%s", data->pid, data->username);
	//MessageBox(str2);

	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, data->pid);
	if (NULL == hProcess)
	{
		AfxMessageBox("打开进程失败");
		return;
	}

	// 分配参数空间
	CString param(FZ_DLL_PATH);
	LPVOID pRemoteParam = ::VirtualAllocEx(hProcess, NULL, param.GetLength() + 1,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (pRemoteParam == NULL)
	{
		AfxMessageBox("分配参数空间失败");
		return;
	}

	// 写入参数到目标进程
	HANDLE pThread;
	DWORD dwWriten = 0;
	BOOL res = ::WriteProcessMemory(hProcess, pRemoteParam, param.GetBuffer(0),
		param.GetLength(), &dwWriten);
	if (!res)
	{
		AfxMessageBox("写入参数到目标进程失败");
		goto END;
	}

	// kernel32是内核函数 GetModuleHandleA
	HMODULE hModule = ::GetModuleHandle("Kernel32.dll");
	PTHREAD_START_ROUTINE pRemoteFunc2 = (PTHREAD_START_ROUTINE)::GetProcAddress(hModule, "GetModuleHandleA");
	if (NULL == pRemoteFunc2)
	{
		AfxMessageBox("获取LoadLibraryA地址失败");
		goto END;
	}

	 pThread = CreateRemoteThread(hProcess, NULL, 0, pRemoteFunc2,
		pRemoteParam, 0, NULL);
	if (NULL == pThread)
	{
		AfxMessageBox("创建远程线程失败");
		goto END;
	}

	// waitfor 
	WaitForSingleObject(pThread, INFINITE);
	DWORD dwVal = 0;
	GetExitCodeThread(pThread, &dwVal);
	PTHREAD_START_ROUTINE pRemoteFunc3 = (PTHREAD_START_ROUTINE)::GetProcAddress(hModule, "FreeLibrary");
	if (NULL == pRemoteFunc3)
	{
		AfxMessageBox("获取LoadLibraryA地址失败");
		goto END;
	}

	 pThread = CreateRemoteThread(hProcess, NULL, 0, pRemoteFunc3,
		(LPVOID)dwVal, 0, NULL);
	if (NULL == pThread)
	{
		AfxMessageBox("创建远程线程失败");
		goto END;
	}

	//DWORD err = GetLastError();
	//str.Format("%d-%d", dwVal, err);
	//AfxMessageBox(str);

	data->openAssist = false;
	data->assist_hwnd = NULL;

	m_GameProcess.SetItemText(data->listIndex, 2, "");

END:
	::VirtualFreeEx(hProcess, pRemoteParam, param.GetLength(), MEM_DECOMMIT);
}


void CMFCAssistManagerDlg::OnBnClickedButtonShowHideGame()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_GameProcess.GetFirstSelectedItemPosition();
	int nPos = m_GameProcess.GetNextSelectedItem(pos);
	if (-1 == nPos)
	{
		AfxMessageBox("请选择一个游戏进程");
		return;
	}

	// 根据进程id获取进程句柄
	ProcessItemData* data = (ProcessItemData*)m_GameProcess.GetItemData(nPos);
	if (data == NULL)
		return;

	//HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, data->pid);
	//if (NULL == hProcess)
	//{
	//	AfxMessageBox("打开进程失败");
	//	return;
	//}
	::ShowWindow(data->game_hwnd, ::IsWindowVisible(data->game_hwnd) ? SW_HIDE : SW_SHOW);

}


void CMFCAssistManagerDlg::OnBnClickedButtonShowHideAssist()
{
	// TODO: 在此添加控件通知处理程序代码

	POSITION pos = m_GameProcess.GetFirstSelectedItemPosition();
	int nPos = m_GameProcess.GetNextSelectedItem(pos);
	if (-1 == nPos)
	{
		AfxMessageBox("请选择一个游戏进程");
		return;
	}

	// 根据进程id获取进程句柄
	ProcessItemData* data = (ProcessItemData*)m_GameProcess.GetItemData(nPos);
	if (data == NULL)
		return;

	::ShowWindow(data->assist_hwnd, ::IsWindowVisible(data->assist_hwnd) ? SW_HIDE : SW_SHOW);
}



void CMFCAssistManagerDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	int* a = NULL;
	*a = 1;
}
