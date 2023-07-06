
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define MY_DEBUG 0
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


void PrintMemCpuInfo()
{


	//SYSTEM_INFO si;
	//GetSystemInfo(&si);

	//OSVERSIONINFO osvi;//定义OSVERSIONINFO数据结构对象
	//memset(&osvi, 0, sizeof(OSVERSIONINFO));//开空间 
	//osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);//定义大小 
	//GetVersionEx(&osvi);//获得版本信息 


	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap)
	{
		PROCESSENTRY32 entry = { sizeof entry };
		for (BOOL bState = Process32First(hSnap, &entry); bState; bState = Process32Next(hSnap, &entry))
		{
			printf("%d \t %s\n", entry.th32ProcessID, entry.szExeFile);
			
		}
		CloseHandle(hSnap);
	}
}

CMFCDllInjectDlg::CMFCDllInjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCDLLINJECT_DIALOG, pParent)
	, m_DllPath(_T(""))
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

	AllocConsole(); // 添加调试窗口
	freopen("conin$", "r+t", stdin); // 将输入流设置为当前调试窗口
	freopen("conout$", "w+t", stdout); // 将输出流设置为当前调试窗口
	SetConsoleTitleA("调试窗口");

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
			process[pe32.szExeFile] = pe32.th32ProcessID;
		}
	#else
			process[pe32.th32ProcessID] = pe32.szExeFile;
	#endif // MY_DEBUG


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

	CString old;
	GetWindowTextA(old);

	CString ns;
	ns.Format("%s, 总计: %d个进程",old,process.size());
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

	ULONG64 funcAddr = 0;
	ULONG64 funcAddrW = 0;

	// 获取LoadLibrary
	BOOL isWow32;
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_Select_Pid);
	IsWow64Process(hProcess, &isWow32);
	CloseHandle(hProcess);
	// 返回true为32位进程
	if (isWow32)
	{
		BOOL ret = GetProcessDllFunctionsByNameX32(m_Select_Pid, "kernel32.dll", "LoadLibraryA", &funcAddr);
		if (!ret)
		{
			printf("32 没有找到函数\n");
			return;
		}
		ret = GetProcessDllFunctionsByNameX32(m_Select_Pid, "kernel32.dll", "LoadLibraryW", &funcAddrW);
		if (!ret)
		{
			printf("32 没有找到函数\n");
		}
	}
	else
	{
		BOOL ret = GetProcessDllFunctionsByNameX64(m_Select_Pid, "kernel32.dll", "LoadLibraryA", &funcAddr);
		if (!ret)
		{
			printf("64 没有找到函数\n");
			return;
		}
		ret = GetProcessDllFunctionsByNameX64(m_Select_Pid, "kernel32.dll", "LoadLibraryW", &funcAddrW);
		if (!ret)
		{
			printf("32 没有找到函数\n");
		}
	}



	printf("找到LoadLibraryA地址: %llp\n", funcAddr);
	printf("找到LoadLibraryW地址: %llp\n", funcAddrW);
	m_LoadLibraryA_Addr = funcAddr;
	m_LoadLibraryW_Addr = funcAddrW;
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
	if (isWow64)
	{
		// 32位
		BOOL ret;
		DWORD cbNeeded = 0;
		HMODULE lphModule[1024] = { 0 };
		MODULEINFO modinfo = { 0 };
		EnumProcessModulesEx(hProcess, lphModule, sizeof(lphModule), &cbNeeded, LIST_MODULES_ALL);

		for (size_t i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			char szModName[MAX_PATH] = { 0 };
			// Get the full path to the module's file.
			ret = GetModuleFileNameExA(hProcess, lphModule[i], szModName, sizeof(szModName) / sizeof(char));
			//ret = GetModuleBaseNameA(hProcess, lphModule[i], szModName, sizeof(szModName) / sizeof(char));
			if (!ret)
			{
				printf("GetModuleFileNameExA err %d\n", GetLastError());
				continue;
			}

			//printf("===========szModName:%s\n", szModName);
			// not match 
			if (0 != _stricmp(szModName, m_DllPath))
			{
				// printf("not match: %s %s\n", m_DllPath, szModName);
				continue;
			}

			/////////// success match target dll
			// GetModuleInformation 
			ret = GetModuleInformation(hProcess, lphModule[i], &modinfo, sizeof(MODULEINFO));
			if (ret == 0)
			{
				printf("GetModuleInformation err : %d\n", GetLastError());
				goto EXIT ;
			}

			break;
		}

		printf("============start uninstall\n");
		ULONG64 funcAddr = 0;
		 ret = GetProcessDllFunctionsByNameX32(m_Select_Pid, "kernel32.dll", "FreeLibrary", &funcAddr);
		if (!ret)
		{
			AfxMessageBox("没有找到函数FreeLibrary");
			goto EXIT;
		}
		
			// 创建远程线程调用 FreeLibrary   
		HANDLE	hThread = CreateRemoteThread(hProcess, NULL, 0, (PTHREAD_START_ROUTINE)funcAddr,
			modinfo.lpBaseOfDll, 0, NULL);
		printf("=======CreateRemoteThread==hThread %X=====, hThread\n", hThread);

		if (NULL == hThread)
		{
			printf("=======CreateRemoteThread==fail %d=====, hThread\n", GetLastError());

			::CloseHandle(hProcess);
			goto EXIT;
		}
		printf("=======CreateRemoteThread=======\n");

		// 等待远程线程结束   
		::WaitForSingleObject(hThread, INFINITE);
		// 清理   
		::CloseHandle(hThread);
		printf("=======WaitForSingleObject=======\n");
	}
	else {
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
		MODULEENTRY32 me32 = { 0 };
		me32.dwSize = sizeof(MODULEENTRY32);

		// 开始遍历   
		if (FALSE == ::Module32First(hModuleSnap, &me32))
		{
			::CloseHandle(hModuleSnap);
			AfxMessageBox("Module32First   ");
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
			AfxMessageBox("没有找到指定模块   ");
			goto EXIT;
		}

		// 从 Kernel32.dll 中获取 FreeLibrary 函数地址   
		//LPTHREAD_START_ROUTINE lpThreadFun = (PTHREAD_START_ROUTINE)::GetProcAddress(
		//	::GetModuleHandle(_T("Kernel32")), "FreeLibrary");

		ULONG64 funcAddr = 0;
		BOOL ret = GetProcessDllFunctionsByNameX64(m_Select_Pid, "kernel32.dll", "FreeLibrary", &funcAddr);
		if (!ret)
		{
			AfxMessageBox("没有找到函数FreeLibrary");
			goto EXIT;
		}
		printf("FreeLibrary addr :%llX \n", funcAddr);
		printf("FreeLibrary addr :%s \n", me32.szModule);
		printf("FreeLibrary addr :%s \n", me32.szExePath);
		
		// 创建远程线程调用 FreeLibrary   
		hThread = ::CreateRemoteThread(hProcess, NULL, 0, (PTHREAD_START_ROUTINE)funcAddr,
			me32.hModule, 0, NULL);
		printf("=======CreateRemoteThread==hThread %X=====, hThread\n", hThread);

		if (NULL == hThread)
		{
			printf("=======CreateRemoteThread==fail %d=====, hThread\n",GetLastError() );

			::CloseHandle(hProcess);
			goto EXIT;
		}
		printf("=======CreateRemoteThread=======\n");

		// 等待远程线程结束   
		::WaitForSingleObject(hThread, INFINITE);
		// 清理   
		::CloseHandle(hThread);
		printf("=======WaitForSingleObject=======\n");
	}


	EXIT:
	CloseHandle(hProcess);

}


// 远程线程注入
void CMFCDllInjectDlg::OnBnClickedButtonRemoteThread()
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

END:
	::VirtualFreeEx(hProcess, pRemoteParam, param.GetLength(), MEM_DECOMMIT);
}



void CMFCDllInjectDlg::OnBnClickedButtonRtlcreateuserthread()
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
		goto END;
	}

	ULONG64 funcAddr = 0;
	BOOL isFind = GetProcessDllFunctionsByNameX64(GetCurrentProcessId(), "ntdll.dll", "RtlCreateUserThread", &funcAddr);
	if (FALSE == isFind)
	{
		AfxMessageBox("获取RtlCreateUserThread地址失败");
		goto END;
	}

	P_RtlCreateUserThread func_RtlCreateUserThread = (P_RtlCreateUserThread)funcAddr;
	HANDLE pThread = NULL;

	if (isWow64)
	{
		func_RtlCreateUserThread(hProcess, NULL, FALSE, 0, 0, 0, (PTHREAD_START_ROUTINE)(DWORD)m_LoadLibraryA_Addr,
			pRemoteParam, &pThread);
	}
	else
	{
		func_RtlCreateUserThread(hProcess, NULL, FALSE, 0, 0, 0, (PTHREAD_START_ROUTINE)m_LoadLibraryA_Addr,
			pRemoteParam, &pThread);
	}


	if (pThread == NULL)
	{
		AfxMessageBox("创建RtlCreateUserThread失败");
		goto END;
	}
	// waitfor 
	WaitForSingleObject(pThread, INFINITE);

END:
	::VirtualFreeEx(hProcess, pRemoteParam, param.GetLength(), MEM_DECOMMIT);
}

void CMFCDllInjectDlg::OnBnClickedButtonNtcreatethreadex()
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
		goto END;
	}

	ULONG64 funcAddr = 0;
	BOOL isFind;
	HANDLE pThread = NULL;

	P_NtCreateThreadEx64 func_NtCreateThreadEx;
	isFind = GetProcessDllFunctionsByNameX64(GetCurrentProcessId(), "ntdll.dll", "NtCreateThreadEx", &funcAddr);
	if (FALSE == isFind)
	{
		AfxMessageBox("获取NtCreateThreadEx地址失败");
		goto END;
	}
	func_NtCreateThreadEx = (P_NtCreateThreadEx64)funcAddr;

	if (isWow64)
	{
		func_NtCreateThreadEx(&pThread, THREAD_ALL_ACCESS, NULL, hProcess, (PVOID)(DWORD)m_LoadLibraryA_Addr,
			pRemoteParam, 0, 0, 0, 0, NULL);
	}
	else
	{
		func_NtCreateThreadEx(&pThread, THREAD_ALL_ACCESS, NULL, hProcess, (PVOID)m_LoadLibraryA_Addr,
			pRemoteParam, 0, 0, 0, 0, NULL);
	}

	if (pThread == NULL)
	{
		AfxMessageBox("创建NtCreateThreadEx失败");
		goto END;
	}
	// waitfor 
	WaitForSingleObject(pThread, INFINITE);
	CloseHandle(pThread);
END:

	::VirtualFreeEx(hProcess, pRemoteParam, param.GetLength(), MEM_DECOMMIT);
	CloseHandle(hProcess);
}


void CMFCDllInjectDlg::OnBnClickedButtonApc()
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

	printf("=============end=========\n");
}


void CMFCDllInjectDlg::OnBnClickedButtonHijack()
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


	// 分配参数空间
	CString param(m_DllPath);
	LPVOID pRemoteParam = ::VirtualAllocEx(hProcess, NULL, 0x1000,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (pRemoteParam == NULL)
	{
		AfxMessageBox("分配参数空间失败");
		return;
	}

	// 写入参数到目标进程 偏移0x600
	SIZE_T dwWriten = 0;
	BOOL res = ::WriteProcessMemory(hProcess, (PUCHAR)pRemoteParam+0x600, param.GetBuffer(0),
		param.GetLength(), &dwWriten);
	if (!res)
	{
		AfxMessageBox("写入参数到目标进程失败");
		return;
	}

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
	SuspendThread(tHandle);


	//构建shellcode
	if (isWow64)
	{
		WOW64_CONTEXT context = { 0 };
		context.ContextFlags = CONTEXT_ALL;
		Wow64GetThreadContext(tHandle, &context);
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
		
		*(PDWORD)&buff[15] = context.Eip - ((DWORD)(PUCHAR)pRemoteParam + 19);

		ret = WriteProcessMemory(hProcess, pRemoteParam, buff, sizeof(buff), &dwWriten);
		if (!ret)
		{
			AfxMessageBox("WriteProcessMemory shellcode失败");
			return;
		}

		context.Eip = (DWORD)pRemoteParam;
		Wow64SetThreadContext(tHandle, &context);

	}
	else
	{
		//获取这个线程的Context

		CONTEXT context = { 0 };
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
	}
	
	// 恢复主线程
	ResumeThread(tHandle);
	printf("end==================\n");

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
