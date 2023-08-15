#include <ntifs.h>
#include "virtualMemoryRW.h"


// Process 目标进程的进程结构
// Address 目标进程的虚拟地址
// Length  读取目标进程的虚拟地址的长度
// Buffer  当前进程的地址，IoControl 传入的输出缓冲区
UINT64 KReadProcessMemory(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN OUT PVOID Buffer)
{
	UINT64 readLen = 0;
	KAPC_STATE apc_state;
	RtlZeroMemory(&apc_state, sizeof(KAPC_STATE));

	//申请内核缓冲区内存,在所有进程通用
	PVOID tmpBuf_Kernel = ExAllocatePool(NonPagedPool, Length);
	if (NULL == tmpBuf_Kernel) // 一般不会失败
	{
		DbgPrint("sys: ExAllocatePool error\n");
		return readLen;
	}

	//中转内存地址 因为 KeStackAttachProcess后 原来的进程R3 Buffer 在目标地址里不存在
	//RtlCopyMemory(tmpBuf_Kernel, Buffer, Length);

	//进入目标进程内存空间
	KeStackAttachProcess((PVOID)Process, &apc_state);
	DbgPrint("sys: 附加到目标进程 Address=%p  Buffer=%p\n", Address, Buffer);

	//判断目标地址是否可以访问
	if (MmIsAddressValid(Address))
	{
		//RtlCopyMemory(Address, tmpBuf_Kernel, Length); 
		RtlCopyMemory(tmpBuf_Kernel, Address, Length);  //一定要用内核内存 中转
		DbgPrint("sys: RtlCopyMemory(Address:%p, Buffer:%p, Length:%d);\n", Address, Buffer, Length);
		readLen = Length;
	}
	else
	{
		DbgPrint("sys: 目标地址不可以访问:Address:%p\n", Address);
	}
	//分离目标进程空间 恢复环境
	KeUnstackDetachProcess(&apc_state);
	DbgPrint("sys: 分离目标进程");

	// 将内核buff的内容写入到当前进程的虚拟空间
	RtlCopyMemory(Buffer, tmpBuf_Kernel, Length);
	ExFreePool(tmpBuf_Kernel);
	return readLen;
}

// Process 目标进程的进程结构
// Address 目标进程的虚拟地址
// Length  读取目标进程的虚拟地址的长度
// Buffer  当前进程的地址，IoControl 传入的输出缓冲区
UINT64 KWriteProcessMemory(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer)
{

	UINT64 writenLen = 0;
	KAPC_STATE apc_state;
	RtlZeroMemory(&apc_state, sizeof(KAPC_STATE));

	// 申请内核缓冲区内存,在所有进程通用
	PVOID tmpBuf_Kernel = ExAllocatePool(NonPagedPool, Length);
	if (NULL == tmpBuf_Kernel) // 一般不会失败
	{
		DbgPrint("sys: ExAllocatePool error\n");
		return writenLen;
	}
	// 先将待写入的数据拷贝到内核缓冲区
	RtlCopyMemory(tmpBuf_Kernel, Buffer, Length);

	// 进入目标进程内存空间
	KeStackAttachProcess((PVOID)Process, &apc_state);
	DbgPrint("sys: 附加到目标进程 Address=%p  Buffer=%p\n", Address, Buffer);

	// 判断目标地址是否可以访问
	if (MmIsAddressValid(Address))
	{
		// RtlCopyMemory(Address, tmpBuf_Kernel, Length); 
		RtlCopyMemory(Address, tmpBuf_Kernel, Length);  //一定要用内核内存 中转
		DbgPrint("sys: RtlCopyMemory(Address:%p, Buffer:%p, Length:%d);\n", Address, Buffer, Length);
		writenLen = Length;
	}
	else
	{
		DbgPrint("sys: 目标地址不可以访问:Address:%p\n", Address);
	}
	// 分离目标进程空间 恢复环境
	KeUnstackDetachProcess(&apc_state);
	DbgPrint("sys: 分离目标进程");

	ExFreePool(tmpBuf_Kernel);
	return writenLen;
}













// Process 目标进程的进程结构
// Address 目标进程的虚拟地址
// Length  读取目标进程的虚拟地址的长度
// Buffer  当前进程的地址，IoControl 传入的输出缓冲区
UINT64 KReadProcessMemory_MDL(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN OUT PVOID Buffer)
{
	UINT64 readLen = 0;
	KAPC_STATE apc_state;
	RtlZeroMemory(&apc_state, sizeof(KAPC_STATE));

	//1.为UserBuffer创建MDL内存描述 创建MDL来读取内存
	PMDL g_pmdl = IoAllocateMdl(Buffer, Length, 0, 0, NULL);
	if (!g_pmdl)
	{
		DbgPrint("sys: IoAllocateMdl 创建MDL内存描述\n");
		return readLen;
	}

	//2.标记为非分页内存
	MmBuildMdlForNonPagedPool(g_pmdl);

	//3.锁定映射用户内存到内核内存
	PVOID Mapped = MmMapLockedPages(g_pmdl, KernelMode);
	if (!Mapped)
	{
		IoFreeMdl(g_pmdl);
		DbgPrint("sys: MmMapLockedPages 映射失败 \n");
		return readLen;
	}

	// 成功映射了地址,切换到目标进程
	KeStackAttachProcess((PVOID)Process, &apc_state);
	DbgPrint("sys: 附加到目标进程 Address=%p  Buffer=%p\n", Address, Buffer);

	//判断目标地址是否可以访问
	if (MmIsAddressValid(Address))
	{
		//如果目标地址 可以访问 直接复制目标地址内容 到映射的内核地址区域
		RtlCopyMemory(Mapped, Address, Length);
		DbgPrint("sys: RtlCopyMemory(Address:%p, Buffer:%p, Length:%d);\n", Address, Buffer, Length);
		readLen = Length;
	}
	else
	{
		DbgPrint("sys: 目标地址不可以访问:Address:%p\n", Address);
	}
	//分离目标进程空间 恢复环境
	KeUnstackDetachProcess(&apc_state);
	DbgPrint("sys: 分离目标进程");

	// MDL清理工作
	MmUnmapLockedPages(Mapped, g_pmdl);
	IoFreeMdl(g_pmdl);
	return readLen;
}

// Process 目标进程的进程结构
// Address 目标进程的虚拟地址
// Length  读取目标进程的虚拟地址的长度
// Buffer  当前进程的地址，IoControl 传入的输出缓冲区
UINT64 KWriteProcessMemory_MDL(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer)
{

	UINT64 WriteLen = 0;
	KAPC_STATE apc_state;
	RtlZeroMemory(&apc_state, sizeof(KAPC_STATE));

	//1.为UserBuffer创建MDL内存描述 创建MDL来读取内存
	PMDL g_pmdl = IoAllocateMdl(Buffer, Length, 0, 0, NULL);
	if (!g_pmdl)
	{
		DbgPrint("sys: IoAllocateMdl 创建MDL内存描述\n");
		return WriteLen;
	}

	//2.标记为非分页内存
	MmBuildMdlForNonPagedPool(g_pmdl);

	//3.锁定映射用户内存到内核内存
	PVOID Mapped = MmMapLockedPages(g_pmdl, KernelMode);
	if (!Mapped)
	{
		IoFreeMdl(g_pmdl);
		DbgPrint("sys: MmMapLockedPages 映射失败 \n");
		return WriteLen;
	}

	// 成功映射了地址,切换到目标进程
	KeStackAttachProcess((PVOID)Process, &apc_state);
	DbgPrint("sys: 附加到目标进程 Address=%p  Buffer=%p\n", Address, Buffer);

	//判断目标地址是否可以访问
	if (MmIsAddressValid(Address))
	{
		//如果目标地址 可以访问 直接复制目标地址内容 到映射的内核地址区域
		RtlCopyMemory(Address, Mapped, Length);
		DbgPrint("sys: RtlCopyMemory(Address:%p, Buffer:%p, Length:%d);\n", Address, Buffer, Length);
		WriteLen = Length;
	}
	else
	{
		DbgPrint("sys: 目标地址不可以访问:Address:%p\n", Address);
	}
	//分离目标进程空间 恢复环境
	KeUnstackDetachProcess(&apc_state);
	DbgPrint("sys: 分离目标进程");

	// MDL清理工作
	MmUnmapLockedPages(Mapped, g_pmdl);
	IoFreeMdl(g_pmdl);
	return WriteLen;
}

// Process 目标进程的进程结构
// Address 目标进程的虚拟地址
// Length  待写入Buffer的长度
// Buffer  写入的输出缓冲区,当前进程的地址
UINT64 KWriteProcessMemory_MDL_只读(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer)
{
	UINT64 retLen = 0;

	KAPC_STATE apc_state;
	RtlZeroMemory(&apc_state, sizeof(KAPC_STATE));

	PVOID Mapped = 0;
	PMDL g_pmdl = 0;

	// 切换到目标进程
	KeStackAttachProcess((PVOID)Process, &apc_state);

	// 判断目标进程的地址是否有效
	BOOLEAN isValid = MmIsAddressValid(Address);
	if (isValid)
	{
		// 1.为目标进程的Address创建MDL内存描述 PS:此处与之前读写不同
		g_pmdl = IoAllocateMdl(Address, Length, 0, 0, NULL);
		if (!g_pmdl)
		{
			KeUnstackDetachProcess(&apc_state);
			return retLen;
		}
		// 2.转换成非分页内存
		MmBuildMdlForNonPagedPool(g_pmdl);

		// | MDL_ALLOCATED_FIXED_SIZE | MDL_PAGES_LOCKED;
		// 更改内存属性
		g_pmdl->MdlFlags = MDL_WRITE_OPERATION;
		// 3.锁定映射用户内存到内核内存
		Mapped = MmGetSystemAddressForMdlSafe(g_pmdl, KernelMode);
		if (!Mapped)
		{
			//映射失败
			IoFreeMdl(g_pmdl);
			KeUnstackDetachProcess(&apc_state);
			return retLen;
		}

	}

	// 分离目标进程空间 恢复环境
	KeUnstackDetachProcess(&apc_state);

	// 目标进程的地址可以操作
	if (isValid && g_pmdl)
	{
		DbgPrint("sys: 通过MDL写只读内存(Address=%p, Buffer=%p, Length=%d) \n",
			Address, Buffer, Length);
		RtlCopyMemory(Mapped, Buffer, Length);
		retLen = Length;
	}
	else
	{
		DbgPrint("sys: 目标地址不可以访问:Address:%p\n", Address);
	}

	// MDL清理工作
	MmUnmapLockedPages(Mapped, g_pmdl);
	IoFreeMdl(g_pmdl);
	return retLen;
}


#include <intrin.h>
//打开可写内存的检测
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


//关闭可写内存的检测
KIRQL DisableCr0WPFlag_X64()
{
	KIRQL irql = KeRaiseIrqlToDpcLevel();
	UINT64 cr0 = __readcr0();
	cr0 &= 0xfffffffffffeffff;
	__writecr0(cr0);
	_disable();
	return irql;

}

// Process 目标进程的进程结构
// Address 目标进程的虚拟地址
// Length  待写入Buffer的长度
// Buffer  写入的输出缓冲区,当前进程的地址
UINT64 KWriteProcessMemory_CR0_只读(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer)
{
	UINT64 retLen = 0;

	KAPC_STATE apc_state;
	RtlZeroMemory(&apc_state, sizeof(KAPC_STATE));


	// 1.为UserBuffer创建MDL内存描述 创建MDL来读取内存
	PMDL g_pmdl = IoAllocateMdl(Buffer, Length, 0, 0, NULL);
	if (!g_pmdl)
	{
		DbgPrint("sys: IoAllocateMdl 创建MDL内存描述\n");
		return retLen;
	}

	// 2.标记为非分页内存
	MmBuildMdlForNonPagedPool(g_pmdl);

	// 3.锁定映射用户内存到内核内存
	PVOID Mapped = MmMapLockedPages(g_pmdl, KernelMode);
	if (!Mapped)
	{
		IoFreeMdl(g_pmdl);
		DbgPrint("sys: MmMapLockedPages 映射失败 \n");
		return retLen;
	}

	// 成功映射了地址,切换到目标进程
	KeStackAttachProcess((PVOID)Process, &apc_state);
	DbgPrint("sys: 附加到目标进程 Address=%p  Buffer=%p\n", Address, Buffer);

	//判断目标地址是否可以访问
	if (MmIsAddressValid(Address))
	{
		// 关闭可写内存的检测
		KIRQL irql = DisableCr0WPFlag_X64();
		DbgPrint("sys: 关闭可写内存的检测  \n");
		// 如果目标地址 可以访问 直接复制目标地址内容 到映射的内核地址区域
		RtlCopyMemory(Address, Mapped, Length);
		DbgPrint("sys: RtlCopyMemory(Address:%p, Buffer:%p, Length:%d);\n", Address, Buffer, Length);
		retLen = Length;

		// 恢复原先环境
		EnsableCr0WPFlag_X64(irql);
		DbgPrint("sys: 打开可写内存的检测  \n");
	}
	else
	{
		DbgPrint("sys: 目标地址不可以访问:Address:%p\n", Address);
	}
	// 分离目标进程空间 恢复环境
	KeUnstackDetachProcess(&apc_state);
	DbgPrint("sys: 分离目标进程");

	// MDL清理工作
	MmUnmapLockedPages(Mapped, g_pmdl);
	IoFreeMdl(g_pmdl);

	return retLen;
}






UINT64 g_maxPhysAddress = 0;
//利用cpuid取出 物理地址Bits   #include <intrin.h>
UINT64 getg_maxPhysAddress(void)
{
	if (g_maxPhysAddress == 0)
	{
		int physicalbits;
		UINT32 r[4]; //四个整数的数组，包含在 EAX、EBX、ECX 和 EDX 中返回的有关 CPU 支持的功能的信息
		__cpuid(r, 0x80000008); //只有r[0]的前 8位和 8至15们有用 后边3个 全保留

		//get max physical address
		physicalbits = r[0] & 0xff;

		g_maxPhysAddress = 0xFFFFFFFFFFFFFFFFULL;
		g_maxPhysAddress = g_maxPhysAddress >> physicalbits; //if physicalbits==36 then g_maxPhysAddress=0x000000000fffffff
		g_maxPhysAddress = ~(g_maxPhysAddress << physicalbits); //<< 36 = 0xfffffff000000000 .  after inverse : 0x0000000fffffffff		
	}

	return g_maxPhysAddress;
}



//获取物理地址
// selectedprocess为指定进程的EPROCESS
// vBaseAddress为指定进程的虚拟地址
//返回为 物理地址
PVOID GetPhysicalAddress(PEPROCESS selectedprocess, PVOID vBaseAddress)
{
	PHYSICAL_ADDRESS physical;//返回物理地址 INT64
	physical.QuadPart = 0; //PHYSICAL_ADDRESS 实际上就是 INT64 8字节的

	//switch to the selected process
	KAPC_STATE apc_state;
	RtlZeroMemory(&apc_state, sizeof(apc_state));

	// 附加到指定进程
	KeStackAttachProcess((PVOID)selectedprocess, &apc_state);
	physical = MmGetPhysicalAddress(vBaseAddress); //把虚拟地址转换成物理地址
	KeUnstackDetachProcess(&apc_state);

	return (PVOID)physical.QuadPart;//返回物理地址
}

// 通过物理地址读写内存
UINT64 KReadProcessMemory_Physical(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer)
{
	UINT64 readLen = 0;
	PVOID physicalBase = GetPhysicalAddress(Process, Address);
	if (physicalBase == NULL)
	{
		DbgPrint("sys: 将虚拟地址转换为物理地址失败\n");
		return 0;
	}

	////////////////////// 方式1
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

	////////////////////// 方式2
	// 下面开始操作物理内存
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
			PHYSICAL_ADDRESS	viewBase;//物理内存地址
			UINT_PTR offset;
			UINT_PTR toread;

			viewBase.QuadPart = (ULONGLONG)(physicalBase);

			length = 0x2000;//pinp->bytestoread; //in case of a overlapping region
			toread = Length;

			UCHAR * vaddress = NULL;

			DbgPrint("sys: ReadPhysicalMemory:viewBase.QuadPart=%x", viewBase.QuadPart);

			//映射物理内存地址到当前进程的虚地址空间
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
		DbgPrint("sys: 将虚拟地址转换为物理地址失败\n");
		return 0;
	}

	//////////////////////////////// 方式1
	//PHYSICAL_ADDRESS Write = {
	//	0 };
	//Write.QuadPart = (LONG64)physicalBase;

	//// 将物理空间映射为虚拟空间
	//PVOID map = MmMapIoSpace(Write, Length, (MEMORY_CACHING_TYPE)PAGE_READWRITE);

	//if (!map)
	//{
	//	DbgPrint("sys: MmMapIoSpace physical memory\n");
	//	return readLen;
	//}

	//// 开始拷贝数据
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


	//////////////////////// 方式二: 
	// 下面开始操作物理内存
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
			PHYSICAL_ADDRESS	viewBase;//物理内存地址
			UINT_PTR offset;
			UINT_PTR toWriteSize = Length;
			UCHAR* vaddress = NULL;

			viewBase.QuadPart = (ULONGLONG)(physicalBase);

			DbgPrint("sys: ReadPhysicalMemory:viewBase.QuadPart=%x", viewBase.QuadPart);

			//映射物理内存地址到当前进程的虚地址空间
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
							DbgPrint("sys: offset：%lld,vaddress:%p \n", offset, vaddress);
							
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