
// TestDriverDlg.h: 头文件
//

#pragma once


// CTestDriverDlg 对话框
class CTestDriverDlg : public CDialogEx
{
// 构造
public:
	CTestDriverDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTDRIVER_DIALOG };
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
	afx_msg void OnBnClickedButtonOpenSys();
	afx_msg void OnBnClickedButtonCloseSys();
	afx_msg void OnBnClickedButtonIoControl();
	DWORD m_PID;
	afx_msg void OnBnClickedButtonGetProcessName();
	afx_msg void OnBnClickedButtonAddPidProtect();
	afx_msg void OnBnClickedButtonRemovePidProtect2();
	afx_msg void OnBnClickedButtonBufferRead();
	afx_msg void OnBnClickedButtonBufferWrite();
	UINT64 m_TargetPid;
	afx_msg void OnBnClickedButtonMdlRead();
	afx_msg void OnBnClickedButtonMdlWrite();
	afx_msg void OnBnClickedButtonMdlWriteReadonly1();
	afx_msg void OnBnClickedButtonCr0WriteReadonly2();
	afx_msg void OnBnClickedButtonPhWriteReadonly3();
	afx_msg void OnBnClickedButtonPhRead();
	afx_msg void OnBnClickedButtonPhWriteReadonly4();
	afx_msg void OnBnClickedButtonPhWriteReadonly5();
	afx_msg void OnBnClickedButtonEnumObCallbacls();
	UINT64 m_Pid_Privilege;
	afx_msg void OnBnClickedButtonRemovePrivilege();
	afx_msg void OnBnClickedButtonAddPrivilege();
};
