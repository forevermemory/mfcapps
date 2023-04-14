
// MFCllkDlg.cpp: 实现文件
//

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
	ON_BN_CLICKED(IDC_BUTTON8, &CMFCllkDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CMFCllkDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CMFCllkDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_CHECK1, &CMFCllkDlg::OnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CMFCllkDlg::OnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON11, &CMFCllkDlg::OnBnClickedButton11)
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
	PrintUI("我的程序.exe", "MYWINDOW", "color 9", 800, 100, 700, 500);
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



void CMFCllkDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

	
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
	DWORD dwShun = 99999;
	dwTemp += 0xABC;
	result = ::WriteProcessMemory(handle, (LPVOID)dwTemp, &dwShun, sizeof(DWORD), NULL);
	if (!result)
	{
		MessageBoxA("WriteProcessMemory");
		return;
	}

	CloseHandle(handle);
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


void CMFCllkDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码

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

	for (size_t i = 0; i < hang; i++)
	{
		m_gamedata2[0][i] = -1;
	}
	m_gamedata2[1][0] = -1;
	m_gamedata2[1][13] = -1;
	m_gamedata2[20][0] = -1;
	m_gamedata2[20][13] = -1;
	for (size_t i = 0; i < hang; i++)
	{
		m_gamedata2[21][i] = 0xffffffff;
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

	m_Data = "";
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

			switch (k)
			{
			case 0:
				tmp = "00 ";
				break;
			case -1:
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


PEle CMFCllkDlg::FindOne(PEle cur)
{
	DWORD MAX = 3;
	PEle  right, down, left, top = 0;

	// 进入: 一折 二折 三折
	// 右 
	DWORD index = 1;
	while (true)
	{
		right = m_gamedataPtr[cur->x][cur->y + index++];
		if (right->val == 0)
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

	// 下 
	index = 1;
	while (true)
	{
		down = m_gamedataPtr[cur->x + index++][cur->y];
		if (down->val == 0)
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

void CMFCllkDlg::SendClick(short x, short y, DWORD mode)
{
	HWND hwnd = ::FindWindowA(NULL, "连连看 v4.1");
	if (NULL == hwnd)
	{
		MessageBoxA("未找到连连看进程");
		return;
	}

	// 初级 162 + 162 


	switch (mode)
	{
	default:
		break;
	}


	//x 列
	//y 行
	x = x - 1;
	y = y - 1;
	unsigned short pos[2] = { 0 }; // 放到lParam
	// 每一格 38*48
	pos[0] = 160 + x * 40 - 20;
	pos[1] = 115 + y * 50 - 20;

	printf("INDEX: %d %d , cor: x %d, y %d\n", x,y, pos[0], pos[1]);
	::PostMessageA(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, *(int*)pos);
	::PostMessageA(hwnd, WM_LBUTTONUP, 0, *(int*)pos);

}

void CMFCllkDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码

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

	printf("第一次遍历\n");
	// 第一次遍历

	for (size_t i = 0; i < 18 + 4 ; i++)
	{
		for (size_t j = 0; j < 14; j++)
		{



			DWORD cur = m_gamedataPtr[i][j]->val;
			if (cur == 0xffffffff)
			{
				continue;
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
				printf("near: %d,%d -- %d,%d - %d \n", i, j,
					i, j+1, m_gamedataPtr[i][j]->val);
				m_gamedataPtr[i][j]->val = 0;
				m_gamedataPtr[i][j+1]->val = 0;
				SendClick(i , j, 0);
				Sleep(100);
				SendClick(i, j + 1, 0);
				m_Total1++;
				continue;
			}
			
			DWORD down = m_gamedataPtr[i+1][j]->val;
			if (cur == down)
			{
				printf("near: %d,%d -- %d,%d - %d \n", i, j,
					i + 1, j, m_gamedataPtr[i][j]->val);
				m_gamedataPtr[i + 1][j]->val = 0;
				m_gamedataPtr[i][j]->val = 0;

				SendClick(i, j, 0);
				Sleep(100);
				SendClick(i + 1, j , 0);
				m_Total1++;
				continue;
			}
			

		}
	}

	CString tmp;
	m_Data = "";

	tmp.Format("%d %d \r\n", m_Total1, m_Total2);
	m_Data += tmp;
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
			case -1:
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



void CMFCllkDlg::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码

	printf("第2次遍历\n");

	// 第二次路径遍历
	DWORD total = 0;

	while (true)
	{
		for (size_t i = 2; i < 18 + 4 - 2; i++)    // 行
		{
			for (size_t j = 2; j < 14 - 2; j++)  // 列
			{
				//CString s1;
				//s1.Format("%02x", m_gamedata2[i][j]);
				//OutputDebugStringA(s1);

				if (m_gamedataPtr[i][j]->val == -1)
				{
					continue;
				}
				if (m_gamedataPtr[i][j]->val == 0)
				{
					continue;
				}

				//printf("FindOne: %d -%d - %02x\n", i, j, m_gamedataPtr[i][j]->val);
				// 
				PEle tmp = FindOne(m_gamedataPtr[i][j]);

				if (tmp != 0)
				{
					printf("找到: %d %d %d -- %d %d %d  \r\n", i, j, m_gamedataPtr[i][j]->val,
						tmp->x, tmp->y, tmp->val);

					SendClick(i, j, 0);
					Sleep(100);
					SendClick(tmp->x, tmp->y, 0);

					m_Total2 += 1;
				}

			}
		}

		if (m_Total2 == 0)
		{
			break;
		}
		else
		{
			m_Total2 = 0;
		}
		total++;
	}
	

	printf("第二次结束 %d \n", total);


	CString tmp;
	m_Data = "";

	tmp.Format("%d %d \r\n", m_Total1, m_Total2);
	m_Data += tmp;
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
			case -1:
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
