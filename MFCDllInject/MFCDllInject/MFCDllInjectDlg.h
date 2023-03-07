
// MFCDllInjectDlg.h: 头文件
//

#pragma once


// CMFCDllInjectDlg 对话框
class CMFCDllInjectDlg : public CDialogEx
{
// 构造
public:
	CMFCDllInjectDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCDLLINJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonInject();
	afx_msg void OnBnClickedButtonUninstall();
	afx_msg void OnBnClickedButtonOpenprocess();
	afx_msg void OnBnClickedButtonSelectDll();


public:
	CString m_DllPath;

	int m_Select_Pid;
	afx_msg void OnBnClickedButtonEipInject();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonOpenprocess2();
};
