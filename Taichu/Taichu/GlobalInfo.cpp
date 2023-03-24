#include "pch.h"
#include "GlobalInfo.h"


GlobalInfo* GlobalInfo::m_pInstance = NULL;

GlobalInfo::GlobalInfo(void)
{
	m_hWndDlgMain = NULL;
}

GlobalInfo::~GlobalInfo()
{
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
	}

}


GlobalInfo* GlobalInfo::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new GlobalInfo;
	}

	return m_pInstance;
}
