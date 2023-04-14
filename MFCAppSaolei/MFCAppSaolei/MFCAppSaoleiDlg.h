
// MFCAppSaoleiDlg.h: 头文件
//

#pragma once


// CMFCAppSaoleiDlg 对话框
class CMFCAppSaoleiDlg : public CDialogEx
{
// 构造
public:
	CMFCAppSaoleiDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPSAOLEI_DIALOG };
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
	int m_remain;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	void InitCwndHandle();
	HWND m_hwnd;
	afx_msg void OnBnClickedButton5();
	CString m_Data;
	afx_msg void OnBnClickedButton6();
};
