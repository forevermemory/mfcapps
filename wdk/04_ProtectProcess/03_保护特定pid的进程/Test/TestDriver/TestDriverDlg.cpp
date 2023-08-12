
// TestDriverDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "TestDriver.h"
#include "TestDriverDlg.h"
#include "afxdialogex.h"
#include "define.h"

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


// CTestDriverDlg 对话框



CTestDriverDlg::CTestDriverDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTDRIVER_DIALOG, pParent)
	, m_PID(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDriverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PID, m_PID);
}

BEGIN_MESSAGE_MAP(CTestDriverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_SYS, &CTestDriverDlg::OnBnClickedButtonOpenSys)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_SYS, &CTestDriverDlg::OnBnClickedButtonCloseSys)
	ON_BN_CLICKED(IDC_BUTTON_IO_CONTROL, &CTestDriverDlg::OnBnClickedButtonIoControl)
	ON_BN_CLICKED(IDC_BUTTON_GET_PROCESS_NAME, &CTestDriverDlg::OnBnClickedButtonGetProcessName)
	ON_BN_CLICKED(IDC_BUTTON_ADD_PID_PROTECT, &CTestDriverDlg::OnBnClickedButtonAddPidProtect)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_PID_PROTECT2, &CTestDriverDlg::OnBnClickedButtonRemovePidProtect2)
END_MESSAGE_MAP()


// CTestDriverDlg 消息处理程序

BOOL CTestDriverDlg::OnInitDialog()
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
	char buf[64] = { 0 };
	DWORD pid = GetCurrentProcessId();
	sprintf_s(buf, "当前pid: %d %X", pid, pid);
	SetWindowTextA(m_hWnd, buf);

	m_PID = pid;
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestDriverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestDriverDlg::OnPaint()
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
HCURSOR CTestDriverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


HANDLE DeviceHandle = NULL;

void CTestDriverDlg::OnBnClickedButtonOpenSys()
{
	//IRP_MJ_CREATE
	DeviceHandle = CreateFileW(
		L"\\??\\MyDriver",
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

}


void CTestDriverDlg::OnBnClickedButtonCloseSys()
{

	if (DeviceHandle)
	{
		CloseHandle(DeviceHandle);//IRP_MJ_CLOSE
	}
}


void CTestDriverDlg::OnBnClickedButtonIoControl()
{
	UINT64 inBuf[6] = { 1,2,3,4,5,6 };//输出缓冲区
	UINT64 outBuf[6] = { 0 };//输出缓冲区
	DWORD dwRetSize = 0;
	DeviceIoControl(
		DeviceHandle,//CreateFile打开驱动设备 返回的句柄
		IOCTL_TEST_1,//控制码 CTL_CODE
		&inBuf,//输入缓冲区指针
		sizeof(inBuf),//输入缓冲区大小
		&outBuf,//返回缓冲区
		sizeof(outBuf),//返回缓冲区大小
		&dwRetSize, //返回字节数
		NULL);

	//打印返回参数
	CStringA csStr;
	csStr.Format("EXE: 接收驱动层返回的参数(%x,%x,%x,%x,%x,%x  dwRetSize=%d",
		outBuf[0], outBuf[1], outBuf[2], outBuf[3], outBuf[4], outBuf[5], dwRetSize);
	OutputDebugStringA(csStr);
}


void CTestDriverDlg::OnBnClickedButtonGetProcessName()
{
	
	UpdateData(TRUE);
	if (m_PID == 0)
	{
		MessageBoxA(m_hWnd, 0, "请输入pid", 0);
		return;
	}
	
	UINT64 inBuf[2] = { m_PID,0 };//输出缓冲区
	char outBuf[255] = { 0 };//输出缓冲区
	DWORD dwRetSize = 0;
	DeviceIoControl(
		DeviceHandle,//CreateFile打开驱动设备 返回的句柄
		IOCTL_GETPROCESS_NAME,//控制码 CTL_CODE
		&inBuf,//输入缓冲区指针
		sizeof(inBuf),//输入缓冲区大小
		&outBuf,//返回缓冲区
		sizeof(outBuf),//返回缓冲区大小
		&dwRetSize, //返回字节数
		NULL);

	//打印返回参数
	CStringA csStr;
	csStr.Format("EXE: 接收驱动层返回的dwRetSize=%lld, 字符串%s",
		dwRetSize, outBuf);
	OutputDebugStringA(csStr);
	MessageBoxA(m_hWnd, 0, csStr, 0);
}




void CTestDriverDlg::OnBnClickedButtonAddPidProtect()
{
	UpdateData(TRUE);
	if (m_PID == 0)
	{
		MessageBoxA(m_hWnd, 0, "请输入pid", 0);
		return;
	}

	UINT64 inBuf[2] = { m_PID,0 };//输出缓冲区
	char outBuf[255] = { 0 };//输出缓冲区
	DWORD dwRetSize = 0;
	DeviceIoControl(
		DeviceHandle,//CreateFile打开驱动设备 返回的句柄
		IOCTL_ADD_PID,//控制码 CTL_CODE
		&inBuf,//输入缓冲区指针
		sizeof(inBuf),//输入缓冲区大小
		&outBuf,//返回缓冲区
		sizeof(outBuf),//返回缓冲区大小
		&dwRetSize, //返回字节数
		NULL);

}


void CTestDriverDlg::OnBnClickedButtonRemovePidProtect2()
{
	UpdateData(TRUE);
	if (m_PID == 0)
	{
		MessageBoxA(m_hWnd, 0, "请输入pid", 0);
		return;
	}

	UINT64 inBuf[2] = { m_PID,0 };//输出缓冲区
	char outBuf[255] = { 0 };//输出缓冲区
	DWORD dwRetSize = 0;
	DeviceIoControl(
		DeviceHandle,//CreateFile打开驱动设备 返回的句柄
		IOCTL_REMOVE_PID,//控制码 CTL_CODE
		&inBuf,//输入缓冲区指针
		sizeof(inBuf),//输入缓冲区大小
		&outBuf,//返回缓冲区
		sizeof(outBuf),//返回缓冲区大小
		&dwRetSize, //返回字节数
		NULL);
}
