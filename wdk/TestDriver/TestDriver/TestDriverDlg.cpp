
// TestDriverDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "TestDriver.h"
#include "TestDriverDlg.h"
#include "afxdialogex.h"
#include "ioctlCode.h"
#include "systemInformationClass.h"
#include "define.h"

#include <psapi.h>

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
	, m_TargetPid(0)
	, m_Pid_Privilege(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDriverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PID, m_PID);
	DDX_Text(pDX, IDC_EDIT2, m_TargetPid);
	DDX_Text(pDX, IDC_EDIT_PRIVILEGE_PID, m_Pid_Privilege);
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
	ON_BN_CLICKED(IDC_BUTTON_BUFFER_READ, &CTestDriverDlg::OnBnClickedButtonBufferRead)
	ON_BN_CLICKED(IDC_BUTTON_BUFFER_WRITE, &CTestDriverDlg::OnBnClickedButtonBufferWrite)
	ON_BN_CLICKED(IDC_BUTTON_MDL_READ, &CTestDriverDlg::OnBnClickedButtonMdlRead)
	ON_BN_CLICKED(IDC_BUTTON_MDL_WRITE, &CTestDriverDlg::OnBnClickedButtonMdlWrite)
	ON_BN_CLICKED(IDC_BUTTON_MDL_WRITE_READONLY1, &CTestDriverDlg::OnBnClickedButtonMdlWriteReadonly1)
	ON_BN_CLICKED(IDC_BUTTON_CR0_WRITE_READONLY2, &CTestDriverDlg::OnBnClickedButtonCr0WriteReadonly2)
	ON_BN_CLICKED(IDC_BUTTON_PH_READ, &CTestDriverDlg::OnBnClickedButtonPhRead)
	ON_BN_CLICKED(IDC_BUTTON_PH_WRITE_READONLY4, &CTestDriverDlg::OnBnClickedButtonPhWriteReadonly4)
	ON_BN_CLICKED(IDC_BUTTON_PH_WRITE_READONLY5, &CTestDriverDlg::OnBnClickedButtonPhWriteReadonly5)
	ON_BN_CLICKED(IDC_BUTTON_ENUM_OB_CALLBACLS, &CTestDriverDlg::OnBnClickedButtonEnumObCallbacls)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_PRIVILEGE, &CTestDriverDlg::OnBnClickedButtonRemovePrivilege)
	ON_BN_CLICKED(IDC_BUTTON_ADD_PRIVILEGE, &CTestDriverDlg::OnBnClickedButtonAddPrivilege)
	ON_BN_CLICKED(IDC_BUTTON_R3_ENUM_HANDLES, &CTestDriverDlg::OnBnClickedButtonR3EnumHandles)
	ON_BN_CLICKED(IDC_BUTTON_R3_NtQueryInformationProcess, &CTestDriverDlg::OnBnClickedButtonR3Ntqueryinformationprocess)
	ON_BN_CLICKED(IDC_BUTTON_R3_ENUM_SYSTEMINFO, &CTestDriverDlg::OnBnClickedButtonR3EnumSysteminfo)
	ON_BN_CLICKED(IDC_BUTTON_R3_OPEN_1000_HANDLE, &CTestDriverDlg::OnBnClickedButtonR3Open1000Handle)
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
	sprintf_s(buf, "TestDriver.exe 当前pid: %d %X", pid, pid);
	SetWindowTextA(m_hWnd, buf);

	m_PID = pid;
	UpdateData(FALSE);

#pragma warning(disable:4996)
	AllocConsole(); // 添加调试窗口
	freopen("conin$", "r+t", stdin); // 将输入流设置为当前调试窗口
	freopen("conout$", "w+t", stdout); // 将输出流设置为当前调试窗口
	SetConsoleTitleA("调试窗口");


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


void CTestDriverDlg::OnBnClickedButtonBufferRead()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_PID == 0)
	{
		MessageBoxA(m_hWnd, 0, "请输入被读取进程的pid", 0);
		return;
	}

	UINT64 virtualAddr = 0x7FF69B468BE8;
	UINT64 inBuf[4] = { m_TargetPid,virtualAddr,8,0 };//输入缓冲区
	UINT64 outBuf  = 0;//输出缓冲区
	DWORD dwRetSize = 0;
	DeviceIoControl(
		DeviceHandle,//CreateFile打开驱动设备 返回的句柄
		IOCTL_缓冲区读数据,//控制码 CTL_CODE
		&inBuf,//输入缓冲区指针
		sizeof(inBuf),//输入缓冲区大小
		&outBuf,//返回缓冲区
		sizeof(outBuf),//返回缓冲区大小
		&dwRetSize, //返回字节数
		NULL);
	char buf[128] = { 0 };
	sprintf_s(buf, "exe: dwRetSize:%lld, pid:%d, va:%p, value:%llX", dwRetSize, m_TargetPid, virtualAddr, outBuf);
	OutputDebugStringA(buf);
	
}


void CTestDriverDlg::OnBnClickedButtonBufferWrite()
{
	UpdateData(TRUE);
	if (m_PID == 0)
	{
		MessageBoxA(m_hWnd, 0, "请输入被读取进程的pid", 0);
		return;
	}

	UINT64 virtualAddr = 0x7FF69B468BE8;
	UINT64 inBuf[4] = { m_TargetPid,virtualAddr,8, 0x1122aabbccdd };//输入缓冲区
	UINT64 outBuf = 0;//输出缓冲区
	DWORD dwRetSize = 0;
	DeviceIoControl(
		DeviceHandle,//CreateFile打开驱动设备 返回的句柄
		IOCTL_缓冲区写数据,//控制码 CTL_CODE
		&inBuf,//输入缓冲区指针
		sizeof(inBuf),//输入缓冲区大小
		&outBuf,//返回缓冲区
		sizeof(outBuf),//返回缓冲区大小
		&dwRetSize, //返回字节数
		NULL);
	char buf[64] = { 0 };
	sprintf_s(buf, "exe: dwRetSize:%lld, pid:%d, va:%p, value:%llX", dwRetSize, m_TargetPid, virtualAddr, outBuf);
	OutputDebugStringA(buf);
}


void CTestDriverDlg::OnBnClickedButtonMdlRead()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_PID == 0)
	{
		MessageBoxA(m_hWnd, 0, "请输入被读取进程的pid", 0);
		return;
	}

	UINT64 virtualAddr = 0x7FF69B468BE8;
	UINT64 inBuf[4] = { m_TargetPid,virtualAddr,8,0 };//输入缓冲区
	UINT64 outBuf = 0;//输出缓冲区
	DWORD dwRetSize = 0;
	DeviceIoControl(
		DeviceHandle,//CreateFile打开驱动设备 返回的句柄
		IOCTL_MDL读数据,//控制码 CTL_CODE
		&inBuf,//输入缓冲区指针
		sizeof(inBuf),//输入缓冲区大小
		&outBuf,//返回缓冲区
		sizeof(outBuf),//返回缓冲区大小
		&dwRetSize, //返回字节数
		NULL);
	char buf[128] = { 0 };
	sprintf_s(buf, "exe: dwRetSize:%lld, pid:%d, va:%p, value:%llX", dwRetSize, m_TargetPid, virtualAddr, outBuf);
	OutputDebugStringA(buf);
	OutputDebugStringA("===================================================");
}


void CTestDriverDlg::OnBnClickedButtonMdlWrite()
{
	UpdateData(TRUE);
	if (m_PID == 0)
	{
		MessageBoxA(m_hWnd, 0, "请输入被读取进程的pid", 0);
		return;
	}

	UINT64 virtualAddr = 0x7FF69B468BE8;
	UINT64 inBuf[4] = { m_TargetPid,virtualAddr,8, 0x1122aabbccdd };//输入缓冲区
	UINT64 outBuf = 0;//输出缓冲区
	DWORD dwRetSize = 0;
	DeviceIoControl(
		DeviceHandle,//CreateFile打开驱动设备 返回的句柄
		IOCTL_MDL写数据,//控制码 CTL_CODE
		&inBuf,//输入缓冲区指针
		sizeof(inBuf),//输入缓冲区大小
		&outBuf,//返回缓冲区
		sizeof(outBuf),//返回缓冲区大小
		&dwRetSize, //返回字节数
		NULL);
	char buf[64] = { 0 };
	sprintf_s(buf, "exe: dwRetSize:%lld, pid:%d, va:%p, value:%llX", dwRetSize, m_TargetPid, virtualAddr, outBuf);
	OutputDebugStringA(buf);
}


void CTestDriverDlg::OnBnClickedButtonMdlWriteReadonly1()
{
	UpdateData(TRUE);
	if (m_PID == 0)
	{
		MessageBoxA(m_hWnd, 0, "请输入被读取进程的pid", 0);
		return;
	}

	UINT64 virtualAddr = 0x1F8DE7D0000; // Ce中添加 xxx.exe即可查询模块基址
	UINT64 inBuf[4] = { m_TargetPid,virtualAddr,8, 0x1122aabbccdd };//输入缓冲区
	UINT64 outBuf = 0;//输出缓冲区
	DWORD dwRetSize = 0;
	DeviceIoControl(
		DeviceHandle,//CreateFile打开驱动设备 返回的句柄
		IOCTL_MDL写只读数据,//控制码 CTL_CODE
		&inBuf,//输入缓冲区指针
		sizeof(inBuf),//输入缓冲区大小
		&outBuf,//返回缓冲区
		sizeof(outBuf),//返回缓冲区大小
		&dwRetSize, //返回字节数
		NULL);
	char buf[64] = { 0 };
	sprintf_s(buf, "exe: dwRetSize:%lld, pid:%d, va:%p, value:%llX", dwRetSize, m_TargetPid, virtualAddr, outBuf);
	OutputDebugStringA(buf);
	
}


void CTestDriverDlg::OnBnClickedButtonCr0WriteReadonly2()
{
	UpdateData(TRUE);
	if (m_PID == 0)
	{
		MessageBoxA(m_hWnd, 0, "请输入被读取进程的pid", 0);
		return;
	}

	UINT64 virtualAddr = 0x1F8DE7D0000; // Ce中添加 xxx.exe即可查询模块基址
	UINT64 inBuf[4] = { m_TargetPid,virtualAddr,8, 0x12 };//输入缓冲区
	UINT64 outBuf = 0;//输出缓冲区
	DWORD dwRetSize = 0;
	DeviceIoControl(
		DeviceHandle,//CreateFile打开驱动设备 返回的句柄
		IOCTL_CR0写只读数据,//控制码 CTL_CODE
		&inBuf,//输入缓冲区指针
		sizeof(inBuf),//输入缓冲区大小
		&outBuf,//返回缓冲区
		sizeof(outBuf),//返回缓冲区大小
		&dwRetSize, //返回字节数
		NULL);
	char buf[64] = { 0 };
	sprintf_s(buf, "exe: dwRetSize:%lld, pid:%d, va:%p, value:%llX", dwRetSize, m_TargetPid, virtualAddr, outBuf);
	OutputDebugStringA(buf);
}




void CTestDriverDlg::OnBnClickedButtonPhRead()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_PID == 0)
	{
		MessageBoxA(m_hWnd, 0, "请输入被读取进程的pid", 0);
		return;
	}

	UINT64 virtualAddr = 0x7FF69B468BE8;
	UINT64 inBuf[4] = { m_TargetPid,virtualAddr,8,0 };//输入缓冲区
	UINT64 outBuf = 0;//输出缓冲区
	DWORD dwRetSize = 0;
	DeviceIoControl(
		DeviceHandle,//CreateFile打开驱动设备 返回的句柄
		IOCTL_物理地址映射读数据,//控制码 CTL_CODE
		&inBuf,//输入缓冲区指针
		sizeof(inBuf),//输入缓冲区大小
		&outBuf,//返回缓冲区
		sizeof(outBuf),//返回缓冲区大小
		&dwRetSize, //返回字节数
		NULL);
	char buf[128] = { 0 };
	sprintf_s(buf, "exe: dwRetSize:%lld, pid:%d, va:%p, value:%llX", dwRetSize, m_TargetPid, virtualAddr, outBuf);
	OutputDebugStringA(buf);
}


void CTestDriverDlg::OnBnClickedButtonPhWriteReadonly4()
{
	UpdateData(TRUE);
	if (m_PID == 0)
	{
		MessageBoxA(m_hWnd, 0, "请输入被读取进程的pid", 0);
		return;
	}

	UINT64 writenVal = 0x13;
	UINT64 virtualAddr = 0x1F8DE7D0000; // Ce中添加 xxx.exe即可查询模块基址
	//UINT64 inBuf[4] = { m_TargetPid,virtualAddr,8,(UINT64)(PVOID)&writenVal };//输入缓冲区
	UINT64 inBuf[4] = { m_TargetPid,virtualAddr,8, writenVal };//输入缓冲区
	UINT64 outBuf = 0;//输出缓冲区
	DWORD dwRetSize = 0;
	DeviceIoControl(
		DeviceHandle,//CreateFile打开驱动设备 返回的句柄
		IOCTL_物理地址映射写数据,//控制码 CTL_CODE
		&inBuf,//输入缓冲区指针
		sizeof(inBuf),//输入缓冲区大小
		&outBuf,//返回缓冲区
		sizeof(outBuf),//返回缓冲区大小
		&dwRetSize, //返回字节数
		NULL);
	char buf[64] = { 0 };
	sprintf_s(buf, "exe: dwRetSize:%lld, pid:%d, va:%p, value:%llX", dwRetSize, m_TargetPid, virtualAddr, outBuf);
	OutputDebugStringA(buf);
}


void CTestDriverDlg::OnBnClickedButtonPhWriteReadonly5()
{
	UpdateData(TRUE);
	if (m_PID == 0)
	{
		MessageBoxA(m_hWnd, 0, "请输入被读取进程的pid", 0);
		return;
	}

	UINT64 virtualAddr = 0x7FF69B468BE8; // Ce中添加 xxx.exe即可查询模块基址
	UINT64 inBuf[4] = { m_TargetPid,virtualAddr,8, 0x13 };//输入缓冲区
	UINT64 outBuf = 0;//输出缓冲区
	DWORD dwRetSize = 0;
	DeviceIoControl(
		DeviceHandle,//CreateFile打开驱动设备 返回的句柄
		IOCTL_物理地址映射写数据,//控制码 CTL_CODE
		&inBuf,//输入缓冲区指针
		sizeof(inBuf),//输入缓冲区大小
		&outBuf,//返回缓冲区
		sizeof(outBuf),//返回缓冲区大小
		&dwRetSize, //返回字节数
		NULL);
	char buf[64] = { 0 };
	sprintf_s(buf, "exe: dwRetSize:%lld, pid:%d, va:%p, value:%llX", dwRetSize, m_TargetPid, virtualAddr, outBuf);
	OutputDebugStringA(buf);
}


void CTestDriverDlg::OnBnClickedButtonEnumObCallbacls()
{

	UINT64 virtualAddr = 0x7FF69B468BE8; // Ce中添加 xxx.exe即可查询模块基址
	UINT64 inBuf[4] = { m_TargetPid,virtualAddr,8, 0x13 };//输入缓冲区
	UINT64 outBuf = 0;//输出缓冲区
	DWORD dwRetSize = 0;
	DeviceIoControl(
		DeviceHandle,//CreateFile打开驱动设备 返回的句柄
		IOCTL_Enum对象回调,//控制码 CTL_CODE
		&inBuf,//输入缓冲区指针
		sizeof(inBuf),//输入缓冲区大小
		&outBuf,//返回缓冲区
		sizeof(outBuf),//返回缓冲区大小
		&dwRetSize, //返回字节数
		NULL);

}


void CTestDriverDlg::OnBnClickedButtonRemovePrivilege()
{
	UpdateData(TRUE);
	if (m_Pid_Privilege == 0)
	{
		MessageBoxA(m_hWnd, 0, "请输入被提权进程的pid", 0);
		return;
	}

	UINT64 virtualAddr = 0x7FF69B468BE8; // Ce中添加 xxx.exe即可查询模块基址
	UINT64 inBuf[4] = { m_Pid_Privilege,virtualAddr,8, 0x13 };//输入缓冲区
	UINT64 outBuf = 0;//输出缓冲区
	DWORD dwRetSize = 0;
	DeviceIoControl(
		DeviceHandle,//CreateFile打开驱动设备 返回的句柄
		IOCTL_移除指定pid提权,//控制码 CTL_CODE
		&inBuf,//输入缓冲区指针
		sizeof(inBuf),//输入缓冲区大小
		&outBuf,//返回缓冲区
		sizeof(outBuf),//返回缓冲区大小
		&dwRetSize, //返回字节数
		NULL);
	char buf[64] = { 0 };
	sprintf_s(buf, "exe: dwRetSize:%lld, pid:%d, va:%p, value:%llX", dwRetSize, m_TargetPid, virtualAddr, outBuf);
	OutputDebugStringA(buf);
}


void CTestDriverDlg::OnBnClickedButtonAddPrivilege()
{
	UpdateData(TRUE);
	if (m_Pid_Privilege == 0)
	{
		MessageBoxA(m_hWnd, 0, "请输入被提权进程的pid", 0);
		return;
	}

	UINT64 virtualAddr = 0x7FF69B468BE8; // Ce中添加 xxx.exe即可查询模块基址
	UINT64 inBuf[4] = { m_Pid_Privilege,virtualAddr,8, 0x13 };//输入缓冲区
	UINT64 outBuf = 0;//输出缓冲区
	DWORD dwRetSize = 0;
	DeviceIoControl(
		DeviceHandle,//CreateFile打开驱动设备 返回的句柄
		IOCTL_给指定pid提权,//控制码 CTL_CODE
		&inBuf,//输入缓冲区指针
		sizeof(inBuf),//输入缓冲区大小
		&outBuf,//返回缓冲区
		sizeof(outBuf),//返回缓冲区大小
		&dwRetSize, //返回字节数
		NULL);
	char buf[64] = { 0 };
	sprintf_s(buf, "exe: dwRetSize:%lld, pid:%d, va:%p, value:%llX", dwRetSize, m_TargetPid, virtualAddr, outBuf);
	OutputDebugStringA(buf);
}


//进程句柄转PID
DWORD HandleToPid(IN HANDLE hProcess)
{
	p_NtQueryInformationProcess NtQueryInformationProcess = (p_NtQueryInformationProcess)GetProcAddress(
		GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");

	PROCESS_BASIC_INFORMATION pbi = { 0 }; //NTDEFS::SYSTEM_BASIC_INFORMATION 	NTDEFS::

	//NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(_PROCESS_BASIC_INFORMATION), 0);
	//返回0表示成功
	NTSTATUS status = NtQueryInformationProcess(hProcess,
		ProcessBasicInformation,
		(PVOID)&pbi,
		sizeof(PROCESS_BASIC_INFORMATION),
		NULL);

	if (!status)
	{
		return (DWORD)pbi.UniqueProcessId;
	}

	return 0;
}



void CTestDriverDlg::OnBnClickedButtonR3EnumHandles()
{

	p_NtQuerySystemInformation NtQuerySystemInformation = (p_NtQuerySystemInformation)GetProcAddress(
		GetModuleHandleA("ntdll.dll"), "NtQuerySystemInformation");

	p_NtQueryObject NtQueryObject = (p_NtQueryObject)GetProcAddress(
		GetModuleHandleA("ntdll.dll"), "NtQueryObject");
	NTSTATUS status;

	///////// 方式1
	// 第一次查询
	SYSTEM_HANDLE_INFORMATION firstInfo = { 0 };
	ULONG totalLen = 0;
	NtQuerySystemInformation(SystemHandleInformation, &firstInfo, sizeof(SYSTEM_HANDLE_INFORMATION), &totalLen);
	PVOID buffer = malloc(totalLen);
	NtQuerySystemInformation(SystemHandleInformation, buffer, totalLen, NULL);


	///////// 方式2
	//// 循环申请内存
	//ULONG bufferSize = 0x8000;
	//PVOID buffer = malloc(bufferSize);
	//while ((status = NtQuerySystemInformation(
	//	SystemHandleInformation,
	//	buffer,
	//	bufferSize,
	//	NULL
	//)) == STATUS_INFO_LENGTH_MISMATCH)
	//{
	//	bufferSize *= 2;
	//	buffer = realloc(buffer, bufferSize);
	//	printf("realloc\n");
	//}


	OBJECT_NAME_INFORMATION* szName = (OBJECT_NAME_INFORMATION*)malloc(1000);
	OBJECT_NAME_INFORMATION* szType = (OBJECT_NAME_INFORMATION*)malloc(1000);

	PSYSTEM_HANDLE_INFORMATION info = (PSYSTEM_HANDLE_INFORMATION)buffer;
	ULONG handleCount = info->HandleCount;
	for (size_t i = 0; i < handleCount; i++)
	{
		if (info->Handles[i].UniqueProcessId == 23624)
		{
			HANDLE newHandle = NULL;
			DWORD dwFlags1 = 0;
			DWORD dwFlags2 = 0;

			SYSTEM_HANDLE cur = info->Handles[i];
			DuplicateHandle(OpenProcess(PROCESS_ALL_ACCESS, FALSE, cur.UniqueProcessId),
				(HANDLE)cur.Handle,
				GetCurrentProcess(),
				&newHandle,
				DUPLICATE_SAME_ACCESS,
				FALSE,
				DUPLICATE_SAME_ACCESS);
			if (newHandle)
			{
				NTSTATUS status1 = NtQueryObject(newHandle, ObjectNameInformation, szName, 512, &dwFlags1);
				NTSTATUS status2 = NtQueryObject(newHandle, ObjectTypeInformation, szType, 128, &dwFlags2);

				if (StrCmpW(szType->Name.Buffer, L"Process") == 0)
				{
					
					DWORD targetPid =  HandleToPid(newHandle);
					
					// 目标pid名称 
					char tempProcName[MAX_PATH] = { 0 };
					//GetModuleBaseNameA(newHandle, NULL, tempProcName, MAX_PATH); // xxx.exe
					GetModuleFileNameExA(newHandle, NULL, tempProcName, MAX_PATH); // C:/xxx.exe
					//GetProcessImageFileNameA(newHandle,  tempProcName, MAX_PATH); // \Device\HarddiskVolume6\xxx.exe
					
					printf("pid:%d, handle:%X, 权限:%X, targetPid:%d, targetName:%s  ",
						info->Handles[i].UniqueProcessId,
						info->Handles[i].Handle,
						info->Handles[i].GrantedAccess,
						targetPid, tempProcName
						);
					wprintf(L"%ws -- %ws\n",  szType->Name.Buffer, szName->Name.Buffer);
				}
		
			}
			
		}

	}

	// Ex 版本
	/*=
	SYSTEM_HANDLE_INFORMATION_EX handleInfo = {0};
	ULONG retLength = 0;
	DWORD ret = 0;


	ULONG bufferSize = 0x8000;
	PVOID buffer = malloc(bufferSize);

	while ((status = NtQuerySystemInformation(
		SystemExtendedHandleInformation,
		buffer,
		bufferSize,
		NULL
	)) == STATUS_INFO_LENGTH_MISMATCH)
	{
		bufferSize *= 2;
		buffer = realloc(buffer, bufferSize);
		printf("realloc\n");
	}

	PSYSTEM_HANDLE_INFORMATION_EX info = (PSYSTEM_HANDLE_INFORMATION_EX)buffer;
	ULONG handleCount = info->NumberOfHandles;

	for (size_t i = 0; i < handleCount; i++)
	{
		
		if (info->Handles[i].UniqueProcessId == 57184)
		{
			printf("pid:%d , handle:%X\n",
				info->Handles[i].UniqueProcessId , info->Handles[i].HandleValue);
		}

	}

	*/


	free(info);


}


void CTestDriverDlg::OnBnClickedButtonR3Ntqueryinformationprocess()
{
	p_NtQueryInformationProcess NtQueryInformationProcess = (p_NtQueryInformationProcess)GetProcAddress(
		GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");

	DWORD pid = 57184;
	pid = 13460;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess)
	{
		printf("打开进程句柄失败\n");
		return;
	}
	UINT64 ret = 0;
	ULONG realLenth = 0;
	////////// 基本信息
	PROCESS_BASIC_INFORMATION pbi = { 0 }; 
	NtQueryInformationProcess(hProcess,
		ProcessBasicInformation,
		(PVOID)&pbi,
		sizeof(PROCESS_BASIC_INFORMATION),
		NULL);
	printf("pid:%d, parent:%d, PPEB:%llX\n", 
			pbi.UniqueProcessId,
			pbi.InheritedFromUniqueProcessId,
			pbi.PebBaseAddress
			);

	// 查询名字系列 ProcessImageFileName  ProcessImageFileNameWin32
	UNICODE_STRING cmdLine = {0};

	DWORD maxBuffSize = 0x100000;
	PVOID buffer = malloc(0x100000);
	printf("sizeof(UNICODE_STRING):%d\n", sizeof(PROCESS_BASIC_INFORMATION));
	ULONG handleCount = 0;
	ret = NtQueryInformationProcess(hProcess,
		ProcessImageFileName,
		(PVOID)buffer,
		200,
		&realLenth);
	wprintf(L"cmdLine:%d ret:%lld %llx , [%ws]\n", handleCount, realLenth, ret, (WCHAR*)((ULONG64)buffer+0x10));
	printf("========\n");



	printf("sizeof(PROCESS_BASIC_INFORMATION):%d\n", sizeof(PROCESS_BASIC_INFORMATION));
	PROCESS_BASIC_INFORMATION pbi2 = { 0 };
	//memset(buffer, 0, maxBuffSize);
	realLenth = 0;
	INT64 IsDebugged = 0;
	ret = NtQueryInformationProcess(hProcess,
		ProcessDebugPort,
		(PVOID)&IsDebugged,
		//154,
		sizeof(INT64),
		&realLenth);
	wprintf(L"ret:%lld %llx isDebug:%llX ,realLenth: %lld\n",  ret, ret, IsDebugged, realLenth);
	printf("========\n");

	free(buffer);
}


void CTestDriverDlg::OnBnClickedButtonR3EnumSysteminfo()
{
	p_NtQuerySystemInformation NtQuerySystemInformation = (p_NtQuerySystemInformation)GetProcAddress(
		GetModuleHandleA("ntdll.dll"), "NtQuerySystemInformation");

	p_NtQueryObject NtQueryObject = (p_NtQueryObject)GetProcAddress(
		GetModuleHandleA("ntdll.dll"), "NtQueryObject");

	NTSTATUS status;

	ULONG realLen = 0;
	PVOID buffer = NULL;
	////////// 处理器信息
	SYSTEM_PROCESSOR_INFORMATION processorInfo = { 0 };
	status = NtQuerySystemInformation(SystemProcessorInformation, &processorInfo,
		sizeof(SYSTEM_PROCESSOR_INFORMATION),
		&realLen);
	printf("处理器信息, 第%d代CPU, Level:%d, Revision:%d, 核心数量:%d\n",
		processorInfo.ProcessorArchitecture,
		processorInfo.ProcessorLevel,
		processorInfo.ProcessorRevision,
		processorInfo.MaximumProcessors);


	// SystemPerformanceInformation
	SYSTEM_PERFORMANCE_INFORMATION performanceInfo = { 0 };
	status = NtQuerySystemInformation(SystemPerformanceInformation, &performanceInfo,
		sizeof(SYSTEM_PERFORMANCE_INFORMATION),
		&realLen); // realLen = 312
	printf("status:%X , realLen:%d\n", status, realLen);

	// SystemTimeOfDayInformation
	SYSTEM_TIMEOFDAY_INFORMATION timeOfDayInfo = { 0 };
	status = NtQuerySystemInformation(SystemTimeOfDayInformation, &timeOfDayInfo,
		sizeof(SYSTEM_TIMEOFDAY_INFORMATION),
		&realLen); // realLen = 48
	printf("status:%X , realLen:%d\n", status, realLen);

	// 获取系统进程的信息
	SYSTEM_PROCESS_INFORMATION processInfo = { 0 };
	status = NtQuerySystemInformation(SystemProcessInformation, &processInfo,
		sizeof(SYSTEM_PROCESS_INFORMATION),
		&realLen); 
	printf("status:%X , realLen:%d\n", status, realLen);
	if (status == STATUS_INFO_LENGTH_MISMATCH)
	{
		buffer = malloc(realLen);
		status = NtQuerySystemInformation(SystemProcessInformation, buffer,
			realLen,
			&realLen);
		PSYSTEM_PROCESS_INFORMATION pInfo = (PSYSTEM_PROCESS_INFORMATION)buffer;
		while (pInfo->NextEntryOffset)
		{
			wprintf(L"pid:%d, thread:%d, name:%ws\n",
				(DWORD)pInfo->UniqueProcessId,pInfo->NumberOfThreads, pInfo->ImageName.Buffer);

			pInfo = (PSYSTEM_PROCESS_INFORMATION)((UINT64)(pInfo)+pInfo->NextEntryOffset);
			// 线程信息
			//for (size_t i = 0; i < pInfo->NumberOfThreads; i++)
			//{
			//	printf("内核态时间:%lld, 优先级:%d, 线程状态:%d\n", 
			//		pInfo->Threads[i].KernelTime.QuadPart,
			//		pInfo->Threads[i].Priority,
			//		pInfo->Threads[i].ThreadState
			//		);
			//}
		}

		printf("\n");
	}

	return;
	// 设备信息
	SYSTEM_DEVICE_INFORMATION deviceInfo = { 0 };
	status = NtQuerySystemInformation(SystemDeviceInformation, &deviceInfo,
		sizeof(SYSTEM_DEVICE_INFORMATION),
		&realLen);
	printf("NumberOfDisks:%d,NumberOfSerialPorts:%d\n", 
		deviceInfo.NumberOfDisks,
		deviceInfo.NumberOfSerialPorts);

	// 驱动模块信息
	RTL_PROCESS_MODULES processModules = { 0 };
	status = NtQuerySystemInformation(SystemModuleInformation, &processModules,
		sizeof(RTL_PROCESS_MODULES),
		&realLen); // realLen = 48
	printf("status:%X , realLen:%d\n", status, realLen);
	if (status == STATUS_INFO_LENGTH_MISMATCH)
	{
		buffer = realloc(buffer,realLen);
		status = NtQuerySystemInformation(SystemModuleInformation, buffer,
			realLen,
			&realLen);
		PRTL_PROCESS_MODULES pInfo = (PRTL_PROCESS_MODULES)buffer;

		for (size_t i = 0; i < processModules.NumberOfModules; i++)
		{
			printf("ImageBase:%p size:%X, FullPathName:%s\n", 
				pInfo->Modules[i].ImageBase,
				pInfo->Modules[i].ImageSize,
				pInfo->Modules[i].FullPathName);
		}
	}


	printf("\n");












	if (buffer != NULL)
	{
		free(buffer);
	}
	printf("=====end=====\n");

}


void CTestDriverDlg::OnBnClickedButtonR3Open1000Handle()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	if (m_Pid_Privilege == 0)
	{
		MessageBoxA(m_hWnd, 0, "请输入被提权进程的pid", 0);
		return;
	}


	for (size_t i = 0; i < 1000; i++)
	{
		HANDLE hh = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_Pid_Privilege);

		printf("HANDLE:%x\n", hh);

	}
	printf("finisned\n");

}
