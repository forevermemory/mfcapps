// CDialogException.cpp: 实现文件
//

#include "pch.h"
#include "LordPE.h"
#include "afxdialogex.h"
#include "CDialogException.h"
#include "CGlobalInfo.h"

// CDialogException 对话框

IMPLEMENT_DYNAMIC(CDialogException, CDialogEx)

CDialogException::CDialogException(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DIRECTORY_EXCEPTION, pParent)
	, m_Exception_RVA(_T(""))
	, m_Exception_Count(_T(""))
{

}

CDialogException::~CDialogException()
{
}

void CDialogException::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EXCEPTION, m_ExceptionList);
	DDX_Text(pDX, IDC_EDIT_EXCEPTION_RVA, m_Exception_RVA);
	DDX_Text(pDX, IDC_EDIT_EXCEPTION_COUNT, m_Exception_Count);
}


BEGIN_MESSAGE_MAP(CDialogException, CDialogEx)
END_MESSAGE_MAP()


// CDialogException 消息处理程序


BOOL CDialogException::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	m_ExceptionList.ModifyStyle(LVS_ICON | LVS_SMALLICON | LVS_LIST, LVS_REPORT);
	m_ExceptionList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_ExceptionList.InsertColumn(0, "index", LVCFMT_LEFT, 70);
	m_ExceptionList.InsertColumn(1, "Start", LVCFMT_LEFT, 70);
	m_ExceptionList.InsertColumn(2, "End", LVCFMT_LEFT, 80);
	m_ExceptionList.InsertColumn(3, "Unwind", LVCFMT_LEFT, 80);

	ParseException();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



BOOL CDialogException::ParseException()
{

#ifdef _WIN64 
	IMAGE_DATA_DIRECTORY directory;

	if (CGlobalInfo::GetInstance()->m_Arch == 64)
	{
		directory = CGlobalInfo::GetInstance()->m_pOptHeader64->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION];
	}
	else
	{
		directory = CGlobalInfo::GetInstance()->m_pOptHeader32->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION];
	}

	int count = directory.Size / sizeof(RUNTIME_FUNCTION);
	CString str1;
	str1.Format("%d", count);
	m_Exception_Count = str1;
	str1.Format("%x", directory.VirtualAddress);
	m_Exception_RVA = str1;

	printf("exportTable:%X\n", directory.Size);
	printf("exportTable:%X\n", directory.VirtualAddress);
	CGlobalInfo* info = CGlobalInfo::GetInstance();

	// 找到在文件中导出表信息的位置

	PRUNTIME_FUNCTION pException = (PRUNTIME_FUNCTION)
		(info->RvaToFoa(directory.VirtualAddress) + info->m_base);


	for (size_t i = 0; i < count; i++)
	{
		printf("%x %x %x \n", pException[i].BeginAddress, pException[i].EndAddress, pException[i].UnwindData);

		int index = m_ExceptionList.GetItemCount();
		CString str;
		str.Format("%d", i + 1);
		m_ExceptionList.InsertItem(index, str);


		str.Format("%X", pException[i].BeginAddress);
		m_ExceptionList.SetItemText(index, 1, str);

		str.Format("%X", pException[i].EndAddress);
		m_ExceptionList.SetItemText(index, 2, str);

		str.Format("%X", pException[i].UnwindData);
		m_ExceptionList.SetItemText(index, 3, str);



	}

#endif // _WIN64 

	

	


	return TRUE;
}
