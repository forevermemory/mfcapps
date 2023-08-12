编译时候需要加上选项,配置如下
项目 -- 属性 -- 链接器 -- 命令行 -- 其它选项
	/INTEGRITYCHECK 

测试环境:
	win10  1903 (18362.30)

功能:
	MDL方式写特定进程的只读虚拟内存
	

使用的函数:
	1.为UserBuffer创建MDL内存描述
	2.标记为非分页内存
	3.锁定映射用户内存到内核内存
	进程挂靠,内存拷贝,解除挂靠,释放MDL

	PsLookupProcessByProcessId
	IoAllocateMdl        IoFreeMdl    
	MmBuildMdlForNonPagedPool
	MmMapLockedPages     MmUnmapLockedPages
	KeStackAttachProcess KeUnstackDetachProcess
	RtlCopyMemory
	
	
实验程序: 
	驱动文件
	被读取的程序(可以自行编写，或者打开个计算器)
	Test程序(MFC),用来和驱动通信


流程:
	为目标进程的Address创建MDL内存描述
	转换成非分页内存
	g_pmdl->MdlFlags = MDL_WRITE_OPERATION
	
主要函数:


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