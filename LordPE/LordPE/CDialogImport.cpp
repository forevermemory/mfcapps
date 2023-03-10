// CDialogImport.cpp: 实现文件
//

#include "pch.h"
#include "LordPE.h"
#include "afxdialogex.h"
#include "CDialogImport.h"
#include "CGlobalInfo.h"


// CDialogImport 对话框

IMPLEMENT_DYNAMIC(CDialogImport, CDialogEx)

CDialogImport::CDialogImport(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DIRECTORY_IMPORT, pParent)
{

}

CDialogImport::~CDialogImport()
{
}

void CDialogImport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DLL_LIST, m_DllList);
	DDX_Control(pDX, IDC_LIST_DLL_ITEM, m_dll_items);
}


BEGIN_MESSAGE_MAP(CDialogImport, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DLL_LIST, &CDialogImport::OnClickListDllList)
END_MESSAGE_MAP()


// CDialogImport 消息处理程序


BOOL CDialogImport::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_DllList.ModifyStyle(LVS_ICON | LVS_SMALLICON | LVS_LIST, LVS_REPORT);
	m_DllList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_DllList.InsertColumn(0, "TimeDateStamp", LVCFMT_LEFT, 90);
	m_DllList.InsertColumn(1, "OriginalFirstThunk", LVCFMT_LEFT, 100);
	m_DllList.InsertColumn(2, "FirstThunk", LVCFMT_LEFT, 94);
	m_DllList.InsertColumn(3, "ForwarderChain", LVCFMT_LEFT, 100);
	m_DllList.InsertColumn(4, "DllName", LVCFMT_LEFT, 186);


	m_dll_items.ModifyStyle(LVS_ICON | LVS_SMALLICON | LVS_LIST, LVS_REPORT);
	m_dll_items.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_dll_items.InsertColumn(0, "RawValue", LVCFMT_LEFT, 80);
	m_dll_items.InsertColumn(1, "Offset", LVCFMT_LEFT, 80);
	m_dll_items.InsertColumn(2, "Hint", LVCFMT_LEFT, 50);
	m_dll_items.InsertColumn(3, "ApiName", LVCFMT_LEFT, 360);

	ParseImport();

	UpdateData(FALSE);

	return TRUE;
}


BOOL CDialogImport::ParseImport()
{

	//m_DllList

	IMAGE_DATA_DIRECTORY directory;

	if (CGlobalInfo::GetInstance()->m_Arch == 64)
	{
		directory = CGlobalInfo::GetInstance()->m_pOptHeader64->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	}
	else
	{
		directory = CGlobalInfo::GetInstance()->m_pOptHeader32->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	}


	/*
	typedef struct _IMAGE_IMPORT_DESCRIPTOR {
		union {
			DWORD   Characteristics;            // 0 for terminating null import descriptor
			DWORD   OriginalFirstThunk;         // // RVA to INT, 包含指向输入名称表 INT 的结构的数组 IMAGE_THUNK_DATA
		} DUMMYUNIONNAME;
		DWORD   TimeDateStamp;                  // 测试一般都为0
												// 如果为0  FirstThunk和OriginalFirstThunk相等 同样指向导入名称表
//												// 如果为-1 则FirstThunk指向真实的地址表
		DWORD   Name;           // RVA 指向被输入的dll的ASCII字符串
		DWORD   FirstThunk;     //  RVA to IAT (if bound this IAT has actual addresses)
								//  包含指向输入地址表的结构的数组 IMAGE_THUNK_DATA
	} IMAGE_IMPORT_DESCRIPTOR;
	typedef IMAGE_IMPORT_DESCRIPTOR UNALIGNED *PIMAGE_IMPORT_DESCRIPTOR;

	*/

	printf("ImportTable:%X\n", directory);
	CGlobalInfo* info = CGlobalInfo::GetInstance();
	// 找到在文件中导出表信息的位置
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)
		(info->RvaToFoa(directory.VirtualAddress) + info->m_base);


	while (pImport->Characteristics)
	{
		char* dllName = (char*)(info->RvaToFoa(pImport->Name) + info->m_base);
		printf("import dll文件名称为: %s,TimeDateStamp: %X \n", dllName, pImport->TimeDateStamp);

		int index = m_DllList.GetItemCount();
		CString str;
		str.Format("%X", pImport->TimeDateStamp);
		m_DllList.InsertItem(index, str);
		str.Format("%X", pImport->OriginalFirstThunk);
		m_DllList.SetItemText(index, 1, str);
		str.Format("%X", pImport->FirstThunk);
		m_DllList.SetItemText(index, 2, str);
		str.Format("%X", pImport->ForwarderChain);
		m_DllList.SetItemText(index, 3, str);
		m_DllList.SetItemText(index, 4, dllName);
		m_DllList.SetItemData(index,(DWORD_PTR)pImport);


		
		pImport++;
	}


	return TRUE;
}

void CDialogImport::OnClickListDllList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	POSITION pos = m_DllList.GetFirstSelectedItemPosition();
	int nItem = m_DllList.GetNextSelectedItem(pos);

	CGlobalInfo* info = CGlobalInfo::GetInstance();
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)m_DllList.GetItemData(nItem);

	char* dllName = (char*)(info->RvaToFoa(pImport->Name) + info->m_base);
	printf("item:%s\n", dllName);

	// 渲染导入函数
	PIMAGE_THUNK_DATA32 pImgTrunkData32 = NULL;
	PIMAGE_THUNK_DATA64 pImgTrunkData64 = NULL;

	m_dll_items.DeleteAllItems();

	//if (pImport->TimeDateStamp == 0)
	//{


	// FirstThunk和OriginalFirstThunk相等 指向导入名称表 INT
		switch (info->m_Arch)
		{
		case 32:
			pImgTrunkData32 = (PIMAGE_THUNK_DATA32)
				(info->RvaToFoa(pImport->OriginalFirstThunk) + info->m_base);
		

			while (pImgTrunkData32->u1.Function)
			{
				int index = m_dll_items.GetItemCount();
				CString str;
				str.Format("%X", pImgTrunkData32->u1.Ordinal);
				m_dll_items.InsertItem(index, str);


				if (pImgTrunkData32->u1.Ordinal & 0x80000000)
				{
					// 最高位为1 按序号导入 其余31位为序号
					DWORD ordinal = pImgTrunkData32->u1.Ordinal & 0x7FFFFFFF;
					// printf("\t按序号导入: %d\n", ordinal);

					str.Format("%X", info->RvaToFoa(ordinal));
					m_dll_items.SetItemText(index, 1, str);

					str.Format("%s", "-");
					m_dll_items.SetItemText(index, 2, str);
					str.Format("Ordinal: %X", ordinal);
					m_dll_items.SetItemText(index, 3, str);
				}
				else
				{
					// 0 - 按名称导入
					PIMAGE_IMPORT_BY_NAME pFuncName = (PIMAGE_IMPORT_BY_NAME)
						(info->RvaToFoa(pImgTrunkData32->u1.AddressOfData) + info->m_base);
					// printf("\t按名称导入: %s\n", pFuncName->Name);

					str.Format("%X", info->RvaToFoa(pImgTrunkData32->u1.Ordinal));
					m_dll_items.SetItemText(index, 1, str);

					str.Format("%X", pFuncName->Hint);
					m_dll_items.SetItemText(index, 2, str);
					str.Format("%s", pFuncName->Name);
					m_dll_items.SetItemText(index, 3, str);
				}
				pImgTrunkData32++;
			}
			break;
		case 64:
			pImgTrunkData64 = (PIMAGE_THUNK_DATA64)
				(info->RvaToFoa(pImport->OriginalFirstThunk) + info->m_base);
			
			while (pImgTrunkData64->u1.Function)
			{
				int index = m_dll_items.GetItemCount();
				CString str;

				str.Format("%X", pImgTrunkData64->u1.Ordinal);
				m_dll_items.InsertItem(index, str);

				
			
				if (pImgTrunkData64->u1.Ordinal & 0x8000000000000000)
				{
					// 最高位为1 按序号导入 其余31位为序号 
					// //去掉最高位才是实际的值,否则RVAToFOA会出错
					
					ULONGLONG ordinal = pImgTrunkData64->u1.Ordinal & 0x7FFFFFFFFFFFFFFF;
					// printf("\t按序号导入: %lld\n", ordinal);

					str.Format("%X", info->RvaToFoa(ordinal));
					m_dll_items.SetItemText(index, 1, str);
					str.Format("%s", "-");
					m_dll_items.SetItemText(index, 2, str);
					str.Format("Ordinal: %X", ordinal);
					m_dll_items.SetItemText(index, 3, str);
				}
				else
				{
					// 0 - 按名称导入
					PIMAGE_IMPORT_BY_NAME pFuncName = (PIMAGE_IMPORT_BY_NAME)
						(info->RvaToFoa(pImgTrunkData64->u1.AddressOfData) + info->m_base);
					// printf("\t按名称导入:%X %s\n", (DWORD)info->RvaToFoa(pImgTrunkData64->u1.AddressOfData), pFuncName->Name);
				
					str.Format("%X", info->RvaToFoa(pImgTrunkData64->u1.Ordinal));
					m_dll_items.SetItemText(index, 1, str);
					str.Format("%X", pFuncName->Hint);
					m_dll_items.SetItemText(index, 2, str);
					str.Format("%s", pFuncName->Name);
					m_dll_items.SetItemText(index, 3, str);
				}
				pImgTrunkData64++;
			}

			break;
		}
	

	////////////////////////////////////////////
	// }
	//else if (pImport->TimeDateStamp == -1)
	//{
	//	//FirstThunk 指向真实的地址表
	//	DWORD* addr = (DWORD*)(info->m_base + info->RvaToFoa(pImport->FirstThunk));
	//	printf("\tdllName:【%s】:\n", (char*)(info->m_base + info->RvaToFoa(pImport->Name)));
	//	while (*addr)
	//	{
	// 	   int index = m_dll_items.GetItemCount();
	//      CString str;
	//
	//      m_dll_items.InsertItem(index, "-");
	//      str.Format("addr: %X", *addr);
	//      m_dll_items.SetItemText(index, 3, str);
	////		printf("\t\tFunction Addr:[%08X]\n", *addr);
	//		addr++;
	//	}
	//}



}
