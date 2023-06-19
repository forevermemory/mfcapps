
// MFCllkDlg.cpp: 实现文件
//

/*
* 
	DWORD index;
	for (size_t i = 2; i < 18 + 4 - 2; i++)    // 行
	{
		for (size_t j = 2; j < 14 - 2; j++)  // 列
		{
			m_gamedataPtr[pos->x][pos->y] ; 当前
			m_gamedataPtr[pos->x + index][pos->y] ; 右边第n个
			m_gamedataPtr[pos->x - index][pos->y] ; 左边第n个
			m_gamedataPtr[pos->x][pos->y + index] ; 下边第n个
			m_gamedataPtr[pos->x][pos->y - index] ; 上边第n个
		}
	}

	即一列一列遍历

*/

#include "pch.h"
#include "framework.h"
#include "MFCllk.h"
#include "MFCllkDlg.h"
#include "afxdialogex.h"
#include "ui.h"
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_KIND 0xffffffff

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


// CMFCllkDlg 对话框



CMFCllkDlg::CMFCllkDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCLLK_DIALOG, pParent)
	, m_Data(_T(""))
	, m_Time(FALSE)
	, m_Tishi(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCllkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Data);
	DDX_Check(pDX, IDC_CHECK1, m_Time);
	DDX_Check(pDX, IDC_CHECK2, m_Tishi);
	DDX_Check(pDX, IDC_CHECK3, m_Life);
	
}

BEGIN_MESSAGE_MAP(CMFCllkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCllkDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCllkDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCllkDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCllkDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CMFCllkDlg::OnBnClickedButton7)
	//ON_BN_CLICKED(IDC_BUTTON8, &CMFCllkDlg::InitGameData)
	ON_BN_CLICKED(IDC_BUTTON8, &CMFCllkDlg::cwllk)
	ON_BN_CLICKED(IDC_BUTTON9, &CMFCllkDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CMFCllkDlg::OnBnClickedButtonxian0)
	ON_BN_CLICKED(IDC_CHECK1, &CMFCllkDlg::OnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CMFCllkDlg::OnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON11, &CMFCllkDlg::OnBnClickedButtonxian1)
	ON_BN_CLICKED(IDC_BUTTON_zhexian2, &CMFCllkDlg::OnBnClickedButtonzhexian2)
	ON_BN_CLICKED(IDC_BUTTON_zhexian3, &CMFCllkDlg::OnBnClickedButtonzhexian3)
	ON_BN_CLICKED(IDC_CHECK3, &CMFCllkDlg::OnBnClickedCheck3)
END_MESSAGE_MAP()


// CMFCllkDlg 消息处理程序

BOOL CMFCllkDlg::OnInitDialog()
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
	PrintUI("我的程序.exe", "MYWINDOW", "color 9", 900, 100, 600, 700);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCllkDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCllkDlg::OnPaint()
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
HCURSOR CMFCllkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCllkDlg::cwllk()
{
	OnBnClickedButtonzhexian3();
}

void CMFCllkDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	//RegisterClassW
	HWND hwnd = ::FindWindowA(NULL, "连连看 v4.1");
	if (NULL == hwnd)
	{
		MessageBoxA("未找到连连看进程");
		return;
	}

	::SendMessage(hwnd, WM_COMMAND, 32772, 0);

}


void CMFCllkDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	HWND hwnd = ::FindWindowA(NULL, "连连看 v4.1");
	if (NULL == hwnd)
	{
		MessageBoxA("未找到连连看进程");
		return;
	}

	::SendMessage(hwnd, WM_COMMAND, 32773, 0);
}


void CMFCllkDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	HWND hwnd = ::FindWindowA(NULL, "连连看 v4.1");
	if (NULL == hwnd)
	{
		MessageBoxA("未找到连连看进程");
		return;
	}

	::SendMessage(hwnd, WM_COMMAND, 32774, 0);
}


void CMFCllkDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	HWND hwnd = ::FindWindowA(NULL, "连连看 v4.1");
	if (NULL == hwnd)
	{
		MessageBoxA("未找到连连看进程");
		return;
	}

	::SendMessage(hwnd, WM_COMMAND, 32775, 0);

}


void CMFCllkDlg::OnBnClickedButton9()
{
	HWND hwnd = ::FindWindowA(NULL, "连连看 v4.1");
	if (NULL == hwnd)
	{
		MessageBoxA("未找到连连看进程");
		return;
	}

	::SendMessage(hwnd, WM_COMMAND, 32784, 0);
}



void CMFCllkDlg::OnBnClickedButton7()
{
	DWORD pid;
	HWND hwnd = ::FindWindowA(NULL, "连连看 v4.1");
	if (NULL == hwnd)
	{
		MessageBoxA("未找到连连看进程");
		return;
	}
	// 通过窗口句柄拿到进程id
	GetWindowThreadProcessId(hwnd, &pid);
	// 通过进程id拿到句柄
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);



	// [[4499A4] + AC0]
	DWORD dwTemp = 0;
	BOOL result;
	result = ReadProcessMemory(handle, (LPCVOID)0x4499A4, &dwTemp, sizeof(dwTemp), NULL);
	if (!result)
	{
		MessageBoxA("ReadProcessMemory");
		return;
	}
	// 设置
	DWORD dwShun = 9999;
	dwTemp += 0xABC;
	result = ::WriteProcessMemory(handle, (LPVOID)dwTemp, &dwShun, sizeof(DWORD), NULL);
	if (!result)
	{
		MessageBoxA("WriteProcessMemory");
		return;
	}

	CloseHandle(handle);
}


void CMFCllkDlg::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码

	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	DWORD pid;
	HWND hwnd = ::FindWindowA(NULL, "连连看 v4.1");
	if (NULL == hwnd)
	{
		MessageBoxA("未找到连连看进程");
		return;
	}
	// 通过窗口句柄拿到进程id
	GetWindowThreadProcessId(hwnd, &pid);
	// 通过进程id拿到句柄
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	//	//0040652B - 48 - dec eax
	//0040652B   90  nop

	// 通过窗口句柄拿到进程id
	

	// [[4499A4] + AC0]
	DWORD dwTemp = 0;
	BOOL result;
	result = ReadProcessMemory(handle, (LPCVOID)0x4499A4, &dwTemp, sizeof(dwTemp), NULL);
	if (!result)
	{
		MessageBoxA("ReadProcessMemory");
		return;
	}
	// 设置
	DWORD dwShun = 9999;
	dwTemp += 0xABC;
	result = ::WriteProcessMemory(handle, (LPVOID)dwTemp, &dwShun, sizeof(DWORD), NULL);
	if (!result)
	{
		MessageBoxA("WriteProcessMemory");
		return;
	}

	char buf[1] = { 0 };
	if (m_Time)
	{
		buf[0] = 0x90;
	}
	else
	{
		buf[0] = 0x48;
	}
	 result = ::WriteProcessMemory(handle, (LPVOID)0x40652B, &buf, sizeof(buf), NULL);
	if (!result)
	{
		MessageBoxA("WriteProcessMemory");
		return;
	}
}

void CMFCllkDlg::OnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	DWORD pid;
	HWND hwnd = ::FindWindowA(NULL, "连连看 v4.1");
	if (NULL == hwnd)
	{
		MessageBoxA("未找到连连看进程");
		return;
	}
	// 通过窗口句柄拿到进程id
	GetWindowThreadProcessId(hwnd, &pid);
	// 通过进程id拿到句柄
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	//4057B5   48  dec eax
	//4057B5   90  nop

	char buf[1] = { 0 };
	if (m_Time)
	{
		buf[0] = 0x90;
	}
	else
	{
		buf[0] = 0x48;
	}
	BOOL result = ::WriteProcessMemory(handle, (LPVOID)0x4057B5, &buf, sizeof(buf), NULL);
	if (!result)
	{
		MessageBoxA("WriteProcessMemory");
		return;
	}


	//00405033 - 89 AE BC0A0000 - mov[esi + 00000ABC], ebp <<
	char buf2[6] = { 0 };
	if (m_Time)
	{
		memset(buf2, 0x90, 6);
	}
	else
	{
		buf[0] = 0x89;
		buf[1] = 0xae;
		buf[2] = 0xbc;
		buf[3] = 0x0a;
		buf[4] = 0;
		buf[5] = 0;
	}
	 result = ::WriteProcessMemory(handle, (LPVOID)0x4057B5, &buf, sizeof(buf), NULL);
	if (!result)
	{
		MessageBoxA("WriteProcessMemory");
		return;
	}
}


void CMFCllkDlg::OnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	DWORD pid;
	HWND hwnd = ::FindWindowA(NULL, "连连看 v4.1");
	if (NULL == hwnd)
	{
		MessageBoxA("未找到连连看进程");
		return;
	}
	// 通过窗口句柄拿到进程id
	GetWindowThreadProcessId(hwnd, &pid);
	// 通过进程id拿到句柄
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	//4057B5   48  dec eax
	//4057B5   90  nop

	char buf[1] = { 0 };
	if (m_Tishi)
	{
		buf[0] = 0x90;
	}
	else
	{
		buf[0] = 0x48;
	}
	BOOL result = ::WriteProcessMemory(handle, (LPVOID)0x40606E, &buf, sizeof(buf), NULL);
	if (!result)
	{
		MessageBoxA("WriteProcessMemory");
		return;
	}
}

void CMFCllkDlg::UpdateText()
{
	CString tmp;
	m_Data = "";
	for (size_t i = 0; i < 18 + 4; i++)
	{
		for (size_t j = 0; j < 14; j++)
		{
			DWORD k = m_gamedataPtr[i][j]->val;
			switch (k)
			{
			case 0:
				tmp = "00 ";
				break;
			case MAX_KIND:
				tmp = "ff ";
				break;
			default:
				tmp.Format("%02x ", k);
			}
			m_Data += tmp;
		}
		m_Data += "\r\n";
	}

	//m_Data += "ff 00 00 00 00 00 00 00 00 00 00 00 00 ff  \r\n";
	//m_Data += "ff ff ff ff ff ff ff ff ff ff ff ff ff ff \r\n";
	UpdateData(FALSE);
}

void CMFCllkDlg::InitGameData()
{
	// TODO: 在此添加控件通知处理程序代码
	//DWORD level = GetGameLevel();
	//CString str;
	//str.Format("%d", level);
	//AfxMessageBox(str);
	//return;
	DWORD pid;
	HWND hwnd = ::FindWindowA(NULL, "连连看 v4.1");
	if (NULL == hwnd)
	{
		MessageBoxA("未找到连连看进程");
		return;
	}
	// 通过窗口句柄拿到进程id
	GetWindowThreadProcessId(hwnd, &pid);
	// 通过进程id拿到句柄
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);



	// [[4499A4] + AC0]  
	// 14列  18行
	DWORD lie = 18;
	DWORD hang = 14;

	DWORD dwTemp = 0;
	BOOL result;
	result = ReadProcessMemory(handle, (LPCVOID)0x4499A4, &dwTemp, sizeof(dwTemp), NULL);
	if (!result)
	{
		MessageBoxA("ReadProcessMemory");
		return;
	}

	dwTemp += 0x650;
	result = ReadProcessMemory(handle, (LPCVOID)dwTemp, &m_gamedata, lie * hang * 4, NULL);
	if (!result)
	{
		MessageBoxA("ReadProcessMemory 2 ");
		return;
	}

	memset(m_gamedata2, 0, sizeof(m_gamedata2));
	for (size_t i = 0; i < hang; i++)
	{
		m_gamedata2[0][i] = MAX_KIND;
	}
	m_gamedata2[1][0] = MAX_KIND;
	m_gamedata2[1][13] = MAX_KIND;
	m_gamedata2[20][0] = MAX_KIND;
	m_gamedata2[20][13] = MAX_KIND;
	for (size_t i = 0; i < hang; i++)
	{
		m_gamedata2[21][i] = MAX_KIND;
	}

	//m_Data += "ff ff ff ff ff ff ff ff ff ff ff ff ff ff \r\n";
	//m_Data += "ff 00 00 00 00 00 00 00 00 00 00 00 00 ff \r\n";

	
	CString tmp;
	for (size_t i = 0; i < lie; i++)
	{
		for (size_t j = 0; j < hang; j++)
		{
			DWORD k = m_gamedata[i][j];
			m_gamedata2[i+2][j] = k;
		}
	}

	for (size_t i = 0; i < 18+4; i++)
	{
		for (size_t j = 0; j < 14; j++)
		{
			DWORD k = m_gamedata2[i][j];
			// 放到内存中 
			PEle e = new Ele();
			e->x = i;
			e->y = j;
			e->val = k;
			e->visited = FALSE;
			m_gamedataPtr[i][j] = e;
		
		}
	}


}


void CMFCllkDlg::resetVisited()
{
	for (size_t i = 0; i < 18 + 4; i++)
	{
		for (size_t j = 0; j < 14; j++)
		{
			m_gamedataPtr[i][j]->visited = FALSE;
		}
	}
}


PEle CMFCllkDlg::FindOneTop(PEle cur, PEle pos)
{
	DWORD MAX = 3;
	PEle  right, down, left, top = 0;

	// 进入: 一折 二折 三折

	// 上 
	DWORD index = 1;
	while (true)
	{
		top = m_gamedataPtr[pos->x][pos->y - index++];
		if (top->val == MAX_KIND)
		{
			break;
		}
		if (top->val == 0)
		{
			continue;
		}
		if (cur == top)
		{
			continue;
		}
		if (top->val == cur->val)
		{
			return top;
		}
		// 此时非0 而且和目标值不相等
		break;
	}

	return NULL;
}


PEle CMFCllkDlg::FindOneDown(PEle cur, PEle pos)
{
	DWORD MAX = 3;
	PEle  right, down, left, top = 0;

	// 进入: 一折 二折 三折
	// 右 
	DWORD index = 1;
	
	// 下 
	index = 1;
	while (true)
	{
		down = m_gamedataPtr[pos->x][pos->y + index++];
		if (down->val == MAX_KIND)
		{
			break;
		}
		if (down->val == 0)
		{
			continue;
		}
		if (cur == down)
		{
			continue;
		}
		if (down->val == cur->val)
		{
			return down;
		}
		// 此时非0 而且和目标值不相等
		break;
	}

	return NULL;
}


PEle CMFCllkDlg::FindOneLeft(PEle cur, PEle pos)
{
	DWORD MAX = 3;
	PEle  right, down, left, top = 0;

	// 进入: 一折 二折 三折
	// 右 
	DWORD index = 1;
	

	// 左 
	index = 1;
	while (true)
	{
		left = m_gamedataPtr[pos->x - index++][pos->y];
		if (left->val == MAX_KIND)
		{
			break;
		}
		if (left->val == 0)
		{
			continue;
		}
		if (cur == left)
		{
			continue;
		}
		if (left->val == cur->val)
		{
			return left;
		}
		// 此时非0 而且和目标值不相等
		break;
	}

	return NULL;
}


PEle CMFCllkDlg::FindOneRight(PEle cur, PEle pos)
{
	DWORD MAX = 3;
	PEle  right, down, left, top = 0;

	// 进入: 一折 二折 三折
	// 右 
	DWORD index = 1;
	while (true)
	{
		right = m_gamedataPtr[pos->x + index++][pos->y];
		if (right->val == MAX_KIND)
		{
			break;
		}
		if (right->val == 0)
		{
			continue;
		}
		if (cur == right)
		{
			continue;
		}
		if (right->val == cur->val)
		{
			return right;
		}
		// 此时非0 而且和目标值不相等
		break;
	}

	return NULL;
}


PEle CMFCllkDlg::FindTwoLeftTopDown(PEle cur, PEle pos)
{
	DWORD MAX = 3;
	PEle  right, down, left, top, mfind = 0;

	// 进入: 二折 三折


	// top 
	DWORD index = 0;
	while (true)
	{
		index++;
		mfind = m_gamedataPtr[pos->x][pos->y - index];
		if (cur == mfind)
		{
			continue;
		}
		if (mfind->val == cur->val)
		{
			return mfind;
		}
		if (mfind->val == MAX_KIND)
		{
			break;
		}

		if (mfind->val == 0)
		{


			// 上和下到头 
			PEle find = FindOneLeft(cur, mfind);
			if (find == 0)
			{
				find = FindOneRight(cur, mfind);
			}
			//printf("find: %d %d [%02x], %d %d [%02x] %x\n", cur->x, cur->y, cur->val,
			//	top->x, top->y, top->val, find);
			if (find && find->val == cur->val)
			{
				return find;
			}
			continue;
		}

		// 此时非0 而且和目标值不相等
		break;
	}


	// down 
	index = 0;
	while (true)
	{
		index++;
		mfind = m_gamedataPtr[pos->x][pos->y + index];
		if (cur == mfind)
		{
			continue;
		}
		if (mfind->val == cur->val)
		{
			return mfind;
		}
		if (mfind->val == MAX_KIND)
		{
			break;
		}

		if (mfind->val == 0)
		{


			// 上和下到头 
			PEle find = FindOneLeft(cur, mfind);
			if (find == 0)
			{
				find = FindOneRight(cur, mfind);
			}
			//printf("find: %d %d [%02x], %d %d [%02x] %x\n", cur->x, cur->y, cur->val,
			//	top->x, top->y, top->val, find);
			if (find && find->val == cur->val)
			{
				return find;
			}
			continue;
		}

		// 此时非0 而且和目标值不相等
		break;
	}
}

PEle CMFCllkDlg::FindTwoRightTopDown(PEle cur, PEle pos)
{
	DWORD MAX = 3;
	PEle  right, down, left, top, mfind = 0;

	// 进入: 二折 三折
	// top 
	DWORD index = 0;
	while (true)
	{
		index++;
		mfind = m_gamedataPtr[pos->x][pos->y - index];
		if (cur == mfind)
		{
			continue;
		}
		if (mfind->val == cur->val)
		{
			return mfind;
		}
		if (mfind->val == MAX_KIND)
		{
			break;
		}

		if (mfind->val == 0)
		{


			PEle find = FindOneLeft(cur, mfind);
			if (find == 0)
			{
				find = FindOneRight(cur, mfind);
			}
			//printf("find: %d %d [%02x], %d %d [%02x] %x\n", cur->x, cur->y, cur->val,
			//	top->x, top->y, top->val, find);
			if (find && find->val == cur->val)
			{
				return find;
			}
			continue;
		}

		// 此时非0 而且和目标值不相等
		break;
	}

	// down 
	index = 0;
	while (true)
	{
		index++;
		mfind = m_gamedataPtr[pos->x][pos->y + index];
		if (cur == mfind)
		{
			continue;
		}
		if (mfind->val == cur->val)
		{
			return mfind;
		}
		if (mfind->val == MAX_KIND)
		{
			break;
		}

		if (mfind->val == 0)
		{


			PEle find = FindOneLeft(cur, mfind);
			if (find == 0)
			{
				find = FindOneRight(cur, mfind);
			}
			//printf("find: %d %d [%02x], %d %d [%02x] %x\n", cur->x, cur->y, cur->val,
			//	top->x, top->y, top->val, find);
			if (find)
			{
				return find;
			}
			continue;
		}

		// 此时非0 而且和目标值不相等
		break;
	}
}


PEle CMFCllkDlg::FindTwoTopLeftRight(PEle cur, PEle pos)
{
	DWORD MAX = 3;
	PEle  right, down, left, top, mfind = 0;

	// 进入: 二折 三折
	// 左 
	DWORD index = 0;
	while (true)
	{
		index++;
		mfind = m_gamedataPtr[pos->x - index][pos->y];
		if (cur == mfind)
		{
			continue;
		}
		if (mfind->val == cur->val)
		{
			return mfind;
		}
		if (mfind->val == MAX_KIND)
		{
			break;
		}

		if (mfind->val == 0)
		{

			// left和右到头 
			PEle find = FindOneTop(cur, mfind);
			if (find == 0)
			{
				find = FindOneDown(cur, mfind);
			}
			//printf("find: %d %d [%02x], %d %d [%02x] %x\n", cur->x, cur->y, cur->val,
			if (find && find->val == cur->val)
			{
				return find;
			}
			continue;
		}

		// 此时非0 而且和目标值不相等
		break;
	}

	// 右 
	index = 0;
	while (true)
	{
		index++;
		mfind = m_gamedataPtr[pos->x + index][pos->y];
		if (cur == mfind)
		{
			continue;
		}
		if (mfind->val == cur->val)
		{
			return mfind;
		}
		if (mfind->val == MAX_KIND)
		{
			break;
		}
		//printf("\t\t\t right find:cur %d %d [%02x], %d %d [%02x] %x\n",
		//	cur->x, cur->y, cur->val, mfind->x, mfind->y, mfind->val);
		if (mfind->val == 0)
		{

			PEle find = FindOneTop(cur, mfind);
			if (find == 0)
			{
				find = FindOneDown(cur, mfind);
			}
			//printf("find: %d %d [%02x], %d %d [%02x] %x\n", cur->x, cur->y, cur->val,
			if (find)
			{
				return find;
			}
			continue;
		}

		// 此时非0 而且和目标值不相等
		break;
	}
	return NULL;
}





PEle CMFCllkDlg::FindTwoDownLeftRight(PEle cur, PEle pos)
{
	DWORD MAX = 3;
	PEle  right, down, left, top, mfind = 0;

	// 进入: 二折 三折

	// left 
	DWORD index = 0;
	while (true)
	{
		index++;
		mfind = m_gamedataPtr[pos->x - index][pos->y];

		if (cur == mfind)
		{
			continue;
		}
		if (mfind->val == cur->val)
		{
			return mfind;
		}
		if (mfind->val == MAX_KIND)
		{
			break;
		}

		if (mfind->val == 0)
		{
			// left和右到头 
			PEle find = FindOneTop(cur, mfind);
			if (find == 0)
			{
				find = FindOneDown(cur, mfind);
			}

			if (find && find->val == cur->val)
			{
				return find;
			}
			continue;
		}

		// 此时非0 而且和目标值不相等
		break;
	}

	// right 
	index = 0;
	while (true)
	{
		index++;
		mfind = m_gamedataPtr[pos->x + index][pos->y];

		//printf("\t\t down find %d: %d %d [%02x], %d %d [%02x] %x\n", 
		//	index, cur->x, cur->y, cur->val,
		//	mfind->x, mfind->y, mfind->val, mfind);


		if (cur == mfind)
		{
			continue;
		}
		if (mfind->val == cur->val)
		{
			//printf("\t\t\t mfind1 %d %d\n", mfind->x , mfind->y);
			return mfind;
		}
		if (mfind->val == MAX_KIND)
		{
			break;
		}

		if (mfind->val == 0)
		{
			// left和右到头 
			PEle find = FindOneTop(cur, mfind);
			if (find == 0)
			{
				find = FindOneDown(cur, mfind);
			}

			if (find)
			{
				//printf("\t\t\t mfind2  %d %d\n", find->x, find->y);
				return find;
			}
			continue;
		}

		// 此时非0 而且和目标值不相等
		break;
	}
	return NULL;
}



void CMFCllkDlg::SendClickMode2(PEle a, PEle b, DWORD mode)
{
	SendClick(a->x, a->y, 0);
	Sleep(50);
	SendClick(b->x, b->y, 0);
	a->val = 0;
	b->val = 0;

	printf("click: %d %d %02x, %d %d %02x\n",
		a->x, a->y, a->val,
		b->x,b->y,b->val
		);
	
}

DWORD CMFCllkDlg::GetGameLevel()
{
	DWORD pid;

	HWND hwnd = ::FindWindowA(NULL, "连连看 v4.1");
	if (NULL == hwnd)
	{
		MessageBoxA("未找到连连看进程");
		return -1;
	}
	// 通过窗口句柄拿到进程id
	GetWindowThreadProcessId(hwnd, &pid);
	// 通过进程id拿到句柄
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);



	// [[4499A4] + AC0]
	DWORD dwTemp = 0;
	BOOL result;
	result = ReadProcessMemory(handle, (LPCVOID)0x4499A4, &dwTemp, sizeof(dwTemp), NULL);
	if (!result)
	{
		MessageBoxA("ReadProcessMemory");
		return -1;
	}
	// 设置
	DWORD dwShun = 0;
	dwTemp += 0xAB4;
	result = ReadProcessMemory(handle, (LPVOID)dwTemp, &dwShun, sizeof(dwShun), NULL);
	if (!result)
	{
		MessageBoxA("WriteProcessMemory");
		return -1 ;
	}

	return dwShun;
}


void CMFCllkDlg::SendClick(short x, short y, DWORD mode)
{
	HWND hwnd = ::FindWindowA(NULL, "连连看 v4.1");
	if (NULL == hwnd)
	{
		MessageBoxA("未找到连连看进程");
		return;
	}

	// 初级 162 + 162 

	DWORD level = GetGameLevel();
	DWORD X = 0;
	DWORD Y = 0;
	switch (level)
	{
	case 1:
		X = 160;
		Y = 115;
		break;
	case 2:
		X = 120;
		Y = 88;
		break;
	case 3:
		X = 80;
		Y = 68;
		break;
	case 4:
		X = 40;
		Y = 15;
		break;
	default:
		break;
	}


	//x 列
	//y 行
	x = x - 1;
	y = y - 1;
	unsigned short pos[2] = { 0 }; // 放到lParam
	// 每一格 38*48
	pos[0] =  X + x * 40 - 20;
	pos[1] =  Y + y * 50 - 20;

	//printf("INDEX: %d %d , cor: x %d, y %d\n", x,y, pos[0], pos[1]);
	::PostMessageA(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, *(int*)pos);
	::PostMessageA(hwnd, WM_LBUTTONUP, 0, *(int*)pos);

}

void CMFCllkDlg::OnBnClickedButtonxian0()
{
	// TODO: 在此添加控件通知处理程序代码
	InitGameData();
	HWND hwnd = ::FindWindowA(NULL, "连连看 v4.1");
	if (NULL == hwnd)
	{
		MessageBoxA("未找到连连看进程");
		return;
	}

	DWORD lie = 18;
	DWORD hang = 14;

	// 处理 m_gamedata2 数据
	/*
	ff ff ff ff ff ff ff ff ff ff ff ff ff ff 
	ff 00 00 00 00 00 00 00 00 00 00 00 00 ff 
	ff 00 0d 2a 00 24 07 0b 0c 1c 26 0a 00 ff   第一列
	ff 00 09 0a 00 16 0b 1b 1a 29 1d 1e 00 ff 
	ff 00 01 1e 27 26 11 0b 02 20 12 1b 00 ff 
	ff 00 23 1c 04 08 0e 05 0a 10 29 27 00 ff 
	ff 00 02 03 1f 20 21 18 08 1c 26 08 00 ff 
	ff 00 0e 15 21 29 11 05 0d 11 18 17 00 ff 
	ff 00 23 03 05 1d 0f 12 1e 0d 07 19 00 ff 
	ff 00 00 1e 06 13 24 25 02 25 14 20 00 ff 
	ff 00 00 1d 07 00 00 16 1e 01 22 1b 00 ff 
	ff 00 21 10 23 06 12 0f 19 22 2a 06 00 ff 
	ff 00 06 23 13 1b 20 25 10 0a 07 1f 00 ff 
	ff 00 1f 28 17 19 1a 1f 04 0c 14 20 00 ff 
	ff 00 27 01 0b 0c 10 04 1e 18 26 2a 00 ff 
	ff 00 16 03 11 17 1f 02 15 25 05 09 00 ff 
	ff 00 29 0c 24 0e 00 00 13 14 1c 27 00 ff 
	ff 00 13 14 1a 04 09 01 22 1d 0d 21 00 ff 
	ff 00 28 17 22 18 22 0e 03 08 16 12 00 ff 
	ff 00 19 1f 24 1a 21 09 2a 20 21 22 00 ff 
	ff 00 00 00 00 00 00 00 00 00 00 00 00 ff 
	ff ff ff ff ff ff ff ff ff ff ff ff ff ff 
	*/

	printf("遍历相邻\n");

	for (size_t i = 2; i < 18 + 4 -2 ; i++)
	{
		for (size_t j = 2; j < 14 -2; j++)
		{

			DWORD cur = m_gamedataPtr[i][j]->val;
			if (cur == MAX_KIND)
			{
				break;
			}
			if (cur == 0)
			{
				continue;
			}
			//printf("step1 %p %p\n", m_gamedataPtr[i][j], m_gamedataPtr[i][j + 1]);
			// 第一层匹配相邻的
			DWORD right = m_gamedataPtr[i][j + 1]->val;

			if (cur == right)
			{
				//printf("near: %d,%d -- %d,%d - %d \n", i, j,
				//	i, j+1, m_gamedataPtr[i][j]->val);

				SendClickMode2(m_gamedataPtr[i][j], m_gamedataPtr[i][j + 1],0);
				m_Total1++;
				continue;
			}
			
			DWORD down = m_gamedataPtr[i+1][j]->val;
			if (cur == down)
			{
				//printf("near: %d,%d -- %d,%d - %d \n", i, j,
				//	i + 1, j, m_gamedataPtr[i][j]->val);
				SendClickMode2(m_gamedataPtr[i+1][j], m_gamedataPtr[i][j], 0);

				m_Total1++;
				continue;
			}
			

		}


	}

	printf("遍历相邻结束\n");

}



void CMFCllkDlg::OnBnClickedButtonxian1()
{
	// TODO: 在此添加控件通知处理程序代码
	InitGameData();
	printf("遍历一条直线\n");

	// 第二次路径遍历
	DWORD total = 0;
	m_Total1 = 0;
	
	while (true)
	{
		for (size_t i = 2; i < 18 + 4 - 2; i++)    // 行
		{
			for (size_t j = 2; j < 14 - 2; j++)  // 列
			{
				//CString s1;
				//s1.Format("%02x", m_gamedata2[i][j]);
				//OutputDebugStringA(s1);

				if (m_gamedataPtr[i][j]->val == MAX_KIND)
				{
					break;
				}
				if (m_gamedataPtr[i][j]->val == 0)
				{
					continue;
				}

				//printf("FindOne: %d -%d - %02x\n", i, j, m_gamedataPtr[i][j]->val);
				// 
				PEle tmp = FindOneRight(m_gamedataPtr[i][j], m_gamedataPtr[i][j]);
				if (tmp == 0)
				{
					tmp = FindOneDown(m_gamedataPtr[i][j], m_gamedataPtr[i][j]);
				}
				if (tmp != 0)
				{
					//printf("找到: %d %d %d -- %d %d %d  \r\n", i, j, m_gamedataPtr[i][j]->val,
					//	tmp->x, tmp->y, tmp->val);

					SendClickMode2(tmp, m_gamedataPtr[i][j], 0);
					m_Total1 += 1;
					goto RETRY;
				}

			}
		}

		

	RETRY:
		InitGameData();

		if (m_Total1 == 0)
		{
			break;
		}
		m_Total1 = 0;
		total++;
	}

	printf("遍历一条直线结束 %d\n", total);
	UpdateText();
}


void CMFCllkDlg::OnBnClickedButtonzhexian2()
{
	InitGameData();
	printf("遍历两条直线\n");
	DWORD total = 0;
	PEle  right, down, left, top, mfind = 0;

	m_Total2 = 0;

	while (true)
	{
		for (size_t i = 2; i < 18 + 4 - 2; i++)    // 行
		{
			for (size_t j = 2; j < 14 - 2; j++)  // 列
			{

				if (m_gamedataPtr[i][j]->val == MAX_KIND)
				{
					break;
				}
				if (m_gamedataPtr[i][j]->val == 0)
				{
					continue;
				}

				PEle cur = m_gamedataPtr[i][j];
				DWORD index = 0;
				// 右
				//printf(" %d %d %02x\n", cur->x, cur->y, cur->val);
				while (true)
				{
					index++;
					mfind = m_gamedataPtr[cur->x + index][cur->y];

					if (mfind->val == MAX_KIND)
					{
						break;
					}

					if (mfind->val == 0)
					{
						PEle res = FindOneTop(cur, mfind);
						if (!res)
						{
							res = FindOneDown(cur, mfind);
						}
						//printf("\t %d %d %02x %p,,, %d %d %02x %p\n",
						//	cur->x, cur->y, cur->val, cur,
						//	mfind->x, mfind->y, mfind->val, res);
						if (res && res != cur)
						{
							SendClickMode2(cur, res, 0);
							m_Total2++;
							goto RETRY;
						}
						continue;
					}
					// 不为0则
					break;
				}

				// 下
				index = 0;
				while (true)
				{
					index++;
					mfind = m_gamedataPtr[cur->x][cur->y + index];

					if (mfind->val == MAX_KIND)
					{
						break;
					}

					if (mfind->val == 0)
					{
						PEle res = FindOneLeft(cur, mfind);
						if (!res)
						{
							res = FindOneRight(cur, mfind);
						}
						if (res && res != cur)
						{
							SendClickMode2(cur, res, 0);
							m_Total2++;
							goto RETRY;
						}
						continue;
					}
					// 不为0则
					break;
				}

				// left
				index = 0;
				while (true)
				{
					index++;
					mfind = m_gamedataPtr[cur->x - index][cur->y];

					if (mfind->val == MAX_KIND)
					{
						break;
					}

					if (mfind->val == 0)
					{
						PEle res = FindOneTop(cur, mfind);
						if (!res)
						{
							res = FindOneDown(cur, mfind);
						}
						if (res && res != cur)
						{
							SendClickMode2(cur, res, 0);
							m_Total2++;
							goto RETRY;
						}
						continue;
					}
					// 不为0则
					break;
				}

				// top
				index = 0;
				while (true)
				{
					index++;
					mfind = m_gamedataPtr[cur->x][cur->y + index];

					if (mfind->val == MAX_KIND)
					{
						break;
					}

					if (mfind->val == 0)
					{
						PEle res = FindOneLeft(cur, mfind);
						if (!res)
						{
							res = FindOneRight(cur, mfind);
						}
						if (res && res != cur)
						{
							SendClickMode2(cur, res, 0);
							m_Total2++;
							goto RETRY;
						}
						continue;
					}
					// 不为0则
					break;
				}

			}
		}

		
	RETRY:
		InitGameData();

		if (m_Total2 == 0)
		{
			break;
		}
		total++;
		m_Total2 = 0;
	}

	printf("遍历两条直线结束 %d\n", total);
	OnBnClickedButtonxian1();
	UpdateText();
}


void CMFCllkDlg::OnBnClickedButtonzhexian3()
{
	printf("遍历三条直线\n");
	DWORD total = 0;
	//InitGameData();
	OnBnClickedButtonxian0();
	OnBnClickedButtonxian1();
	OnBnClickedButtonzhexian2();

	PEle  right, down, left, top, mfind = 0;

	while (true)
	{

		for (size_t i = 2; i < 18 + 4 - 2; i++)    // 行
		{
			//printf("============%d\n" ,i);
			for (size_t j = 2; j < 14 - 2; j++)  // 列
			{
				//CString s1;
				//s1.Format("%02x", m_gamedata2[i][j]);
				//OutputDebugStringA(s1);
				if (m_gamedataPtr[i][j]->val == MAX_KIND)
				{
					continue;
				}
				if (m_gamedataPtr[i][j]->val == 0)
				{
					continue;
				}

				//printf("FindOne: %d -%d - %02x\n", i, j, m_gamedataPtr[i][j]->val);

				/////// 分析 :
				// 此时的策略是 上下左右移动1-n格， 每一步进行二折线查找
				PEle cur = m_gamedataPtr[i][j];
				DWORD index = 0;
				//printf("cur: %d %d %02x %p\n", cur->x, cur->y, cur->val, cur);


				// top
				index = 0;
				while (true)
				{
					index++;
					mfind = m_gamedataPtr[cur->x][cur->y - index];

					if (mfind->val != 0)
					{
						//printf("\t\t top mfind: %d %d %02x break\n", mfind->x,mfind->y,mfind->val);
						break;
					}

					if (mfind->val == 0)
					{
						PEle res = FindTwoTopLeftRight(cur, mfind);
						if (res && res != cur && res->val == cur->val)
						{
							m_Total3++;
							SendClickMode2(cur, res, 0);
							//printf("\t TOP %d %d %02x %p \n",
							//	res->x, res->y, res->val, res);
							goto RETRY;

						}
						continue;
					}

				}

				// 右
				index = 0;
				while (true)
				{
					index++;
					mfind = m_gamedataPtr[cur->x + index][cur->y];

					if (mfind->val != 0)
					{
						break;
					}

					if (mfind->val == 0)
					{
						PEle res = FindTwoRightTopDown(cur, mfind);
						if (res && res != cur && res->val == cur->val)
						{
							m_Total3++;
							SendClickMode2(cur, res, 0);
							//printf("\t RIGHT %d %d %02x %p \n",
							//	res->x, res->y, res->val, res);
							goto RETRY;
						}
						continue;
					}

				}

				// 下
				index = 0;
				while (true)
				{
					index++;
					mfind = m_gamedataPtr[cur->x][cur->y + index];

					if (mfind->val != 0)
					{
						break;
					}

					if (mfind->val == 0)
					{
						PEle res = FindTwoDownLeftRight(cur, mfind);

						if (res && res != cur && res->val == cur->val)
						{
							m_Total3++;
							SendClickMode2(cur, res, 0);
							//printf("\t DWON %d %d %02x %p , mfind: %d %d\n",
							//	res->x, res->y, res->val, res, mfind->x, mfind->y);
							goto RETRY;
						}
						continue;
					}

				}

				// left
				index = 0;
				while (true)
				{
					index++;
					mfind = m_gamedataPtr[cur->x - index][cur->y];

					if (mfind->val != 0)
					{
						break;
					}

					if (mfind->val == 0)
					{
						PEle res = FindTwoLeftTopDown(cur, mfind);

						if (res && res != cur && res->val == cur->val)
						{
							m_Total3++;
							SendClickMode2(cur, res, 0);
							//printf("\t LEFT %d %d %02x %p\n",
							//	res->x, res->y, res->val, res);
							goto RETRY;

						}
						continue;
					}

				}

			}
		}

		

	RETRY:
		InitGameData();
		if (m_Total3 == 0)
		{
			break;
		}

		OnBnClickedButtonxian0();
		OnBnClickedButtonxian1();
		OnBnClickedButtonzhexian2();
		m_Total3 = 0;
		total++;
	}
	

	printf("遍历三条直线结束 %d \n", total);

	
	UpdateText();
}


