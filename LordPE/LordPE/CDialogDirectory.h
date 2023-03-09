#pragma once
#include "afxdialogex.h"


// CDialogDirectory 对话框

class CDialogDirectory : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogDirectory)

public:
	CDialogDirectory(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogDirectory();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DIRECTORY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	BOOL ParseExport();

public:
	virtual BOOL OnInitDialog();
	CString m_Export_RVA;
	
	CString m_Export_Size;
	CString m_Import_RVA;
	CString m_Import_Size;
	CString m_Resource_RVA;
	CString m_Resource_Size;
	CString m_Exception_RVA;
	CString m_Exception_Size;
	CString m_Security_RVA;
	CString m_Security_Size;
	CString m_Relocation_RVA;
	CString m_Relocation_Size;
	CString m_Debug_RVA;
	CString m_Debug_Size;
	CString m_Architecture_RVA;
	CString m_Architecture_Size;
	CString m_GlobalPtr_RVA;
	CString m_GlobalPtr_Size;
	CString m_TLS_RVA;
	CString m_TLS_Size;
	CString m_LoadConfig_RVA;
	CString m_LoadConfig_Size;
	CString m_BoundImport_RVA;
	CString m_BoundImport_Size;
	CString m_IAT_RVA;
	CString m_IAT_Size;
	CString m_DelayLoad_RVA;
	CString m_DelayLoad_Size;
	CString m_COM_RVA;
	CString m_COM_Size;
	CString m_Reserved_RVA;
	CString m_Reserved_Size;
};
