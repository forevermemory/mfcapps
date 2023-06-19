#pragma once
#include "afxdialogex.h"


// CDialogSubsystem 对话框

class CDialogSubsystem : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogSubsystem)

public:
	CDialogSubsystem(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogSubsystem();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SUBSYSTEM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButton m_Btn_Unknown;
	CButton m_Btn_Native;
	CButton m_Btn_Windows_gui;
	CButton m_Btn_Windows_Cui;
	CButton m_Btn_OS2_Cui;
	CButton m_Btn_EFI_application;
	CButton m_Btn_EFI_Boot_Service_driver;
	CButton m_Btn_EFI_Runtime_Driver;
	CButton m_Btn_EFI_Rom;
	CButton m_Btn_XBOX;
	CButton m_Btn_Boot_Application;
	CButton m_Btn_Posix_Cui;
	CButton m_Btn_Windows_Ce_GUI;
	virtual BOOL OnInitDialog();
};
