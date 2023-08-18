#pragma once
#include <ntddk.h>


// dllBase �ں�ģ���ַ
// size  �ں�ģ���С
// code  �����������׵�ַ
// len  �������С
// pFound ����ָ�룬���봫������
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
			// �жϵ�ַ�Ƿ���Ч  ntoskrnl.exe��ʱ��ַ��Ч����������PAGE FAULED IN NONPAGED AREA
			if (FALSE == MmIsAddressValid((PVOID)(base+i + j)))
			{
				//DbgPrint("ntoskrnl.exe��ַ��Ч %X j=%d %p\n", i,j, base + i);
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
			DbgPrint("================================������ƥ���ַ:%d [%p]\n", i, (ULONG64)dllBase + i);

			*((UINT64*)pFound) = (ULONG64)dllBase + i;
			DbgPrint("������ƥ���ַ:%d [%p]\n", i, (ULONG64)dllBase + i);
			return STATUS_SUCCESS;
		}
		
	}
	return 2;
}
