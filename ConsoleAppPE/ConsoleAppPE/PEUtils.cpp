#include "PEUtils.h"

#include <stdio.h>
#pragma warning(disable: 4996)
PEUtils::PEUtils()
{
}

PEUtils::~PEUtils()
{
	delete[] filebuff;
}

BOOL PEUtils::LoadFile(const char* path)
{
	HANDLE hFile = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	fileSize = GetFileSize(hFile, NULL);
	filebuff = new char[fileSize];

	BOOL ret;
	DWORD realRead = 0;
	ret = ReadFile(hFile, filebuff, fileSize, &realRead, NULL);
	if (!ret)
	{
		goto FAIL;
	}

	CloseHandle(hFile);
	return TRUE;

FAIL:

	CloseHandle(hFile);
	return FALSE;
}

BOOL PEUtils::InitPeInfo()
{
	// ����DOSͷ
	pDosHeader = (PIMAGE_DOS_HEADER)filebuff;
	if (pDosHeader->e_magic != 0x5A4D)
	{
		printf("������Ч��PE�ļ�\n");
		return FALSE;
	}

	printf("e_lfanew = %x \n", pDosHeader->e_lfanew);

	// ����NTͷ
	pNtHeader = (PIMAGE_NT_HEADERS)(pDosHeader->e_lfanew + (DWORD)filebuff);
	if (0x4550 != pNtHeader->Signature)
	{
		printf("������Ч��PE�ļ�Signature\n");
		return FALSE;
	}

	pFileHeader = &pNtHeader->FileHeader;
	pOptHeader = &pNtHeader->OptionalHeader;

	printf("pFileHeader->Machine=%d \n", pFileHeader->Machine);
	printf("pFileHeader->SizeOfOptionalHeader=%d \n", pFileHeader->SizeOfOptionalHeader);
	printf("pFileHeader->Characteristics=%d \n", pFileHeader->Characteristics);

	// ��������ͷ

	return TRUE;
}

void PEUtils::PrintSectionHeaders()
{
	PIMAGE_SECTION_HEADER pSectionHeaders = IMAGE_FIRST_SECTION(pNtHeader);

	for (size_t i = 0; i < pFileHeader->NumberOfSections; i++)
	{
		char name[9] = { 0 };
		memcpy(name, pSectionHeaders->Name, 8);
		printf("��������: %s\n", name);
		pSectionHeaders++;
	}
}
