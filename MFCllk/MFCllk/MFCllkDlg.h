
// MFCllkDlg.h: 头文件
//

#pragma once

typedef struct _Ele {
	DWORD x;
	DWORD y;
	DWORD val;
	BOOL visited;

} Ele, * PEle;
// CMFCllkDlg 对话框
class CMFCllkDlg : public CDialogEx
{
// 构造
public:
	CMFCllkDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCLLK_DIALOG };
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
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	void InitGameData();
	afx_msg void cwllk();
	
	afx_msg void OnBnClickedButton9();
	CString m_Data;
	afx_msg void OnBnClickedButtonxian0();

public:
	DWORD m_gamedata[18][14] = { 0 };
	DWORD m_gamedata2[22][14] = { 0 };
	PEle m_gamedataPtr[22][14] = { 0 };
	BOOL m_Time;
	BOOL m_Life;
	
	afx_msg void OnClickedCheck1();
	BOOL m_Tishi;
	afx_msg void OnClickedCheck2();

	void resetVisited();
	afx_msg void OnBnClickedButtonxian1();


	void SendClick(short x, short y, DWORD mode);
	void SendClickMode2(PEle a, PEle b, DWORD mode);
	DWORD GetGameLevel();

	DWORD m_Total1 = 0;
	DWORD m_Total2 = 0;
	DWORD m_Total3 = 0;
	afx_msg void OnBnClickedButtonzhexian2();

	void UpdateText();
	
	// 查找一折
	PEle FindOneTop(PEle cur, PEle pos);
	PEle FindOneDown(PEle cur, PEle pos);
	PEle FindOneLeft(PEle cur, PEle pos);
	PEle FindOneRight(PEle cur, PEle pos);

	// 查找二折
	PEle FindTwoTopLeftRight(PEle cur, PEle pos);
	PEle FindTwoDownLeftRight(PEle cur, PEle pos);

	PEle FindTwoLeftTopDown(PEle cur, PEle pos);
	PEle FindTwoRightTopDown(PEle cur, PEle pos);

	afx_msg void OnBnClickedButtonzhexian3();
	afx_msg void OnBnClickedCheck3();
};
