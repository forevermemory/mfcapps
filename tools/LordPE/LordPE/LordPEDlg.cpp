
// LordPEDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "LordPE.h"
#include "LordPEDlg.h"
#include "afxdialogex.h"
#include "CDialogSubsystem.h"
#include "CDialogSections.h"
#include "CDialogDirectory.h"
#include "CGlobalInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable: 4996)


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


// CLordPEDlg 对话框



CLordPEDlg::CLordPEDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LORDPE_DIALOG, pParent)
	, m_Machine(_T(""))
	, m_NumberOfSections(_T(""))
	, m_SizeOfOptionalHeader(_T(""))
	, m_TimeDateStamp(_T(""))
	, m_Subsystem(_T(""))
	, m_AddressOfEntryPoint(_T(""))
	, m_ImageBase(_T(""))
	, m_SizeOfImage(_T(""))
	, m_BaseOfCode(_T(""))
	, m_BaseOfData(_T(""))
	, m_NumberOfRvaAndSizes(_T(""))
	, m_FileAlignment(_T(""))
	, m_SectionAlignment(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLordPEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Machine, m_Machine);
	DDX_Text(pDX, IDC_EDIT_NumberOfSections, m_NumberOfSections);
	DDX_Text(pDX, IDC_EDIT_SizeOfOptionalHeader, m_SizeOfOptionalHeader);
	DDX_Text(pDX, IDC_EDIT_TimeDateStamp, m_TimeDateStamp);
	DDX_Text(pDX, IDC_EDIT_Subsystem, m_Subsystem);
	DDX_Control(pDX, BTN_SECTIONS, m_Btn_Sections);
	DDX_Control(pDX, BTN_SELECT_FILE, m_Btn_SelectFile);
	DDX_Control(pDX, BTN_DIRECTORY, m_Btn_Directory);
	DDX_Control(pDX, IDC_BTN_POSITION_CALE, m_Btn_Position_Calc);
	DDX_Text(pDX, IDC_EDIT1, m_AddressOfEntryPoint);
	DDX_Text(pDX, IDC_EDIT2, m_ImageBase);
	DDX_Text(pDX, IDC_EDIT3, m_SizeOfImage);
	DDX_Text(pDX, IDC_EDIT4, m_BaseOfCode);
	DDX_Text(pDX, IDC_EDIT5, m_BaseOfData);
	DDX_Text(pDX, IDC_EDIT16, m_NumberOfRvaAndSizes);
	DDX_Text(pDX, IDC_EDIT7, m_FileAlignment);
	DDX_Text(pDX, IDC_EDIT6, m_SectionAlignment);
}

BEGIN_MESSAGE_MAP(CLordPEDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_DIRECTORY, &CLordPEDlg::OnBnClickedDirectory)
	ON_BN_CLICKED(BTN_SECTIONS, &CLordPEDlg::OnBnClickedSections)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(BTN_SELECT_FILE, &CLordPEDlg::OnBnClickedSelectFile)
	ON_BN_CLICKED(BTN_BTN_SUBSYSTEM_EXT, &CLordPEDlg::OnBnClickedBtnSubsystemExt)
	ON_BN_CLICKED(IDC_BTN_POSITION_CALE, &CLordPEDlg::OnBnClickedBtnPositionCale)
END_MESSAGE_MAP()


// CLordPEDlg 消息处理程序

BOOL CLordPEDlg::OnInitDialog()
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
	ShowWindow(SW_MINIMIZE | SW_SHOWNA);

	// Release



	// 
	// debug
	//
		//CButton * bt =  (CButton*)GetDlgItem(BTN_SECTIONS);


	//PrintUI();
	
	//m_szPath2 = "C:\\Windows\\System32\\ntdll.dll";
	//ParsePeFile();


	//m_Btn_Sections.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
	//m_Btn_Sections.SetFaceColor(RGB(229, 241, 251), TRUE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLordPEDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLordPEDlg::OnPaint()
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
HCURSOR CLordPEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CLordPEDlg::PrintUI()
{
	///////// 文件移动和重命名
	//char oldPathName[MAX_PATH] = { 0 };
	//char newPathName[MAX_PATH] = { 0 };
	//GetModuleFileNameA(NULL, oldPathName, MAX_PATH); // 获取当前程序路径
	//strcpy(newPathName, oldPathName);
	//for (size_t i = strlen(newPathName); i > 0; i--)
	//{
	//    if (newPathName[i - 1] == '\\')
	//    {
	//        newPathName[i] = 0;
	//        strcat(newPathName, exeName);
	//        break;
	//    }
	//}

	//char cmd[MAXBYTE] = { 0 };
	//strcat(cmd, "del/f/q \"");
	//strcat(cmd, newPathName);
	//strcat(cmd, "\">nul 2>nul");
	//std::cout << cmd << std::endl;

	//system(cmd);
	//rename(oldPathName, newPathName);


	AllocConsole(); // 添加调试窗口
	freopen("conin$", "r+t", stdin); // 将输入流设置为当前调试窗口
	freopen("conout$", "w+t", stdout); // 将输出流设置为当前调试窗口
	SetConsoleTitleA("设置窗口标题");  // 设置窗口标题
	system("color C");
	// 0=黑色 #000000 8=灰色 #808080
	// 1=深蓝色 #000080 9=蓝色 #0000FF
	// 2=绿色 #008000 A=草绿色 #00FF00
	// 3=蓝绿色 #008080 B=浅蓝色 #00FFFF
	// 4=枣红色 #800000 C=红色 #FF0000
	// 5=紫色 #800080 D=粉色 #FF00FF
	// 6=橄榄色 #808000 E=黄色 #FFFF00
	// 7=浅灰色 #C0C0C0 F=白色 #FFFFFF
	HWND ui = GetConsoleWindow();
	
	std::wcout.imbue(std::locale("CHS")); // 支持中文

}



void CLordPEDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	DragQueryFileA(hDropInfo, 0, m_szPath, MAX_PATH);
	//MessageBoxA(NULLm_szPath);
	
	m_szPath2 = m_szPath;
	CDialogEx::OnDropFiles(hDropInfo);
}


void CLordPEDlg::OnBnClickedSelectFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, NULL, NULL, 4 | 2);
	if (dlg.DoModal() == IDOK)
	{
		m_szPath2 = dlg.GetPathName();
		//DWORD dwAttrs = GetFileAttributesA(m_szPath2);
		//if (dwAttrs & FILE_ATTRIBUTE_DIRECTORY)
		//{
		//	m_szPath2 = "";
		//	MessageBoxA("请选择有效的PE文件");
		//}

	}
	else
	{
		return;
	}
	// MessageBoxA(m_szPath2);

	ParsePeFile();
	UpdateData(FALSE);
}



BOOL CLordPEDlg::ParsePeFile()
{
#ifdef DEBUG
	SetWindowTextA("PE查看器 - " + m_szPath2);
#else
	SetWindowTextA(m_hWnd, "PE查看器 - " + m_szPath2);
#endif // DEBUG

	DWORD fileSize;

	// read file
	CFile fp(m_szPath2, CFile::modeRead);
	fileSize = fp.GetLength();
	char* filebuff = (char*)VirtualAlloc(NULL, fileSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	fp.Read(filebuff, fileSize);
	fp.Close();

	CGlobalInfo::GetInstance()->m_base = (ULONGLONG)filebuff;

	printf("filepath: %s\n", m_szPath2.GetBuffer());
	printf("fileSize: %d\n", fileSize);
	printf("filebuff: %p\n", filebuff);

	// parse DOS header
	m_pDosHeader = (PIMAGE_DOS_HEADER)filebuff;
	printf("xxx:%X,e_lfanew:%d\n", m_pDosHeader->e_magic, m_pDosHeader->e_lfanew);

	// 解析 NT_HEADER  filebuff需要用LONG64
	m_pNtHeader = (PIMAGE_NT_HEADERS)(m_pDosHeader->e_lfanew + (LONG64)filebuff);
	printf("pNtHeader:%p\n", m_pNtHeader);
	printf("Signature:%x\n",m_pNtHeader->Signature);

	if (0x4550 != m_pNtHeader->Signature)
	{
		printf("不是有效的PE文件Signature\n");
		return FALSE;
	}


	m_pFileHeader = &m_pNtHeader->FileHeader;

	//////////////// fill data ////////////////
	// NumberOfSections
	char buf[64] = { 0 };
	sprintf(buf, "%d", m_pFileHeader->NumberOfSections);
	m_NumberOfSections = buf;

	// TimeDateStamp
	memset(buf, 0, 64);
	CTime tm(m_pFileHeader->TimeDateStamp);
	sprintf(buf, "%04d/%02d/%02d %02d:%02d:%02d", tm.GetYear(),
		tm.GetMonth(),
		tm.GetDay(),
		tm.GetHour(),
		tm.GetMinute(),
		tm.GetSecond());
	m_TimeDateStamp = buf;

	// m_SizeOfOptionalHeader
	memset(buf, 0, 64);
	sprintf(buf, "%X", m_pFileHeader->SizeOfOptionalHeader);
	m_SizeOfOptionalHeader = buf;

	// Machine
	switch (m_pFileHeader->Machine)
	{
	case IMAGE_FILE_MACHINE_I386:
		m_Machine = "x86";
		m_pOptHeader32 =(PIMAGE_OPTIONAL_HEADER32)&m_pNtHeader->OptionalHeader;
		break;
	case IMAGE_FILE_MACHINE_IA64:
		m_Machine = "Intel Itanium";
		break;
	case IMAGE_FILE_MACHINE_AMD64:
		m_pOptHeader64 = &m_pNtHeader->OptionalHeader;
		m_Machine = "x64";
		break;
	default:
		break;
	}
	

	////////////////////////////// x86和x64可选头结构体定义不一样
	if (IMAGE_FILE_MACHINE_AMD64 == m_pFileHeader->Machine)
	{
		printf("====================================x64\n");
		// Subsystem
		memset(buf, 0, 64);
		sprintf(buf, "%02X", m_pOptHeader64->Subsystem);
		m_Subsystem = buf;

		// 

		memset(buf, 0, 64);
		sprintf(buf, "%X", m_pOptHeader64->AddressOfEntryPoint);
		m_AddressOfEntryPoint = buf;


		memset(buf, 0, 64);
		sprintf(buf, "%X", m_pOptHeader64->ImageBase);
		m_ImageBase = buf;

		memset(buf, 0, 64);
		sprintf(buf, "%X", m_pOptHeader64->SizeOfImage);
		m_SizeOfImage = buf;

		memset(buf, 0, 64);
		sprintf(buf, "%X", m_pOptHeader64->BaseOfCode);
		m_BaseOfCode = buf;


		memset(buf, 0, 64);
		sprintf(buf, "%X", m_pOptHeader64->SectionAlignment);
		m_SectionAlignment = buf;

		memset(buf, 0, 64);
		sprintf(buf, "%X", m_pOptHeader64->FileAlignment);
		m_FileAlignment = buf;

		memset(buf, 0, 64);
		sprintf(buf, "%X", m_pOptHeader64->NumberOfRvaAndSizes);
		m_NumberOfRvaAndSizes = buf;

		CGlobalInfo::GetInstance()->m_pOptHeader64 = m_pOptHeader64;
		CGlobalInfo::GetInstance()->m_Arch = 64;
		
	}
	else
	{
		printf("====================================x86\n");
		// 32
		// Subsystem
		memset(buf, 0, 64);
		sprintf(buf, "%02X", m_pOptHeader32->Subsystem);
		m_Subsystem = buf;

		// 

		memset(buf, 0, 64);
		sprintf(buf, "%X", m_pOptHeader32->AddressOfEntryPoint);
		m_AddressOfEntryPoint = buf;


		memset(buf, 0, 64);
		sprintf(buf, "%X", m_pOptHeader32->ImageBase);
		m_ImageBase = buf;

		memset(buf, 0, 64);
		sprintf(buf, "%X", m_pOptHeader32->SizeOfImage);
		m_SizeOfImage = buf;

		memset(buf, 0, 64);
		sprintf(buf, "%X", m_pOptHeader32->BaseOfCode);
		m_BaseOfCode = buf;

		memset(buf, 0, 64);
		sprintf(buf, "%X", m_pOptHeader32->BaseOfData);
		m_BaseOfData = buf;

		memset(buf, 0, 64);
		sprintf(buf, "%X", m_pOptHeader32->SectionAlignment);
		m_SectionAlignment = buf;

		memset(buf, 0, 64);
		sprintf(buf, "%X", m_pOptHeader32->FileAlignment);
		m_FileAlignment = buf;

		memset(buf, 0, 64);
		sprintf(buf, "%X", m_pOptHeader32->NumberOfRvaAndSizes);
		m_NumberOfRvaAndSizes = buf;

		CGlobalInfo::GetInstance()->m_Arch = 32;
		CGlobalInfo::GetInstance()->m_pOptHeader32 = m_pOptHeader32;
	}
	
	
	CGlobalInfo::GetInstance()->m_pNtHeader = m_pNtHeader;
	CGlobalInfo::GetInstance()->m_pDosHeader = m_pDosHeader;
	CGlobalInfo::GetInstance()->m_pFileHeader = m_pFileHeader;

	UpdateData(FALSE);
	//VirtualFree(buf, len, MEM_RELEASE);
	return TRUE;
}

void CLordPEDlg::OnBnClickedBtnSubsystemExt()
{
	// TODO: 在此添加控件通知处理程序代码

	CDialogSubsystem dlg;
	if (IMAGE_FILE_MACHINE_AMD64 == m_pFileHeader->Machine)
	{
		CGlobalInfo::GetInstance()->m_CDialogSubsystemStatus = m_pOptHeader64->Subsystem;

	}
	else
	{
		CGlobalInfo::GetInstance()->m_CDialogSubsystemStatus = m_pOptHeader32->Subsystem;

	}
	dlg.DoModal();
}

void CLordPEDlg::OnBnClickedDirectory()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogDirectory dlg;
	dlg.DoModal();
}


void CLordPEDlg::OnBnClickedSections()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogSections dlg;
	dlg.DoModal();
}

void CLordPEDlg::OnBnClickedBtnPositionCale()
{
	// TODO: 在此添加控件通知处理程序代码
}
