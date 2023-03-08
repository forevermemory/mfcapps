#pragma once

#include <Windows.h>

class CGlobalInfo
{
public:
	static CGlobalInfo* GetInstance();

	CGlobalInfo(void);
	~CGlobalInfo(void);

private :
	static CGlobalInfo* m_pInstance;

public:
	WORD m_CDialogSubsystemStatus = -1;

	PIMAGE_DOS_HEADER m_pDosHeader;
	PIMAGE_FILE_HEADER m_pFileHeader;
	PIMAGE_NT_HEADERS m_pNtHeader;

};

