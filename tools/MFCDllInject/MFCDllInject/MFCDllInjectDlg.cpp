
// MFCDllInjectDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCDllInject.h"
#include "MFCDllInjectDlg.h"
#include "ui.h"
#include "afxdialogex.h"
#include <tlhelp32.h>
#include <assert.h>

#include "inject.h"
#include <iostream>
#include <algorithm>
#include <map>
#include <psapi.h>

#include <winnt.h>
#include <winternl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define MY_DEBUG 1
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCDllInjectDlg 对话框


CMFCDllInjectDlg::CMFCDllInjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCDLLINJECT_DIALOG, pParent)

#ifdef MY_DEBUG
	
	, m_DllPath(_T("C:\\Users\\Administrator\\Desktop\\1\\MFCLibrary-x64.dll"))

#else
	, m_DllPath(_T(""))
#endif // MY_DEBUG


	, m_Select_Pid(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDllInjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DLL_PATH, m_DllPath);
	DDX_Text(pDX, IDC_EDIT_PID, m_Select_Pid);
	DDX_Control(pDX, IDC_LIST_PROCESS_LIST, m_Process_List);
}

BEGIN_MESSAGE_MAP(CMFCDllInjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SELECT_DLL, &CMFCDllInjectDlg::OnBnClickedButtonSelectDll)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PROCESS_LIST, &CMFCDllInjectDlg::OnClickListProcessList)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_PROCESS, &CMFCDllInjectDlg::OnBnClickedButtonRefreshProcess)

	
	ON_BN_CLICKED(IDC_BUTTON_REMOTE_THREAD, &CMFCDllInjectDlg::OnBnClickedButtonRemoteThread)
	ON_BN_CLICKED(IDC_BUTTON_UNINSTALL, &CMFCDllInjectDlg::OnBnClickedButtonUninstall)
	ON_BN_CLICKED(IDC_BUTTON_RtlCreateUserThread, &CMFCDllInjectDlg::OnBnClickedButtonRtlcreateuserthread)
	ON_BN_CLICKED(IDC_BUTTON_NtCreateThreadEx, &CMFCDllInjectDlg::OnBnClickedButtonNtcreatethreadex)
	ON_BN_CLICKED(IDC_BUTTON_APC, &CMFCDllInjectDlg::OnBnClickedButtonApc)
	ON_BN_CLICKED(IDC_BUTTON_HIJACK, &CMFCDllInjectDlg::OnBnClickedButtonHijack)
	ON_BN_CLICKED(IDC_BUTTON_HOOK, &CMFCDllInjectDlg::OnBnClickedButtonHook)
	ON_BN_CLICKED(IDC_BUTTON_REFLECT, &CMFCDllInjectDlg::OnBnClickedButtonReflect)
	ON_BN_CLICKED(IDC_BUTTON_DUAN_PEB, &CMFCDllInjectDlg::OnBnClickedButtonDuanPeb)
END_MESSAGE_MAP()


// CMFCDllInjectDlg 消息处理程序

BOOL CMFCDllInjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//PrintUI("我的程序.exe", "MYWINDOW", "color 9", 800, 100, 700, 500);

	m_Process_List.ModifyStyle(LVS_ICON | LVS_SMALLICON | LVS_LIST, LVS_REPORT);
	m_Process_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_Process_List.InsertColumn(0, "PID", LVCFMT_LEFT, 50);
	m_Process_List.InsertColumn(1, "ARCH", LVCFMT_LEFT, 50);
	m_Process_List.InsertColumn(2, "进程名称", LVCFMT_LEFT, 184);

#ifdef MY_DEBUG
	AllocConsole(); // 添加调试窗口
	freopen("conin$", "r+t", stdin); // 将输入流设置为当前调试窗口
	freopen("conout$", "w+t", stdout); // 将输出流设置为当前调试窗口
	SetConsoleTitleA("调试窗口");
#endif // MY_DEBUG



	m_LoadLibraryA_Addr = 0;
	GetProcessList();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCDllInjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCDllInjectDlg::OnPaint()
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
HCURSOR CMFCDllInjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


typedef std::pair<DWORD, CString> PAIR;

bool cmp_val(const PAIR& left, const PAIR& right)
{
	return left.second < right.second;
}

void CMFCDllInjectDlg::GetProcessList()
{

	m_Process_List.DeleteAllItems();
	//for (std::vector<PPROCESS_INFO>::iterator it = m_plistVec.begin(); it != m_plistVec.end(); it++) 
	//{
	//	free(*it);
	//}

	// 实现方式是获取进程快照
	HANDLE hSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	BOOL ret = ::Process32First(hSnapShot, &pe32);

	// 自定义排序规则


	//std::vector<PROCESSENTRY32> process;
	//std::map<CString, DWORD>  process;
	std::map<DWORD, CString>  process; // [pid] = name

	// 遍历每个进程名称和目标进程比较
	while (ret)
	{
		CString tmp = pe32.szExeFile;
	#ifdef MY_DEBUG
		if (0 == _stricmp(tmp, "notepad.exe"))
		{
			process[pe32.th32ProcessID] = pe32.szExeFile;

		}
		if (0 == _stricmp(tmp, "notepad32.exe"))
		{
			process[pe32.th32ProcessID] = pe32.szExeFile;

	}
	#else
			process[pe32.th32ProcessID] = pe32.szExeFile;
	#endif // MY_DEBUG

			//process[pe32.th32ProcessID] = pe32.szExeFile;
		ret = ::Process32Next(hSnapShot, &pe32);
	}

	::CloseHandle(hSnapShot);

	// 将map 放到了vector，然后再排序vector
	std::vector<PAIR> vec(process.begin(), process.end());
	std::sort(vec.begin(), vec.end(), cmp_val);
	for (std::vector<PAIR>::iterator it = vec.begin(); it != vec.end(); ++it)
	{
		CString str_pid;
		CString str_name = it->second;
		BOOL isWow64;


		PPROCESS_INFO info = new PROCESS_INFO;
		HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, it->first);
		if (NULL != hProcess)
		{
			IsWow64Process(hProcess, &isWow64); // 返回true为32位进程
			//printf("isWow64 %d \n", isWow64);
			CloseHandle(hProcess);
			if (isWow64)
			{
				info->arch = 32;
			}
			else
			{
				info->arch = 64;
			}
		}
		else {
			continue;
		}

		int index = m_Process_List.GetItemCount();

		str_pid.Format("%d", it->first);
		m_Process_List.InsertItem(index, str_pid);
		str_pid.Format("%d", info->arch);
		m_Process_List.SetItemText(index, 1, str_pid);
		m_Process_List.SetItemText(index, 2, str_name);

		info->pid = it->first;
		info->processName = str_name;
		m_Process_List.SetItemData(index, (DWORD_PTR)info);
		m_plistVec.push_back(info);
	}
	//for (std::map<DWORD, CString>::iterator it = process.begin(); it != process.end(); it++)
	//{

	//	
	//}

	CString ns;
	ns.Format("dll注入器, 总计: %d个进程",process.size());
	SetWindowTextA(ns);

}






void CMFCDllInjectDlg::OnBnClickedButtonSelectDll()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, NULL, NULL, 4 | 2, "*.dll||");
	if (dlg.DoModal() == IDOK)
	{
		m_DllPath = dlg.GetPathName();
	}

	UpdateData(FALSE);
}


//struct ShellCodeStruct {
//	// 寄存器保护
//	BYTE push = 0x68;  // 实际上这里会进行字节对齐 
//	DWORD eip_address = 0;
//
//	BYTE pushfd = 0x9c;		
//	BYTE pushad = 0x60;
//
//	// 调用loadLibrary
//	BYTE mov_eax = 0xB8;
//	DWORD mov_eax_address = 0;// 保存LoadLibrary参数地址
//	BYTE push_eax = 0x50;
//	BYTE mov_ecx = 0xB9;
//	DWORD mov_ecx_address = 0;// 保存LoadLibrary地址
//	WORD call_ecx = 0xFFD1;
//
//	// 恢复寄存器和堆栈平衡  D1FF619DC3
//	BYTE popad = 0x61;
//	BYTE popfd = 0x9D;
//	BYTE retn = 0xC3;
//	CHAR DLLPath[MAX_PATH] = {0};
//};

//BYTE ShellCodeArray[MAX_PATH + 23] = { 
//	0x68,
//	0, 0, 0, 0, // 1
//	0x9c,
//	0x60,
//	0xb8,
//	0, 0, 0, 0, // 8
//	0x50,
//	0xb9,
//	0, 0, 0, 0, // 14
//	0xff, 0xd1,
//	0x61,
//	0x9d,
//	0xc3,
//};

#define shellCodeSize  32










void CMFCDllInjectDlg::OnClickListProcessList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	POSITION pos = m_Process_List.GetFirstSelectedItemPosition();
	int nItem = m_Process_List.GetNextSelectedItem(pos);

	PPROCESS_INFO info = (PPROCESS_INFO)m_Process_List.GetItemData(nItem);
	m_Select_Pid = info->pid;

	m_Select_Process = info;
	UpdateData(FALSE);



	// 获取LoadLibrary
	BOOL isWow64;
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_Select_Pid);
	IsWow64Process(hProcess, &isWow64);
#ifdef _WIN64
	if (isWow64)
	{
		AfxMessageBox("请选择64位进程");
		return;
	}

	m_LoadLibraryA_Addr = (ULONG64)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	m_LoadLibraryW_Addr = (ULONG64)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryW");
	m_FreeLibrary_Addr = (ULONG64)GetProcAddress(GetModuleHandleA("kernel32.dll"), "FreeLibrary");

	printf("找到LoadLibraryA地址: %llX\n", m_LoadLibraryA_Addr);
	printf("找到LoadLibraryW地址: %llX\n", m_LoadLibraryW_Addr);
	printf("找到FreeLibrary地址:  %llX\n", m_FreeLibrary_Addr);

#else
	if (!isWow64)
	{
		AfxMessageBox("请选择32位进程");
		return;
	}
	m_LoadLibraryA_Addr = (UINT32)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	m_LoadLibraryW_Addr = (UINT32)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryW");
	m_FreeLibrary_Addr = (UINT32)GetProcAddress(GetModuleHandleA("kernel32.dll"), "FreeLibrary");

	printf("找到LoadLibraryA地址: %X\n", m_LoadLibraryA_Addr);
	printf("找到LoadLibraryW地址: %X\n", m_LoadLibraryW_Addr);
	printf("找到FreeLibrary地址:  %X\n", m_FreeLibrary_Addr);

#endif // _WIN64

	CloseHandle(hProcess);




}


void CMFCDllInjectDlg::OnBnClickedButtonRefreshProcess()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Select_Pid = 0;
	m_Select_Process = NULL;
	UpdateData(FALSE);
	GetProcessList();
}


// 卸载dll
void CMFCDllInjectDlg::OnBnClickedButtonUninstall()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_DllPath.GetLength() == 0)
	{
		AfxMessageBox("请选择dll");
		return;
	}

	if (m_Select_Process == NULL)
	{
		AfxMessageBox("未选择进程");
		return;
	}

	// 获取目标进程句柄   
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_Select_Pid);
	if (NULL == hProcess)
	{
		AfxMessageBox("获取目标进程句柄");
		return;
	}
	BOOL isWow64;
	IsWow64Process(hProcess, &isWow64); // 返回true为32位进程
	printf("isWow64 %d \n", isWow64);



	MODULEENTRY32 me32 = { 0 };
	// 分配参数空间
	CString param(m_DllPath);
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;

	HANDLE hThread = NULL;
	// 获取模块快照
	hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_Select_Pid);
	if (INVALID_HANDLE_VALUE == hModuleSnap)
	{
		AfxMessageBox("CreateToolhelp32Snapshot   ");
		goto EXIT;
	}

	me32.dwSize = sizeof(MODULEENTRY32);

	// 开始遍历   
	if (FALSE == ::Module32First(hModuleSnap, &me32))
	{
		::CloseHandle(hModuleSnap);
		AfxMessageBox("Module32First");
		goto EXIT;
	}
	// 遍历查找指定模块   
	bool isFound = false;
	do
	{
		isFound = (0 == ::_tcsicmp(me32.szModule, m_DllPath) || 0 == ::_tcsicmp(me32.szExePath, m_DllPath));
		if (isFound) // 找到指定模块   
		{
			printf("找到指定模块....\n");
			break;
		}
	} while (TRUE == ::Module32Next(hModuleSnap, &me32));

	::CloseHandle(hModuleSnap);
	if (false == isFound)
	{
		AfxMessageBox("没有找到指定模块");
		goto EXIT;
	}

	// 创建远程线程调用 FreeLibrary   
	hThread = ::CreateRemoteThread(hProcess, NULL, 0, (PTHREAD_START_ROUTINE)m_FreeLibrary_Addr,
		me32.hModule, 0, NULL);
	printf("=======CreateRemoteThread==hThread %X=====, hThread\n", hThread);

	if (NULL == hThread)
	{
		printf("=======CreateRemoteThread==fail %d=====, hThread\n", GetLastError());
		goto EXIT;
	}
	printf("=======CreateRemoteThread=======\n");

	// 等待远程线程结束   
	WaitForSingleObject(hThread, 3000);
	// 清理   
	CloseHandle(hThread);
	printf("=======WaitForSingleObject=======\n");


	//if (isWow64)
	//{
	//	// 32位
	//	BOOL ret;
	//	DWORD cbNeeded = 0;
	//	HMODULE lphModule[1024] = { 0 };
	//	MODULEINFO modinfo = { 0 };
	//	EnumProcessModulesEx(hProcess, lphModule, sizeof(lphModule), &cbNeeded, LIST_MODULES_ALL);

	//	for (size_t i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
	//	{
	//		char szModName[MAX_PATH] = { 0 };
	//		// Get the full path to the module's file.
	//		ret = GetModuleFileNameExA(hProcess, lphModule[i], szModName, sizeof(szModName) / sizeof(char));
	//		//ret = GetModuleBaseNameA(hProcess, lphModule[i], szModName, sizeof(szModName) / sizeof(char));
	//		if (!ret)
	//		{
	//			printf("GetModuleFileNameExA err %d\n", GetLastError());
	//			continue;
	//		}

	//		//printf("===========szModName:%s\n", szModName);
	//		// not match 
	//		if (0 != _stricmp(szModName, m_DllPath))
	//		{
	//			// printf("not match: %s %s\n", m_DllPath, szModName);
	//			continue;
	//		}

	//		/////////// success match target dll
	//		// GetModuleInformation 
	//		ret = GetModuleInformation(hProcess, lphModule[i], &modinfo, sizeof(MODULEINFO));
	//		if (ret == 0)
	//		{
	//			printf("GetModuleInformation err : %d\n", GetLastError());
	//			goto EXIT ;
	//		}

	//		break;
	//	}

	//	printf("============start uninstall\n");
	//	ULONG64 funcAddr = 0;
	//	 ret = GetProcessDllFunctionsByNameX32(m_Select_Pid, "kernel32.dll", "FreeLibrary", &funcAddr);
	//	if (!ret)
	//	{
	//		AfxMessageBox("没有找到函数FreeLibrary");
	//		goto EXIT;
	//	}
	//	
	//		// 创建远程线程调用 FreeLibrary   
	//	HANDLE	hThread = CreateRemoteThread(hProcess, NULL, 0, (PTHREAD_START_ROUTINE)funcAddr,
	//		modinfo.lpBaseOfDll, 0, NULL);
	//	printf("=======CreateRemoteThread==hThread %X=====, hThread\n", hThread);

	//	if (NULL == hThread)
	//	{
	//		printf("=======CreateRemoteThread==fail %d=====, hThread\n", GetLastError());

	//		::CloseHandle(hProcess);
	//		goto EXIT;
	//	}
	//	printf("=======CreateRemoteThread=======\n");

	//	// 等待远程线程结束   
	//	WaitForSingleObject(hThread, 3000);
	//	// 清理   
	//	CloseHandle(hThread);
	//	printf("=======WaitForSingleObject=======\n");
	//}



EXIT:
	CloseHandle(hProcess);

}


// 远程线程注入
void CMFCDllInjectDlg::OnBnClickedButtonRemoteThread()
{

	if (m_DllPath.GetLength() == 0)
	{
		AfxMessageBox("请选择dll");
		return;
	}
	if (m_Select_Process == NULL)
	{
		AfxMessageBox("请打开进程");
		return;
	}

	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_Select_Pid);
	if (NULL == hProcess)
	{
		AfxMessageBox("打开进程失败");
		return;
	}

	BOOL isWow64;
	IsWow64Process(hProcess, &isWow64); // 返回true为32位进程
	printf("isWow64 %d \n", isWow64);

	// 分配参数空间
	CString param(m_DllPath);
	LPVOID pRemoteParam = ::VirtualAllocEx(hProcess, NULL, 0x1000,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (pRemoteParam == NULL)
	{
		AfxMessageBox("分配参数空间失败");
		return;
	}

	// 写入参数到目标进程
	SIZE_T dwWriten = 0;
	BOOL res = ::WriteProcessMemory(hProcess, pRemoteParam, param.GetBuffer(0),
		param.GetLength(), &dwWriten);
	if (!res)
	{
		AfxMessageBox("写入参数到目标进程失败");
		goto END;
	}


	if (0 == m_LoadLibraryA_Addr)
	{
		AfxMessageBox("获取LoadLibraryA地址失败");
		goto END;
	}

	HANDLE pThread = 0;

	if (isWow64)
	{
		pThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)(DWORD)m_LoadLibraryA_Addr,
			pRemoteParam, 0, NULL);
	}
	else
	{
		pThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)m_LoadLibraryA_Addr,
			pRemoteParam, 0, NULL);
	}

	if (NULL == pThread)
	{
		AfxMessageBox("创建远程线程失败");
		goto END;
	}

	// waitfor 
	WaitForSingleObject(pThread, INFINITE);
	CloseHandle(pThread);

END:
	VirtualFreeEx(hProcess, pRemoteParam, 0x1000, MEM_DECOMMIT);
	CloseHandle(hProcess);
}



void CMFCDllInjectDlg::OnBnClickedButtonRtlcreateuserthread()
{
	if (m_DllPath.GetLength() == 0)
	{
		AfxMessageBox("请选择dll");
		return;
	}
	if (m_Select_Process == NULL)
	{
		AfxMessageBox("请打开进程");
		return;
	}

	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_Select_Pid);
	if (NULL == hProcess)
	{
		AfxMessageBox("打开进程失败");
		return;
	}

	BOOL isWow64;
	IsWow64Process(hProcess, &isWow64); // 返回true为32位进程

	printf("isWow64 %d \n", isWow64);

#ifdef _WIN64
	if (isWow64)
	{
		AfxMessageBox("请选择64位进程");
		return;
	}
#else
	if (!isWow64)
	{
		AfxMessageBox("请选择32位进程");
		return;
	}
#endif // _WIN64

	AfxMessageBox("暂未确定x64该函数的参数类型");
	return;

	// 分配参数空间
	CString param(m_DllPath);
	LPVOID pRemoteParam = ::VirtualAllocEx(hProcess, NULL, 0x1000,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (pRemoteParam == NULL)
	{
		AfxMessageBox("分配参数空间失败");
		return;
	}

	// 写入参数到目标进程
	SIZE_T dwWriten = 0;
	BOOL res = ::WriteProcessMemory(hProcess, pRemoteParam, param.GetBuffer(0),
		param.GetLength(), &dwWriten);
	if (!res)
	{
		AfxMessageBox("写入参数到目标进程失败");
		goto END;
	}

	HANDLE pThread = NULL;
	ULONG64 funcAddr = 0;
	ULONG64 ret = 0;
#ifdef _WIN64

	pRtlCreateUserThread64 func_RtlCreateUserThread = (pRtlCreateUserThread64)GetProcAddress(
		GetModuleHandleA("ntdll.dll"), "RtlCreateUserThread");
	if (0 == func_RtlCreateUserThread)
	{
		AfxMessageBox("获取RtlCreateUserThread地址失败");
		goto END;
	}
	ret = func_RtlCreateUserThread(hProcess, NULL, FALSE,  0,0,0);
#else

	pRtlCreateUserThread32 func_RtlCreateUserThread = (pRtlCreateUserThread32)GetProcAddress(
		GetModuleHandleA("ntdll.dll"), "RtlCreateUserThread");
	if (0 == func_RtlCreateUserThread)
	{
		AfxMessageBox("获取RtlCreateUserThread地址失败");
		goto END;
	}
	ret = func_RtlCreateUserThread(hProcess, NULL, FALSE, 0, 0, 0, (PTHREAD_START_ROUTINE)(DWORD)m_LoadLibraryA_Addr,
		pRemoteParam, &pThread, NULL);
#endif // _WIN64




	if (pThread == NULL)
	{
		AfxMessageBox("创建RtlCreateUserThread失败");
		goto END;
	}
	// waitfor 
	//WaitForSingleObject(pThread, INFINITE);
	//printf("VirtualFreeEx\n");
	//CloseHandle(pThread);

END:
	printf("END\n");

	VirtualFreeEx(hProcess, pRemoteParam, 0x1000, MEM_DECOMMIT);
	printf("VirtualFreeEx\n");
	CloseHandle(hProcess);
	printf("CloseHandle\n");
	printf("执行完毕\n");
}

void CMFCDllInjectDlg::OnBnClickedButtonNtcreatethreadex()
{
	if (m_DllPath.GetLength() == 0)
	{
		AfxMessageBox("请选择dll");
		return;
	}
	if (m_Select_Process == NULL)
	{
		AfxMessageBox("请打开进程");
		return;
	}

	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_Select_Pid);
	if (NULL == hProcess)
	{
		AfxMessageBox("打开进程失败");
		return;
	}

	BOOL isWow64;
	IsWow64Process(hProcess, &isWow64); // 返回true为32位进程
	printf("isWow64 %d \n", isWow64);
#ifdef _WIN64
	if (isWow64)
	{
		AfxMessageBox("请选择64位进程");
		return;
	}
#else
	if (!isWow64)
	{
		AfxMessageBox("请选择32位进程");
		return;
	}
#endif // _WIN64

	// 分配参数空间
	CString param(m_DllPath);
	LPVOID pRemoteParam = ::VirtualAllocEx(hProcess, NULL, 0x1000,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (pRemoteParam == NULL)
	{
		AfxMessageBox("分配参数空间失败");
		return;
	}

	// 写入参数到目标进程
	SIZE_T dwWriten = 0;
	BOOL res = ::WriteProcessMemory(hProcess, pRemoteParam, param.GetBuffer(0),
		param.GetLength(), &dwWriten);
	if (!res)
	{
		AfxMessageBox("写入参数到目标进程失败");
		goto END;
	}

	ULONG64 funcAddr = 0;
	BOOL isFind;
	HANDLE pThread = NULL;

#ifdef _WIN64

	P_NtCreateThreadEx64 func_NtCreateThreadEx = (P_NtCreateThreadEx64)GetProcAddress(
		GetModuleHandleA("ntdll.dll"), "NtCreateThreadEx");
	func_NtCreateThreadEx(&pThread, THREAD_ALL_ACCESS, NULL, hProcess, (PVOID)m_LoadLibraryA_Addr,
		pRemoteParam, 0, 0, 0, 0, NULL);
#else

	P_NtCreateThreadEx32 func_NtCreateThreadEx = (P_NtCreateThreadEx32)GetProcAddress(
		GetModuleHandleA("ntdll.dll"), "NtCreateThreadEx");
	func_NtCreateThreadEx(&pThread, THREAD_ALL_ACCESS, NULL, hProcess, (LPTHREAD_START_ROUTINE)(DWORD)m_LoadLibraryA_Addr,
		pRemoteParam, FALSE, 0, 0, 0, NULL);
#endif // _WIN64


	if (pThread == NULL)
	{
		AfxMessageBox("创建NtCreateThreadEx失败");
		goto END;
	}
	// waitfor 
	WaitForSingleObject(pThread, INFINITE);
	CloseHandle(pThread);
END:

	VirtualFreeEx(hProcess, pRemoteParam, 0x1000, MEM_DECOMMIT);
	CloseHandle(hProcess);
}


void CMFCDllInjectDlg::OnBnClickedButtonApc()
{
	if (m_DllPath.GetLength() == 0)
	{
		AfxMessageBox("请选择dll");
		return;
	}
	if (m_Select_Process == NULL)
	{
		AfxMessageBox("请打开进程");
		return;
	}

	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_Select_Pid);
	if (NULL == hProcess)
	{
		AfxMessageBox("打开进程失败");
		return;
	}

	BOOL isWow64;
	IsWow64Process(hProcess, &isWow64); // 返回true为32位进程
	printf("isWow64 %d \n", isWow64);

	// 分配参数空间
	CString param(m_DllPath);
	LPVOID pRemoteParam = ::VirtualAllocEx(hProcess, NULL, param.GetLength() + 1,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (pRemoteParam == NULL)
	{
		AfxMessageBox("分配参数空间失败");
		return;
	}

	// 写入参数到目标进程
	SIZE_T dwWriten = 0;
	BOOL res = ::WriteProcessMemory(hProcess, pRemoteParam, param.GetBuffer(0),
		param.GetLength(), &dwWriten);
	if (!res)
	{
		AfxMessageBox("写入参数到目标进程失败");
		return;
	}

	// 遍历目标进程的线程
	HANDLE hSnapshot;
	THREADENTRY32 pe32;
	BOOL ret;
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, m_Select_Pid);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		AfxMessageBox("CreateToolhelp32Snapshot目标进程失败");
		return;
	}

	pe32.dwSize = sizeof(THREADENTRY32);
	ret = Thread32First(hSnapshot, &pe32);
	while (ret)
	{
		if (m_Select_Pid == pe32.th32OwnerProcessID)
		{
			printf("threadid: %d\n", pe32.th32ThreadID);
			// 给所有线程都注入APC,因为有些线程不一定会被调度
			HANDLE threadHandle = OpenThread(THREAD_ALL_ACCESS, FALSE, pe32.th32ThreadID);
			if (isWow64)
			{
				void* tmp = (PAPCFUNC)(DWORD)m_LoadLibraryA_Addr;
				printf("tmp r = %p \n", tmp);

				DWORD r = QueueUserAPC((PAPCFUNC)(DWORD)m_LoadLibraryA_Addr, threadHandle, (ULONG_PTR)pRemoteParam);
				printf("DWORD r = %d \n",r);
			}
			else
			{
				DWORD r = QueueUserAPC((PAPCFUNC)m_LoadLibraryA_Addr, threadHandle, (ULONG_PTR)pRemoteParam);
			}
		}

		ret = Thread32Next(hSnapshot, &pe32);
	}

	CloseHandle(hSnapshot);
	CloseHandle(hProcess);

	printf("=============end=========\n");
}


void CMFCDllInjectDlg::OnBnClickedButtonHijack()
{
	if (m_DllPath.GetLength() == 0)
	{
		AfxMessageBox("请选择dll");
		return;
	}
	if (m_Select_Process == NULL)
	{
		AfxMessageBox("请打开进程");
		return;
	}

	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_Select_Pid);
	if (NULL == hProcess)
	{
		AfxMessageBox("打开进程失败");
		return;
	}

	BOOL isWow64;
	IsWow64Process(hProcess, &isWow64); // 返回true为32位进程
	printf("isWow64 %d \n", isWow64);

#ifdef _WIN64
	if (isWow64)
	{
		AfxMessageBox("请选择64位进程");
		return;
	}
#else
	if (!isWow64)
	{
		AfxMessageBox("请选择32位进程");
		return;
	}
#endif // _WIN64
	CONTEXT context = { 0 };
	WOW64_CONTEXT context32 = { 0 };
	// 分配参数空间
	CString param(m_DllPath);
	LPVOID pRemoteParam = ::VirtualAllocEx(hProcess, NULL, 0x1000,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (pRemoteParam == NULL)
	{
		AfxMessageBox("分配参数空间失败");
		CloseHandle(hProcess);
		return;
	}

	// 写入参数到目标进程 偏移0x600
	SIZE_T dwWriten = 0;
	BOOL res = ::WriteProcessMemory(hProcess, (PUCHAR)pRemoteParam+0x600, param.GetBuffer(0),
		param.GetLength(), &dwWriten);
	if (!res)
	{
		AfxMessageBox("写入参数到目标进程失败");
		goto END;
	}

	// 遍历目标进程的所有线程-找到主线程
	HANDLE hSnapshot;
	THREADENTRY32 pe32;
	BOOL ret;
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, m_Select_Pid);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		AfxMessageBox("CreateToolhelp32Snapshot目标进程失败");
		goto END;
	}

	DWORD mainThreadID = 0;
	pe32.dwSize = sizeof(THREADENTRY32);
	ret = Thread32First(hSnapshot, &pe32);
	while (ret)
	{
		if (m_Select_Pid == pe32.th32OwnerProcessID)
		{
			printf("threadid: %d\n", pe32.th32ThreadID);
			mainThreadID = pe32.th32ThreadID;
			break;
		}

		ret = Thread32Next(hSnapshot, &pe32);
	}

	//挂起主线程
	HANDLE tHandle = OpenThread(THREAD_ALL_ACCESS, FALSE, mainThreadID);
	if (NULL == tHandle)
	{
		AfxMessageBox("打开线程失败");
		CloseHandle(hProcess);
		return;
	}

	SuspendThread(tHandle);

#ifdef _WIN64
	//获取这个线程的Context
	
	context.ContextFlags = CONTEXT_ALL;
	ret = GetThreadContext(tHandle, &context);
	if (!ret)
	{
		AfxMessageBox("GetThreadContext失败");
		return;
	}


	char buff[] =
	{
		0x50,                                              // push rax
		0x51,                                              // push rcx
		0x48,0xB9,0x89,0x67,0x45,0x23,0x01,0x00,0x00,0x00, // mov rcx,123456789 // 参数1
		0x48,0xB8,0x89,0x67,0x45,0x23,0x01,0x00,0x00,0x00, // mov rax,123456789 // LoadLibraryA地址
		0x48,0x83,0xEC,0x48,                               // sub rsp,48
		0xFF,0xD0,                                         // call rax
		0x48,0x83,0xC4,0x48,                               // add rsp,48
		0x59,                                              // pop rcx
		0x58,                                              // pop rax
		0xFF,0x25,0x00,0x00,0x00,0x00,                     // ff25 间隔4字节后面8个字节为跳转地址
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00            // 跳转的地址
	};
	*(PULONG64)&buff[4] = (ULONG64)(PUCHAR)pRemoteParam + 0x600;
	*(PULONG64)&buff[14] = m_LoadLibraryA_Addr;
	*(PULONG64)&buff[40] = context.Rip;
	ret = WriteProcessMemory(hProcess, pRemoteParam, buff, sizeof(buff), &dwWriten);
	// 将shellcode写入到目标进程
	if (!ret)
	{
		AfxMessageBox("WriteProcessMemory shellcode失败");
		return;
	}
	// 设置线程新的Rip
	context.Rip = (ULONG64)pRemoteParam;
	SetThreadContext(tHandle, &context);
#else


	context32.ContextFlags = CONTEXT_ALL;
	Wow64GetThreadContext(tHandle, &context32);
	char buff[] =
	{
		0x60,                          // pushad             
		0xB8, 0x78, 0x56, 0x34, 0x12,  // mov eax,12345678  // LoadlibraryA地址   
		0x68, 0x78, 0x56, 0x34, 0x12,  // push 12345678     // 参数地址
		0xFF, 0xD0,                    // call eax
		0x61,                          // popad
		0xE9, 0, 0, 0, 0,              // jmp xxxxxxxx
	};
	*(PDWORD)&buff[2] = (DWORD)m_LoadLibraryA_Addr;
	*(PDWORD)&buff[7] = (DWORD)(PUCHAR)pRemoteParam + 0x600;

	//jmp context.Eip = E9 (context.Eip - cur) 

	*(PDWORD)&buff[15] = context32.Eip - ((DWORD)(PUCHAR)pRemoteParam + 19);

	ret = WriteProcessMemory(hProcess, pRemoteParam, buff, sizeof(buff), &dwWriten);
	if (!ret)
	{
		AfxMessageBox("写入shellcode到目标进程失败");
		goto END;
	}

	context32.Eip = (DWORD)pRemoteParam;
	Wow64SetThreadContext(tHandle, &context32);

#endif // _WIN64
	
	// 恢复主线程
	ResumeThread(tHandle);

END:
	printf("end==================\n");
	// 这里不能释放
	//VirtualFreeEx(hProcess, pRemoteParam, 0x1000, MEM_DECOMMIT);

	CloseHandle(hProcess);

}

static int g_Total = 1;
void FunctionAddress()
{
	g_Total++;
	printf("===%d===\n", g_Total);
}

LRESULT CALLBACK myHookStartFunc(int nCode, WPARAM wParam, LPARAM lParam)
{
	//printf( "MyProc======== %d %x %x \n", nCode, wParam, lParam);


	// home按键触发
	// lParam The previous key state.
	// 30	The value is 1 if the key is down before the message is sent; it is 0 if the key is up*/

	if (nCode == HC_ACTION && wParam == VK_HOME && (lParam & 0xC0000000))
	{
		OutputDebugStringA("=========VK_HOME==DWON===========");
		MessageBoxA(0, "hook-content", "title", 0);

		// todo ...
	}
	//继续调用钩子过程
	return CallNextHookEx(0, nCode, wParam, lParam);
}

void CMFCDllInjectDlg::OnBnClickedButtonHook()
{
	if (m_DllPath.GetLength() == 0)
	{
		MessageBox("请选择dll");
		return;
	}
	if (m_Select_Process == NULL)
	{
		AfxMessageBox("请打开进程");
		return;
	}

	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_Select_Pid);
	if (NULL == hProcess)
	{
		AfxMessageBox("打开进程失败");
		return;
	}

	BOOL isWow64;
	IsWow64Process(hProcess, &isWow64); // 返回true为32位进程
	printf("isWow64 %d \n", isWow64);
	if (isWow64)
	{
		AfxMessageBox("暂不支持x32");
		return;
	}

	//const char* g_dllPath = "D:\\win32\\projects\\template\\MFCLibrary\\x64\\Release\\MFCLibrary.dll";
	const char* g_dllPath = "D:\\win32\\projects\\tools\\MFCDllInject-x32\\x64\\Debug\\setWindowHookEx_Dll.dll";
	HMODULE hModule= LoadLibraryA(g_dllPath);
	FARPROC myHookStartFunc =  GetProcAddress(hModule, "myHookStartFunc");
	
	// 遍历目标进程的所有线程-找到主线程
	HANDLE hSnapshot;
	THREADENTRY32 pe32;
	BOOL ret;
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, m_Select_Pid);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		AfxMessageBox("CreateToolhelp32Snapshot目标进程失败");
		return;
	}
	pe32.dwSize = sizeof(THREADENTRY32);
	ret = Thread32First(hSnapshot, &pe32);
	while (ret)
	{
		if (m_Select_Pid == pe32.th32OwnerProcessID)
		{
			printf("threadid: %d\n", pe32.th32ThreadID);
			SetWindowsHookExA(WH_KEYBOARD, (HOOKPROC)myHookStartFunc, hModule, pe32.th32ThreadID);
		}
		ret = Thread32Next(hSnapshot, &pe32);
	}

	CloseHandle(hModule);
	AfxMessageBox("注入成功,按Home键触发");
}


void CMFCDllInjectDlg::OnBnClickedButtonReflect()
{
	if (m_DllPath.GetLength() == 0)
	{
		AfxMessageBox("请选择dll");
		return;
	}
	if (m_Select_Process == NULL)
	{
		AfxMessageBox("请打开进程");
		return;
	}

	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_Select_Pid);
	if (NULL == hProcess)
	{
		AfxMessageBox("打开进程失败");
		return;
	}

	BOOL isWow64;
	IsWow64Process(hProcess, &isWow64); // 返回true为32位进程
	printf("isWow64 %d \n", isWow64);
	if (isWow64)
	{
		//AfxMessageBox("暂不支持x32");
		//return;
	}

	// do
	HANDLE hFile = NULL;
	HANDLE hModule = NULL;
	HANDLE hToken = NULL;
	LPVOID lpBuffer = NULL;
	DWORD dwLength = 0;
	DWORD dwBytesRead = 0;
	TOKEN_PRIVILEGES priv = { 0 };
	hFile = CreateFileA(m_DllPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("CreateFileA失败");
		return;
	}


	dwLength = GetFileSize(hFile, NULL);
	if (dwLength == INVALID_FILE_SIZE || dwLength == 0)
	{
		AfxMessageBox("Failed to get the DLL file size 失败");
		return;
	}

	lpBuffer = HeapAlloc(GetProcessHeap(), 0, dwLength);
	if (!lpBuffer)
	{
		AfxMessageBox("HeapAlloc 失败");
		return;
	}

	if (ReadFile(hFile, lpBuffer, dwLength, &dwBytesRead, NULL) == FALSE)
	{
		AfxMessageBox("ReadFile 失败");
		goto END;;
	}

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		priv.PrivilegeCount = 1;
		priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &priv.Privileges[0].Luid))
			AdjustTokenPrivileges(hToken, FALSE, &priv, 0, NULL, NULL);

		CloseHandle(hToken);
	}

	hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | 
		PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, m_Select_Pid);
	if (!hProcess)
	{
		AfxMessageBox("OpenProcess 2 失败");
		goto END;;
	}

	if (isWow64)
	{
		hModule = LoadRemoteLibraryR32(hProcess, lpBuffer, dwLength, NULL);
		if (!hModule)
		{
			AfxMessageBox("LoadRemoteLibraryR32 失败");
			goto END;
		}
		//printf("[+] Injected the '%s' DLL into process %d.", m_DllPath, m_Select_Pid);
		//WaitForSingleObject(hModule, -1);
	}
	else
	{
		hModule = LoadRemoteLibraryR(hProcess, lpBuffer, dwLength, NULL);
		if (!hModule)
		{
			AfxMessageBox("LoadRemoteLibraryR 失败");
			goto END;
		}
		//printf("[+] Injected the '%s' DLL into process %d.", m_DllPath, m_Select_Pid);
		//WaitForSingleObject(hModule, -1);
	}

END:
	CloseHandle(hFile);

}


void CMFCDllInjectDlg::OnBnClickedButtonDuanPeb()
{
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_Select_Pid);
	if (NULL == hProcess)
	{
		AfxMessageBox("打开进程失败");
		return;
	}

	BOOL isWow64;
	IsWow64Process(hProcess, &isWow64); // 返回true为32位进程
	printf("isWow64 %d \n", isWow64);
#ifdef _WIN64
	if (isWow64)
	{
		AfxMessageBox("请选择64位进程");
		return;
	}
#else
	if (!isWow64)
	{
		AfxMessageBox("请选择32位进程");
		return;
	}
#endif // _WIN64

	// 分配参数空间
	CString param(m_DllPath);
	LPVOID pRemoteParam = ::VirtualAllocEx(hProcess, NULL, 0x1000,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (pRemoteParam == NULL)
	{
		AfxMessageBox("分配参数空间失败");
		return;
	}
	/////////////////////////////////////////
	CString dllPath = m_DllPath;

	char* pszMultiByte = m_DllPath.GetBuffer();  //strlen(pwsUnicode)=5
	int iSize;
	wchar_t* dllPathUnicode;

	//返回接受字符串所需缓冲区的大小，已经包含字符结尾符'\0'
	iSize = MultiByteToWideChar(CP_ACP, 0, pszMultiByte, -1, NULL, 0); //iSize =wcslen(pwsUnicode)+1=6
	dllPathUnicode = (wchar_t*)malloc(iSize * sizeof(wchar_t)); //不需要 pwszUnicode = (wchar_t *)malloc((iSize+1)*sizeof(wchar_t))
	MultiByteToWideChar(CP_ACP, 0, pszMultiByte, -1, dllPathUnicode, iSize);
	printf("dllPathUnicode: %ws\n", dllPathUnicode);
	/////////////////////////////////////////

	SIZE_T written = 0;
	BOOL ret;
	UINT32 lpPeb = 0;
	// get PEB
	PEB_LDR_DATA32 ldrData32 = { 0 };

	// 循环遍历链表
	LDR_DATA_TABLE_ENTRY32 entry32     = { 0 };
	LDR_DATA_TABLE_ENTRY32 lastEntry   = { 0 };
	LDR_DATA_TABLE_ENTRY32 nextEntry   = { 0 };
	DWORD index = 0;
	PEB32 peb32 = { 0 };

	PEB_LDR_DATA64 ldrData64           = { 0 };
	LDR_DATA_TABLE_ENTRY64 entry64     = { 0 };
	LDR_DATA_TABLE_ENTRY64 lastEntry64 = { 0 };
	LDR_DATA_TABLE_ENTRY64 nextEntry64 = { 0 };

#ifdef _WIN64
	if (isWow64)
	{
		AfxMessageBox("请选择64位进程");
		return;
	}
	void * ress = NtCurrentTeb();
	printf("ress %p\n", ress);
	//printf("__readgsqword: %llX\n", __readgsqword(0));
	//printf("__readgsqword: %llX\n", __readgsqword(0x60));
	typedef NTSTATUS(WINAPI* PNT_QUERY_INFORMATION_PROCESS)(
		HANDLE ProcessHandle,
		PROCESSINFOCLASS ProcessInformationClass,
		PVOID ProcessInformation,
		ULONG ProcessInformationLength,
		PULONG ReturnLength
		);
	PNT_QUERY_INFORMATION_PROCESS NtQueryInformationProcess = (PNT_QUERY_INFORMATION_PROCESS)GetProcAddress(
		GetModuleHandleA("ntdll"), "NtQueryInformationProcess");
	if (NtQueryInformationProcess == NULL)
	{
		AfxMessageBox("获取NtQueryInformationProcess函数指针失败");
		return;
	}

	PROCESS_BASIC_INFORMATION pbi;
	ULONG returnLength;
	NTSTATUS status = NtQueryInformationProcess(hProcess, ProcessBasicInformation, 
		(PVOID)&pbi, sizeof(PROCESS_BASIC_INFORMATION), &returnLength);
	if (status != 0)
	{
		AfxMessageBox("执行NtQueryInformationProcess失败");
		return;
	}

	printf("peb: %llX\n", pbi.PebBaseAddress);
	PEB64 peb64 = { 0 };
	ret = ReadProcessMemory(hProcess,(PVOID) pbi.PebBaseAddress, &peb64, sizeof(PEB64), &written);
	if (!ret)
	{
		AfxMessageBox("ReadProcessMemory 1 失败");
		goto END;
	}

	printf("peb64  %llx\n", peb64.Ldr);
	ret = ReadProcessMemory(hProcess, (PVOID)peb64.Ldr, &ldrData64, sizeof(PEB_LDR_DATA64), &written);
	if (!ret)
	{
		AfxMessageBox("ReadProcessMemory 2 失败");
		goto END;
	}


	UINT64 ptr = ldrData64.InLoadOrderLinks.Flink;
	ULONG64 offset = 0;
	//offset = sizeof(LIST_ENTRY64)*2;
	index = 1;
	while (true)
	{
		//printf("===ptr:%llx\n",ptr);
		// 
		if (!ptr)
		{
			printf("ptr is null \n");
			break;
		}
		//ret = ReadProcessMemory(hProcess, (PVOID)ptr, (PVOID)((ULONG64)&entry64 + offset),
			ret = ReadProcessMemory(hProcess, (PVOID)ptr, (PVOID)&entry64,
			sizeof(LDR_DATA_TABLE_ENTRY64), &written);
		if (!ret)
		{
			AfxMessageBox("ReadProcessMemory 3 失败");
			goto END;
		}
		if (entry64.SizeOfImage == 0)
		{
			printf("entry64.SizeOfImage == 0 break \n");
			break;
		}

		// print name
		WCHAR name[1024] = { 0 };
		ret = ReadProcessMemory(hProcess, (PVOID)entry64.FullDllName.Buffer, 
			name, entry64.FullDllName.Length, &written);
		if (!ret)
		{
			printf("entry64.FullDllName.Buffer:%llX \n", entry64.FullDllName.Buffer);
			AfxMessageBox("ReadProcessMemory 4 失败 ");
			goto END;
		}

		if (0 == StrCmpW(dllPathUnicode, name))
		{
			// 读取当后一个entry
			ret = ReadProcessMemory(hProcess, (PVOID)(entry64.InLoadOrderLinks.Flink), (PVOID)&nextEntry64,
				sizeof(LDR_DATA_TABLE_ENTRY64), &written);
			if (!ret)
			{
				printf("ReadProcessMemory 3 \n");
				AfxMessageBox("ReadProcessMemory 3 失败 ");
				goto END;
			}

			printf("开始断链处理、\n");

			// 读取当前一个entry
			ret = ReadProcessMemory(hProcess, (PVOID)(entry64.InLoadOrderLinks.Blink), (PVOID)&lastEntry64,
				sizeof(LDR_DATA_TABLE_ENTRY64), &written);
			if (!ret)
			{
				printf("ReadProcessMemory 4 \n");
				AfxMessageBox("ReadProcessMemory 4 失败 ");
				goto END;
			}
	
			// 前一个Flink = 后一个Flink
			ret = WriteProcessMemory(hProcess, (LPVOID)entry64.InLoadOrderLinks.Blink,
				(LPVOID)&entry64.InLoadOrderLinks.Flink, 8, &written);
			if (!ret)
			{
				printf("WriteProcessMemory 1 \n");
			}
			// 后一个Blink = 前一个Blink
			ret = WriteProcessMemory(hProcess, (LPVOID)(UINT64(entry64.InLoadOrderLinks.Flink)+ 0x8),
				(LPVOID)&entry64.InLoadOrderLinks.Blink, 8, &written);
			if (!ret)
			{
				printf("WriteProcessMemory 2 \n");
			}
			printf("断链处理完成、\n");
			break;

		}


		printf("index:%d dllBase:%llX, size:%llX \tname: %ws\n", index++, entry64.DllBase, entry64.SizeOfImage, name);
		ptr = entry64.InLoadOrderLinks.Flink;


	}

#else
	// x32
			// 50               | push eax
			// 53               | push ebx
			// E8 F9000000      | call 77461D33 调用偏移+0x100，获得eip，放到eax
			// 64:8B1D 30000000 | mov ebx, dword ptr fs : [30]
			// 83E8 07          | sub eax, 7
			// 8998 00020000    | mov dword ptr ds : [eax + 200] , ebx
			// 5B               | pop ebx
			// 58               | pop eax

	//void* ress = NtCurrentTeb(); // 获取到是自己当前线程的

	char buff[] = {
		0x50,
		0x53,
		0xE8, 0xF9, 0x00, 0x00, 0x00,
		0x64, 0x8B, 0x1D, 0x30, 0x00, 0x00, 0x00,
		0x83, 0xE8, 0x07,
		0x89, 0x98, 0x00, 0x02, 0x00, 0x00,  //把PEB给到+0x200处内存
		0x5B,
		0x58,
		0xC3,
	};
	ret = WriteProcessMemory(hProcess, pRemoteParam, buff, sizeof(buff), &written);
	if (!ret)
	{
		AfxMessageBox("WriteProcessMemory 1失败");
		goto END;
	}
	// +0x100处
	// 8B0424 | mov eax, dword ptr ss : [esp]
	// C3 | ret
	char buff2[] = {
		0xCC,0xCC,
		0x8B, 0x04, 0x24,
		0xC3,
	};
	ret = WriteProcessMemory(hProcess, (LPVOID)(DWORD(pRemoteParam) + 0x100 - 2), buff2, sizeof(buff2), &written);
	if (!ret)
	{
		AfxMessageBox("WriteProcessMemory 2失败");
		goto END;
	}

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pRemoteParam, NULL, 0, 0);
	if (!hThread)
	{
		AfxMessageBox("CreateRemoteThread 失败");
		goto END;
	}
	WaitForSingleObject(hThread, INFINITE);
	printf("remote thread exec finished\n");

	// read 
	ret = ReadProcessMemory(hProcess, (LPVOID)(DWORD(pRemoteParam) + 0x200), (LPVOID)&lpPeb, 4, &written);
	if (!ret)
	{
		AfxMessageBox("ReadProcessMemory失败");
		goto END;
	}
	printf("PEB addr %p\n", lpPeb);
	printf("PEB addr %X\n", lpPeb);


	///////////////////////////// 

	ret = ReadProcessMemory(hProcess, (LPVOID)(lpPeb), (LPVOID)&peb32, sizeof(PEB32), &written);
	if (!ret)
	{
		AfxMessageBox("ReadProcessMemory PEB32失败");
		goto END;
	}

	printf("peb32.Ldr: %X\n", peb32.Ldr);

	// get LDR 

	ret = ReadProcessMemory(hProcess, (LPVOID)(peb32.Ldr), (LPVOID)&ldrData32, sizeof(PEB_LDR_DATA32), &written);
	if (!ret)
	{
		AfxMessageBox("ReadProcessMemory PEB_LDR_DATA32失败");
		goto END;
	}

	printf("===========开始处理InLoadOrderModuleList +0x0c ==========\n");


	UINT32 prev = ldrData32.InLoadOrderModuleList.Flink;
	while (true)
	{
		WCHAR dllName[255] = { 0 };
		// read next 
		ret = ReadProcessMemory(hProcess, (LPVOID)(prev), (LPVOID)&entry32,
			sizeof(LDR_DATA_TABLE_ENTRY32), &written);
		if (!ret)
		{
			AfxMessageBox("ReadProcessMemory LDR_DATA_TABLE_ENTRY32失败");
			goto END;
		}

		if (entry32.DllBase == 0)
		{
			break;
		}

		ret = ReadProcessMemory(hProcess, (LPVOID)(entry32.FullDllName.Buffer), (LPVOID)dllName,
			entry32.FullDllName.Length, &written);
		printf("%d\t Flink:%X Blink:%X dllbase:%X \tsize:%X \t %ws \n", index++,
			entry32.InLoadOrderModuleList.Flink, entry32.InLoadOrderModuleList.Blink,
			entry32.DllBase, entry32.SizeOfImage, dllName);

		//if(strcmpi)
		if (0 == StrCmpW(dllPathUnicode, dllName))
		{
			// 找到自己的dll
			printf("find===================================================\n");
			// 继续向后读一个entry
			ret = ReadProcessMemory(hProcess, (LPVOID)(entry32.InLoadOrderModuleList.Flink), (LPVOID)&nextEntry,
				sizeof(LDR_DATA_TABLE_ENTRY32), &written);
			if (!ret)
			{
				AfxMessageBox("ReadProcessMemory LDR_DATA_TABLE_ENTRY32 23失败");
				goto END;
			}

			// 断链操作


			DWORD oldFlag = 0;
			//lastEntry.InLoadOrderModuleList.Flink = nextEntry.InLoadOrderModuleList.Flink;
			//nextEntry.InLoadOrderModuleList.Blink = lastEntry.InLoadOrderModuleList.Blink;
			///////////  entry32为当前  
			/////////// 把nextEntry的Blink指向前一个Flink
			ret = VirtualProtectEx(hProcess, (LPVOID)entry32.InLoadOrderModuleList.Flink,
				sizeof(PEB_LDR_DATA32), PAGE_EXECUTE_READWRITE, &oldFlag);
			if (!ret)
			{
				printf("VirtualProtectEx err: %X\n", GetLastError());
			}

			ret = WriteProcessMemory(hProcess, (LPVOID)(entry32.InLoadOrderModuleList.Flink + 4),
				(LPVOID)&entry32.InLoadOrderModuleList.Blink, 4, &written);
			if (!ret)
			{
				printf("WriteProcessMemory err: %X\n", GetLastError());
			}
			VirtualProtectEx(hProcess, (LPVOID)entry32.InLoadOrderModuleList.Flink,
				sizeof(PEB_LDR_DATA32), oldFlag, &oldFlag);

			/////////// 把lastEntry的Flink指向后一个Blink
			ret = VirtualProtectEx(hProcess, (LPVOID)entry32.InLoadOrderModuleList.Blink,
				sizeof(PEB_LDR_DATA32), PAGE_EXECUTE_READWRITE, &oldFlag);
			if (!ret)
			{
				printf("VirtualProtectEx err: %X\n", GetLastError());
			}
			ret = WriteProcessMemory(hProcess, (LPVOID)(entry32.InLoadOrderModuleList.Blink),
				(LPVOID)&entry32.InLoadOrderModuleList.Flink, 4, &written);
			if (!ret)
			{
				printf("WriteProcessMemory err: %X\n", GetLastError());
			}
			VirtualProtectEx(hProcess, (LPVOID)entry32.InLoadOrderModuleList.Blink,
				sizeof(PEB_LDR_DATA32), oldFlag, &oldFlag);

			printf("写入完成===================================================\n");
			break;
		}
		prev = entry32.InLoadOrderModuleList.Flink;
		lastEntry = entry32;
	}


	printf("===========开始处理InMemoryOrderModuleList +0x14 ==========\n");

	entry32 = { 0 };
	lastEntry = { 0 };
	nextEntry = { 0 };
	index = 0;
	prev = ldrData32.InLoadOrderModuleList.Flink;
	while (true)
	{
		WCHAR dllName[255] = { 0 };
		// read next 
		//printf("2 prev:%X\n", prev);
		ret = ReadProcessMemory(hProcess, (LPVOID)(prev), (LPVOID)&entry32,
			sizeof(LDR_DATA_TABLE_ENTRY32), &written);
		if (!ret)
		{
			printf("2 ReadProcessMemory LDR_DATA_TABLE_ENTRY32失败 %X\n", GetLastError());
			goto END;
		}

		if (entry32.DllBase == 0)
		{
			break;
		}

		ret = ReadProcessMemory(hProcess, (LPVOID)(entry32.FullDllName.Buffer), (LPVOID)dllName,
			entry32.FullDllName.Length, &written);
		printf("%d\t Flink:%X Blink:%X dllbase:%X \tsize:%X \t %ws \n", index++,
			entry32.InMemoryOrderModuleList.Flink, entry32.InMemoryOrderModuleList.Blink,
			entry32.DllBase, entry32.SizeOfImage, dllName);

		//if(strcmpi)
		if (0 == StrCmpW(dllPathUnicode, dllName))
		{
			// 找到自己的dll
			printf("2 find===================================================\n");
			// 继续向后读一个entry
			ret = ReadProcessMemory(hProcess, (LPVOID)(entry32.InMemoryOrderModuleList.Flink), (LPVOID)&nextEntry,
				sizeof(LDR_DATA_TABLE_ENTRY32), &written);
			if (!ret)
			{
				printf("2 ReadProcessMemory LDR_DATA_TABLE_ENTRY32 23失败 \n");
				goto END;
			}

			// 断链操作


			DWORD oldFlag = 0;
			//lastEntry.InLoadOrderModuleList.Flink = nextEntry.InLoadOrderModuleList.Flink;
			//nextEntry.InLoadOrderModuleList.Blink = lastEntry.InLoadOrderModuleList.Blink;
			///////////  entry32为当前  
			/////////// 把nextEntry的Blink指向前一个Flink
			ret = VirtualProtectEx(hProcess, (LPVOID)entry32.InMemoryOrderModuleList.Flink,
				sizeof(PEB_LDR_DATA32), PAGE_EXECUTE_READWRITE, &oldFlag);
			if (!ret)
			{
				printf("2 VirtualProtectEx err: %X\n", GetLastError());
			}

			ret = WriteProcessMemory(hProcess, (LPVOID)(entry32.InMemoryOrderModuleList.Flink + 4),
				(LPVOID)&entry32.InMemoryOrderModuleList.Blink, 4, &written);
			if (!ret)
			{
				printf("2 WriteProcessMemory err: %X\n", GetLastError());
			}
			VirtualProtectEx(hProcess, (LPVOID)entry32.InMemoryOrderModuleList.Flink,
				sizeof(PEB_LDR_DATA32), oldFlag, &oldFlag);

			/////////// 把lastEntry的Flink指向后一个Blink
			ret = VirtualProtectEx(hProcess, (LPVOID)entry32.InMemoryOrderModuleList.Blink,
				sizeof(PEB_LDR_DATA32), PAGE_EXECUTE_READWRITE, &oldFlag);
			if (!ret)
			{
				printf("2 VirtualProtectEx err: %X\n", GetLastError());
			}
			ret = WriteProcessMemory(hProcess, (LPVOID)(entry32.InMemoryOrderModuleList.Blink),
				(LPVOID)&entry32.InMemoryOrderModuleList.Flink, 4, &written);
			if (!ret)
			{
				printf("2 WriteProcessMemory err: %X\n", GetLastError());
			}
			VirtualProtectEx(hProcess, (LPVOID)entry32.InMemoryOrderModuleList.Blink,
				sizeof(PEB_LDR_DATA32), oldFlag, &oldFlag);

			printf("2 写入完成===================================================\n");
			break;
		}
		prev = entry32.InLoadOrderModuleList.Flink;
		if (prev == 0)
		{
			printf("InMemoryOrderModuleList.Flink is 0 \n");
			break;
		}
		lastEntry = entry32;
	}

	printf("===========开始处理InInitializationOrderModuleList +0x1c ==========\n");

	entry32 = { 0 };
	lastEntry = { 0 };
	nextEntry = { 0 };
	index = 0;
	prev = ldrData32.InLoadOrderModuleList.Flink;
	while (true)
	{
		WCHAR dllName[255] = { 0 };
		// read next 
		//printf("3 prev:%X\n", prev);
		ret = ReadProcessMemory(hProcess, (LPVOID)(prev), (LPVOID)&entry32,
			sizeof(LDR_DATA_TABLE_ENTRY32), &written);
		if (!ret)
		{
			printf("3 ReadProcessMemory LDR_DATA_TABLE_ENTRY32失败 %X\n", GetLastError());
			goto END;
		}

		if (entry32.DllBase == 0)
		{
			break;
		}

		ret = ReadProcessMemory(hProcess, (LPVOID)(entry32.FullDllName.Buffer), (LPVOID)dllName,
			entry32.FullDllName.Length, &written);
		printf("%d\t Flink:%X Blink:%X dllbase:%X \tsize:%X \t %ws \n", index++,
			entry32.InInitializationOrderModuleList.Flink, entry32.InInitializationOrderModuleList.Blink,
			entry32.DllBase, entry32.SizeOfImage, dllName);

		//if(strcmpi)
		if (0 == StrCmpW(dllPathUnicode, dllName))
		{
			// 找到自己的dll
			printf("3 find===================================================\n");
			// 继续向后读一个entry
			ret = ReadProcessMemory(hProcess, (LPVOID)(entry32.InInitializationOrderModuleList.Flink), (LPVOID)&nextEntry,
				sizeof(LDR_DATA_TABLE_ENTRY32), &written);
			if (!ret)
			{
				printf("3 ReadProcessMemory 继续向后读一个 23失败 \n");
				goto END;
			}

			// 断链操作


			DWORD oldFlag = 0;
			//lastEntry.InLoadOrderModuleList.Flink = nextEntry.InLoadOrderModuleList.Flink;
			//nextEntry.InLoadOrderModuleList.Blink = lastEntry.InLoadOrderModuleList.Blink;
			///////////  entry32为当前  
			/////////// 把nextEntry的Blink指向前一个Flink
			ret = VirtualProtectEx(hProcess, (LPVOID)entry32.InInitializationOrderModuleList.Flink,
				sizeof(PEB_LDR_DATA32), PAGE_EXECUTE_READWRITE, &oldFlag);
			if (!ret)
			{
				printf("3 VirtualProtectEx err: %X\n", GetLastError());
			}

			ret = WriteProcessMemory(hProcess, (LPVOID)(entry32.InInitializationOrderModuleList.Flink + 4),
				(LPVOID)&entry32.InInitializationOrderModuleList.Blink, 4, &written);
			if (!ret)
			{
				printf("3 WriteProcessMemory err: %X\n", GetLastError());
			}
			VirtualProtectEx(hProcess, (LPVOID)entry32.InInitializationOrderModuleList.Flink,
				sizeof(PEB_LDR_DATA32), oldFlag, &oldFlag);

			/////////// 把lastEntry的Flink指向后一个Blink
			ret = VirtualProtectEx(hProcess, (LPVOID)entry32.InInitializationOrderModuleList.Blink,
				sizeof(PEB_LDR_DATA32), PAGE_EXECUTE_READWRITE, &oldFlag);
			if (!ret)
			{
				printf("3 VirtualProtectEx err: %X\n", GetLastError());
			}
			ret = WriteProcessMemory(hProcess, (LPVOID)(entry32.InInitializationOrderModuleList.Blink),
				(LPVOID)&entry32.InInitializationOrderModuleList.Flink, 4, &written);
			if (!ret)
			{
				printf("3 WriteProcessMemory err: %X\n", GetLastError());
			}
			VirtualProtectEx(hProcess, (LPVOID)entry32.InInitializationOrderModuleList.Blink,
				sizeof(PEB_LDR_DATA32), oldFlag, &oldFlag);

			printf("3 写入完成===================================================\n");
			break;
		}

		// 这里位置不变
		prev = entry32.InLoadOrderModuleList.Flink;
		if (prev == 0)
		{
			printf("3 InInitializationOrderModuleList.Flink is 0 \n");
			break;
		}
		lastEntry = entry32;
	}
#endif // _WIN64



	END:
	VirtualFreeEx(hProcess, pRemoteParam, 0x1000, MEM_DECOMMIT);
	CloseHandle(hProcess);
}
