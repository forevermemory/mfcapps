
// MFCPlantzombieDlg.h: 头文件
//

#pragma once


// CMFCPlantzombieDlg 对话框
class CMFCPlantzombieDlg : public CDialogEx
{
// 构造
public:
	CMFCPlantzombieDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCPLANTZOMBIE_DIALOG };
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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCheck1();
	BOOL m_PlatCD;
	afx_msg void OnBnClickedButton4();
	//void __stdcall PutBoom(UINT plantid, UINT x, UINT y);
	afx_msg void OnCbnSelchangeComPlants();
	CComboBox m_Plants;
	afx_msg void OnBnClickedButton3();
	afx_msg void AutoCallPutPlant();

	void AutoCallPutZombie();
	UINT m_plantid;
	UINT m_zombieid;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedCheck2();
	BOOL m_gameStatus;
	afx_msg void OnBnClickedCheckBlood();
	BOOL m_Plant_blood;
	CComboBox m_Zombies;
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedCheckNoDec();
	BOOL m_NoSun;
};
