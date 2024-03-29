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

ULONGLONG CGlobalInfo::RvaToFoa(ULONGLONG rva)
{
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(m_pNtHeader);

	for (size_t i = 0; i < m_pFileHeader->NumberOfSections; i++)
	{
		// 判断va是否在当前区段
		if (rva >= pSectionHeader->VirtualAddress &&
			rva < pSectionHeader->VirtualAddress + pSectionHeader->Misc.VirtualSize)
		{
			// 数据FOA -区段的FOA = 数据的RVA - 区段的RVA 
			// 数据FOA = 数据的RVA - 区段的RVA + 区段的FOA
			return rva - pSectionHeader->VirtualAddress + pSectionHeader->PointerToRawData;
		}
		pSectionHeader++;
	}
	return 0;
}

PIMAGE_SECTION_HEADER CGlobalInfo::RvaFindSection(ULONGLONG rva)
{
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(m_pNtHeader);

	for (size_t i = 0; i < m_pFileHeader->NumberOfSections; i++)
	{
		// 判断va是否在当前区段
		if (rva >= pSectionHeader->VirtualAddress &&
			rva < pSectionHeader->VirtualAddress + pSectionHeader->Misc.VirtualSize)
		{
			// 数据FOA -区段的FOA = 数据的RVA - 区段的RVA 
			// 数据FOA = 数据的RVA - 区段的RVA + 区段的FOA
			return pSectionHeader;
		}
		pSectionHeader++;
	}
	return 0;
}