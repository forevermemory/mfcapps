#pragma once
#include "afxdialogex.h"


// CAssistDialog 对话框

class CAssistDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAssistDialog)

public:
	CAssistDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAssistDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAssistDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
