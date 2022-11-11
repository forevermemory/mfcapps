#pragma once

#include <Windows.h>



class PEUtils
{
public:
	PEUtils();
	~PEUtils();

	BOOL LoadFile(const char* path);
	BOOL InitPeInfo();
	void PrintSectionHeaders();
	void PrintDataDirectorys();
	DWORD RvaToFoa(DWORD rva);
	void PrintDataDirectory_0(); // ����������
	void PrintDataDirectory_1(); // ���������
	void PrintDataDirectory_5(); // �����ض�λ��
	void PrintDataDirectory_9(); // ����TLS��

private:

	char* filebuff;
	DWORD fileSize;

	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNtHeader;
	PIMAGE_FILE_HEADER pFileHeader;
	PIMAGE_OPTIONAL_HEADER pOptHeader;
};

