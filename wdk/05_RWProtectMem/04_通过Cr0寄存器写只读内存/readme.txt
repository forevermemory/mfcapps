编译时候需要加上选项,配置如下
项目 -- 属性 -- 链接器 -- 命令行 -- 其它选项
	/INTEGRITYCHECK 

测试环境:
	win10  1903 (18362.30)

功能:
	通过修改Cr0寄存器的WP位写特定进程的只读虚拟内存
		WP位0：禁用写保护的功能
		WP位1：开启写保护的功能

使用的函数:
	

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


内存类型:
PagedPool 和 NonPagedPool
	前者可以被置换到硬盘中,一般存储数据, 如打开一个大文件, 数据结构. 
	后者不能被置换到硬盘中,驻留在内存中,一般用来存储代码.  
	如果执行代码到PagedPool的内存中去了,很有可能导致蓝屏
	此外,在内核空间中所有内存都是可读可写可执行,故没有类似用户态下的VirtualProtect改变内存属性的函数
	但是并不意味着可以随意执行和改写内存中的代码.要满足2个条件: 一个是关闭内存写保护, 二个是提升IRQL级别.(防止执行出错)
	对于关闭内存写保护通过操作cr0寄存器,后者使用KeRaiseIrqlToDpcLevel 和KeLowerIrql 实现

	
主要函数:



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
