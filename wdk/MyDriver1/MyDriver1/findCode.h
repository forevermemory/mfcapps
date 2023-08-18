#pragma once
#include <ntddk.h>


// dllBase 内核模块基址
// size  内核模块大小
// code  特征码数组首地址
// len  特征码大小
// pFound 二级指针，传入传出参数
NTSTATUS SearchPattern(PVOID dllBase, int size, PUCHAR code, int len, PVOID pFound)
{
	DbgPrint("enter SearchPattern,dllBase:%llX\n", dllBase);
	DbgPrint("enter SearchPattern,size:%llX\n", size);
	DbgPrint("enter SearchPattern,code length:%d\n", len);

	PUCHAR base = (PUCHAR)(dllBase);
	//798aad68   fffff806`798a7000
	for (int i = 0; i < size; i++)
	{
		if (i % 0x10000 == 0)
		{
			DbgPrint("MmIsAddressValid:%X %p\n", i, base+i);
		}
		

		BOOLEAN found = TRUE;
		for (int j = 0; j < len; j++)
		{
			// 判断地址是否有效  ntoskrnl.exe有时地址无效，蓝屏报错：PAGE FAULED IN NONPAGED AREA
			if (FALSE == MmIsAddressValid((PVOID)(base+i + j)))
			{
				//DbgPrint("ntoskrnl.exe地址无效 %X j=%d %p\n", i,j, base + i);
				found = FALSE;
				break;
			}

			if (code[j] != base[i + j])
			{
				found = FALSE;
				break;
			}

		}

		if (found)
		{
			DbgPrint("================================特征码匹配地址:%d [%p]\n", i, (ULONG64)dllBase + i);

			*((UINT64*)pFound) = (ULONG64)dllBase + i;
			DbgPrint("特征码匹配地址:%d [%p]\n", i, (ULONG64)dllBase + i);
			return STATUS_SUCCESS;
		}
		
	}
	return 2;
}
