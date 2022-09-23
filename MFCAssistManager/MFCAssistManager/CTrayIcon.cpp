#include "pch.h"
#include "CTrayIcon.h"

#pragma warning(disable:4996)
CTrayIcon::CTrayIcon()
{
	memset(&m_nid, 0, sizeof(NOTIFYICONDATA));
}

CTrayIcon::~CTrayIcon()
{
	Shell_NotifyIcon(NIM_DELETE, &m_nid);
}

 
void CTrayIcon::InitTrayIcon(CWnd* pWnd)
{
	m_nid.cbSize = sizeof(NOTIFYICONDATA);
	m_nid.hWnd = pWnd->GetSafeHwnd();

	CString str;
	pWnd->GetWindowText(str);
	strcpy(m_nid.szTip, str);

	// 单机或者双击发送自定义消息
	m_nid.uCallbackMessage = MSG_ICON_NOTIFY;
	m_nid.uID = IDI_ICON1;
	m_nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
	m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &m_nid);

}