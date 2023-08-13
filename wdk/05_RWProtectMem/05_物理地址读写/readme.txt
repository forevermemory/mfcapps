编译时候需要加上选项,配置如下
项目 -- 属性 -- 链接器 -- 命令行 -- 其它选项
	/INTEGRITYCHECK 

测试环境:
	win10  1903 (18362.30)


通过物理地址读写


// 获取进程虚拟地址的物理地址，可能此时获取的是空的
// selectedprocess为指定进程的EPROCESS
// vBaseAddress为指定进程的虚拟地址
// 返回为 物理地址
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



UINT64 KReadProcessMemory_Physical(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer)
{
	UINT64 readLen = 0;
	PVOID physicalBase = GetPhysicalAddress(Process, Address);
	if (physicalBase == NULL)
	{
		DbgPrint("sys: 将虚拟地址转换为物理地址失败\n");
		return 0;
	}


	MM_COPY_ADDRESS Read = {0 };
	Read.PhysicalAddress.QuadPart = (LONG64)physicalBase;
	__try
	{
		MmCopyMemory(Buffer, Read, Length, MM_COPY_MEMORY_PHYSICAL, &readLen);
	}
	__except (1)
	{
		DbgPrint("ys: MmCopyMemory================>\n");
	}
	
	
	return readLen;
}



UINT64 KWriteProcessMemory_Physical(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer)
{
	UINT64 retLen = 0;
	PVOID physicalBase = GetPhysicalAddress(Process, Address);
	if (physicalBase == NULL)
	{
		DbgPrint("sys: 将虚拟地址转换为物理地址失败\n");
		return 0;
	}


	PHYSICAL_ADDRESS Write = { 0 };
	Write.QuadPart = (LONG64)physicalBase;

	// 将物理空间映射为虚拟空间
	PVOID map = MmMapIoSpace(Write, Length, (MEMORY_CACHING_TYPE)PAGE_READWRITE);

	if (!map)
	{
		DbgPrint("sys: MmMapIoSpace physical memory\n");
		return retLen;
	}

	// 开始拷贝数据
	__try
	{
		RtlCopyMemory(map, Buffer, Length);
		retLen = Length;
	}
	__except (1)
	{
		DbgPrint("sys: RtlCopyMemory physical memory\n");
	}

	MmUnmapIoSpace(map, Length);

	return retLen;
}
