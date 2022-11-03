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
	// 解析DOS头
	pDosHeader = (PIMAGE_DOS_HEADER)filebuff;
	if (pDosHeader->e_magic != 0x5A4D)
	{
		printf("不是有效的PE文件\n");
		return FALSE;
	}

	printf("e_lfanew = %x \n", pDosHeader->e_lfanew);

	// 解析NT头
	pNtHeader = (PIMAGE_NT_HEADERS)(pDosHeader->e_lfanew + (DWORD)filebuff);
	if (0x4550 != pNtHeader->Signature)
	{
		printf("不是有效的PE文件Signature\n");
		return FALSE;
	}

	pFileHeader = &pNtHeader->FileHeader;
	pOptHeader = &pNtHeader->OptionalHeader;

	printf("pFileHeader->Machine=%d \n", pFileHeader->Machine);
	printf("pFileHeader->SizeOfOptionalHeader=%d \n", pFileHeader->SizeOfOptionalHeader);
	printf("pFileHeader->Characteristics=%d \n", pFileHeader->Characteristics);
	printf("pOptHeader->AddressOfEntryPoint=%d \n", pOptHeader->AddressOfEntryPoint);
	printf("pOptHeader->NumberOfRvaAndSizes=%d \n", pOptHeader->NumberOfRvaAndSizes);
	
	

	// 解析区段头

	return TRUE;
}

void PEUtils::PrintSectionHeaders()
{
	PIMAGE_SECTION_HEADER pSectionHeaders = IMAGE_FIRST_SECTION(pNtHeader);

	for (size_t i = 0; i < pFileHeader->NumberOfSections; i++)
	{
		char name[9] = { 0 };
		memcpy(name, pSectionHeaders->Name, 8);
		printf("区段名称: %s\n", name);
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
		// 判断va是否在当前区段
		if (rva >= pSectionHeaders->VirtualAddress &&
			rva < pSectionHeaders->VirtualAddress + pSectionHeaders->Misc.VirtualSize)
		{
			// 数据FOA = 数据的RVA - 区段的RVA + 区段的FOA
			return rva - pSectionHeaders->VirtualAddress + pSectionHeaders->PointerToRawData;
		}
		pSectionHeaders++;
	}
	return 0;
}

void PEUtils::PrintDataDirectory_0()
{
	// 0 是导出表
	IMAGE_DATA_DIRECTORY directory = pOptHeader->DataDirectory[0];

	// 找到在文件中导出表信息的位置
	PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)(RvaToFoa(directory.VirtualAddress) + filebuff);
	// dll的名称
	char* dllName = RvaToFoa(pExport->Name) + filebuff;
	printf("dllname: %s\n", dllName);

	DWORD* funcaddr = (DWORD*)(RvaToFoa(pExport->AddressOfFunctions) + filebuff);
	WORD* peot = (WORD*)(RvaToFoa(pExport->AddressOfNameOrdinals) + filebuff);
	DWORD* pent = (DWORD*)(RvaToFoa(pExport->AddressOfNames) + filebuff);

	for (size_t i = 0; i < pExport->NumberOfFunctions; i++)
	{
		printf("函数地址为: %x \n", *funcaddr);
		for (size_t j = 0; j < pExport->NumberOfNames; j++)
		{
			if (peot[j] == i)
			{
				char* funcName = (RvaToFoa(pent[j]) + filebuff);
				printf("函数名称: %s\n", funcName);
				break;
			}
		}
		funcaddr++;
	}
	
}



void PEUtils::PrintDataDirectory_1()
{
	// 1 是导出表
	IMAGE_DATA_DIRECTORY directory = pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

	// 找到在文件中导入表信息的位置
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)(RvaToFoa(directory.VirtualAddress) + filebuff);

	// pImport 是个数组 最后一个数组里面全是0
	while (pImport->OriginalFirstThunk)
	{
		char* dllName = RvaToFoa(pImport->Name) + filebuff;
		printf("dll的TimeDateStamp:[%d] 文件名称为: %s\n", pImport->TimeDateStamp, dllName);

		// 遍历每个dll里面引入的函数
		if (pImport->TimeDateStamp == 0)
		{
			//FirstThunk和OriginalFirstThunk相等 指向导入名称表
			PIMAGE_THUNK_DATA pImgTrunkData = (PIMAGE_THUNK_DATA)(RvaToFoa(pImport->OriginalFirstThunk) + filebuff);
			//包含指向输入名称表的结构的数组 IMAGE_THUNK_DATA
			while (pImgTrunkData->u1.Function)
			{
				if (pImgTrunkData->u1.Ordinal & 0x80000000)
				{
					// 最高位为1 按序号导入 其余31位为序号
					printf("\t按序号导入: %d\n", pImgTrunkData->u1.Ordinal & 0x7FFFFFFF);
				}
				else
				{
					// 0 - 按名称导入
					PIMAGE_IMPORT_BY_NAME pFuncName = (PIMAGE_IMPORT_BY_NAME)(RvaToFoa(pImgTrunkData->u1.AddressOfData) + filebuff);
					printf("\t按名称导入: %s\n", pFuncName->Name);
				}
				pImgTrunkData++;
			}
		}
		else if (pImport->TimeDateStamp == -1)
		{
			//FirstThunk指向真实的地址表
		}

		pImport++;
	}

}
