// CMainDialog.cpp: 实现文件
//

#include "pch.h"
#include "MFCLibrary1.h"
#include "afxdialogex.h"
#include "CMainDialog.h"
#include "GlobalInfo.h"
#include "Form1.h"
#include "Form2.h"
#include "Form3.h"
#include "FormBase.h"


// CMainDialog 对话框

IMPLEMENT_DYNAMIC(CMainDialog, CDialogEx)

CMainDialog::CMainDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CMainDialog::~CMainDialog()
{
}

void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ITEMS, m_Assist_items);
}


BEGIN_MESSAGE_MAP(CMainDialog, CDialogEx)
	ON_MESSAGE(WM_DLL_EXIT_DLG, &CMainDialog::OnDllExitDlg)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_ITEMS, &CMainDialog::OnClickListItems)
END_MESSAGE_MAP()


// CMainDialog 消息处理程序


afx_msg LRESULT CMainDialog::OnDllExitDlg(WPARAM wParam, LPARAM lParam)
{
	// 释放资源


	OnClose();
	return 0;
}


void CMainDialog::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}

void CMainDialog::CreateAssistItems()
{
	CImageList imgList;
	imgList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 0, 0);
	m_Assist_items.SetImageList(&imgList, LVSIL_SMALL);

	imgList.Add(::AfxGetApp()->LoadIconA(IDI_ICON1));
	imgList.Detach();

	CWnd * img =  GetDlgItem(IDC_PIC_CONTAINER);
	CRect rect;
	//img->GetClientRect(&rect);
	img->GetWindowRect(&rect);


	m_Assist_items.InsertItem(0, "挂机", 0);
	Form1* f1 = new Form1;
	f1->Create(IDD_FORM1, this);
	f1->MoveWindow(&rect);
	f1->ShowWindow(SW_SHOW);
	m_Assist_items.SetItemData(0, (DWORD)(FormBase*)f1);

	m_Assist_items.InsertItem(1, "技能", 0);
	Form2* f2 = new Form2;
	f2->Create(IDD_FORM2, this);
	f2->MoveWindow(&rect);
	m_Assist_items.SetItemData(1, (DWORD)(FormBase*)f2);

	m_Assist_items.InsertItem(2, "保护", 0);
	Form3* f3 = new Form3;
	f3->Create(IDD_FORM3, this);
	f3->MoveWindow(&rect);
	m_Assist_items.SetItemData(2, (DWORD)(FormBase*)f3);
}

BOOL CMainDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CTime tm = CTime::GetCurrentTime();

	CString str;
	str.Format("用户名:%s-完美辅助", tm.Format("%H:%M:%S"));
	SetWindowText(str);

	// 保存窗口句柄
	GlobalInfo::GetInstance()->m_hWndDlgMain = m_hWnd;

	// 添加辅助项 m_Assist_items
	CreateAssistItems();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CMainDialog::OnClickListItems(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMItemActivate->iItem == -1)return;

	//CString str;
	//str.Format("cur:%d, %p", pNMItemActivate->iItem, pForm);
	//AfxMessageBox(str);
	
	// 隐藏所有
	FormBase* pForm = NULL;
	for (int i = 0; i < m_Assist_items.GetItemCount(); i++)
	{
		pForm = (FormBase*)m_Assist_items.GetItemData(i);

		if (pForm) pForm->GetWnd()->ShowWindow(SW_HIDE);
	}

	// 显示当前点中
	pForm = (FormBase*)m_Assist_items.GetItemData(pNMItemActivate->iItem);
	if (pForm) pForm->GetWnd()->ShowWindow(SW_SHOW);

	*pResult = 0;
}
