#pragma once
#include "Winuser.h"
#include "Resource.h"

#define MSG_ICON_NOTIFY WM_USER + 2666
class CTrayIcon
{
public:
	CTrayIcon();
	~CTrayIcon();

public:
	void InitTrayIcon(CWnd* pWnd);

public:
	NOTIFYICONDATA m_nid;
	CWnd* m_pWnd;
};

