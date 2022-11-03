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
	printf("pOptHeader->AddressOfEntryPoint=%d \n", pOptHeader->AddressOfEntryPoint);
	printf("pOptHeader->NumberOfRvaAndSizes=%d \n", pOptHeader->NumberOfRvaAndSizes);
	
	

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

void PEUtils::PrintDataDirectorys()
{



}

DWORD PEUtils::RvaToFoa(DWORD rva)
{
	PIMAGE_SECTION_HEADER pSectionHeaders = IMAGE_FIRST_SECTION(pNtHeader);

	for (size_t i = 0; i < pFileHeader->NumberOfSections; i++)
	{
		// �ж�va�Ƿ��ڵ�ǰ����
		if (rva >= pSectionHeaders->VirtualAddress &&
			rva < pSectionHeaders->VirtualAddress + pSectionHeaders->Misc.VirtualSize)
		{
			// ����FOA = ���ݵ�RVA - ���ε�RVA + ���ε�FOA
			return rva - pSectionHeaders->VirtualAddress + pSectionHeaders->PointerToRawData;
		}
		pSectionHeaders++;
	}
	return 0;
}

void PEUtils::PrintDataDirectory_0()
{
	// 0 �ǵ�����
	IMAGE_DATA_DIRECTORY directory = pOptHeader->DataDirectory[0];

	// �ҵ����ļ��е�������Ϣ��λ��
	PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)(RvaToFoa(directory.VirtualAddress) + filebuff);
	// dll������
	char* dllName = RvaToFoa(pExport->Name) + filebuff;
	printf("dllname: %s\n", dllName);

	DWORD* funcaddr = (DWORD*)(RvaToFoa(pExport->AddressOfFunctions) + filebuff);
	WORD* peot = (WORD*)(RvaToFoa(pExport->AddressOfNameOrdinals) + filebuff);
	DWORD* pent = (DWORD*)(RvaToFoa(pExport->AddressOfNames) + filebuff);

	for (size_t i = 0; i < pExport->NumberOfFunctions; i++)
	{
		printf("������ַΪ: %x \n", *funcaddr);
		for (size_t j = 0; j < pExport->NumberOfNames; j++)
		{
			if (peot[j] == i)
			{
				char* funcName = (RvaToFoa(pent[j]) + filebuff);
				printf("��������: %s\n", funcName);
				break;
			}
		}
		funcaddr++;
	}
	
}



void PEUtils::PrintDataDirectory_1()
{
	// 1 �ǵ�����
	IMAGE_DATA_DIRECTORY directory = pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

	// �ҵ����ļ��е������Ϣ��λ��
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)(RvaToFoa(directory.VirtualAddress) + filebuff);

	// pImport �Ǹ����� ���һ����������ȫ��0
	while (pImport->OriginalFirstThunk)
	{
		char* dllName = RvaToFoa(pImport->Name) + filebuff;
		printf("dll��TimeDateStamp:[%d] �ļ�����Ϊ: %s\n", pImport->TimeDateStamp, dllName);

		// ����ÿ��dll��������ĺ���
		if (pImport->TimeDateStamp == 0)
		{
			//FirstThunk��OriginalFirstThunk��� ָ�������Ʊ�
			PIMAGE_THUNK_DATA pImgTrunkData = (PIMAGE_THUNK_DATA)(RvaToFoa(pImport->OriginalFirstThunk) + filebuff);
			//����ָ���������Ʊ�Ľṹ������ IMAGE_THUNK_DATA
			while (pImgTrunkData->u1.Function)
			{
				if (pImgTrunkData->u1.Ordinal & 0x80000000)
				{
					// ���λΪ1 ����ŵ��� ����31λΪ���
					printf("\t����ŵ���: %d\n", pImgTrunkData->u1.Ordinal & 0x7FFFFFFF);
				}
				else
				{
					// 0 - �����Ƶ���
					PIMAGE_IMPORT_BY_NAME pFuncName = (PIMAGE_IMPORT_BY_NAME)(RvaToFoa(pImgTrunkData->u1.AddressOfData) + filebuff);
					printf("\t�����Ƶ���: %s\n", pFuncName->Name);
				}
				pImgTrunkData++;
			}
		}
		else if (pImport->TimeDateStamp == -1)
		{
			//FirstThunkָ����ʵ�ĵ�ַ��
		}

		pImport++;
	}

}
