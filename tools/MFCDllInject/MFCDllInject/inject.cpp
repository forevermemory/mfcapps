#include "pch.h"
#include "inject.h"
#include <TlHelp32.h>
#include <Windows.h>
#include <psapi.h>

// 32λ�ĳ���dll��ַ�ռ���ܲ��������ģ��м��п����ǶϿ���
// ����: kernel32.dll����ʼ��ַΪ0x77660000 ��СΪ:0xF0000
// ֱ�Ӷ�ȡ��ַ77660000����СΪF0000����ᱨ��299�����������ڴ�
BOOL GetProcessDllFunctionsByNameX32(IN DWORD processID, IN CString dllname, IN CString findFuncName, OUT ULONG64* funcAddr)
{

	BOOL isWow32;
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	IsWow64Process(hProcess, &isWow32);
	// ����trueΪ32λ����
	if (!isWow32)
	{
		printf("����processid %d is x64\n", processID);
		return FALSE;
	}

	BOOL ret;
	BOOL isFind = FALSE;
	DWORD cbNeeded = 0;
	void* buffer = 0;
	HMODULE lphModule[1024] = { 0 };
	MODULEINFO modinfo = {0};
	EnumProcessModulesEx(hProcess, lphModule, sizeof(lphModule), &cbNeeded, LIST_MODULES_ALL);

	for (size_t i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
	{
		char szModName[MAX_PATH] = {0};
		// Get the full path to the module's file.
		//ret = GetModuleFileNameExA(hProcess, lphModule[i], szModName, sizeof(szModName) / sizeof(char));
		ret = GetModuleBaseNameA(hProcess, lphModule[i], szModName, sizeof(szModName) / sizeof(char));
		if (!ret)
		{
			printf("GetModuleFileNameExA err %d\n", GetLastError());
			continue;
		}

		// not match 
		if (0 != _stricmp(szModName, dllname))
		{
			printf("not match: %s %s\n", dllname, szModName);
			continue;
		}

		/////////// success match target dll
		// GetModuleInformation 
		ret = GetModuleInformation(hProcess, lphModule[i], &modinfo, sizeof(MODULEINFO));
		if (ret == 0)
		{
			printf("GetModuleInformation err : %d\n", GetLastError());
			return FALSE;
		}

		printf("\t GetModuleInformation ret %d \n", ret);
		printf("\t GetModuleInformation %llX \n", modinfo.lpBaseOfDll);
		printf("\t GetModuleInformation %llX \n", modinfo.SizeOfImage);

		// read process memory 
		// 
		//buffer = (void*)VirtualAlloc(NULL, modinfo.SizeOfImage, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		//SIZE_T realRead = 0;
		// ��������,��������м��п�ȱҳ�ڴ棬�޷�����ֱ��ȫ����ȡ����
		//ret = ReadProcessMemory(hProcess, modinfo.lpBaseOfDll, buffer, modinfo.SizeOfImage, &realRead);
	

		break;
	}



	printf("start parse =============\n");
	// ����������

	LPVOID dllBase = modinfo.lpBaseOfDll;

	buffer = (void*)VirtualAlloc(NULL, modinfo.SizeOfImage, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	SIZE_T realRead = 0;
	ReadProcessMemory(hProcess, dllBase, buffer, sizeof(IMAGE_DOS_HEADER), &realRead);
	PIMAGE_DOS_HEADER	pDosHeader = (PIMAGE_DOS_HEADER)(buffer);


	ReadProcessMemory(hProcess, dllBase, buffer, 
		sizeof(IMAGE_NT_HEADERS32)+ sizeof(IMAGE_DOS_HEADER)+ pDosHeader->e_lfanew, &realRead);
	PIMAGE_NT_HEADERS32	pNtHeader = (PIMAGE_NT_HEADERS32)(ULONG64(buffer) + pDosHeader->e_lfanew);

	//printf("\t realRead %X %d %d \n", realRead, realRead, sizeof(IMAGE_NT_HEADERS32));
	//printf("\t DosHeader %p \n", pDosHeader);
	//printf("\t pNtHeader %p \n", pNtHeader);
	//printf("\t DosHeader e_lfanew %d \n", pDosHeader->e_lfanew);

	// ������
	IMAGE_DATA_DIRECTORY exp = pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

	//printf("\t exportRVA exp %p \n", exp);
	//printf("\t exportRVA Size %X \n", exp.Size);
	//printf("\t exportRVA VirtualAddress %X \n", exp.VirtualAddress);

	// ��ȡ������
	ReadProcessMemory(hProcess,(LPVOID)((DWORD)modinfo.lpBaseOfDll + exp.VirtualAddress), buffer, exp.Size, &realRead);

	PIMAGE_EXPORT_DIRECTORY	pExport = (PIMAGE_EXPORT_DIRECTORY)buffer;
	//printf("\t pExport->NumberOfFunctions %X \n", pExport->NumberOfFunctions);
	//printf("\t pExport->NumberOfFunctions %X \n", pExport->NumberOfFunctions);


	if (pExport->NumberOfFunctions > 0)
	{
		DWORD mSize = pExport->NumberOfFunctions * sizeof(DWORD);
		// ������ַ�� DWORD
		DWORD* funcaddr = (DWORD*)VirtualAlloc(NULL, mSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		// ����������ű� WORD
		WORD* peot = (WORD*)VirtualAlloc(NULL, mSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		// �������Ʊ� DWORD
		DWORD* pent = (DWORD*)VirtualAlloc(NULL, mSize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

		// ������ַ�� DWORD
		ReadProcessMemory(hProcess, (LPVOID)((DWORD)dllBase+ pExport->AddressOfFunctions), (LPVOID)funcaddr, mSize, &realRead);
		printf("\t\t funcaddr %p %d \n", funcaddr, realRead);

		// ����������ű� WORD
		ReadProcessMemory(hProcess, (LPVOID)((DWORD)dllBase + pExport->AddressOfNameOrdinals), (LPVOID)peot, mSize, &realRead);
		printf("\t\t peot %p %d \n", peot, realRead);

		// �������Ʊ� DWORD
		ReadProcessMemory(hProcess, (LPVOID)((DWORD)dllBase + pExport->AddressOfNames), (LPVOID)pent, mSize, &realRead);
		printf("\t\t pent %p %d \n", pent, realRead);

		for (int i = 0; i < pExport->NumberOfFunctions; i++)
		{
			DWORD tmp = *funcaddr;
			DWORD addr = tmp + (DWORD)dllBase;

			// ��Щ����������û������
			BOOL existName = FALSE;
			for (int j = 0; j < pExport->NumberOfNames; j++)
			{
				if (peot[j] == i)
				{
					char funcName[MAX_PATH] = { 0 };
					ReadProcessMemory(hProcess, (LPVOID)((DWORD)dllBase + pent[j]), (LPVOID)funcName, MAX_PATH, &realRead);
					// char* funcName = (char*)(pent[j] + (DWORD)modinfo.lpBaseOfDll);
					// printf("\t\t��������: %s\n",  funcName);

					if (0 == _stricmp(funcName, findFuncName))
					{
						printf("\t\t������ַΪ: %llX ����������: %s\n", addr, funcName);
						*funcAddr = addr;
						isFind = TRUE;
						return TRUE;
					}
					existName = TRUE;
					break;
				}
			}

			if (!existName)
			{
				printf("\t\t������ַΪ: %llX ����������: sub_%X\n", addr, tmp);
			}

			funcaddr++;
		}

		VirtualFree(funcaddr, mSize, MEM_RELEASE);
		VirtualFree(peot, mSize, MEM_RELEASE);
		VirtualFree(pent, mSize, MEM_RELEASE);

	}


	VirtualFree(buffer, modinfo.SizeOfImage, MEM_RELEASE);

}

// 64λ�ĳ���dll��ַ�ռ侭�����ԣ���δ���ֲ�����
BOOL GetProcessDllFunctionsByNameX64(IN DWORD processID, IN CString dllname, IN CString findFuncName, OUT ULONG64* funcAddr)
{

	////////////////////////////////// x64
	// 
	BOOL isWow32;
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	IsWow64Process(hProcess, &isWow32);
	// ����trueΪ32λ����
	if (isWow32)
	{
		printf("����processid %d is x32\n", processID);
		return FALSE;
	}

	// ʵ�ַ�ʽ�ǻ�ȡ���̿���
	HANDLE hSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
	MODULEENTRY32 pe32;
	pe32.dwSize = sizeof(MODULEENTRY32);
	BOOL ret = ::Module32First(hSnapShot, &pe32);

	UINT_PTR ModuleBase = NULL;
	BOOL isFind = FALSE;
	// ����ÿ���������ƺ�Ŀ����̱Ƚ�
	while (ret)
	{
		if (0 == _stricmp(pe32.szModule, dllname))
		{
			printf("path: %s ,modebase: %p %p , size:%X\n", pe32.szModule, pe32.hModule, pe32.modBaseAddr, pe32.modBaseSize);

			ModuleBase = (UINT_PTR)pe32.modBaseAddr;
			void* buffer = (void*)VirtualAlloc(NULL, pe32.modBaseSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

			SIZE_T realRead = 0;
			ReadProcessMemory(hProcess, (PVOID)ModuleBase, buffer, pe32.modBaseSize, &realRead);
			printf("\t ReadProcessMemory realRead %X \n", realRead);
			printf("\t buffer buffer %p \n", buffer);
			// parse ...
			PIMAGE_DOS_HEADER	pDosHeader = (PIMAGE_DOS_HEADER)(buffer);
			PIMAGE_NT_HEADERS	pNtHeader = (PIMAGE_NT_HEADERS)(ULONG64(buffer) + pDosHeader->e_lfanew);

			//printf("\t DosHeader %p \n", pDosHeader);
			//printf("\t pNtHeader %p \n", pNtHeader);
			//printf("\t DosHeader e_lfanew %d \n", pDosHeader->e_lfanew);

			IMAGE_DATA_DIRECTORY exp = pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];


			//printf("\t exportRVA exp %p \n", exp);
			//printf("\t exportRVA Size %X \n", exp.Size);
			//printf("\t exportRVA VirtualAddress %X \n", exp.VirtualAddress);

			PIMAGE_EXPORT_DIRECTORY	pExport = (PIMAGE_EXPORT_DIRECTORY)(ULONG64(buffer) + exp.VirtualAddress);
			//printf("\t pExport->NumberOfFunctions %X \n", pExport->NumberOfFunctions);

			if (pExport->NumberOfFunctions > 0)
			{
				// ������ַ�� DWORD
				DWORD* funcaddr = (DWORD*)(ULONG64(buffer) + pExport->AddressOfFunctions);
				// ����������ű� WORD
				WORD* peot = (WORD*)(ULONG64(buffer) + pExport->AddressOfNameOrdinals);
				// �������Ʊ� DWORD
				DWORD* pent = (DWORD*)(ULONG64(buffer) + pExport->AddressOfNames);
				//printf("\t\t funcaddr %p \n", funcaddr);
				//printf("\t\t peot %p \n", peot);
				//printf("\t\t pent %p \n", pent);

				for (int i = 0; i < pExport->NumberOfFunctions; i++)
				{
					DWORD tmp = *funcaddr;
					ULONG64 addr = tmp + (ULONG64)ModuleBase;

					// ��Щ����������û������
					BOOL existName = FALSE;
					for (int j = 0; j < pExport->NumberOfNames; j++)
					{
						if (peot[j] == i)
						{
							char* funcName = (char*)(pent[j] + (ULONG64)ModuleBase);
							// printf("\t\t��������: %s\n",  funcName);

							if (0 == _stricmp(funcName, findFuncName))
							{
								printf("\t\t������ַΪ: %llX ����������: %s\n", addr, funcName);
								*funcAddr = addr;
								isFind = TRUE;
								goto EXIT;
							}
							existName = TRUE;
							break;
						}
					}

					if (!existName)
					{
						printf("\t\t������ַΪ: %llX ����������: sub_%X\n", addr, tmp);
					}

					funcaddr++;
				}
			}
		}

		// next
		ret = ::Module32Next(hSnapShot, &pe32);
	}

	EXIT:
	::CloseHandle(hSnapShot);
	return isFind;
}