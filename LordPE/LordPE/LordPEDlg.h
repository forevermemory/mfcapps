
// LordPEDlg.h: 头文件
//

#pragma once
#include <iostream>

// CLordPEDlg 对话框
class CLordPEDlg : public CDialogEx
{
// 构造
public:
	CLordPEDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LORDPE_DIALOG };
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
	afx_msg void OnBnClickedDirectory();
	afx_msg void OnBnClickedSections();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedSelectFile();

	// self
public:
	BOOL ParsePeFile();
	void PrintUI();
	//CString toStr(ULONGLONG d);
  
public:
	PIMAGE_DOS_HEADER m_pDosHeader;
	PIMAGE_FILE_HEADER m_pFileHeader;
	PIMAGE_NT_HEADERS m_pNtHeader;
	
	PIMAGE_OPTIONAL_HEADER32 m_pOptHeader32;
	PIMAGE_OPTIONAL_HEADER64 m_pOptHeader64;
	//#ifdef _WIN64
	//	PIMAGE_NT_HEADERS64 m_pNtHeader;
	//	PIMAGE_OPTIONAL_HEADER64 m_pOptHeader;
	//#else
	//	PIMAGE_OPTIONAL_HEADER32 m_pOptHeader;
	//	PIMAGE_NT_HEADERS32 m_pNtHeader;
	//#endif 

public:
	char m_szPath[MAX_PATH] = { 0 };
	
	CString m_szPath2;
	CString m_Machine;
	CString m_NumberOfSections;
	CString m_SizeOfOptionalHeader;
	CString m_TimeDateStamp;
	CString m_Subsystem;
	afx_msg void OnBnClickedBtnSubsystemExt();
	CMFCButton m_Btn_Sections;
	CMFCButton m_Btn_SelectFile;
	CMFCButton m_Btn_Directory;
	CMFCButton m_Btn_Position_Calc;
	afx_msg void OnBnClickedBtnPositionCale();
	CString m_AddressOfEntryPoint;
	CString m_ImageBase;
	CString m_SizeOfImage;
	CString m_BaseOfCode;
	CString m_BaseOfData;
	CString m_NumberOfRvaAndSizes;
	CString m_FileAlignment;
	CString m_SectionAlignment;
};
