// CDialogRelocation.cpp: 实现文件
//

#include "pch.h"
#include "LordPE.h"
#include "afxdialogex.h"
#include "CDialogRelocation.h"
#include "CGlobalInfo.h"


// CDialogRelocation 对话框

IMPLEMENT_DYNAMIC(CDialogRelocation, CDialogEx)

CDialogRelocation::CDialogRelocation(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DIRECTORY_RELOCATION, pParent)
{

}

CDialogRelocation::~CDialogRelocation()
{
}

void CDialogRelocation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DLL_LIST, m_Sec);
	DDX_Control(pDX, IDC_LIST_DLL_ITEM, m_Item);
}


BEGIN_MESSAGE_MAP(CDialogRelocation, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DLL_LIST, &CDialogRelocation::OnClickListDllList)
END_MESSAGE_MAP()


// CDialogRelocation 消息处理程序


BOOL CDialogRelocation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Sec.ModifyStyle(LVS_ICON | LVS_SMALLICON | LVS_LIST, LVS_REPORT);
	m_Sec.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_Sec.InsertColumn(0, "Index", LVCFMT_LEFT, 90);
	m_Sec.InsertColumn(1, "Section", LVCFMT_LEFT, 100);
	m_Sec.InsertColumn(2, "RVA", LVCFMT_LEFT, 94);
	m_Sec.InsertColumn(3, "Items", LVCFMT_LEFT, 100);


	m_Item.ModifyStyle(LVS_ICON | LVS_SMALLICON | LVS_LIST, LVS_REPORT);
	m_Item.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_Item.InsertColumn(0, "Index", LVCFMT_LEFT, 90);
	m_Item.InsertColumn(1, "RVA", LVCFMT_LEFT, 100);
	m_Item.InsertColumn(2, "Offset", LVCFMT_LEFT, 94);
	m_Item.InsertColumn(3, "Data", LVCFMT_LEFT, 286);

	ParseRelocation();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL CDialogRelocation::ParseRelocation()
{
	// TODO: 在此添加控件通知处理程序代码
	IMAGE_DATA_DIRECTORY directory;

	if (CGlobalInfo::GetInstance()->m_Arch == 64)
	{
		directory = CGlobalInfo::GetInstance()->m_pOptHeader64->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	}
	else
	{
		directory = CGlobalInfo::GetInstance()->m_pOptHeader32->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	}

	if (directory.VirtualAddress == 0)
	{
		MessageBox("不存在重定位表");
		printf("不存在重定位表\n");
		return TRUE;
	}
	printf("pReloc:%X\n", directory);
	CGlobalInfo* info = CGlobalInfo::GetInstance();

	// 找到在文件中导出表信息的位置
	PIMAGE_BASE_RELOCATION pReloc = (PIMAGE_BASE_RELOCATION)
		(info->RvaToFoa(directory.VirtualAddress) + info->m_base);


	int count = 1;
	while (pReloc->VirtualAddress)
	{

		PIMAGE_SECTION_HEADER header = info->RvaFindSection(pReloc->VirtualAddress);
		char buf[9] = { 0 };
		memcpy(buf, header->Name, 8);

		DWORD number = (pReloc->SizeOfBlock - 8) / 2; // 前8个字节 之后每两个字节都是相关基址
		WORD* pRelocOffset = (WORD*)((ULONGLONG)pReloc + 8); // 跳过前8个字节

		printf("section name:%s %d rva:%X\n", buf, number, header->VirtualAddress);

		int index = m_Sec.GetItemCount();
		CString str;
		str.Format("%d", count);
		m_Sec.InsertItem(index, str);

		str.Format("%s", buf);
		m_Sec.SetItemText(index, 1, str);
		str.Format("%X", header->VirtualAddress);
		m_Sec.SetItemText(index, 2, str);
		str.Format("%Xh , %dd", number, number);
		m_Sec.SetItemText(index, 3, str);
		m_Sec.SetItemData(index, (DWORD_PTR)pReloc);


		//for (int i = 0; i < number; i++)
		//{
		//	//if ((*pRelocOffset & 0x3000) == 0x3000) // 前4个字节=3的话 有效
		//	//{
		//	//	// 数据为后面12个字节
		//	//	DWORD rva = (*pRelocOffset & 0x0fff) + pReloc->VirtualAddress;
		//	//	printf("rav=%X\n", rva);
		//	//}
		//	// 数据为后面12个字节
		//	DWORD rva = (*pRelocOffset & 0x0fff) + pReloc->VirtualAddress;

		//	DWORD foa = info->RvaToFoa(rva);
		//	printf("rav=%X foa=%X\n", rva, foa);
		//	pRelocOffset++;
		//}

		count++;
		pReloc = (PIMAGE_BASE_RELOCATION)((ULONGLONG)pReloc + pReloc->SizeOfBlock);
	}


	return TRUE;
}

void CDialogRelocation::OnClickListDllList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	POSITION pos = m_Sec.GetFirstSelectedItemPosition();
	int nItem = m_Sec.GetNextSelectedItem(pos);

	CGlobalInfo* info = CGlobalInfo::GetInstance();
	PIMAGE_BASE_RELOCATION pReloc = (PIMAGE_BASE_RELOCATION)m_Sec.GetItemData(nItem);

	m_Item.DeleteAllItems();


	PIMAGE_SECTION_HEADER header = info->RvaFindSection(pReloc->VirtualAddress);
	char buf[9] = { 0 };
	memcpy(buf, header->Name, 8);

	DWORD number = (pReloc->SizeOfBlock - 8) / 2; // 前8个字节 之后每两个字节都是相关基址
	WORD* pRelocOffset = (WORD*)((ULONGLONG)pReloc + 8); // 跳过前8个字节

	printf("section name:%s %d rva:%X\n", buf, number, header->VirtualAddress);
	

	int count = 1;
	for (int i = 0; i < number; i++)
	{
		//if ((*pRelocOffset & 0x3000) == 0x3000) // 前4个字节=3的话 有效
		//{
		//	// 数据为后面12个字节
		//	DWORD rva = (*pRelocOffset & 0x0fff) + pReloc->VirtualAddress;
		//	printf("rav=%X\n", rva);
		//}
		// 数据为后面12个字节
		DWORD rva = (*pRelocOffset & 0x0fff) + pReloc->VirtualAddress;

		DWORD foa = info->RvaToFoa(rva);
		//printf("rav=%X foa=%X\n", rva, foa);

		int index = m_Item.GetItemCount();
		CString str;
		str.Format("%d", count);
		m_Item.InsertItem(index, str);
		str.Format("%X", rva);
		m_Item.SetItemText(index, 1, str);
		str.Format("%X", foa);
		m_Item.SetItemText(index, 2, str);



		pRelocOffset++;
		count++;
	}
}
