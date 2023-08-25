


#include "enumSystemInfo.h"
#include "utils.h"
#include "findCode.h"

//PspCidTable PspCidTable�����¼�������н��̺��̵߳���Ϣ

// ˵��:
// ͨ��ZwQuerySystemInformation��ȡ���Ľ���ֻ��R3������������Կ����Ľ���
// ��ͨ��PspCidTable��ȡ���Ĳ��˺ܶ�


UINT64 FindPspCidTable()
{
	// ͨ��PsLoadedModuleList�ҵ��ں�ģ��
	UNICODE_STRING usPsLoadedModuleList = RTL_CONSTANT_STRING(L"PsLoadedModuleList");
	PVOID PsLoadedListModule = MmGetSystemRoutineAddress(&usPsLoadedModuleList);
	if (NULL == PsLoadedListModule)
	{
		DbgPrint("��ȡPsLoadedListModuleʧ��\n");
		return 0;
	}

	PLDR_DATA_TABLE_ENTRY Begin = (PLDR_DATA_TABLE_ENTRY)PsLoadedListModule;
	PLIST_ENTRY Head = NULL;
	PLIST_ENTRY Next = NULL;


	if (Begin == NULL)
		return 0;

	Head = (PLIST_ENTRY)Begin->InLoadOrderLinks.Flink;
	Next = Head->Flink;

	UNICODE_STRING ntoskrnl = RTL_CONSTANT_STRING(L"ntoskrnl.exe");

	do
	{
		PLDR_DATA_TABLE_ENTRY Entry = CONTAINING_RECORD(Next, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		Next = Next->Flink;

		// �ҵ��ں�ģ���ַ
		LONG isEqual = RtlCompareUnicodeString(&(Entry->BaseDllName), &ntoskrnl, TRUE);
		DbgPrint("�ں�ģ������:%wZ, base:%llX [%d]\n", Entry->BaseDllName, Entry->DllBase, isEqual);
		if (isEqual == 0)
		{

			/*
			kd > u PspReferenceCidTableEntry L50
				nt!PspReferenceCidTableEntry:
				fffff806`79f15e70 48895c2408      mov     qword ptr[rsp + 8], rbx
				fffff806`79f15e75 48896c2410      mov     qword ptr[rsp + 10h], rbp
				fffff806`79f15e7a 4889742418      mov     qword ptr[rsp + 18h], rsi
				fffff806`79f15e7f 57              push    rdi
				fffff806`79f15e80 4883ec40        sub     rsp, 40h
				fffff806`79f15e84 33c0 xor eax, eax
				fffff806`79f15e86 0fb6f2          movzx   esi, dl
				fffff806`79f15e89 4889442420      mov     qword ptr[rsp + 20h], rax
				fffff806`79f15e8e 4889442428      mov     qword ptr[rsp + 28h], rax
				fffff806`79f15e93 488b05 7e06f0ff  mov     rax, qword ptr[nt!PspCidTable(fffff806`79e16518)]
				fffff806`79f15e9a f7c1fc030000    test    ecx, 3FCh

			*/

			// �����������������
			UCHAR code[] = { 0x48,0x89,0x5c,0x24,0x08,0x48,0x89,0x6c,0x24,
				0x10 ,0x48,0x89,0x74,0x24,0x18 ,0x57 ,0x48,0x83,0xec,0x40 ,0x33,0xc0,0x0f,0xb6,0xf2
				,0x48,0x89,0x44,0x24,0x20 };
			UINT64 startAddr = 0;
			NTSTATUS ret = SearchPattern(Entry->DllBase, Entry->SizeOfImage, code, sizeof(code), &startAddr);
			if (ret != STATUS_SUCCESS)
			{
				DbgPrint("SearchPatternʧ��\n");
				return 0;
			}
			//addr & 0xffffffff00000000 + (int*)(header + 0x23 + 0x3) + 7 + UINT32(addr)

			UINT32 offset = *((UINT32*)(startAddr + 0x23 + 0x3));
			UINT64 PspCidTable = (startAddr & 0xffffffff00000000) | (offset + 0x7 + 0x23 + (UINT32)startAddr);

			return PspCidTable;
		}

	} while (Next != Head->Flink);
}


//kd > dq ObpKernelHandleTable
//fffff806`79ce97c0  ffffdc82`06404bc0 00000000`00000000
//fffff806`79ce97d0  00000000`00000000 00000000`00000000

UINT64 FindObpKernelHandleTable()
{
	// ͨ��PsLoadedModuleList�ҵ��ں�ģ��
	UNICODE_STRING usPsLoadedModuleList = RTL_CONSTANT_STRING(L"PsLoadedModuleList");
	PVOID PsLoadedListModule = MmGetSystemRoutineAddress(&usPsLoadedModuleList);
	if (NULL == PsLoadedListModule)
	{
		DbgPrint("��ȡPsLoadedListModuleʧ��\n");
		return 0;
	}

	PLDR_DATA_TABLE_ENTRY Begin = (PLDR_DATA_TABLE_ENTRY)PsLoadedListModule;
	PLIST_ENTRY Head = NULL;
	PLIST_ENTRY Next = NULL;

	
	if (Begin == NULL)
		return 0;

	Head = (PLIST_ENTRY)Begin->InLoadOrderLinks.Flink;
	Next = Head->Flink;

	UNICODE_STRING ntoskrnl = RTL_CONSTANT_STRING(L"ntoskrnl.exe");

	do
	{
		PLDR_DATA_TABLE_ENTRY Entry = CONTAINING_RECORD(Next, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		Next = Next->Flink;

		// �ҵ��ں�ģ���ַ
		LONG isEqual = RtlCompareUnicodeString(&(Entry->BaseDllName), &ntoskrnl, TRUE);
		DbgPrint("�ں�ģ������:%wZ, base:%llX [%d]\n", Entry->BaseDllName, Entry->DllBase, isEqual);
		if (isEqual == 0)
		{

			/*
			kd> u ObpReferenceObjectByHandleWithTag L50
			nt!ObpReferenceObjectByHandleWithTag:
			fffff806`79ec47e0 44884c2420      mov     byte ptr [rsp+20h],r9b
			fffff806`79ec47e5 4c89442418      mov     qword ptr [rsp+18h],r8
			fffff806`79ec47ea 89542410        mov     dword ptr [rsp+10h],edx
			fffff806`79ec47ee 53              push    rbx
			fffff806`79ec47ef 55              push    rbp
			fffff806`79ec47f0 57              push    rdi
			fffff806`79ec47f1 4154            push    r12
			fffff806`79ec47f3 4155            push    r13
			fffff806`79ec47f5 4157            push    r15
			fffff806`79ec47f7 4883ec58        sub     rsp,58h
			fffff806`79ec47fb 654c8b3c2588010000 mov   r15,qword ptr gs:[188h]
			fffff806`79ec4804 488be9          mov     rbp,rcx
			fffff806`79ec4807 4c8ba424b8000000 mov     r12,qword ptr [rsp+0B8h]
			fffff806`79ec480f 33c9            xor     ecx,ecx
			fffff806`79ec4811 4c8bac24c8000000 mov     r13,qword ptr [rsp+0C8h]
			...
			fffff806`79ec48a5 0f8496361900    je      nt!ObpReferenceObjectByHandleWithTag+0x193761 (fffff806`7a057f41)
			fffff806`79ec48ab 4c3b0d 0e4fe2ff  cmp     r9,qword ptr [nt!ObpKernelHandleTable (fffff806`79ce97c0)]

			*/

			// �����������������
			UCHAR code[] = { 0x44,0x88,0x4c,0x24,0x20 ,0x4c,0x89,0x44,0x24,0x18 ,0x89,
				0x54,0x24,0x10 ,0x53 ,0x55 ,0x57 ,0x41,0x54 ,0x41,0x55
				,0x41,0x57 ,0x48,0x83,0xec,0x58 };
			UINT64 startAddr = 0;
			NTSTATUS ret = SearchPattern(Entry->DllBase, Entry->SizeOfImage, code, sizeof(code), &startAddr);
			if (ret != STATUS_SUCCESS)
			{
				DbgPrint("SearchPatternʧ��\n");
				return 0;
			}
			//addr & 0xffffffff00000000 + (int*)(header + 0xCB + 0x3) + 7 + UINT32(addr)

			UINT32 offset = *((UINT32*)(startAddr + 0xCB + 0x3));
			UINT64 findAddr = (startAddr & 0xffffffff00000000) | (offset + 0x7 + 0xCB + (UINT32)startAddr);

			return findAddr;
		}

	} while (Next != Head->Flink);
}

//index2���ڼ���������
//index3���ڼ���������
void EnumGlobalHandleTables_1(UINT64 BaseAddr, UINT32 index2, UINT32 index3)
{
	// ����һ����ÿ�������С 16 �������С 4k�����Ա��� 4096/16 = 526 ��
	for (INT i = 0; i < 256; i++)
	{
		UINT64 ObjectAddr = *(UINT64*)(BaseAddr + (i * 16));

		// win10
		// ���� 8504e7a6`9380fffb  --> ffff 8504e7a6`9380
		ObjectAddr = ((INT64)ObjectAddr >> 0x10) & 0xfffffffffffffff0;
		if (!MmIsAddressValid(ObjectAddr))
		{
			continue;
		}


		PEPROCESS process = (PEPROCESS)ObjectAddr;
		POBJECT_TYPE objType = NULL;
		UNICODE_STRING objTypeStr = RTL_CONSTANT_STRING(L"UNKNOWN");
		UNICODE_STRING ProcessStr = RTL_CONSTANT_STRING(L"Process");

		__try
		{
			//// ��ʽ1

						/// // 3333
			UINT32 INDEX = i * 4 + 1024 * index2 + 512 * index3 * 1024;
			objType = ObGetObjectType(ObjectAddr);
			if (objType)
			{
				objTypeStr = objType->Name;
			}
			//DbgPrint("sys: ObjectAddr:%llx, index: %d %x, type: %wZ\n", ObjectAddr, INDEX, INDEX, objTypeStr);

			if (RtlCompareUnicodeString(&objTypeStr, &ProcessStr, TRUE) == 0)
			{
				char* PName = PsGetProcessImageFileName(process);
				//	DbgPrint("sys: process:%llx, PID: %d,PNAME: %s\n", process, PID, PName);
				DbgPrint("sys: ObjectAddr:%llx, index: %d %x, type: %wZ, processName:%s\n",
					ObjectAddr, INDEX, INDEX, objTypeStr, PName);
			}


			//// ��ʽ2
			//// �ж��ǽ��̻����߳�
			//PEPROCESS p_eprocess = NULL;

			//if (PsLookupProcessByProcessId(_PID, &p_eprocess) == STATUS_SUCCESS)
			//{
			//	DbgPrint("\tsys: PID:%ld , i:%d , addr:%p , object:%llx\n", _PID, i, BaseAddr + i * 0x10, ObjectAddr);
			//}
			//else if (PsLookupThreadByThreadId(i_id, &p_ethread) == STATUS_SUCCESS) 
			//{
			//	DbgPrint("sys: TID:%d , i:%d , addr:%p , object:%llx\n", i_id, i, BaseAddr + i * 0x10, ul_decode);
			//}



		}
		__except (1)
		{
			DbgPrint("process except:%llx\n", process);
		}

	}
}

//index3���ڼ���������
void EnumGlobalHandleTables_2(UINT64 BaseAddr, UINT32 index3)
{
	// ����������ÿ�������С 8��,���С 4k�����Ա��� 4096/8 = 512 ��
	ULONG64 ul_baseAddr_1 = 0;
	for (INT i = 0; i < 512; i++) {
		if (!MmIsAddressValid((PVOID64)(BaseAddr + i * 8))) {
			//DbgPrint("[LYSM] �Ƿ�������ָ�루1��:%p\n", BaseAddr + i * 8);
			continue;
		}
		if (!MmIsAddressValid((PVOID64) * (PULONG64)(BaseAddr + i * 8))) {
			//DbgPrint("[LYSM] �Ƿ�������ָ�루2��:%p\n", BaseAddr + i * 8);
			continue;
		}
		ul_baseAddr_1 = *(PULONG64)(BaseAddr + i * 8);
		DbgPrint("sys: level2: %d , %llx\n", i + 1, ul_baseAddr_1);
		EnumGlobalHandleTables_1(ul_baseAddr_1, i, index3);
	}

}


void EnumGlobalHandleTables_3(UINT64 BaseAddr)
{
	// ����������ÿ�������С 8��,���С 4k�����Ա��� 4096/8 = 512 ��
	// ����������ÿ�������С 8��,���С 4k�����Ա��� 4096/8 = 512 ��
	ULONG64 ul_baseAddr_2 = 0;
	for (INT i = 0; i < 512; i++) {
		if (!MmIsAddressValid((PVOID64)(BaseAddr + i * 8)))
		{
			continue;
		}
		if (!MmIsAddressValid((PVOID64) * (PULONG64)(BaseAddr + i * 8)))
		{
			continue;
		}
		ul_baseAddr_2 = *(PULONG64)(BaseAddr + i * 8);
		EnumGlobalHandleTables_2(ul_baseAddr_2, i);
	}

}

//extern UINT64 ObpKernelHandleTable;
// ȫ�־����
void EnumPspCidTable()
{

	////////// ���ں�ģ������PspCidTable
	//UINT64 PspCidTable =  FindPspCidTable();
	//if (PspCidTable == 0)
	//{
	//	DbgPrint("��ȡPspCidTableʧ��\n");
	//	return;
	//}

	//DbgPrint("find PspCidTable [%llX]\n", PspCidTable);
	//PVOID tmp = (PVOID)PspCidTable;
	//PHANDLE_TABLE handleTable = (PHANDLE_TABLE)(*(UINT64*)tmp);


	PHANDLE_TABLE handleTable = (PHANDLE_TABLE)(0xFFFFDC8206405D40);
	DbgPrint("find handleTable [%llX]\n", handleTable);
	DbgPrint("find TableCode [%llX]\n", handleTable->TableCode);


	// �����������������ж��Ǽ����� 
	UINT32 level = handleTable->TableCode & 3;

	DbgPrint("PspCidTable level [%d]\n", level + 1);
	switch (level)
	{
	case 0:
	{
		// [UINT64]
		EnumGlobalHandleTables_1(handleTable->TableCode & (~3), 0, 0);
		break;
	}
	case 1:
	{
		// [UINT64,UINT64]  ==> ָ��{UINT64,UINT64}
		EnumGlobalHandleTables_2(handleTable->TableCode & (~3), 0);
		break;
	}
	case 2:
	{
		EnumGlobalHandleTables_3(handleTable->TableCode & (~3));
		break;
	}
	default:
		break;
	}

}










//index2���ڼ���������
//index3���ڼ���������
void EnumObpKernelHandleTable_1(UINT64 BaseAddr, UINT32 index2, UINT32 index3)
{
	// ����һ����ÿ�������С 16 �������С 4k�����Ա��� 4096/16 = 526 ��
	for (INT i = 0; i < 256; i++)
	{
		UINT64 ObjectAddr = *(UINT64*)(BaseAddr + (i * 16));
		UINT64 accessMask = *(UINT64*)(BaseAddr + (i * 16) + 8);

		// win10
		// ���� 8504e7a6`9380fffb  --> ffff 8504e7a6`9380
		ObjectAddr = ((INT64)ObjectAddr >> 0x10) & 0xfffffffffffffff0;
		if (!MmIsAddressValid(ObjectAddr))
		{
			continue;
		}


		POBJECT_TYPE objType = NULL;
		UNICODE_STRING objTypeStr = RTL_CONSTANT_STRING(L"UNKNOWN");
		UNICODE_STRING ProcessStr = RTL_CONSTANT_STRING(L"Process");

		__try
		{
			UINT32 INDEX = i * 4 + 1024 * index2 + 512 * index3 * 1024;
			objType = ObGetObjectType(ObjectAddr);

			if (MmIsAddressValid(objType))
			{
				DbgPrint("sys: ObjectAddr:%llx, index: %d %x Mask:[%llx], type: %wZ\n",
					ObjectAddr, INDEX, INDEX, accessMask, objType->Name);
			}
			else
			{
				DbgPrint("sys: ObjectAddr:%llx, index: %d %x Mask:[%llx], type: *****\n",
					ObjectAddr, INDEX, INDEX, accessMask);
			}
		
		}
		__except (1)
		{
			DbgPrint(" process except ObjectAddr\n");
		}

	}
}

//index3���ڼ���������
void EnumObpKernelHandleTable_2(UINT64 BaseAddr, UINT32 index3)
{
	// ����������ÿ�������С 8��,���С 4k�����Ա��� 4096/8 = 512 ��
	ULONG64 ul_baseAddr_1 = 0;
	for (INT i = 0; i < 512; i++) {
		if (!MmIsAddressValid((PVOID64)(BaseAddr + i * 8))) {
			//DbgPrint("[LYSM] �Ƿ�������ָ�루1��:%p\n", BaseAddr + i * 8);
			continue;
		}
		if (!MmIsAddressValid((PVOID64) * (PULONG64)(BaseAddr + i * 8))) {
			//DbgPrint("[LYSM] �Ƿ�������ָ�루2��:%p\n", BaseAddr + i * 8);
			continue;
		}
		ul_baseAddr_1 = *(PULONG64)(BaseAddr + i * 8);
		DbgPrint("sys: level2: %d , %llx\n", i + 1, ul_baseAddr_1);
		EnumObpKernelHandleTable_1(ul_baseAddr_1, i, index3);
	}

}


void EnumObpKernelHandleTable_3(UINT64 BaseAddr)
{
	// ����������ÿ�������С 8��,���С 4k�����Ա��� 4096/8 = 512 ��
	// ����������ÿ�������С 8��,���С 4k�����Ա��� 4096/8 = 512 ��
	ULONG64 ul_baseAddr_2 = 0;
	for (INT i = 0; i < 512; i++) {
		if (!MmIsAddressValid((PVOID64)(BaseAddr + i * 8)))
		{
			continue;
		}
		if (!MmIsAddressValid((PVOID64) * (PULONG64)(BaseAddr + i * 8)))
		{
			continue;
		}
		ul_baseAddr_2 = *(PULONG64)(BaseAddr + i * 8);
		EnumObpKernelHandleTable_2(ul_baseAddr_2, i);
	}

}

void EnumObpKernelHandleTable()
{

	////////// ���ں�ģ������ObpKernelHandleTable
	//UINT64 ObpKernelHandleTable =  FindObpKernelHandleTable();
	//if (ObpKernelHandleTable == 0)
	//{
	//	DbgPrint("��ȡObpKernelHandleTableʧ��\n");
	//	return;
	//}

	//DbgPrint("find ObpKernelHandleTable [%llX]\n", ObpKernelHandleTable);
	//PVOID tmp = (PVOID)ObpKernelHandleTable;
	//PHANDLE_TABLE handleTable = (PHANDLE_TABLE)(*(UINT64*)tmp);

	PHANDLE_TABLE handleTable = (PHANDLE_TABLE)(0xffffdc8206404bc0);
	DbgPrint("find handleTable [%llX]\n", handleTable);
	DbgPrint("find TableCode [%llX]\n", handleTable->TableCode);


	// �����������������ж��Ǽ����� 
	UINT32 level = handleTable->TableCode & 3;

	DbgPrint("ObpKernelHandleTable level [%d]\n", level + 1);
	switch (level)
	{
	case 0:
	{
		// [UINT64]
		EnumObpKernelHandleTable_1(handleTable->TableCode & (~3), 0, 0);
		break;
	}
	case 1:
	{
		// [UINT64,UINT64]  ==> ָ��{UINT64,UINT64}
		EnumObpKernelHandleTable_2(handleTable->TableCode & (~3), 0);
		break;
	}
	case 2:
	{
		EnumObpKernelHandleTable_3(handleTable->TableCode & (~3));
		break;
	}
	default:
		break;
	}
}



void Test_ZwQuerySystemInformation()
{
	DbgPrint("============Test_ZwQuerySystemInformation==========\n");
	NTSTATUS status;
	ULONG realLen = 0;
	PVOID buffer = NULL;
	// ��ȡϵͳ���̵���Ϣ
	SYSTEM_PROCESS_INFORMATION processInfo = { 0 };
	status = ZwQuerySystemInformation(SystemProcessInformation, &processInfo,
		sizeof(SYSTEM_PROCESS_INFORMATION),
		&realLen);
	DbgPrint("status:%X , realLen:%d\n", status, realLen);

	if (status == STATUS_INFO_LENGTH_MISMATCH)
	{

		buffer = ExAllocatePool(NonPagedPool, realLen);
		status = ZwQuerySystemInformation(SystemProcessInformation, buffer,
			realLen,
			&realLen);
		PSYSTEM_PROCESS_INFORMATION pInfo = (PSYSTEM_PROCESS_INFORMATION)buffer;
		while (pInfo->NextEntryOffset)
		{
			DbgPrint("pid:%d, threadNum:%d, name:%ws\n",
				(UINT32)pInfo->UniqueProcessId, pInfo->NumberOfThreads, pInfo->ImageName.Buffer);

			pInfo = (PSYSTEM_PROCESS_INFORMATION)((UINT64)(pInfo)+pInfo->NextEntryOffset);

		}
	}
	if (buffer)
	{
		ExFreePool(buffer);
	}

}



void EnumDriverObjects(PDRIVER_OBJECT driverObject)
{
	ULONG count = 0;
	NTSTATUS Status;
	PLDR_DATA_TABLE_ENTRY  pLdr;

	if (NULL == driverObject)
	{
		//// ��ʽ1��ͨ��PsLoadedModuleList WIN10+
		UNICODE_STRING usPsLoadedModuleList = RTL_CONSTANT_STRING(L"PsLoadedModuleList");
		PVOID PsLoadedListModule = MmGetSystemRoutineAddress(&usPsLoadedModuleList);
		if (NULL == PsLoadedListModule)
		{
			DbgPrint("��ȡPsLoadedListModuleʧ��\n");
			return;
		}
		pLdr = (PLDR_DATA_TABLE_ENTRY)PsLoadedListModule;
	}
	else
	{
		// ��ʽ��: ʹ�ô����driverObject
		pLdr = (PLDR_DATA_TABLE_ENTRY)driverObject->DriverSection;
	}



	PLIST_ENTRY start = pLdr->InLoadOrderLinks.Flink;
	PLIST_ENTRY Current = start->Flink;

	PLDR_DATA_TABLE_ENTRY pEntry = NULL;
	while (Current != start)
	{

		pEntry = CONTAINING_RECORD(Current, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

		Current = Current->Flink;

		///// ����ģ�����ƻ�ȡDriverObject   \\Driver\\xxx
		UNICODE_STRING  Source = pEntry->BaseDllName;
		Source.Length -= 8; // ȥ�� .sys��׺

		WCHAR dst_buf[256];
		UNICODE_STRING driverName1;
		UNICODE_STRING prefix = RTL_CONSTANT_STRING(L"\\Driver\\");
		RtlInitEmptyUnicodeString(&driverName1, dst_buf, sizeof(dst_buf));
		
		Status = RtlAppendUnicodeStringToString(&driverName1, &prefix);
		Status = RtlAppendUnicodeStringToString(&driverName1, &Source);

		PDRIVER_OBJECT Driver = NULL;
		ObReferenceObjectByName(&driverName1, FILE_ALL_ACCESS, 0, 0,
			*IoDriverObjectType, KernelMode, NULL, &Driver);// ͨ�������ĵ������Ķ���
		//DbgPrint("===:%x [%wZ]  [%wZ]\n", Status, driverName1, Source);

		if (pEntry->BaseDllName.Buffer != 0)
		{
			DbgPrint("OBJECT:%llx, ��ַ:%p --->��С:%X---> ģ����:%wZ \r\n",
				Driver, pEntry->DllBase,
				pEntry->SizeOfImage, pEntry->BaseDllName);
		}

	}


}









int total = 0;

//index2���ڼ���������
//index3���ڼ���������
void EnumProcessHandleTable_1(UINT64 BaseAddr, UINT32 index2, UINT32 index3)
{

	UNICODE_STRING objTypeStr = RTL_CONSTANT_STRING(L"UNKNOWN");

	// ����һ����ÿ�������С 16 �������С 4k�����Ա��� 4096/16 = 526 ��
	for (INT i = 0; i < 256; i++)
	{
		UINT32 INDEX = i * 4 + 1024 * index2 + 512 * index3 * 1024;
		UINT64 ObjectHeader = *(UINT64*)(BaseAddr + (i * 16)); 
		UINT64 Mask = *(UINT64*)(BaseAddr + (i * 16) + 8);

		// win10
		// ���� 8504e7a6`9380fffb  --> ffff 8504e7a6`9380
		ObjectHeader = ((INT64)ObjectHeader >> 0x10) & 0xfffffffffffffff0;

		//DbgPrint("sys: ObjectHeader:%llx, index: %d %x\n", ObjectHeader, INDEX, INDEX);

		if (!MmIsAddressValid(ObjectHeader))
		{
			continue;
		}

		POBJECT_TYPE objType = NULL;

		__try
		{

			//// ��ʽ1
			objType = ObGetObjectType(ObjectHeader+0x30);
			if (objType)
			{
				objTypeStr = objType->Name;
			}
			

			//DbgPrint("sys: ObjectHeader:%llx, Mask:%llx, index: %d %x\n", ObjectHeader, Mask, INDEX, INDEX);
			DbgPrint("sys: ObjectHeader:%llx, Mask:%llx, index: %d %x, type: %wZ\n",
				ObjectHeader, Mask, INDEX, INDEX, objTypeStr);

			////if (RtlCompareUnicodeString(&objTypeStr, &ProcessStr, TRUE) == 0)
			//{
			//	char* PName = PsGetProcessImageFileName(process);
			//	//	DbgPrint("sys: process:%llx, PID: %d,PNAME: %s\n", process, PID, PName);
			//	DbgPrint("sys: ObjectAddr:%llx, index: %d %x, type: %wZ, processName:%s\n",
			//		ObjectAddr, INDEX, INDEX, objTypeStr, PName);
			//}


		}
		__except (1)
		{
			DbgPrint("process ObjectAddr:%llx\n", ObjectHeader);
		}

	}
}

//index3���ڼ���������
void EnumProcessHandleTable_2(UINT64 BaseAddr, UINT32 index3)
{
	// ����������ÿ�������С 8��,���С 4k�����Ա��� 4096/8 = 512 ��
	ULONG64 ul_baseAddr_1 = 0;
	for (INT i = 0; i < 512; i++) {
		if (!MmIsAddressValid((PVOID64)(BaseAddr + i * 8))) {
			//DbgPrint("[LYSM] �Ƿ�������ָ�루1��:%p\n", BaseAddr + i * 8);
			continue;
		}
		if (!MmIsAddressValid((PVOID64) * (PULONG64)(BaseAddr + i * 8))) {
			//DbgPrint("[LYSM] �Ƿ�������ָ�루2��:%p\n", BaseAddr + i * 8);
			continue;
		}
		ul_baseAddr_1 = *(PULONG64)(BaseAddr + i * 8);
		DbgPrint("sys: level2: %d , %llx\n", i + 1, ul_baseAddr_1);
		EnumProcessHandleTable_1(ul_baseAddr_1, i, index3);
	}

}


void EnumProcessHandleTable_3(UINT64 BaseAddr)
{
	// ����������ÿ�������С 8��,���С 4k�����Ա��� 4096/8 = 512 ��
	// ����������ÿ�������С 8��,���С 4k�����Ա��� 4096/8 = 512 ��
	ULONG64 ul_baseAddr_2 = 0;
	for (INT i = 0; i < 512; i++) {
		if (!MmIsAddressValid((PVOID64)(BaseAddr + i * 8)))
		{
			continue;
		}
		if (!MmIsAddressValid((PVOID64) * (PULONG64)(BaseAddr + i * 8)))
		{
			continue;
		}
		ul_baseAddr_2 = *(PULONG64)(BaseAddr + i * 8);
		EnumProcessHandleTable_2(ul_baseAddr_2, i);
	}

}


void EnumProcessHandleTable(PEPROCESS pEprocess)
{
	DbgPrint("sys: start EnumProcessHandleTable�� pEprocess:%llx \n", pEprocess);

	// ��ȡHandleTable 
	//+0x418 ObjectTable      : 0xffffdc82`0f7ed9c0 _HANDLE_TABLE 
	PHANDLE_TABLE handleTable =  (PHANDLE_TABLE) (*(UINT64*)((UINT64)pEprocess + 0x418));

	DbgPrint("find handleTable [%llX]\n", handleTable);
	DbgPrint("find TableCode [%llX]\n", handleTable->TableCode);


	// �����������������ж��Ǽ����� 
	UINT32 level = handleTable->TableCode & 3;

	DbgPrint("PEPROCESS handle level [%d]\n",(level + 1));
	switch (level)
	{
	case 0:
	{
		// [UINT64]
		EnumProcessHandleTable_1(handleTable->TableCode & (~3), 0, 0);
		break;
	}
	case 1:
	{
		// [UINT64,UINT64]  ==> ָ��{UINT64,UINT64}
		EnumProcessHandleTable_2(handleTable->TableCode & (~3), 0);
		break;
	}
	case 2:
	{
		EnumProcessHandleTable_3(handleTable->TableCode & (~3));
		break;
	}
	default:
		break;
	}

}









void EnumIoDriverObjectType()
{
	//UNICODE_STRING pIoDriverObjectType = RTL_CONSTANT_STRING(L"IoDriverObjectType");
	//PVOID IoDriverObjectType = MmGetSystemRoutineAddress(&pIoDriverObjectType);
	//if (NULL == IoDriverObjectType)
	//{
	//	DbgPrint("��ȡIoDriverObjectTypeʧ��\n");
	//	return ;
	//}

	DbgPrint("sys: IoDriverObjectType:%llx\n", IoDriverObjectType);

	ULONG64 ObDriverCallbackListHead = *(ULONG64*)IoDriverObjectType + 0xc8;

	PLIST_ENTRY CurrEntry = NULL;
	POB_CALLBACK pObCallback;

	CurrEntry = ((PLIST_ENTRY)ObDriverCallbackListHead)->Flink;
	if (CurrEntry == NULL || !MmIsAddressValid(CurrEntry))
	{
		DbgPrint("��ȡCurrEntryʧ��\n");
		return ;
	}

	WCHAR* szDriverBaseName =  ExAllocatePool(NonPagedPool, 600);
	if (szDriverBaseName == NULL)
	{
		DbgPrint("sys: ����szDriverBaseNameʧ��\n");
		return;
	}
	RtlZeroMemory(szDriverBaseName, 600);
	//DbgBreakPoint();
	do
	{
		pObCallback = (POB_CALLBACK)CurrEntry;
		if (pObCallback->ObHandle != 0)
		{
			if (ObGetDriverNameByPoint(pObCallback->PreCall, szDriverBaseName))
			{
				DbgPrint("sys: DriverName=%S ObHandle=%p  Index=%wZ PreCall=%p PostCall=%p \n",
					szDriverBaseName,
					pObCallback->ObHandle,
					&pObCallback->ObHandle->AltitudeString,
					pObCallback->PreCall,
					pObCallback->PostCall);
			}
	
		}

		CurrEntry = CurrEntry->Flink;

	} while (CurrEntry != (PLIST_ENTRY)ObDriverCallbackListHead);

	ExFreePool(szDriverBaseName);


}