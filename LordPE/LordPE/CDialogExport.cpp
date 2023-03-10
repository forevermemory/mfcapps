// CDialogExport.cpp: 实现文件
//

#include "pch.h"
#include "LordPE.h"
#include "afxdialogex.h"
#include "CDialogExport.h"
#include "CGlobalInfo.h"


// CDialogExport 对话框

IMPLEMENT_DYNAMIC(CDialogExport, CDialogEx)

CDialogExport::CDialogExport(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DIRECTORY_EXPORT, pParent)
	, m_Func_Addr(_T(""))
	, m_Func_Name(_T(""))
	, m_Func_Ord(_T(""))
	, m_DllName(_T(""))
	, m_Num_func(_T(""))
	, m_Num_funcname(_T(""))
{

}

CDialogExport::~CDialogExport()
{
}

void CDialogExport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EXPORT, m_ListExport);
	DDX_Text(pDX, IDC_EDIT_FUNC_ADDR, m_Func_Addr);
	DDX_Text(pDX, IDC_EDIT_FUNC_NAME, m_Func_Name);
	DDX_Text(pDX, IDC_EDIT_FUNC_ORD, m_Func_Ord);
	DDX_Text(pDX, IDC_EDIT_Name, m_DllName);
	DDX_Text(pDX, IDC_EDIT_Num_FUNC, m_Num_func);
	DDX_Text(pDX, IDC_EDIT_Num_Name, m_Num_funcname);
}


BEGIN_MESSAGE_MAP(CDialogExport, CDialogEx)
END_MESSAGE_MAP()


// CDialogExport 消息处理程序


BOOL CDialogExport::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	m_ListExport.ModifyStyle(LVS_ICON | LVS_SMALLICON | LVS_LIST, LVS_REPORT);
	m_ListExport.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_ListExport.InsertColumn(0, "Ordinal", LVCFMT_LEFT, 70);
	m_ListExport.InsertColumn(1, "RVA", LVCFMT_LEFT, 80);
	m_ListExport.InsertColumn(2, "Offset", LVCFMT_LEFT, 80);
	m_ListExport.InsertColumn(3, "Funtion Name", LVCFMT_LEFT, 276);

	ParseExport();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}





BOOL CDialogExport::ParseExport()
{
	IMAGE_DATA_DIRECTORY directory;

	if (CGlobalInfo::GetInstance()->m_Arch == 64)
	{
		directory = CGlobalInfo::GetInstance()->m_pOptHeader64->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	}
	else
	{
		directory = CGlobalInfo::GetInstance()->m_pOptHeader32->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

	}


	printf("exportTable:%X\n", directory);
	CGlobalInfo* info = CGlobalInfo::GetInstance();

	// 找到在文件中导出表信息的位置
	PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)
		(info->RvaToFoa(directory.VirtualAddress) + info->m_base);




	/*
	typedef struct _IMAGE_EXPORT_DIRECTORY {
		DWORD   Characteristics;
		DWORD   TimeDateStamp;
		WORD    MajorVersion;
		WORD    MinorVersion;
		DWORD   Name;   // RVA
		DWORD   Base;   // 导出函数起始序号
		DWORD   NumberOfFunctions; // 导出函数数量(@max-@min+1)
		DWORD   NumberOfNames;     // 以名称导出函数数量
		DWORD   AddressOfFunctions;     // RVA from base of image
		DWORD   AddressOfNames;         // RVA from base of image
		DWORD   AddressOfNameOrdinals;  // RVA from base of image
	} IMAGE_EXPORT_DIRECTORY, * PIMAGE_EXPORT_DIRECTORY;

	*/

	//printf("RVA:%X\n", directory.VirtualAddress);
	//printf("info:%X\n", info);
	//printf("pExport:%X\n", pExport);
	//printf("AddressOfFunctions: %X\n", pExport->AddressOfFunctions);
	//printf("AddressOfNames: %X\n", pExport->AddressOfNames);
	//printf("AddressOfNameOrdinals: %X\n", pExport->AddressOfNameOrdinals);
	//printf("Base: %X\n", pExport->Base);
	//printf("Characteristics: %X\n", pExport->Characteristics);
	//printf("TimeDateStamp: %X\n", pExport->TimeDateStamp);
	//printf("MajorVersion: %X\n", pExport->MajorVersion);
	//printf("MinorVersion: %X\n", pExport->MinorVersion);
	//printf("NumberOfFunctions: %X\n", pExport->NumberOfFunctions);
	//printf("NumberOfNames: %X\n", pExport->NumberOfNames);
	//printf("Name: %X\n", pExport->Name);
	//printf("Name: %s\n", (char*)(info->RvaToFoa(pExport->Name) + info->m_base));

	////////////////////////////////////////////////////
	CString str;
	str.Format("%X", pExport->AddressOfFunctions);
	m_Func_Addr = str;
	str.Format("%X", pExport->AddressOfNames);
	m_Func_Name = str;
	str.Format("%X", pExport->AddressOfNameOrdinals);
	m_Func_Ord = str;

	str.Format("%d", pExport->NumberOfFunctions);
	m_Num_func = str;
	str.Format("%d", pExport->NumberOfNames);
	m_Num_funcname = str;
	m_DllName = (char*)(info->RvaToFoa(pExport->Name) + info->m_base);
	////////////////////////////////////////////////////


	// 函数地址表 DWORD
	DWORD* funcaddr = (DWORD*)(info->RvaToFoa(pExport->AddressOfFunctions) + info->m_base);
	// 函数名称序号表 WORD
	WORD* peot = (WORD*)(info->RvaToFoa(pExport->AddressOfNameOrdinals) + info->m_base);
	// 函数名称表 DWORD (RVA)
	DWORD* pent = (DWORD*)(info->RvaToFoa(pExport->AddressOfNames) + info->m_base);

	for (int i = 0; i < pExport->NumberOfFunctions; i++)
	{
		////////////////////////////////////
		int index = m_ListExport.GetItemCount();
		CString str;
		str.Format("%04X",i+1);
		m_ListExport.InsertItem(index, str);



		//printf("函数地址为: %x ", *funcaddr); // RVA
		//printf("offset: %x ", info->RvaToFoa(*funcaddr)); // 距离文件开头偏移

		str.Format("%X",*funcaddr);
		m_ListExport.SetItemText(index, 1, str);

		str.Format("%X", info->RvaToFoa(*funcaddr));
		m_ListExport.SetItemText(index, 2, str);

		// 有些导出函数是没有名称
		BOOL existName = FALSE;
		for (int j = 0; j < pExport->NumberOfNames; j++)
		{
			if (peot[j] == i)
			{
				char* funcName = (char*)(info->RvaToFoa(pent[j]) + info->m_base);
				//printf("函数序号: %d  ", j);
				//printf("函数名称: %s\n", funcName);

				str.Format("%s", funcName);
				m_ListExport.SetItemText(index, 3, str);

				existName = TRUE;
				break;
			}
		}

		if (!existName)
		{
			//printf("函数序号: %d  ", 0);
			//printf("函数名称: sub_%x\n", *funcaddr);

			str.Format("sub_%x", info->RvaToFoa(*funcaddr));
			m_ListExport.SetItemText(index, 3, str);
		}



		funcaddr++;
	}


	return TRUE;
}
