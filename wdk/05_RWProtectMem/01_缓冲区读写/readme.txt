编译时候需要加上选项,配置如下
项目 -- 属性 -- 链接器 -- 命令行 -- 其它选项
	/INTEGRITYCHECK 

测试环境:
	win10  1903 (18362.30)

功能:
	内核缓冲区方式读写特定进程的虚拟内存

使用的函数:
	PsLookupProcessByProcessId
	ExAllocatePool    ExFreePool 
	KeStackAttachProcess KeUnstackDetachProcess
	RtlCopyMemory
	
	
实验程序: 
	驱动文件
	被读取的程序(可以自行编写，或者打开个计算器)
	Test程序(MFC),用来和驱动通信


实验结果:
	未开启保护时候,Test程序和ce可以正常读取目标程序
	开启保护时候,Test程序通过驱动正常读写，ce获取新的句柄则无法读写
	

结论:
	内核缓冲区适合小内存的读写,因为要多次拷贝内存
	