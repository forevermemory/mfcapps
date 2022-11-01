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

private:

	char* filebuff;
	DWORD fileSize;

	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNtHeader;
	PIMAGE_FILE_HEADER pFileHeader;
	PIMAGE_OPTIONAL_HEADER pOptHeader;
};

