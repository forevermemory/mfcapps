// CDialogDirectory.cpp: 实现文件
//

#include "pch.h"
#include "LordPE.h"
#include "afxdialogex.h"
#include "CDialogDirectory.h"
#include "CGlobalInfo.h"
#include <string>

// CDialogDirectory 对话框

IMPLEMENT_DYNAMIC(CDialogDirectory, CDialogEx)

CDialogDirectory::CDialogDirectory(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DIRECTORY, pParent)
	, m_Export_RVA(_T(""))
	, m_Export_Size(_T(""))
	, m_Import_RVA(_T(""))
	, m_Import_Size(_T(""))
	, m_Resource_RVA(_T(""))
	, m_Resource_Size(_T(""))
	, m_Exception_RVA(_T(""))
	, m_Exception_Size(_T(""))
	, m_Security_RVA(_T(""))
	, m_Security_Size(_T(""))
	, m_Relocation_RVA(_T(""))
	, m_Relocation_Size(_T(""))
	, m_Debug_RVA(_T(""))
	, m_Debug_Size(_T(""))
	, m_Architecture_RVA(_T(""))
	, m_Architecture_Size(_T(""))
	, m_GlobalPtr_RVA(_T(""))
	, m_GlobalPtr_Size(_T(""))
	, m_TLS_RVA(_T(""))
	, m_TLS_Size(_T(""))
	, m_LoadConfig_RVA(_T(""))
	, m_LoadConfig_Size(_T(""))
	, m_BoundImport_RVA(_T(""))
	, m_BoundImport_Size(_T(""))
	, m_IAT_RVA(_T(""))
	, m_IAT_Size(_T(""))
	, m_DelayLoad_RVA(_T(""))
	, m_DelayLoad_Size(_T(""))
	, m_COM_RVA(_T(""))
	, m_COM_Size(_T(""))
	, m_Reserved_RVA(_T(""))
	, m_Reserved_Size(_T(""))
{

}

CDialogDirectory::~CDialogDirectory()
{
}

void CDialogDirectory::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_D_Export_RVA, m_Export_RVA);
	DDX_Text(pDX, IDC_EDIT_D_Export_Size, m_Export_Size);
	DDX_Text(pDX, IDC_EDIT_D_Import_RVA, m_Import_RVA);
	DDX_Text(pDX, IDC_EDIT_D_Import_Size, m_Import_Size);
	DDX_Text(pDX, IDC_EDIT_D_Resource_RVA, m_Resource_RVA);
	DDX_Text(pDX, IDC_EDIT_D_Resource_Size, m_Resource_Size);
	DDX_Text(pDX, IDC_EDIT_D_Exception_RVA, m_Exception_RVA);
	DDX_Text(pDX, IDC_EDIT_D_Exception_Size, m_Exception_Size);
	DDX_Text(pDX, IDC_EDIT_D_Security_RVA, m_Security_RVA);
	DDX_Text(pDX, IDC_EDIT_D_Security_Size, m_Security_Size);
	DDX_Text(pDX, IDC_EDIT_D_Relocation_RVA, m_Relocation_RVA);
	DDX_Text(pDX, IDC_EDIT_D_Relocation_Size, m_Relocation_Size);
	DDX_Text(pDX, IDC_EDIT_D_Debug_RVA, m_Debug_RVA);
	DDX_Text(pDX, IDC_EDIT_D_Debug_Size, m_Debug_Size);
	DDX_Text(pDX, IDC_EDIT_D_Architecture_RVA, m_Architecture_RVA);
	DDX_Text(pDX, IDC_EDIT_D_Architecture_Size, m_Architecture_Size);
	DDX_Text(pDX, IDC_EDIT_D_GlobalPtr_RVA, m_GlobalPtr_RVA);
	DDX_Text(pDX, IDC_EDIT_D_GlobalPtr_Size, m_GlobalPtr_Size);
	DDX_Text(pDX, IDC_EDIT_D_TLS_RVA, m_TLS_RVA);
	DDX_Text(pDX, IDC_EDIT_D_TLS_Size, m_TLS_Size);
	DDX_Text(pDX, IDC_EDIT_D_LoadConfig_RVA, m_LoadConfig_RVA);
	DDX_Text(pDX, IDC_EDIT_D_LoadConfig_Size, m_LoadConfig_Size);
	DDX_Text(pDX, IDC_EDIT_D_BoundImport_RVA, m_BoundImport_RVA);
	DDX_Text(pDX, IDC_EDIT_D_BoundImport_Size, m_BoundImport_Size);
	DDX_Text(pDX, IDC_EDIT_D_IAT_RVA, m_IAT_RVA);
	DDX_Text(pDX, IDC_EDIT_D_IAT_Size, m_IAT_Size);
	DDX_Text(pDX, IDC_EDIT_D_DelayLoad_RVA, m_DelayLoad_RVA);
	DDX_Text(pDX, IDC_EDIT_D_DelayLoad_Size, m_DelayLoad_Size);
	DDX_Text(pDX, IDC_EDIT_D_COM_RVA, m_COM_RVA);
	DDX_Text(pDX, IDC_EDIT_D_COM_Size, m_COM_Size);
	DDX_Text(pDX, IDC_EDIT_D_Reserved_RVA, m_Reserved_RVA);
	DDX_Text(pDX, IDC_EDIT_D_Reserved_Size, m_Reserved_Size);
}


BEGIN_MESSAGE_MAP(CDialogDirectory, CDialogEx)
END_MESSAGE_MAP()


// CDialogDirectory 消息处理程序


BOOL CDialogDirectory::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	// TODO:  在此添加额外的初始化
	//IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR
	CString str;

	// 区分32位pe文件还是64位pe文件


	if (CGlobalInfo::GetInstance()->m_Arch == 32)
	{
		IMAGE_DATA_DIRECTORY pExport = CGlobalInfo::GetInstance()->m_pOptHeader32
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
		str.Format("%X", pExport.VirtualAddress);
		m_Export_RVA = str;
		str.Format("%X", pExport.Size);
		m_Export_Size = str;

		IMAGE_DATA_DIRECTORY pImport = CGlobalInfo::GetInstance()->m_pOptHeader32
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
		str.Format("%X", pImport.VirtualAddress);
		m_Import_RVA = str;
		str.Format("%X", pImport.Size);
		m_Import_Size = str;

		IMAGE_DATA_DIRECTORY pResource = CGlobalInfo::GetInstance()->m_pOptHeader32
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE];
		str.Format("%X", pResource.VirtualAddress);
		m_Resource_RVA = str;
		str.Format("%X", pResource.Size);
		m_Resource_Size = str;

		IMAGE_DATA_DIRECTORY pException = CGlobalInfo::GetInstance()->m_pOptHeader32
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION];
		str.Format("%X", pException.VirtualAddress);
		m_Exception_RVA = str;
		str.Format("%X", pException.Size);
		m_Exception_Size = str;

		IMAGE_DATA_DIRECTORY pSecurity = CGlobalInfo::GetInstance()->m_pOptHeader32
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY];
		str.Format("%X", pSecurity.VirtualAddress);
		m_Security_RVA = str;
		str.Format("%X", pSecurity.Size);
		m_Security_Size = str;

		IMAGE_DATA_DIRECTORY pRelocation = CGlobalInfo::GetInstance()->m_pOptHeader32
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
		str.Format("%X", pRelocation.VirtualAddress);
		m_Relocation_RVA = str;
		str.Format("%X", pRelocation.Size);
		m_Relocation_Size = str;

		IMAGE_DATA_DIRECTORY pDebug = CGlobalInfo::GetInstance()->m_pOptHeader32
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG];
		str.Format("%X", pDebug.VirtualAddress);
		m_Debug_RVA = str;
		str.Format("%X", pDebug.Size);
		m_Debug_Size = str;

		IMAGE_DATA_DIRECTORY pArchitecture = CGlobalInfo::GetInstance()->m_pOptHeader32
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE];
		str.Format("%X", pArchitecture.VirtualAddress);
		m_Architecture_RVA = str;
		str.Format("%X", pArchitecture.Size);
		m_Architecture_Size = str;

		IMAGE_DATA_DIRECTORY pGlobalPtr = CGlobalInfo::GetInstance()->m_pOptHeader32
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR];
		str.Format("%X", pGlobalPtr.VirtualAddress);
		m_GlobalPtr_RVA = str;
		str.Format("%X", pGlobalPtr.Size);
		m_GlobalPtr_Size = str;

		IMAGE_DATA_DIRECTORY pTLS = CGlobalInfo::GetInstance()->m_pOptHeader32
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS];
		str.Format("%X", pTLS.VirtualAddress);
		m_TLS_RVA = str;
		str.Format("%X", pTLS.Size);
		m_TLS_Size = str;

		IMAGE_DATA_DIRECTORY pLoadConfig = CGlobalInfo::GetInstance()->m_pOptHeader32
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG];
		str.Format("%X", pLoadConfig.VirtualAddress);
		m_LoadConfig_RVA = str;
		str.Format("%X", pLoadConfig.Size);
		m_LoadConfig_Size = str;

		IMAGE_DATA_DIRECTORY pBoundImport = CGlobalInfo::GetInstance()->m_pOptHeader32
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT];
		str.Format("%X", pBoundImport.VirtualAddress);
		m_BoundImport_RVA = str;
		str.Format("%X", pBoundImport.Size);
		m_BoundImport_Size = str;

		IMAGE_DATA_DIRECTORY pIAT = CGlobalInfo::GetInstance()->m_pOptHeader32
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT];
		str.Format("%X", pIAT.VirtualAddress);
		m_IAT_RVA = str;
		str.Format("%X", pIAT.Size);
		m_IAT_Size = str;

		IMAGE_DATA_DIRECTORY pDelayLoad = CGlobalInfo::GetInstance()->m_pOptHeader32
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT];
		str.Format("%X", pDelayLoad.VirtualAddress);
		m_DelayLoad_RVA = str;
		str.Format("%X", pDelayLoad.Size);
		m_DelayLoad_Size = str;

		IMAGE_DATA_DIRECTORY pCOM = CGlobalInfo::GetInstance()->m_pOptHeader32
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR];
		str.Format("%X", pCOM.VirtualAddress);
		m_COM_RVA = str;
		str.Format("%X", pCOM.Size);
		m_COM_Size = str;

		IMAGE_DATA_DIRECTORY pReversed = CGlobalInfo::GetInstance()->m_pOptHeader32
			->DataDirectory[15];
		m_Reserved_RVA = "0";
		m_Reserved_Size = "0";
	}
	else
	{
		IMAGE_DATA_DIRECTORY pExport = CGlobalInfo::GetInstance()->m_pOptHeader64
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
		str.Format("%X", pExport.VirtualAddress);
		m_Export_RVA = str;
		str.Format("%X", pExport.Size);
		m_Export_Size = str;

		IMAGE_DATA_DIRECTORY pImport = CGlobalInfo::GetInstance()->m_pOptHeader64
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
		str.Format("%X", pImport.VirtualAddress);
		m_Import_RVA = str;
		str.Format("%X", pImport.Size);
		m_Import_Size = str;

		IMAGE_DATA_DIRECTORY pResource = CGlobalInfo::GetInstance()->m_pOptHeader64
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE];
		str.Format("%X", pResource.VirtualAddress);
		m_Resource_RVA = str;
		str.Format("%X", pResource.Size);
		m_Resource_Size = str;

		IMAGE_DATA_DIRECTORY pException = CGlobalInfo::GetInstance()->m_pOptHeader64
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION];
		str.Format("%X", pException.VirtualAddress);
		m_Exception_RVA = str;
		str.Format("%X", pException.Size);
		m_Exception_Size = str;

		IMAGE_DATA_DIRECTORY pSecurity = CGlobalInfo::GetInstance()->m_pOptHeader64
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY];
		str.Format("%X", pSecurity.VirtualAddress);
		m_Security_RVA = str;
		str.Format("%X", pSecurity.Size);
		m_Security_Size = str;

		IMAGE_DATA_DIRECTORY pRelocation = CGlobalInfo::GetInstance()->m_pOptHeader64
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
		str.Format("%X", pRelocation.VirtualAddress);
		m_Relocation_RVA = str;
		str.Format("%X", pRelocation.Size);
		m_Relocation_Size = str;

		IMAGE_DATA_DIRECTORY pDebug = CGlobalInfo::GetInstance()->m_pOptHeader64
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG];
		str.Format("%X", pDebug.VirtualAddress);
		m_Debug_RVA = str;
		str.Format("%X", pDebug.Size);
		m_Debug_Size = str;

		IMAGE_DATA_DIRECTORY pArchitecture = CGlobalInfo::GetInstance()->m_pOptHeader64
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE];
		str.Format("%X", pArchitecture.VirtualAddress);
		m_Architecture_RVA = str;
		str.Format("%X", pArchitecture.Size);
		m_Architecture_Size = str;

		IMAGE_DATA_DIRECTORY pGlobalPtr = CGlobalInfo::GetInstance()->m_pOptHeader64
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR];
		str.Format("%X", pGlobalPtr.VirtualAddress);
		m_GlobalPtr_RVA = str;
		str.Format("%X", pGlobalPtr.Size);
		m_GlobalPtr_Size = str;

		IMAGE_DATA_DIRECTORY pTLS = CGlobalInfo::GetInstance()->m_pOptHeader64
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS];
		str.Format("%X", pTLS.VirtualAddress);
		m_TLS_RVA = str;
		str.Format("%X", pTLS.Size);
		m_TLS_Size = str;

		IMAGE_DATA_DIRECTORY pLoadConfig = CGlobalInfo::GetInstance()->m_pOptHeader64
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG];
		str.Format("%X", pLoadConfig.VirtualAddress);
		m_LoadConfig_RVA = str;
		str.Format("%X", pLoadConfig.Size);
		m_LoadConfig_Size = str;

		IMAGE_DATA_DIRECTORY pBoundImport = CGlobalInfo::GetInstance()->m_pOptHeader64
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT];
		str.Format("%X", pBoundImport.VirtualAddress);
		m_BoundImport_RVA = str;
		str.Format("%X", pBoundImport.Size);
		m_BoundImport_Size = str;

		IMAGE_DATA_DIRECTORY pIAT = CGlobalInfo::GetInstance()->m_pOptHeader64
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT];
		str.Format("%X", pIAT.VirtualAddress);
		m_IAT_RVA = str;
		str.Format("%X", pIAT.Size);
		m_IAT_Size = str;

		IMAGE_DATA_DIRECTORY pDelayLoad = CGlobalInfo::GetInstance()->m_pOptHeader64
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT];
		str.Format("%X", pDelayLoad.VirtualAddress);
		m_DelayLoad_RVA = str;
		str.Format("%X", pDelayLoad.Size);
		m_DelayLoad_Size = str;

		IMAGE_DATA_DIRECTORY pCOM = CGlobalInfo::GetInstance()->m_pOptHeader64
			->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR];
		str.Format("%X", pCOM.VirtualAddress);
		m_COM_RVA = str;
		str.Format("%X", pCOM.Size);
		m_COM_Size = str;

		IMAGE_DATA_DIRECTORY pReversed = CGlobalInfo::GetInstance()->m_pOptHeader64
			->DataDirectory[15];
		m_Reserved_RVA = "0";
		m_Reserved_Size = "0";
	}
	

	ParseExport();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



BOOL CDialogDirectory::ParseExport()
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
	CGlobalInfo * info = CGlobalInfo::GetInstance();
	// 找到在文件中导出表信息的位置
	PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)
		(info->RvaToFoa(directory.VirtualAddress) + info->m_base);

	printf("RVA:%X\n", directory.VirtualAddress);
	printf("info:%X\n", info);
	printf("pExport:%X\n", pExport);


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


	printf("AddressOfFunctions: %X\n", pExport->AddressOfFunctions);
	printf("AddressOfNames: %X\n", pExport->AddressOfNames);
	printf("AddressOfNameOrdinals: %X\n", pExport->AddressOfNameOrdinals);
	printf("Base: %X\n", pExport->Base);
	printf( "Characteristics: %X\n", pExport->Characteristics);
	printf( "TimeDateStamp: %X\n", pExport->TimeDateStamp);
	printf( "MajorVersion: %X\n", pExport->MajorVersion);
	printf( "MinorVersion: %X\n", pExport->MinorVersion);
	printf("NumberOfFunctions: %X\n", pExport->NumberOfFunctions);
	printf("NumberOfNames: %X\n", pExport->NumberOfNames);
	printf( "Name: %X\n", pExport->Name);
	printf("Name: %s\n",(char *)(info->RvaToFoa(pExport->Name) + info->m_base));

	// 函数地址表 DWORD
	DWORD* funcaddr = (DWORD*)(info->RvaToFoa(pExport->AddressOfFunctions) + info->m_base);
	// 函数名称序号表 WORD
	WORD* peot = (WORD*)(info->RvaToFoa(pExport->AddressOfNameOrdinals) + info->m_base);
	// 函数名称表 DWORD (RVA)
	DWORD* pent = (DWORD*)(info->RvaToFoa(pExport->AddressOfNames) + info->m_base);

	for (int i = 0; i < pExport->NumberOfFunctions; i++)
	{
		printf("函数地址为: %x ", *funcaddr); // RVA
		printf("offset: %x ", info->RvaToFoa(*funcaddr)); // 距离文件开头偏移
		// 有些导出函数是没有名称
		BOOL existName = FALSE;
		for (int j = 0; j < pExport->NumberOfNames; j++)
		{
			if (peot[j] == i)
			{
				char* funcName = (char*)(info->RvaToFoa(pent[j]) + info->m_base);
				printf("函数序号: %d  ", j);
				printf("函数名称: %s\n", funcName);
				existName = TRUE;
				break;
			}
		}

		if (!existName)
		{
			printf("函数序号: %d  ", 0);
			printf("函数名称: sub_%x\n", *funcaddr);
		}
		funcaddr++;
	}


	return TRUE;
}