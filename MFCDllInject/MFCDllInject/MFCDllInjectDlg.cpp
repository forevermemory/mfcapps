
// MFCDllInjectDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCDllInject.h"
#include "MFCDllInjectDlg.h"
#include "CDlgProcessList.h"
#include "ui.h"
#include "afxdialogex.h"
#include <tlhelp32.h>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
}

BEGIN_MESSAGE_MAP(CMFCDllInjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_INJECT, &CMFCDllInjectDlg::OnBnClickedButtonInject)
	ON_BN_CLICKED(IDC_BUTTON_UNINSTALL, &CMFCDllInjectDlg::OnBnClickedButtonUninstall)
	ON_BN_CLICKED(IDC_BUTTON_OPENPROCESS, &CMFCDllInjectDlg::OnBnClickedButtonOpenprocess)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_DLL, &CMFCDllInjectDlg::OnBnClickedButtonSelectDll)
	ON_BN_CLICKED(IDC_BUTTON_EIP_INJECT, &CMFCDllInjectDlg::OnBnClickedButtonEipInject)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCDllInjectDlg::OnBnClickedButton2)
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



void CMFCDllInjectDlg::OnBnClickedButtonInject()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_DllPath.GetLength()==0)
	{
		MessageBox("请选择dll");
		return;
	}
	if (m_Select_Pid <=1)
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

	// kernel32是内核函数 每个进程都会加载 在操作系统分配的地址都是一致的
	HMODULE hModule = ::GetModuleHandle("Kernel32.dll");
	PTHREAD_START_ROUTINE pRemoteFunc = (PTHREAD_START_ROUTINE)::GetProcAddress(hModule, "LoadLibraryA");
	if (NULL == pRemoteFunc)
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

END:
	::VirtualFreeEx(hProcess, pRemoteParam, param.GetLength(), MEM_DECOMMIT);
}


void CMFCDllInjectDlg::OnBnClickedButtonUninstall()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_DllPath.GetLength() == 0)
	{
		AfxMessageBox("请选择dll");
		return;
	}
	if (m_Select_Pid <= 1)
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
	HANDLE pThread;
	SIZE_T dwWriten = 0;
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


END:
	::VirtualFreeEx(hProcess, pRemoteParam, param.GetLength(), MEM_DECOMMIT);
}


void CMFCDllInjectDlg::OnBnClickedButtonOpenprocess()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgProcessList dlg;
	int res = dlg.DoModal();
	if (res == IDOK)
	{
		m_Select_Pid = dlg.m_Pid;
		UpdateData(FALSE);
	}
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


void CMFCDllInjectDlg::OnBnClickedButtonEipInject()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_DllPath.GetLength() == 0)
	{
		MessageBox("请选择dll");
		return;
	}
	if (m_Select_Pid <= 1)
	{
		AfxMessageBox("请打开进程");
		return;
	}


	// 创建线程快照
	HANDLE hSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	
	// 遍历所有线程
	THREADENTRY32 pe32;
	pe32.dwSize = sizeof(THREADENTRY32);
	BOOL ret = ::Thread32First(hSnapShot, &pe32);
	while (ret)
	{
		//printf("thread32 : processid: %d  threadid: %d\n", pe32.th32OwnerProcessID , pe32.th32ThreadID);
		if (pe32.th32OwnerProcessID == m_Select_Pid)
		{
			break;
		}
		ret = ::Thread32Next(hSnapShot, &pe32);
	}

	printf("获取到的线程id为 %d \n", pe32.th32ThreadID);


	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, false, pe32.th32ThreadID);
	if (hThread == NULL)
	{
		::MessageBoxA(NULL, "OpenThread失败", NULL, 0);
		return;
	}

	SuspendThread(hThread); // 挂起线程

	// 获取进程句柄
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_Select_Pid);


	//////////////////////////////////////////.
	
	// 获取线程上下文
	CONTEXT threadContext;
	threadContext.ContextFlags = CONTEXT_FULL;
	ret = GetThreadContext(hThread, &threadContext);

	// 分配虚拟内存 
	// 第一步 先写入参数
	DWORD dwSize = MAX_PATH;
	BYTE * pRemoteAddr = (BYTE*)::VirtualAllocEx(hProcess, NULL, dwSize,
		MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (pRemoteAddr==NULL)
	{
		::MessageBoxA(NULL, "VirtualAllocEx失败", NULL, 0);
		return;
	}
	printf("pRemoteAddr: %X \n ", pRemoteAddr);

	::WriteProcessMemory(hProcess, (pRemoteAddr + shellCodeSize), m_DllPath.GetBuffer(), m_DllPath.GetLength() + 1, NULL);

	BYTE shellCode[shellCodeSize] = {
		0x60,  // pushad
		0x9c,  // pushfd
		0x68,   // push
		0, 0, 0, 0, // 3 4 5 6   
		0xe8, // call
		0, 0, 0, 0, // 8 9 10 11      
		0x9d, // popfd
		0x61,  // popad
		0xe9, //jmp 
		0, 0, 0 ,0  // 15 16 17 18
	};
	DWORD* pdwAddr = NULL;
	
	pdwAddr = (DWORD*)&shellCode[3]; 
	*pdwAddr = (DWORD)(pRemoteAddr + shellCodeSize);

	pdwAddr = (DWORD*)&shellCode[8]; //shellcode[8 9 10 11]
	*pdwAddr = (DWORD)LoadLibraryA -(((DWORD)pRemoteAddr + 7) + 5); // 因为直接call地址了，所以对应机器码需要转换，计算VA 

	pdwAddr = (DWORD*)&shellCode[15]; //shellcode[15 16 17 18]
	*pdwAddr = threadContext.Eip - ((DWORD)(pRemoteAddr+14)+5); // 因为直接jmp地址了，所以对应机器码需要转换，计算VA

	// 第二步 写入shellcode
	ret = WriteProcessMemory(hProcess, pRemoteAddr, shellCode, sizeof(shellCode), NULL);
	if (!ret)
	{
		::MessageBoxA(NULL, "WriteProcessMemory失败", NULL, 0);
		return;
	}
	//::MessageBoxA(NULL, "WriteProcessMemory", NULL, 0);

	// 设定eip地址
	threadContext.Eip = (DWORD)pRemoteAddr;
	SetThreadContext(hThread, &threadContext);
	ResumeThread(hThread);

	CloseHandle(hProcess);
	CloseHandle(hThread);
}

using namespace std;

void CMFCDllInjectDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

		// 创建线程快照
	HANDLE hSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	// 遍历所有线程
	THREADENTRY32 pe32;
	pe32.dwSize = sizeof(THREADENTRY32);
	BOOL ret = ::Thread32First(hSnapShot, &pe32);
	while (ret)
	{
		if (pe32.th32OwnerProcessID == 3884)
		{
			printf("thread32 : processid: %d  threadid: %d \n", pe32.th32OwnerProcessID, pe32.th32ThreadID);

		}
		ret = ::Thread32Next(hSnapShot, &pe32);
	}
}
