// CDialogIAT.cpp: 实现文件
//

#include "pch.h"
#include "LordPE.h"
#include "afxdialogex.h"
#include "CDialogIAT.h"
#include "CGlobalInfo.h"


// CDialogIAT 对话框

IMPLEMENT_DYNAMIC(CDialogIAT, CDialogEx)

CDialogIAT::CDialogIAT(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_IAT, pParent)
	, m_Detail(_T(""))
	//, m_Detail(_T(""))
{

}

CDialogIAT::~CDialogIAT()
{
}

void CDialogIAT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_STATIC_DETAIL, m_Detail);
	//DDX_Text(pDX, IDC_LIST2, m_Detail);
	DDX_Text(pDX, IDC_EDIT2, m_Detail);
}


BEGIN_MESSAGE_MAP(CDialogIAT, CDialogEx)
END_MESSAGE_MAP()


// CDialogIAT 消息处理程序


BOOL CDialogIAT::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ParseIAT();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL CDialogIAT::ParseIAT()
{

	// TODO: 在此添加控件通知处理程序代码
	IMAGE_DATA_DIRECTORY directory;

	if (CGlobalInfo::GetInstance()->m_Arch == 64)
	{
		directory = CGlobalInfo::GetInstance()->m_pOptHeader64->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	}
	else
	{
		directory = CGlobalInfo::GetInstance()->m_pOptHeader32->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

	}

	CGlobalInfo* info = CGlobalInfo::GetInstance();
	PIMAGE_IMPORT_DESCRIPTOR  pImport = (PIMAGE_IMPORT_DESCRIPTOR)(info->RvaToFoa(directory.VirtualAddress) + info->m_base);


	while (pImport->Characteristics)
	{
		if (pImport->TimeDateStamp == -1) {

			char* dllName = (char*)(info->RvaToFoa(pImport->Name) + info->m_base);
			printf("\tdllName:【%s】:\n", dllName);

			m_Detail += dllName;
			m_Detail += "\r\n";

			DWORD* addr = (DWORD*)(info->RvaToFoa(pImport->FirstThunk) + info->m_base);
			while (*addr)
			{
				printf("\tFunction Addr:[%08X]\n", *addr);
				CString tmp;
				tmp.Format("\tFunction Addr:[%08X]\r\n", *addr);
				m_Detail += tmp;
				addr++;
			}
		}
		else if (pImport->TimeDateStamp == 0) {//等同于INT表
			printf("\t等同于INT表!\n");
			m_Detail += "等同于INT表!\n";
			break;
		}

		pImport++;
	}

	UpdateData(FALSE);
	return TRUE;
}