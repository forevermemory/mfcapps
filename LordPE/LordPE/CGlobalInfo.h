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
	ULONGLONG RvaToFoa(ULONGLONG rva);

public:
	WORD m_CDialogSubsystemStatus = -1;

	DWORD m_Arch = 0;
	ULONGLONG m_base = 0;
	PIMAGE_DOS_HEADER m_pDosHeader;
	PIMAGE_FILE_HEADER m_pFileHeader;
	PIMAGE_NT_HEADERS m_pNtHeader;

	PIMAGE_OPTIONAL_HEADER32 m_pOptHeader32;
	PIMAGE_OPTIONAL_HEADER64 m_pOptHeader64;

};
