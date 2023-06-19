
// MFCPlantzombieDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCPlantzombie.h"
#include "MFCPlantzombieDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


typedef struct CorInfo
{
	UINT x;
	UINT y;
	UINT plantid;

}CorInfo;

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


// CMFCPlantzombieDlg 对话框



CMFCPlantzombieDlg::CMFCPlantzombieDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCPLANTZOMBIE_DIALOG, pParent)
	, m_PlatCD(FALSE)
	, m_gameStatus(FALSE)
	, m_Plant_blood(FALSE)
	, m_NoSun(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_plantid = 0;
}

void CMFCPlantzombieDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_NO_CD, m_PlatCD);
	DDX_Control(pDX, IDC_COM_PLANTS, m_Plants);
	DDX_Check(pDX, IDC_CHECK_NO_SUSPEND, m_gameStatus);
	DDX_Check(pDX, IDC_CHECK_BLOOD, m_Plant_blood);
	DDX_Control(pDX, IDC_COMBO2, m_Zombies);
	DDX_Check(pDX, IDC_CHECK_NO_DEC, m_NoSun);
}

BEGIN_MESSAGE_MAP(CMFCPlantzombieDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCPlantzombieDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK_NO_CD, &CMFCPlantzombieDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCPlantzombieDlg::OnBnClickedButton4)
	ON_CBN_SELCHANGE(IDC_COM_PLANTS, &CMFCPlantzombieDlg::OnCbnSelchangeComPlants)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCPlantzombieDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCPlantzombieDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCPlantzombieDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CMFCPlantzombieDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CMFCPlantzombieDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CMFCPlantzombieDlg::OnBnClickedButton9)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_CHECK_NO_SUSPEND, &CMFCPlantzombieDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK_BLOOD, &CMFCPlantzombieDlg::OnBnClickedCheckBlood)
	ON_BN_CLICKED(IDC_BUTTON10, &CMFCPlantzombieDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CMFCPlantzombieDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_CHECK_NO_DEC, &CMFCPlantzombieDlg::OnBnClickedCheckNoDec)
END_MESSAGE_MAP()


// CMFCPlantzombieDlg 消息处理程序

BOOL CMFCPlantzombieDlg::OnInitDialog()
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
	m_Plants.AddString(L"豌豆射手");
	m_Plants.AddString(L"向日葵");
	m_Plants.AddString(L"樱桃炸弹");
	m_Plants.AddString(L"坚果");
	m_Plants.AddString(L"土豆地雷");
	m_Plants.AddString(L"雪花豌豆");
	m_Plants.AddString(L"大嘴花(咀嚼着)");
	m_Plants.AddString(L"双发豌豆");
	m_Plants.AddString(L"小喷蘑菇");
	m_Plants.AddString(L"阳光蘑菇");
	m_Plants.AddString(L"大喷蘑菇"); // 10
	m_Plants.AddString(L"墓碑吞噬者");
	m_Plants.AddString(L"催眠蘑菇");
	m_Plants.AddString(L"胆小蘑菇");
	m_Plants.AddString(L"冰蘑菇");
	m_Plants.AddString(L"末日蘑菇");
	m_Plants.AddString(L"睡莲");
	m_Plants.AddString(L"倭瓜");
	m_Plants.AddString(L"三联装豌豆");
	m_Plants.AddString(L"缠绕海带");
	m_Plants.AddString(L"墨西哥辣椒"); // 20
	m_Plants.AddString(L"荆草");
	m_Plants.AddString(L"火炬木");
	m_Plants.AddString(L"高坚果");
	m_Plants.AddString(L"海蘑菇");
	m_Plants.AddString(L"灯笼草");
	m_Plants.AddString(L"仙人掌");
	m_Plants.AddString(L"三叶草"); 
	m_Plants.AddString(L"分裂豌豆");
	m_Plants.AddString(L"杨桃");
	m_Plants.AddString(L"南瓜"); // 30
	m_Plants.AddString(L"磁体蘑菇");
	m_Plants.AddString(L"卷心菜投手");
	m_Plants.AddString(L"花篮");
	m_Plants.AddString(L"玉米投手");
	m_Plants.AddString(L"咖啡豆");
	m_Plants.AddString(L"大蒜");
	m_Plants.AddString(L"莴苣");
	m_Plants.AddString(L"金盏花");
	m_Plants.AddString(L"西瓜投手");
	m_Plants.AddString(L"加特林豌豆"); // 40
	m_Plants.AddString(L"双子向日葵");
	m_Plants.AddString(L"忧郁蘑菇");
	m_Plants.AddString(L"香蒲");
	m_Plants.AddString(L"冰西瓜");
	m_Plants.AddString(L"吸金磁");
	m_Plants.AddString(L"尖刺石");
	m_Plants.AddString(L"玉米加农炮");
	m_Plants.AddString(L"模仿者");
	m_Plants.AddString(L"反向双发射手");

	m_Plants.SetCurSel(0);


	// 填充僵尸
	m_Zombies.AddString(L"普通僵尸");
	m_Zombies.AddString(L"扛旗");
	m_Zombies.AddString(L"黄帽子");
	m_Zombies.AddString(L"撑杆");
	m_Zombies.AddString(L"铁头");
	m_Zombies.AddString(L"报纸");
	m_Zombies.AddString(L"铁丝网");
	m_Zombies.AddString(L"橄榄");
	m_Zombies.AddString(L"跳舞1");
	m_Zombies.AddString(L"跳舞2");
	m_Zombies.AddString(L"鸭子"); // 10
	m_Zombies.AddString(L"潜水");
	m_Zombies.AddString(L""); // 12 没有
	m_Zombies.AddString(L"雪橇");
	m_Zombies.AddString(L"骑海豚");
	m_Zombies.AddString(L"白衣");
	m_Zombies.AddString(L"气球");
	m_Zombies.AddString(L""); // 17 没有
	m_Zombies.AddString(L"弹跳");
	m_Zombies.AddString(L"白猩猩");
	m_Zombies.AddString(L"射箭");
	m_Zombies.AddString(L"楼梯");
	m_Zombies.AddString(L"");// 22 没有
	m_Zombies.AddString(L"背儿子");
	m_Zombies.AddString(L"迷你僵尸");
	m_Zombies.AddString(L"");// 25 没有
	m_Zombies.AddString(L"豌豆僵尸");
	m_Zombies.AddString(L"南瓜");
	m_Zombies.AddString(L"辣椒");
	m_Zombies.AddString(L"加特林");
	m_Zombies.AddString(L"倭瓜");
	m_Zombies.AddString(L"大南瓜");
	m_Zombies.AddString(L"红眼");
	m_Zombies.AddString(L"飞翔的豌豆");
	m_Zombies.AddString(L"飞翔的蘑菇");
	m_Zombies.AddString(L"草"); // 35
	m_Zombies.AddString(L"红衣");
	m_Zombies.AddString(L"草");
	m_Zombies.AddString(L"向日葵");
	/*0 --普通僵尸
		1 --扛旗
		2 --黄帽子
		3 --撑杆
		4 --铁头
		5 --报纸
		6 --铁丝网
		7 --橄榄
		8 --跳舞
		9 --跳舞
		10 --鸭子
		11 --潜水

		13 --雪橇
		14 --骑海豚 e
		15 --白衣 f
		16 --气球

		18 --弹跳
		19 --白猩猩
		20 --射箭
		21 --楼梯

		23 --背儿子
		24 --迷你僵尸

		1a 豌豆僵尸 // 26
		1b 南瓜
		1c 辣椒
		1d 加特林
		1e 倭瓜
		1f 大南瓜
		20 --红眼
		21 --飞翔的反向豌豆
		22 --飞翔的反向蘑菇
		23 草
		24 红衣
		26 草
		27 向日葵*/

	m_Zombies.SetCurSel(19);

	SetWindowText(L"植物大战僵尸中文版-辅助");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCPlantzombieDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCPlantzombieDlg::OnPaint()
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
HCURSOR CMFCPlantzombieDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCPlantzombieDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	DWORD pid;
	HWND hwnd = ::FindWindow(NULL, L"植物大战僵尸中文版");
	if (NULL == hwnd)
	{
		MessageBox(L"读取植物大战僵尸中文版进程失败");
		return;
	}
	// 通过窗口句柄拿到进程id
	GetWindowThreadProcessId(hwnd, &pid);
	// 通过进程id拿到句柄
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	// 读取内存数据
	
	DWORD dwTemp = 0;
	BOOL result;
	result = ReadProcessMemory(handle, (LPCVOID)0x6A9EC0, &dwTemp, sizeof(dwTemp), NULL);
	
	dwTemp += 0x768;
	result = ReadProcessMemory(handle, (LPCVOID)dwTemp, &dwTemp, sizeof(DWORD), NULL);

	if (!result)
	{
		MessageBox(L"ReadProcessMemory");
		return;
	}

	// 设置阳光数量
	DWORD dwShun = 999999;
	dwTemp += 0x5560;
	result = ::WriteProcessMemory(handle, (LPVOID)dwTemp, &dwShun, sizeof(DWORD), NULL);
	if (!result)
	{
		MessageBox(L"ReadProcessMemory");
		return;
	}

	CloseHandle(handle);
	

	UpdateData(FALSE);
}


void CMFCPlantzombieDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码

	DWORD pid;
	HWND hwnd = ::FindWindow(NULL, L"植物大战僵尸中文版");
	if (NULL == hwnd)
	{
		MessageBox(L"读取植物大战僵尸中文版进程失败");
		return;
	}
	// 通过窗口句柄拿到进程id
	GetWindowThreadProcessId(hwnd, &pid);
	// 通过进程id拿到句柄
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	UpdateData(TRUE);

	UCHAR buf[2]={0};
	BOOL result;
	if (m_PlatCD)
	{
		buf[0] = 0x90;
		buf[1] = 0x90;
	}
	else
	{
		buf[0] = 0x7E ;
		buf[1] = 0x14 ;
	}
	result = ::WriteProcessMemory(handle, (LPVOID)0x487296, &buf,
		sizeof(buf), NULL);

	if (!result)
	{
		MessageBox(L"ReadProcessMemory");
		return;
	}

	CloseHandle(handle);

}
//UINT plantid = 0x2;
//UINT y = 0;
//UINT x = 0;


//void  __stdcall PutBoom()
void  __stdcall PutBoom(CorInfo * cor)
{
	UINT plantid = cor->plantid;
	UINT y = cor->y;
	UINT x = cor->x;

	__asm {
		pushad
		push - 1
		push plantid
		mov eax, x
		push y
		mov ebx, ds: [0x6A9EC0]
		mov ebx, ds : [ebx + 0x768]
		push ebx
		mov ebx, 0x40D120
		call ebx
		popad
	}
}

//void  __stdcall PutBoom()
void  __stdcall PutZombie(CorInfo* cor)
{
	UINT plantid = cor->plantid;
	UINT x = cor->x;

	__asm {
		pushad

		push x // 水平位置x坐标
		push plantid // 僵尸编号
		mov eax, ds: [0x6A9EC0]
		mov eax, ds : [eax + 0x768]  // 参数3 [ [0x6A9EC0] + 0x768]
		mov ebx, 0x0040DDC0
		call ebx

		popad
	}
}


__declspec(naked) void PutBoomV1()
{
	__asm {
		pushad
		push -1
		push 0x2
		mov eax, 0
		push 0
		mov ebx,ds:[0x6A9EC0]
		mov ebx,ds:[ebx + 0x768]
		push ebx
		mov ebx,0x40D120
		call ebx
		popad
		ret
	}
}



void CMFCPlantzombieDlg::AutoCallPutPlant()
{
	// TODO: 在此添加控件通知处理程序代码
	
	// 第一步 打开目标进程获取到句柄
	DWORD pid;
	HWND hwnd = ::FindWindow(NULL, L"植物大战僵尸中文版");
	if (NULL == hwnd)
	{
		MessageBox(L"读取植物大战僵尸中文版进程失败");
		return;
	}
	// 通过窗口句柄拿到进程id
	GetWindowThreadProcessId(hwnd, &pid);
	// 通过进程id拿到句柄
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	// 第二步
	// 2.1 目标进程分配函数参数空间
	LPVOID paramsAddr = VirtualAllocEx(hProcess, NULL, 6,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (paramsAddr == NULL)
	{
		AfxMessageBox(_T("get paramsCall failed"));
		return;
	}

	// 2.2 在目标进程分配代码空间
	LPVOID codeAddr = VirtualAllocEx(hProcess, NULL, 4096,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (0 == codeAddr)
	{
		MessageBox(L"分配内存失败");
		return;
	}

	// 第三步 往分配的内存空间写入代码
	DWORD byWrite = 0;
	BOOL result = WriteProcessMemory(hProcess, codeAddr, PutBoom,
		4096, &byWrite);
	if (FALSE == result)
	{
		MessageBox(L"WriteProcessMemory");
		return;
	}


	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 9; j++)
		{

			CorInfo corinfo;
			corinfo.x = i;
			corinfo.y = j;
			corinfo.plantid = m_plantid;


			// 写入参数
			BOOL result = WriteProcessMemory(hProcess, paramsAddr, (LPCVOID)&corinfo,
				sizeof(CorInfo), &byWrite);
			if(!result)
			{
				AfxMessageBox(_T("Write CorInfo failed"));
				return;
			}

			// 第四步 执行目标进程指定地址的代码
			CorInfo * ptAddr = (CorInfo*)paramsAddr;
			HANDLE hTread = CreateRemoteThread(hProcess, NULL, NULL,
				(LPTHREAD_START_ROUTINE)codeAddr, (LPVOID)ptAddr, NULL, NULL);
			if (0 == hTread)
			{
				MessageBox(L"执行目标进程指定地址的代码");
				return;
			}

			////// 第五步 等待远程线程执行完成
			DWORD res = WaitForSingleObject(hTread, 1000);
			if (WAIT_TIMEOUT == res)
			{
				MessageBox(L"等待远程线程执行完成");
				return;
			}

			//Sleep(100);

		}
	}

	//// 第六步释放目标进程空间
	VirtualFreeEx(hProcess, paramsAddr, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, codeAddr, 0, MEM_RELEASE);
	// 第七步 关闭目标进程句柄
	CloseHandle(hProcess);
	UpdateData(FALSE);

}


void CMFCPlantzombieDlg::AutoCallPutZombie()
{
	// TODO: 在此添加控件通知处理程序代码

	// 第一步 打开目标进程获取到句柄
	DWORD pid;
	HWND hwnd = ::FindWindow(NULL, L"植物大战僵尸中文版");
	if (NULL == hwnd)
	{
		MessageBox(L"读取植物大战僵尸中文版进程失败");
		return;
	}
	// 通过窗口句柄拿到进程id
	GetWindowThreadProcessId(hwnd, &pid);
	// 通过进程id拿到句柄
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	// 第二步
	// 2.1 目标进程分配函数参数空间
	LPVOID paramsAddr = VirtualAllocEx(hProcess, NULL, 6,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (paramsAddr == NULL)
	{
		AfxMessageBox(_T("get paramsCall failed"));
		return;
	}

	// 2.2 在目标进程分配代码空间
	LPVOID codeAddr = VirtualAllocEx(hProcess, NULL, 4096,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (0 == codeAddr)
	{
		MessageBox(L"分配内存失败");
		return;
	}

	// 第三步 往分配的内存空间写入代码
	DWORD byWrite = 0;
	BOOL result = WriteProcessMemory(hProcess, codeAddr, PutZombie,
		4096, &byWrite);
	if (FALSE == result)
	{
		MessageBox(L"WriteProcessMemory");
		return;
	}


	for (int i = 0; i < 6; i++)
	{
		

		CorInfo corinfo;
		corinfo.x = i;
		corinfo.plantid = m_zombieid;

		// 写入参数
		BOOL result = WriteProcessMemory(hProcess, paramsAddr, (LPCVOID)&corinfo,
			sizeof(CorInfo), &byWrite);
		if (!result)
		{
			AfxMessageBox(_T("Write CorInfo failed"));
			return;
		}

		// 第四步 执行目标进程指定地址的代码
		CorInfo* ptAddr = (CorInfo*)paramsAddr;
		HANDLE hTread = CreateRemoteThread(hProcess, NULL, NULL,
			(LPTHREAD_START_ROUTINE)codeAddr, (LPVOID)ptAddr, NULL, NULL);
		if (0 == hTread)
		{
			MessageBox(L"执行目标进程指定地址的代码");
			return;
		}

		////// 第五步 等待远程线程执行完成
		DWORD res = WaitForSingleObject(hTread, 1000);
		if (WAIT_TIMEOUT == res)
		{
			MessageBox(L"等待远程线程执行完成");
			return;
		}

		//Sleep(100);

	}

	//// 第六步释放目标进程空间
	VirtualFreeEx(hProcess, paramsAddr, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, codeAddr, 0, MEM_RELEASE);
	// 第七步 关闭目标进程句柄
	CloseHandle(hProcess);
	UpdateData(FALSE);

}



void CMFCPlantzombieDlg::OnCbnSelchangeComPlants()
{

	//int nIndex = m_Plants.GetCurSel(); 
	//CString str;
	//str.Format(L"::%d\n", nIndex);
	//AfxMessageBox(str);
}


void CMFCPlantzombieDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CComboBox* plants = (CComboBox*)GetDlgItem(IDC_COM_PLANTS);
	int nIndex = plants->GetCurSel();
	m_plantid = (UINT)nIndex;

	AutoCallPutPlant();
}


void CMFCPlantzombieDlg::OnBnClickedButton4()
{
	m_plantid = 0x2;
	AutoCallPutPlant();
}


void CMFCPlantzombieDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_plantid = 15;
	AutoCallPutPlant();
}


void CMFCPlantzombieDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	m_plantid = 40;
	AutoCallPutPlant();
}


void CMFCPlantzombieDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	m_plantid = 11;
	AutoCallPutPlant();
}


void CMFCPlantzombieDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	m_plantid = 27;
	AutoCallPutPlant();
}


void CMFCPlantzombieDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	m_plantid = 43;
	AutoCallPutPlant();
}


void CMFCPlantzombieDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);


	// TODO: 在此处添加消息处理程序代码
}


void CMFCPlantzombieDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码

	DWORD pid;
	HWND hwnd = ::FindWindow(NULL, L"植物大战僵尸中文版");
	if (NULL == hwnd)
	{
		MessageBox(L"读取植物大战僵尸中文版进程失败");
		return;
	}
	// 通过窗口句柄拿到进程id
	GetWindowThreadProcessId(hwnd, &pid);
	// 通过进程id拿到句柄
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	UpdateData(TRUE);

	UCHAR bufNop[8] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	UCHAR bufRestore[8] = { 0xFF, 0xD2, 0xFF, 0x15, 0x40, 0x23, 0x65, 0x00};
	BOOL result;

	//0054EC05 | .FFD2              call edx;  PlantsVs.00539BD0
	//0054EC07 | .FF15 40236500     call dword ptr ds : [<&USER32.ReleaseCaptu>; [ReleaseCapture

	if (m_gameStatus)
	{
		result = ::WriteProcessMemory(handle, (LPVOID)0x54EC05, &bufNop,
			sizeof(bufNop), NULL);
	
	}
	else
	{
		result = ::WriteProcessMemory(handle, (LPVOID)0x54EC05, &bufRestore,
			sizeof(bufRestore), NULL);
	}


	if (!result)
	{
		MessageBox(L"ReadProcessMemory");
		return;
	}

	CloseHandle(handle);
}



void CMFCPlantzombieDlg::OnBnClickedCheckBlood()
{
	// TODO: 在此添加控件通知处理程序代码

	DWORD pid;
	HWND hwnd = ::FindWindow(NULL, L"植物大战僵尸中文版");
	if (NULL == hwnd)
	{
		MessageBox(L"读取植物大战僵尸中文版进程失败");
		return;
	}
	// 通过窗口句柄拿到进程id
	GetWindowThreadProcessId(hwnd, &pid);
	// 通过进程id拿到句柄
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	UpdateData(TRUE);



	UCHAR raw[4] = { 0x83, 0x46, 0x40, 0xFC };
	UCHAR replace[4] = { 0x83, 0x46, 0x40, 0 };
	BOOL result;

	// 0052FCF0 - 83 46 40 00 - add dword ptr[esi + 40], 00

	if (m_Plant_blood)
	{
		result = ::WriteProcessMemory(handle, (LPVOID)0x52FCF0, &replace,
			sizeof(replace), NULL);

	}
	else
	{
		result = ::WriteProcessMemory(handle, (LPVOID)0x52FCF0, &raw,
			sizeof(raw), NULL);
	}


	if (!result)
	{
		MessageBox(L"ReadProcessMemory");
		return;
	}

	CloseHandle(handle);
}


void CMFCPlantzombieDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码

	CComboBox* zombies = (CComboBox*)GetDlgItem(IDC_COMBO2);
	int nIndex = zombies->GetCurSel();
	m_zombieid = (UINT)nIndex;
	AutoCallPutZombie();
}


void CMFCPlantzombieDlg::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	m_zombieid = 32;
	AutoCallPutZombie();
}






void CMFCPlantzombieDlg::OnBnClickedCheckNoDec()
{
	// TODO: 在此添加控件通知处理程序代码
	//0041BA72 | . / 7F 0C             jg short PlantsVs.0041BA80
	//0041BA74 | . | 2BF3              sub esi, ebx
	//0041BA76 | . | 89B7 60550000     mov dword ptr ds : [edi + 0x5560] , esi;  PlantsVs. < ModuleEntryPoint>
	//0041BA7C | . | B0 01             mov al, 0x1
	//0041BA7E | . | 5E                pop esi;  kernel32.75F8FA29

	DWORD pid;
	HWND hwnd = ::FindWindow(NULL, L"植物大战僵尸中文版");
	if (NULL == hwnd)
	{
		MessageBox(L"读取植物大战僵尸中文版进程失败");
		return;
	}
	// 通过窗口句柄拿到进程id
	GetWindowThreadProcessId(hwnd, &pid);
	// 通过进程id拿到句柄
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	UpdateData(TRUE);

	UCHAR buf[2] = { 0 };
	BOOL result;
	if (m_NoSun)
	{
		buf[0] = 0x03;
		buf[1] = 0xF3;
	}
	else
	{
		buf[0] = 0x2B;
		buf[1] = 0xF3;
	}
	result = ::WriteProcessMemory(handle, (LPVOID)0x41BA74, &buf,
		sizeof(buf), NULL);

	if (!result)
	{
		MessageBox(L"ReadProcessMemory");
		return;
	}

	CloseHandle(handle);
}
