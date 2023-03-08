#include "pch.h"
#include "CGlobalInfo.h"


CGlobalInfo* CGlobalInfo::m_pInstance = NULL;

CGlobalInfo::CGlobalInfo(void)
{

}


CGlobalInfo::~CGlobalInfo(void)
{

}

CGlobalInfo* CGlobalInfo::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CGlobalInfo;
	}
	return m_pInstance;
}