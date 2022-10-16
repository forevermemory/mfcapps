// CMainDialog.cpp: 实现文件
//

#include "pch.h"
#include "ZombieLib.h"
#include "afxdialogex.h"
#include "CMainDialog.h"
#include "GlobalInfo.h"
#include "HookGameMainThread.h"

#pragma warning(disable:4996)

HookGameMainThread m_HookMainThread;
// CMainDialog 对话框

IMPLEMENT_DYNAMIC(CMainDialog, CDialogEx)

CMainDialog::CMainDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ZOMBIE_MAIN, pParent)

	, m_zwwd(FALSE)
	, m_zwwcd(FALSE)
	, m_ygbjs(FALSE)
	, m_yxbzt(FALSE)
{

}

CMainDialog::~CMainDialog()
{
}

void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COM_PLANTS, m_Plants);
	DDX_Control(pDX, IDC_COMBO_Zombies, m_Zombies);
	DDX_Check(pDX, IDC_CHECK3, m_zwwcd);
	DDX_Check(pDX, IDC_CHECK4, m_ygbjs);
	DDX_Check(pDX, IDC_CHECK2, m_zwwd);
	DDX_Check(pDX, IDC_CHECK1, m_yxbzt);
}


BEGIN_MESSAGE_MAP(CMainDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_INFINATE_SUN, &CMainDialog::OnBnClickedButtonInfinateSun)
	ON_BN_CLICKED(IDC_BUTTON1, &CMainDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMainDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMainDialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMainDialog::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON7, &CMainDialog::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON6, &CMainDialog::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON5, &CMainDialog::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON8, &CMainDialog::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CMainDialog::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_CHECK1, &CMainDialog::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CMainDialog::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CMainDialog::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CMainDialog::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_BUTTON10, &CMainDialog::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CMainDialog::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CMainDialog::OnBnClickedButton12)
END_MESSAGE_MAP()


// CMainDialog 消息处理程序


BOOL CMainDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
		// 保存窗口句柄

	// TODO: 在此添加额外的初始化代码
	m_Plants.AddString("豌豆射手");
	m_Plants.AddString("向日葵");
	m_Plants.AddString("樱桃炸弹");
	m_Plants.AddString("坚果");
	m_Plants.AddString("土豆地雷");
	m_Plants.AddString("雪花豌豆");
	m_Plants.AddString("大嘴花(咀嚼着)");
	m_Plants.AddString("双发豌豆");
	m_Plants.AddString("小喷蘑菇");
	m_Plants.AddString("阳光蘑菇");
	m_Plants.AddString("大喷蘑菇"); // 10
	m_Plants.AddString("墓碑吞噬者");
	m_Plants.AddString("催眠蘑菇");
	m_Plants.AddString("胆小蘑菇");
	m_Plants.AddString("冰蘑菇");
	m_Plants.AddString("末日蘑菇");
	m_Plants.AddString("睡莲");
	m_Plants.AddString("倭瓜");
	m_Plants.AddString("三联装豌豆");
	m_Plants.AddString("缠绕海带");
	m_Plants.AddString("墨西哥辣椒"); // 20
	m_Plants.AddString("荆草");
	m_Plants.AddString("火炬木");
	m_Plants.AddString("高坚果");
	m_Plants.AddString("海蘑菇");
	m_Plants.AddString("灯笼草");
	m_Plants.AddString("仙人掌");
	m_Plants.AddString("三叶草");
	m_Plants.AddString("分裂豌豆");
	m_Plants.AddString("杨桃");
	m_Plants.AddString("南瓜"); // 30
	m_Plants.AddString("磁体蘑菇");
	m_Plants.AddString("卷心菜投手");
	m_Plants.AddString("花篮");
	m_Plants.AddString("玉米投手");
	m_Plants.AddString("咖啡豆");
	m_Plants.AddString("大蒜");
	m_Plants.AddString("莴苣");
	m_Plants.AddString("金盏花");
	m_Plants.AddString("西瓜投手");
	m_Plants.AddString("加特林豌豆"); // 40
	m_Plants.AddString("双子向日葵");
	m_Plants.AddString("忧郁蘑菇");
	m_Plants.AddString("香蒲");
	m_Plants.AddString("冰西瓜");
	m_Plants.AddString("吸金磁");
	m_Plants.AddString("尖刺石");
	m_Plants.AddString("玉米加农炮");
	m_Plants.AddString("模仿者");
	m_Plants.AddString("反向双发射手");

	m_Plants.SetCurSel(0);

	// 填充僵尸
	m_Zombies.AddString("普通僵尸");
	m_Zombies.AddString("扛旗");
	m_Zombies.AddString("黄帽子");
	m_Zombies.AddString("撑杆");
	m_Zombies.AddString("铁头");
	m_Zombies.AddString("报纸");
	m_Zombies.AddString("铁丝网");
	m_Zombies.AddString("橄榄");
	m_Zombies.AddString("跳舞1");
	m_Zombies.AddString("跳舞2");
	m_Zombies.AddString("鸭子"); // 10
	m_Zombies.AddString("潜水");
	m_Zombies.AddString(""); // 12 没有
	m_Zombies.AddString("雪橇");
	m_Zombies.AddString("骑海豚");
	m_Zombies.AddString("白衣");
	m_Zombies.AddString("气球");
	m_Zombies.AddString(""); // 17 没有
	m_Zombies.AddString("弹跳");
	m_Zombies.AddString("白猩猩");
	m_Zombies.AddString("射箭");
	m_Zombies.AddString("楼梯");
	m_Zombies.AddString("");// 22 没有
	m_Zombies.AddString("背儿子");
	m_Zombies.AddString("迷你僵尸");
	m_Zombies.AddString("");// 25 没有
	m_Zombies.AddString("豌豆僵尸");
	m_Zombies.AddString("南瓜");
	m_Zombies.AddString("辣椒");
	m_Zombies.AddString("加特林");
	m_Zombies.AddString("倭瓜");
	m_Zombies.AddString("大南瓜");
	m_Zombies.AddString("红眼");
	m_Zombies.AddString("飞翔的豌豆");
	m_Zombies.AddString("飞翔的蘑菇");
	m_Zombies.AddString("草"); // 35
	m_Zombies.AddString("红衣");
	m_Zombies.AddString("草");
	m_Zombies.AddString("向日葵");
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


	FILE* fp1;
	fp1 = fopen("a.txt", "w+");
	if (NULL == fp1)
		AfxMessageBox("open file error");
	fprintf(fp1, "helloworld %d\n", fp1);
	fflush(fp1);
	fclose(fp1);
	GlobalInfo::GetInstance()->m_hWndDlgMain = m_hWnd;
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CMainDialog::OnBnClickedButtonInfinateSun()
{
	// TODO: 在此添加控件通知处理程序代码

	// [[0x6A9EC0] + 0x768] +  0x5560
	// 

	DWORD tmp = *(DWORD*)0x6A9EC0;
	DWORD tmp2 = *(DWORD*)(tmp + 0x768);
	*(DWORD*)(tmp2 + 0x5560) = 999999;

}


void CMainDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CComboBox* plants = (CComboBox*)GetDlgItem(IDC_COM_PLANTS);
	int nIndex = plants->GetCurSel();
	m_plantid = (UINT)nIndex;

	AutoCallPutPlant();
}


void PutBoom(UINT plantid, UINT x, UINT y)
{
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

void CMainDialog::AutoCallPutPlant()
{

	for (UINT i = 0; i < 6; i++)
	{
		for (UINT j = 0; j < 9; j++)
		{
			PutBoom(m_plantid, i, j);
			//Sleep(100);
		}
	}
}


void CMainDialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_plantid = 43;
	AutoCallPutPlant();
}


void CMainDialog::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_plantid = 2;
	AutoCallPutPlant();
}


void CMainDialog::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_plantid = 15;
	AutoCallPutPlant();
}


void CMainDialog::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	m_plantid = 27;
	AutoCallPutPlant();
}


void CMainDialog::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	m_plantid = 40;
	AutoCallPutPlant();
}


void CMainDialog::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_plantid = 11;
	AutoCallPutPlant();
}


void   PutZombie(UINT zombieid , UINT x)
{

	__asm {
		pushad

		push x // 水平位置x坐标
		push zombieid // 僵尸编号
		mov eax, ds: [0x6A9EC0]
		mov eax, ds : [eax + 0x768]  // 参数3 [ [0x6A9EC0] + 0x768]
		mov ebx, 0x0040DDC0
		call ebx

		popad
	}
}

void CMainDialog::AutoCallPutZombie()
{
	for (UINT i = 0; i < 6; i++)
	{
		PutZombie(UINT(m_zombieid), i);
	}
}



void CMainDialog::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	CComboBox* zombies = (CComboBox*)GetDlgItem(IDC_COMBO_Zombies);
	int nIndex = zombies->GetCurSel();
	m_zombieid = (UINT)nIndex;


	AutoCallPutZombie();
}


void CMainDialog::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	m_zombieid = 32;
	AutoCallPutZombie();
}


void CMainDialog::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	UCHAR replace[8] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	UCHAR raw[8] = { 0xFF, 0xD2, 0xFF, 0x15, 0x40, 0x23, 0x65, 0x00 };


	BOOL result;
	// 通过窗口句柄拿到进程id
	DWORD pid;
	GetWindowThreadProcessId(this->m_hWnd, &pid);
	// 通过进程id拿到句柄
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if (m_yxbzt)
	{
		result = ::WriteProcessMemory(handle, (LPVOID)0x54EC05, &replace,
			sizeof(replace), NULL);
	}
	else
	{
		result = ::WriteProcessMemory(handle, (LPVOID)0x54EC05, &raw,
			sizeof(raw), NULL);
	}

	if (!result)
	{
		MessageBox("ReadProcessMemory");
		return;
	}

}


void CMainDialog::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	UCHAR raw[4] = { 0x83, 0x46, 0x40, 0xFC };
	UCHAR replace[4] = { 0x83, 0x46, 0x40, 0 };

	// 0052FCF0 - 83 46 40 00 - add dword ptr[esi + 40], 00
	//if (m_Plant_blood)
	//{
		//result = ::WriteProcessMemory(handle, (LPVOID)0x52FCF0, &replace,

	BOOL result;
	// 通过窗口句柄拿到进程id
	DWORD pid;
	GetWindowThreadProcessId(this->m_hWnd, &pid);
	// 通过进程id拿到句柄
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if (m_zwwd)
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
		MessageBox("ReadProcessMemory");
		return;
	}
}


void CMainDialog::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	UCHAR replace[2] = { 0x90,0x90 };
	UCHAR raw[2] = { 0x7E,0x14 };


	BOOL result;
	// 通过窗口句柄拿到进程id
	DWORD pid;
	GetWindowThreadProcessId(this->m_hWnd, &pid);
	// 通过进程id拿到句柄
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if (m_zwwcd)
	{
		result = ::WriteProcessMemory(handle, (LPVOID)0x487296, &replace,
			sizeof(replace), NULL);
	}
	else
	{
		result = ::WriteProcessMemory(handle, (LPVOID)0x487296, &raw,
			sizeof(raw), NULL);
	}

	if (!result)
	{
		MessageBox("ReadProcessMemory");
		return;
	}
}


void CMainDialog::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);


	// TODO: 在此添加控件通知处理程序代码
	//0041BA72 | . / 7F 0C             jg short PlantsVs.0041BA80
	//0041BA74 | . | 2BF3              sub esi, ebx
	//0041BA76 | . | 89B7 60550000     mov dword ptr ds : [edi + 0x5560] , esi;  PlantsVs. < ModuleEntryPoint>
	//0041BA7C | . | B0 01             mov al, 0x1
	//0041BA7E | . | 5E                pop esi;  kernel32.75F8FA29


	// 通过窗口句柄拿到进程id
	DWORD pid;
	GetWindowThreadProcessId(this->m_hWnd, &pid);
	// 通过进程id拿到句柄
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	
	UCHAR replace[2] = { 0x03,0xF3 };
	UCHAR raw[2] = { 0x2B,0xF3 };
	
	BOOL result;
	if (m_ygbjs)
	{
		result = ::WriteProcessMemory(handle, (LPVOID)0x41BA74, &replace,
			sizeof(replace), NULL);
	}
	else
	{
		result = ::WriteProcessMemory(handle, (LPVOID)0x41BA74, &raw,
			sizeof(raw), NULL);
	}

	if (!result)
	{
		MessageBox("ReadProcessMemory");
		return;
	}
}


void CMainDialog::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL result = 	m_HookMainThread.HookMainThread();
	if (!result)
	{
		MessageBox("HookMainThread err");
		return;
	}
}


void CMainDialog::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL result = m_HookMainThread.UnHookMainThread();
	if (!result)
	{
		MessageBox("HookMainThread err");
		return;
	}
}


void CMainDialog::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	m_HookMainThread.PutPlants("挂接主线程");
}
