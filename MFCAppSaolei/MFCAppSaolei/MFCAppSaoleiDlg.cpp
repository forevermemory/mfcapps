
// MFCAppSaoleiDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCAppSaolei.h"
#include "MFCAppSaoleiDlg.h"
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


// CMFCAppSaoleiDlg 对话框



CMFCAppSaoleiDlg::CMFCAppSaoleiDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPSAOLEI_DIALOG, pParent)
	, m_remain(0)
	, m_Data(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCAppSaoleiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_remain);
	DDX_Text(pDX, IDC_L_DATA, m_Data);
}

BEGIN_MESSAGE_MAP(CMFCAppSaoleiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCAppSaoleiDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCAppSaoleiDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCAppSaoleiDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCAppSaoleiDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCAppSaoleiDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCAppSaoleiDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CMFCAppSaoleiDlg 消息处理程序

BOOL CMFCAppSaoleiDlg::OnInitDialog()
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
	InitCwndHandle();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCAppSaoleiDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCAppSaoleiDlg::OnPaint()
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
HCURSOR CMFCAppSaoleiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//初级 0x209
//中级 0x20a
//高级 0x20b
//自定义 0x20c

void CMFCAppSaoleiDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	::SendMessage(m_hwnd, WM_COMMAND, 0x209, 0);
	//::SendMessage(m_hwnd, WM_COMMAND, 0x251, 0);
}


void CMFCAppSaoleiDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	::SendMessage(m_hwnd, WM_COMMAND, 0x20a, 0);
}


void CMFCAppSaoleiDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	::SendMessage(m_hwnd, WM_COMMAND, 0x20b, 0);
}


void CMFCAppSaoleiDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD pid;
	HWND hwnd = ::FindWindow(NULL, L"扫雷");
	if (NULL == hwnd)
	{
		MessageBox(L"读取扫雷进程失败");
		return;
	}
	// 通过窗口句柄拿到进程id
	GetWindowThreadProcessId(hwnd, &pid);
	// 通过进程id拿到句柄
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	// 读取内存数据
	ReadProcessMemory(handle, (LPCVOID)0x1005194, &m_remain, sizeof(m_remain),NULL);
	UpdateData(FALSE);
}


void CMFCAppSaoleiDlg::InitCwndHandle()
{
	// TODO: 在此处添加实现代码.
	m_hwnd = ::FindWindow(NULL, L"扫雷");
	if (NULL == m_hwnd)
		MessageBox(L"读取扫雷进程失败");
	
}


void CMFCAppSaoleiDlg::OnBnClickedButton5()
{

	DWORD pid;
	HWND hwnd = ::FindWindow(NULL, L"扫雷");
	if (NULL == hwnd)
	{
		MessageBox(L"读取扫雷进程失败");
		return;
	}
	// 通过窗口句柄拿到进程id
	GetWindowThreadProcessId(hwnd, &pid);
	// 通过进程id拿到句柄
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	// 读取内存数据
	// 通过ce搜索 最终定位到
	/*雷区的基地址为 1005361
		宽度 10056AC
		高度 10056A8

		分析雷区数据意义
		0x8f代表雷
		0x10 结束标记*/
	// 获取高度
	DWORD dwHeight = 0;
	BOOL result;
    unsigned char gamedata[24][32] = { 0 };
	m_Data.Empty();
	result = ReadProcessMemory(handle, (LPCVOID)0x10056A8, &dwHeight, sizeof(dwHeight), NULL);
	result = ReadProcessMemory(handle, (LPCVOID)0x1005361, &gamedata, 24*32, NULL);

	if (!result)
	{
		MessageBox(L"ReadProcessMemory");
		return;
	}

	CString tmp;
	short gamex = 20;
	short gamey = 60;
	unsigned short pos[2] = { 0 }; // 放到lParam
	for (int i = 0; i < dwHeight; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			tmp.Empty();
			if (0x10 == gamedata[i][j])
				break;
			// 点击非雷区
			if (0x8f == gamedata[i][j])
			{
				pos[0] = gamex + j * 16;
				pos[1] = gamey + i * 16;
				::PostMessage(hwnd, WM_RBUTTONDOWN, MK_RBUTTON, *(int*)pos);
				::PostMessage(hwnd, WM_RBUTTONUP, 0, *(int*)pos);
			}
		
			tmp.Format(L"%02X ", gamedata[i][j]);
			m_Data += tmp;
		}
		m_Data += L"\r\n";
	}

	
	UpdateData(FALSE);
}


void CMFCAppSaoleiDlg::OnBnClickedButton6()
{
	DWORD pid;
	HWND hwnd = ::FindWindow(NULL, L"扫雷");
	if (NULL == hwnd)
	{
		MessageBox(L"读取扫雷进程失败");
		return;
	}
	// 通过窗口句柄拿到进程id
	GetWindowThreadProcessId(hwnd, &pid);
	// 通过进程id拿到句柄
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	// 读取内存数据
	// 通过ce搜索 最终定位到
	/*雷区的基地址为 1005361
		宽度 10056AC
		高度 10056A8

		分析雷区数据意义
		0x8f代表雷
		0x10 结束标记*/
		// 获取高度
	DWORD dwHeight = 0;
	BOOL result;
	unsigned char gamedata[24][32] = { 0 };
	m_Data.Empty();
	result = ReadProcessMemory(handle, (LPCVOID)0x10056A8, &dwHeight, sizeof(dwHeight), NULL);
	result = ReadProcessMemory(handle, (LPCVOID)0x1005361, &gamedata, 24 * 32, NULL);

	if (!result)
	{
		MessageBox(L"ReadProcessMemory");
		return;
	}

	CString tmp;
	short gamex = 20;
	short gamey = 60;
	unsigned short pos[2] = { 0 }; // 放到lParam
	for (int i = 0; i < dwHeight; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			tmp.Empty();
			if (0x10 == gamedata[i][j])
				break;
			// 点击非雷区
			if (0x8f != gamedata[i][j])
			{
				pos[0] = gamex + j * 16;
				pos[1] = gamey + i * 16;
				::PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, *(int*)pos);
				::PostMessage(hwnd, WM_LBUTTONUP, 0, *(int*)pos);
			}

			tmp.Format(L"%02X ", gamedata[i][j]);
			m_Data += tmp;
		}
		m_Data += L"\r\n";
	}


	UpdateData(FALSE);
}
