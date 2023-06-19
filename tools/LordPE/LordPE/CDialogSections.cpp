// CDialogSections.cpp: 实现文件
//

#include "pch.h"
#include "LordPE.h"
#include "afxdialogex.h"
#include "CDialogSections.h"
#include "CGlobalInfo.h"
#include <string>

// CDialogSections 对话框

IMPLEMENT_DYNAMIC(CDialogSections, CDialogEx)

CDialogSections::CDialogSections(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SECTIONS, pParent)
{

}

CDialogSections::~CDialogSections()
{
}

void CDialogSections::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SECTIONS, m_Sections);
}


BEGIN_MESSAGE_MAP(CDialogSections, CDialogEx)
END_MESSAGE_MAP()


// CDialogSections 消息处理程序


BOOL CDialogSections::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	PIMAGE_NT_HEADERS pNtHeader = CGlobalInfo::GetInstance()->m_pNtHeader;
	PIMAGE_FILE_HEADER pFileHeader =  CGlobalInfo::GetInstance()->m_pFileHeader;


	m_Sections.ModifyStyle(LVS_ICON | LVS_SMALLICON | LVS_LIST, LVS_REPORT);
	m_Sections.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_Sections.InsertColumn(0, "名称", LVCFMT_LEFT, 70);
	m_Sections.InsertColumn(1, "RVA", LVCFMT_LEFT, 85);
	m_Sections.InsertColumn(2, "VSize", LVCFMT_LEFT, 85);
	m_Sections.InsertColumn(3, "FOA", LVCFMT_LEFT, 90);
	m_Sections.InsertColumn(4, "FSize", LVCFMT_LEFT, 90);

	PIMAGE_SECTION_HEADER pSectionHeaders = IMAGE_FIRST_SECTION(pNtHeader);
	for (size_t i = 0; i < pFileHeader->NumberOfSections; i++)
	{
		int index = m_Sections.GetItemCount();

		char name[9] = { 0 };
		memcpy(name, pSectionHeaders->Name, 8);
		m_Sections.InsertItem(index, name);
		CString str;
		str.Format("%X", pSectionHeaders->VirtualAddress);
		m_Sections.SetItemText(index, 1, str);

		str.Format("%X", pSectionHeaders->Misc.VirtualSize);
		m_Sections.SetItemText(index, 2, str);

		str.Format("%X", pSectionHeaders->PointerToRawData);
		m_Sections.SetItemText(index, 3, str);

		str.Format("%X", pSectionHeaders->SizeOfRawData);
		m_Sections.SetItemText(index, 4, str);

		//std::string str = std::to_string(1);

	
		printf("区段名称: %s\n", name);

		pSectionHeaders++;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
