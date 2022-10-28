// CDlgProcessList.cpp: 实现文件
//

#include "pch.h"
#include "MFCDllInject.h"
#include "afxdialogex.h"
#include "CDlgProcessList.h"
#include <tlhelp32.h>

// CDlgProcessList 对话框

IMPLEMENT_DYNAMIC(CDlgProcessList, CDialogEx)

CDlgProcessList::CDlgProcessList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PROCESS_LIST, pParent)
{

}

CDlgProcessList::~CDlgProcessList()
{
}

void CDlgProcessList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST3, m_Process);
}


BEGIN_MESSAGE_MAP(CDlgProcessList, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS_CONFIRM, &CDlgProcessList::OnBnClickedButtonProcessConfirm)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS_REFRESH, &CDlgProcessList::OnBnClickedButtonProcessRefresh)
END_MESSAGE_MAP()


// CDlgProcessList 消息处理程序


BOOL CDlgProcessList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	RefreshProcessList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDlgProcessList::RefreshProcessList()
{
	// 实现方式是获取进程快照
	HANDLE hSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	BOOL ret = ::Process32First(hSnapShot, &pe32);

	// 遍历每个进程名称和目标进程比较
	while (ret)
	{
		CString tmp = pe32.szExeFile;

		int index = m_Process.AddString(tmp);
		m_Process.SetItemData(index, pe32.th32ProcessID);

		//if (tmp.Compare("WXWork.exe")==0)
		//{
		//	int index = m_Process.AddString(tmp);
		//	m_Process.SetItemData(index, pe32.th32ProcessID);
		//}


		///////////////////////////////////////
		//int index2 = m_Precsss_lists.GetItemCount();
		//m_Precsss_lists.InsertItem(index2, tmp);
		//m_Precsss_lists.SetItemText(index2, 1, tmp);
		//m_Precsss_lists.SetItemData(index2, pe32.th32ProcessID);

		//LoadBitmapA()
		//m_Process.SetItemData(index, IDI_ICON1);
		ret = ::Process32Next(hSnapShot, &pe32);
	}

	::CloseHandle(hSnapShot);
}

void CDlgProcessList::OnBnClickedButtonProcessConfirm()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_Process.GetCurSel();

	DWORD pid =  m_Process.GetItemData(index);
	//CString str;
	//str.Format("--%d--%d", index, pid);
	//AfxMessageBox(str);
	// 保存下来
	m_Pid = pid;

	OnOK();
}


void CDlgProcessList::OnBnClickedButtonProcessRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Process.ResetContent();
	RefreshProcessList();
}
