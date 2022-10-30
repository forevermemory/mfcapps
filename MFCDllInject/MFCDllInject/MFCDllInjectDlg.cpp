
// MFCDllInjectDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCDllInject.h"
#include "MFCDllInjectDlg.h"
#include "CDlgProcessList.h"
#include "afxdialogex.h"


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
