// CDialogSubsystem.cpp: 实现文件
//

#include "pch.h"
#include "LordPE.h"
#include "afxdialogex.h"
#include "CDialogSubsystem.h"
#include "CGlobalInfo.h"


// CDialogSubsystem 对话框

IMPLEMENT_DYNAMIC(CDialogSubsystem, CDialogEx)

CDialogSubsystem::CDialogSubsystem(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SUBSYSTEM, pParent)
{

}

CDialogSubsystem::~CDialogSubsystem()
{
}

void CDialogSubsystem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_SUBSYSTEM_UNKNOWN, m_Btn_Unknown);
	DDX_Control(pDX, IDC_CHECKSUBSYSTEM_NATIVE, m_Btn_Native);
	DDX_Control(pDX, IDC_CHECK_SUBSYSTEM_WINDOWS_GUI, m_Btn_Windows_gui);
	DDX_Control(pDX, IDC_CHECK_SUBSYSTEM_WINDOWS_CUI, m_Btn_Windows_Cui);
	DDX_Control(pDX, IDC_CHECK_SUBSYSTEM_OS2_CUI, m_Btn_OS2_Cui);
	DDX_Control(pDX, IDC_CHECK_SUBSYSTEM_EFI_APPLICATION, m_Btn_EFI_application);
	DDX_Control(pDX, IDC_CHECK_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER, m_Btn_EFI_Boot_Service_driver);
	DDX_Control(pDX, IDC_CHECK_SUBSYSTEM_EFI_RUNTIME_DRIVER, m_Btn_EFI_Runtime_Driver);
	DDX_Control(pDX, IDC_CHECK_SUBSYSTEM_EFI_ROM, m_Btn_EFI_Rom);
	DDX_Control(pDX, IDC_CHECK_SUBSYSTEM_XBOX, m_Btn_XBOX);
	DDX_Control(pDX, IDC_CHECK_SUBSYSTEM_WINDOWS_BOOT_APPLICATION, m_Btn_Boot_Application);
	DDX_Control(pDX, IDC_CHECK_SUBSYSTEM_POSIX_CUI, m_Btn_Posix_Cui);
	DDX_Control(pDX, IDC_CHECK_SUBSYSTEM_WINDOWS_CE_GUI, m_Btn_Windows_Ce_GUI);
}


BEGIN_MESSAGE_MAP(CDialogSubsystem, CDialogEx)
END_MESSAGE_MAP()


// CDialogSubsystem 消息处理程序


BOOL CDialogSubsystem::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	switch (CGlobalInfo::GetInstance()->m_CDialogSubsystemStatus)
	{
	case IMAGE_SUBSYSTEM_UNKNOWN:
		m_Btn_Unknown.SetCheck(1);
		break;
	case IMAGE_SUBSYSTEM_NATIVE:
		m_Btn_Native.SetCheck(1);
		break;
	case IMAGE_SUBSYSTEM_WINDOWS_GUI:
		m_Btn_Windows_gui.SetCheck(1);
		break;
	case IMAGE_SUBSYSTEM_WINDOWS_CUI:
		m_Btn_Windows_Cui.SetCheck(1);
		break;
	case IMAGE_SUBSYSTEM_OS2_CUI:
		m_Btn_OS2_Cui.SetCheck(1);
		break;
	case IMAGE_SUBSYSTEM_POSIX_CUI:
		m_Btn_Posix_Cui.SetCheck(1);
		break;
	case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:
		m_Btn_Windows_Ce_GUI.SetCheck(1);
		break;
	case IMAGE_SUBSYSTEM_EFI_APPLICATION:
		m_Btn_EFI_application.SetCheck(1);
		break;
	case IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER:
		m_Btn_EFI_Boot_Service_driver.SetCheck(1);
		break;
	case IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER:
		m_Btn_EFI_Runtime_Driver.SetCheck(1);
		break;
	case IMAGE_SUBSYSTEM_EFI_ROM:
		m_Btn_EFI_Rom.SetCheck(1);
		break;
	case IMAGE_SUBSYSTEM_XBOX:
		m_Btn_XBOX.SetCheck(1);
		break;
	case IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION:
		m_Btn_Boot_Application.SetCheck(1);
		break;
	default:
		break;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
