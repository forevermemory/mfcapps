#include <ntifs.h>
#include "virtualMemoryRW.h"


// Process Ŀ����̵Ľ��̽ṹ
// Address Ŀ����̵������ַ
// Length  ��ȡĿ����̵������ַ�ĳ���
// Buffer  ��ǰ���̵ĵ�ַ��IoControl ��������������
UINT64 KReadProcessMemory(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN OUT PVOID Buffer)
{
	UINT64 readLen = 0;
	KAPC_STATE apc_state;
	RtlZeroMemory(&apc_state, sizeof(KAPC_STATE));

	//�����ں˻������ڴ�,�����н���ͨ��
	PVOID tmpBuf_Kernel = ExAllocatePool(NonPagedPool, Length);
	if (NULL == tmpBuf_Kernel) // һ�㲻��ʧ��
	{
		DbgPrint("sys: ExAllocatePool error\n");
		return readLen;
	}

	//��ת�ڴ��ַ ��Ϊ KeStackAttachProcess�� ԭ���Ľ���R3 Buffer ��Ŀ���ַ�ﲻ����
	//RtlCopyMemory(tmpBuf_Kernel, Buffer, Length);

	//����Ŀ������ڴ�ռ�
	KeStackAttachProcess((PVOID)Process, &apc_state);
	DbgPrint("sys: ���ӵ�Ŀ����� Address=%p  Buffer=%p\n", Address, Buffer);

	//�ж�Ŀ���ַ�Ƿ���Է���
	if (MmIsAddressValid(Address))
	{
		//RtlCopyMemory(Address, tmpBuf_Kernel, Length); 
		RtlCopyMemory(tmpBuf_Kernel, Address, Length);  //һ��Ҫ���ں��ڴ� ��ת
		DbgPrint("sys: RtlCopyMemory(Address:%p, Buffer:%p, Length:%d);\n", Address, Buffer, Length);
		readLen = Length;
	}
	else
	{
		DbgPrint("sys: Ŀ���ַ�����Է���:Address:%p\n", Address);
	}
	//����Ŀ����̿ռ� �ָ�����
	KeUnstackDetachProcess(&apc_state);
	DbgPrint("sys: ����Ŀ�����");

	// ���ں�buff������д�뵽��ǰ���̵�����ռ�
	RtlCopyMemory(Buffer, tmpBuf_Kernel, Length);
	ExFreePool(tmpBuf_Kernel);
	return readLen;
}

// Process Ŀ����̵Ľ��̽ṹ
// Address Ŀ����̵������ַ
// Length  ��ȡĿ����̵������ַ�ĳ���
// Buffer  ��ǰ���̵ĵ�ַ��IoControl ��������������
UINT64 KWriteProcessMemory(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer)
{

	UINT64 writenLen = 0;
	KAPC_STATE apc_state;
	RtlZeroMemory(&apc_state, sizeof(KAPC_STATE));

	// �����ں˻������ڴ�,�����н���ͨ��
	PVOID tmpBuf_Kernel = ExAllocatePool(NonPagedPool, Length);
	if (NULL == tmpBuf_Kernel) // һ�㲻��ʧ��
	{
		DbgPrint("sys: ExAllocatePool error\n");
		return writenLen;
	}
	// �Ƚ���д������ݿ������ں˻�����
	RtlCopyMemory(tmpBuf_Kernel, Buffer, Length);

	// ����Ŀ������ڴ�ռ�
	KeStackAttachProcess((PVOID)Process, &apc_state);
	DbgPrint("sys: ���ӵ�Ŀ����� Address=%p  Buffer=%p\n", Address, Buffer);

	// �ж�Ŀ���ַ�Ƿ���Է���
	if (MmIsAddressValid(Address))
	{
		// RtlCopyMemory(Address, tmpBuf_Kernel, Length); 
		RtlCopyMemory(Address, tmpBuf_Kernel, Length);  //һ��Ҫ���ں��ڴ� ��ת
		DbgPrint("sys: RtlCopyMemory(Address:%p, Buffer:%p, Length:%d);\n", Address, Buffer, Length);
		writenLen = Length;
	}
	else
	{
		DbgPrint("sys: Ŀ���ַ�����Է���:Address:%p\n", Address);
	}
	// ����Ŀ����̿ռ� �ָ�����
	KeUnstackDetachProcess(&apc_state);
	DbgPrint("sys: ����Ŀ�����");

	ExFreePool(tmpBuf_Kernel);
	return writenLen;
}













// Process Ŀ����̵Ľ��̽ṹ
// Address Ŀ����̵������ַ
// Length  ��ȡĿ����̵������ַ�ĳ���
// Buffer  ��ǰ���̵ĵ�ַ��IoControl ��������������
UINT64 KReadProcessMemory_MDL(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN OUT PVOID Buffer)
{
	UINT64 readLen = 0;
	KAPC_STATE apc_state;
	RtlZeroMemory(&apc_state, sizeof(KAPC_STATE));

	//1.ΪUserBuffer����MDL�ڴ����� ����MDL����ȡ�ڴ�
	PMDL g_pmdl = IoAllocateMdl(Buffer, Length, 0, 0, NULL);
	if (!g_pmdl)
	{
		DbgPrint("sys: IoAllocateMdl ����MDL�ڴ�����\n");
		return readLen;
	}

	//2.���Ϊ�Ƿ�ҳ�ڴ�
	MmBuildMdlForNonPagedPool(g_pmdl);

	//3.����ӳ���û��ڴ浽�ں��ڴ�
	PVOID Mapped = MmMapLockedPages(g_pmdl, KernelMode);
	if (!Mapped)
	{
		IoFreeMdl(g_pmdl);
		DbgPrint("sys: MmMapLockedPages ӳ��ʧ�� \n");
		return readLen;
	}

	// �ɹ�ӳ���˵�ַ,�л���Ŀ�����
	KeStackAttachProcess((PVOID)Process, &apc_state);
	DbgPrint("sys: ���ӵ�Ŀ����� Address=%p  Buffer=%p\n", Address, Buffer);

	//�ж�Ŀ���ַ�Ƿ���Է���
	if (MmIsAddressValid(Address))
	{
		//���Ŀ���ַ ���Է��� ֱ�Ӹ���Ŀ���ַ���� ��ӳ����ں˵�ַ����
		RtlCopyMemory(Mapped, Address, Length);
		DbgPrint("sys: RtlCopyMemory(Address:%p, Buffer:%p, Length:%d);\n", Address, Buffer, Length);
		readLen = Length;
	}
	else
	{
		DbgPrint("sys: Ŀ���ַ�����Է���:Address:%p\n", Address);
	}
	//����Ŀ����̿ռ� �ָ�����
	KeUnstackDetachProcess(&apc_state);
	DbgPrint("sys: ����Ŀ�����");

	// MDL������
	MmUnmapLockedPages(Mapped, g_pmdl);
	IoFreeMdl(g_pmdl);
	return readLen;
}

// Process Ŀ����̵Ľ��̽ṹ
// Address Ŀ����̵������ַ
// Length  ��ȡĿ����̵������ַ�ĳ���
// Buffer  ��ǰ���̵ĵ�ַ��IoControl ��������������
UINT64 KWriteProcessMemory_MDL(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer)
{

	UINT64 WriteLen = 0;
	KAPC_STATE apc_state;
	RtlZeroMemory(&apc_state, sizeof(KAPC_STATE));

	//1.ΪUserBuffer����MDL�ڴ����� ����MDL����ȡ�ڴ�
	PMDL g_pmdl = IoAllocateMdl(Buffer, Length, 0, 0, NULL);
	if (!g_pmdl)
	{
		DbgPrint("sys: IoAllocateMdl ����MDL�ڴ�����\n");
		return WriteLen;
	}

	//2.���Ϊ�Ƿ�ҳ�ڴ�
	MmBuildMdlForNonPagedPool(g_pmdl);

	//3.����ӳ���û��ڴ浽�ں��ڴ�
	PVOID Mapped = MmMapLockedPages(g_pmdl, KernelMode);
	if (!Mapped)
	{
		IoFreeMdl(g_pmdl);
		DbgPrint("sys: MmMapLockedPages ӳ��ʧ�� \n");
		return WriteLen;
	}

	// �ɹ�ӳ���˵�ַ,�л���Ŀ�����
	KeStackAttachProcess((PVOID)Process, &apc_state);
	DbgPrint("sys: ���ӵ�Ŀ����� Address=%p  Buffer=%p\n", Address, Buffer);

	//�ж�Ŀ���ַ�Ƿ���Է���
	if (MmIsAddressValid(Address))
	{
		//���Ŀ���ַ ���Է��� ֱ�Ӹ���Ŀ���ַ���� ��ӳ����ں˵�ַ����
		RtlCopyMemory(Address, Mapped, Length);
		DbgPrint("sys: RtlCopyMemory(Address:%p, Buffer:%p, Length:%d);\n", Address, Buffer, Length);
		WriteLen = Length;
	}
	else
	{
		DbgPrint("sys: Ŀ���ַ�����Է���:Address:%p\n", Address);
	}
	//����Ŀ����̿ռ� �ָ�����
	KeUnstackDetachProcess(&apc_state);
	DbgPrint("sys: ����Ŀ�����");

	// MDL������
	MmUnmapLockedPages(Mapped, g_pmdl);
	IoFreeMdl(g_pmdl);
	return WriteLen;
}

// Process Ŀ����̵Ľ��̽ṹ
// Address Ŀ����̵������ַ
// Length  ��д��Buffer�ĳ���
// Buffer  д������������,��ǰ���̵ĵ�ַ
UINT64 KWriteProcessMemory_MDL_ֻ��(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer)
{
	UINT64 retLen = 0;

	KAPC_STATE apc_state;
	RtlZeroMemory(&apc_state, sizeof(KAPC_STATE));

	PVOID Mapped = 0;
	PMDL g_pmdl = 0;

	// �л���Ŀ�����
	KeStackAttachProcess((PVOID)Process, &apc_state);

	// �ж�Ŀ����̵ĵ�ַ�Ƿ���Ч
	BOOLEAN isValid = MmIsAddressValid(Address);
	if (isValid)
	{
		// 1.ΪĿ����̵�Address����MDL�ڴ����� PS:�˴���֮ǰ��д��ͬ
		g_pmdl = IoAllocateMdl(Address, Length, 0, 0, NULL);
		if (!g_pmdl)
		{
			KeUnstackDetachProcess(&apc_state);
			return retLen;
		}
		// 2.ת���ɷǷ�ҳ�ڴ�
		MmBuildMdlForNonPagedPool(g_pmdl);

		// | MDL_ALLOCATED_FIXED_SIZE | MDL_PAGES_LOCKED;
		// �����ڴ�����
		g_pmdl->MdlFlags = MDL_WRITE_OPERATION;
		// 3.����ӳ���û��ڴ浽�ں��ڴ�
		Mapped = MmGetSystemAddressForMdlSafe(g_pmdl, KernelMode);
		if (!Mapped)
		{
			//ӳ��ʧ��
			IoFreeMdl(g_pmdl);
			KeUnstackDetachProcess(&apc_state);
			return retLen;
		}

	}

	// ����Ŀ����̿ռ� �ָ�����
	KeUnstackDetachProcess(&apc_state);

	// Ŀ����̵ĵ�ַ���Բ���
	if (isValid && g_pmdl)
	{
		DbgPrint("sys: ͨ��MDLдֻ���ڴ�(Address=%p, Buffer=%p, Length=%d) \n",
			Address, Buffer, Length);
		RtlCopyMemory(Mapped, Buffer, Length);
		retLen = Length;
	}
	else
	{
		DbgPrint("sys: Ŀ���ַ�����Է���:Address:%p\n", Address);
	}

	// MDL������
	MmUnmapLockedPages(Mapped, g_pmdl);
	IoFreeMdl(g_pmdl);
	return retLen;
}


#include <intrin.h>
//�򿪿�д�ڴ�ļ��
UINT64 EnsableCr0WPFlag_X64(KIRQL irql)
{
	UINT64 cr0 = __readcr0();
	UINT64 old_cr0 = cr0;
	cr0 |= 0x10000;
	_enable();
	__writecr0(cr0);
	KeLowerIrql(irql);
	return old_cr0;
}


//�رտ�д�ڴ�ļ��
KIRQL DisableCr0WPFlag_X64()
{
	KIRQL irql = KeRaiseIrqlToDpcLevel();
	UINT64 cr0 = __readcr0();
	cr0 &= 0xfffffffffffeffff;
	__writecr0(cr0);
	_disable();
	return irql;

}

// Process Ŀ����̵Ľ��̽ṹ
// Address Ŀ����̵������ַ
// Length  ��д��Buffer�ĳ���
// Buffer  д������������,��ǰ���̵ĵ�ַ
UINT64 KWriteProcessMemory_CR0_ֻ��(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer)
{
	UINT64 retLen = 0;

	KAPC_STATE apc_state;
	RtlZeroMemory(&apc_state, sizeof(KAPC_STATE));


	// 1.ΪUserBuffer����MDL�ڴ����� ����MDL����ȡ�ڴ�
	PMDL g_pmdl = IoAllocateMdl(Buffer, Length, 0, 0, NULL);
	if (!g_pmdl)
	{
		DbgPrint("sys: IoAllocateMdl ����MDL�ڴ�����\n");
		return retLen;
	}

	// 2.���Ϊ�Ƿ�ҳ�ڴ�
	MmBuildMdlForNonPagedPool(g_pmdl);

	// 3.����ӳ���û��ڴ浽�ں��ڴ�
	PVOID Mapped = MmMapLockedPages(g_pmdl, KernelMode);
	if (!Mapped)
	{
		IoFreeMdl(g_pmdl);
		DbgPrint("sys: MmMapLockedPages ӳ��ʧ�� \n");
		return retLen;
	}

	// �ɹ�ӳ���˵�ַ,�л���Ŀ�����
	KeStackAttachProcess((PVOID)Process, &apc_state);
	DbgPrint("sys: ���ӵ�Ŀ����� Address=%p  Buffer=%p\n", Address, Buffer);

	//�ж�Ŀ���ַ�Ƿ���Է���
	if (MmIsAddressValid(Address))
	{
		// �رտ�д�ڴ�ļ��
		KIRQL irql = DisableCr0WPFlag_X64();
		DbgPrint("sys: �رտ�д�ڴ�ļ��  \n");
		// ���Ŀ���ַ ���Է��� ֱ�Ӹ���Ŀ���ַ���� ��ӳ����ں˵�ַ����
		RtlCopyMemory(Address, Mapped, Length);
		DbgPrint("sys: RtlCopyMemory(Address:%p, Buffer:%p, Length:%d);\n", Address, Buffer, Length);
		retLen = Length;

		// �ָ�ԭ�Ȼ���
		EnsableCr0WPFlag_X64(irql);
		DbgPrint("sys: �򿪿�д�ڴ�ļ��  \n");
	}
	else
	{
		DbgPrint("sys: Ŀ���ַ�����Է���:Address:%p\n", Address);
	}
	// ����Ŀ����̿ռ� �ָ�����
	KeUnstackDetachProcess(&apc_state);
	DbgPrint("sys: ����Ŀ�����");

	// MDL������
	MmUnmapLockedPages(Mapped, g_pmdl);
	IoFreeMdl(g_pmdl);

	return retLen;
}






UINT64 g_maxPhysAddress = 0;
//����cpuidȡ�� �����ַBits   #include <intrin.h>
UINT64 getg_maxPhysAddress(void)
{
	if (g_maxPhysAddress == 0)
	{
		int physicalbits;
		UINT32 r[4]; //�ĸ����������飬������ EAX��EBX��ECX �� EDX �з��ص��й� CPU ֧�ֵĹ��ܵ���Ϣ
		__cpuid(r, 0x80000008); //ֻ��r[0]��ǰ 8λ�� 8��15������ ���3�� ȫ����

		//get max physical address
		physicalbits = r[0] & 0xff;

		g_maxPhysAddress = 0xFFFFFFFFFFFFFFFFULL;
		g_maxPhysAddress = g_maxPhysAddress >> physicalbits; //if physicalbits==36 then g_maxPhysAddress=0x000000000fffffff
		g_maxPhysAddress = ~(g_maxPhysAddress << physicalbits); //<< 36 = 0xfffffff000000000 .  after inverse : 0x0000000fffffffff		
	}

	return g_maxPhysAddress;
}



//��ȡ�����ַ
// selectedprocessΪָ�����̵�EPROCESS
// vBaseAddressΪָ�����̵������ַ
//����Ϊ �����ַ
PVOID GetPhysicalAddress(PEPROCESS selectedprocess, PVOID vBaseAddress)
{
	PHYSICAL_ADDRESS physical;//���������ַ INT64
	physical.QuadPart = 0; //PHYSICAL_ADDRESS ʵ���Ͼ��� INT64 8�ֽڵ�

	//switch to the selected process
	KAPC_STATE apc_state;
	RtlZeroMemory(&apc_state, sizeof(apc_state));

	// ���ӵ�ָ������
	KeStackAttachProcess((PVOID)selectedprocess, &apc_state);
	physical = MmGetPhysicalAddress(vBaseAddress); //�������ַת���������ַ
	KeUnstackDetachProcess(&apc_state);

	return (PVOID)physical.QuadPart;//���������ַ
}

// ͨ�������ַ��д�ڴ�
UINT64 KReadProcessMemory_Physical(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer)
{
	UINT64 readLen = 0;
	PVOID physicalBase = GetPhysicalAddress(Process, Address);
	if (physicalBase == NULL)
	{
		DbgPrint("sys: �������ַת��Ϊ�����ַʧ��\n");
		return 0;
	}

	////////////////////// ��ʽ1
	//MM_COPY_ADDRESS Read = {0 };
	//Read.PhysicalAddress.QuadPart = (LONG64)physicalBase;
	//__try
	//{
	//	MmCopyMemory(Buffer, Read, Length, MM_COPY_MEMORY_PHYSICAL, &readLen);
	//}
	//__except (1)
	//{
	//	DbgPrint("ys: MmCopyMemory================>\n");
	//}
	//DbgPrint("ys: MmCopyMemory END================>\n");
	//return readLen;

	////////////////////// ��ʽ2
	// ���濪ʼ���������ڴ�
	DbgPrint("sys: ReadPhysicalMemory(%p, %d, %p)", physicalBase, Length, Buffer);
	if (((UINT64)physicalBase > getg_maxPhysAddress()) || ((UINT64)physicalBase + Length > getg_maxPhysAddress()))
	{
		DbgPrint("sys: Invalid physical address\n");
		return readLen;
	}

	__try
	{
		UNICODE_STRING	physmemString;
		const	WCHAR* physmemName = L"\\device\\physicalmemory";
		RtlInitUnicodeString(&physmemString, physmemName);

		OBJECT_ATTRIBUTES attributes;
		HANDLE			physmem;

		InitializeObjectAttributes(&attributes, &physmemString, OBJ_CASE_INSENSITIVE, NULL, NULL);
		NTSTATUS ntStatus = ZwOpenSection(&physmem, SECTION_ALL_ACCESS, &attributes);
		if (ntStatus == STATUS_SUCCESS)
		{
			//hey look, it didn't kill it
			SIZE_T length;
			PHYSICAL_ADDRESS	viewBase;//�����ڴ��ַ
			UINT_PTR offset;
			UINT_PTR toread;

			viewBase.QuadPart = (ULONGLONG)(physicalBase);

			length = 0x2000;//pinp->bytestoread; //in case of a overlapping region
			toread = Length;

			UCHAR * vaddress = NULL;

			DbgPrint("sys: ReadPhysicalMemory:viewBase.QuadPart=%x", viewBase.QuadPart);

			//ӳ�������ڴ��ַ����ǰ���̵����ַ�ռ�
			ntStatus = ZwMapViewOfSection(
				physmem,  //sectionhandle
				NtCurrentProcess(), //processhandle (should be -1)
				&vaddress, //BaseAddress
				0L, //ZeroBits
				length, //CommitSize
				&viewBase, //SectionOffset
				&length, //ViewSize
				ViewShare,
				0,
				PAGE_READWRITE);

			if ((ntStatus == STATUS_SUCCESS) && (vaddress != NULL))
			{
				if (toread > length)
					toread = length;

				if (toread)
				{
					__try
					{

						offset = (UINT_PTR)(physicalBase)-(UINT_PTR)viewBase.QuadPart;//0

						if (offset + toread > length)
						{
							DbgPrint("yjx:Too small map\n");
						}
						else
						{
							RtlCopyMemory(Buffer, &vaddress[offset], toread);
							readLen = toread;
						}

						ZwUnmapViewOfSection(NtCurrentProcess(), vaddress);
					}
					__except (1)
					{
						DbgPrint("ys: Failure mapping physical memory");
					}
				}
			}
			else
			{
				DbgPrint("ys: ReadPhysicalMemory error:ntStatus=%x", ntStatus);
			}

			ZwClose(physmem);
		};
	}
	__except (1)
	{
		DbgPrint("sys: Error while reading physical memory\n");
	}



	return readLen;
}

UINT64 KWriteProcessMemory_Physical(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer)
{
	UINT64 readLen = 0;
	PVOID physicalBase = GetPhysicalAddress(Process, Address);
	if (physicalBase == NULL)
	{
		DbgPrint("sys: �������ַת��Ϊ�����ַʧ��\n");
		return 0;
	}

	//////////////////////////////// ��ʽ1
	//PHYSICAL_ADDRESS Write = {
	//	0 };
	//Write.QuadPart = (LONG64)physicalBase;

	//// ������ռ�ӳ��Ϊ����ռ�
	//PVOID map = MmMapIoSpace(Write, Length, (MEMORY_CACHING_TYPE)PAGE_READWRITE);

	//if (!map)
	//{
	//	DbgPrint("sys: MmMapIoSpace physical memory\n");
	//	return readLen;
	//}

	//// ��ʼ��������
	//__try
	//{
	//	RtlCopyMemory(map, Buffer, Length);
	//	readLen = Length;
	//}
	//__except (1)
	//{
	//	DbgPrint("sys: RtlCopyMemory physical memory\n");
	//}

	//MmUnmapIoSpace(map, Length);
	//DbgPrint("sys: RtlCopyMemory physical memory==================>\n");
	//return readLen;


	//////////////////////// ��ʽ��: 
	// ���濪ʼ���������ڴ�
	DbgPrint("sys: ReadPhysicalMemory(%p, %d, %p)", physicalBase, Length, Buffer);
	if (((UINT64)physicalBase > getg_maxPhysAddress()) || ((UINT64)physicalBase + Length > getg_maxPhysAddress()))
	{
		DbgPrint("sys: Invalid physical address\n");
		return 0;
	}

	__try
	{
		UNICODE_STRING	physmemString;
		const	WCHAR* physmemName = L"\\device\\physicalmemory";
		RtlInitUnicodeString(&physmemString, physmemName);

		OBJECT_ATTRIBUTES attributes;
		HANDLE			physmem;

		InitializeObjectAttributes(&attributes, &physmemString, OBJ_CASE_INSENSITIVE, NULL, NULL);
		NTSTATUS ntStatus = ZwOpenSection(&physmem, SECTION_ALL_ACCESS, &attributes);
		if (ntStatus == STATUS_SUCCESS)
		{
			//hey look, it didn't kill it
			SIZE_T length = 0x2000;//pinp->bytestoread; //in case of a overlapping region
			PHYSICAL_ADDRESS	viewBase;//�����ڴ��ַ
			UINT_PTR offset;
			UINT_PTR toWriteSize = Length;
			UCHAR* vaddress = NULL;

			viewBase.QuadPart = (ULONGLONG)(physicalBase);

			DbgPrint("sys: ReadPhysicalMemory:viewBase.QuadPart=%x", viewBase.QuadPart);

			//ӳ�������ڴ��ַ����ǰ���̵����ַ�ռ�
			ntStatus = ZwMapViewOfSection(
				physmem,  //sectionhandle
				NtCurrentProcess(), //processhandle (should be -1)
				&vaddress, //BaseAddress
				0L, //ZeroBits
				length, //CommitSize
				&viewBase, //SectionOffset
				&length, //ViewSize
				ViewShare,
				0,
				PAGE_READWRITE);

			if ((ntStatus == STATUS_SUCCESS) && (vaddress != NULL))
			{
				if (toWriteSize > length)
					toWriteSize = length;

				if (toWriteSize)
				{
					__try
					{

						offset = (UINT_PTR)(physicalBase)-(UINT_PTR)viewBase.QuadPart;

						if (offset + toWriteSize > length)
						{
							DbgPrint("sys: Too small map\n");
						}
						else
						{
							DbgPrint("sys: offset��%lld,vaddress:%p \n", offset, vaddress);
							
							//RtlCopyMemory(Buffer, &vaddress[offset], toread);
							RtlCopyMemory(&vaddress[offset], Buffer, toWriteSize);
							readLen = toWriteSize;
						}

						ZwUnmapViewOfSection(NtCurrentProcess(), vaddress);
					}
					__except (1)
					{
						DbgPrint("ys: Failure RtlCopyMemory physical memory");
					}
				}
			}
			else
			{
				DbgPrint("ys: ReadPhysicalMemory error:ntStatus=%x", ntStatus);
			}

			ZwClose(physmem);
		};
	}
	__except (1)
	{
		DbgPrint("sys: Error while reading physical memory\n");
	}


	return readLen;
}