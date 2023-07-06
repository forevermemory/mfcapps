

#include <ntddk.h>

// dllBase 内核模块基址
// size  内核模块大小
// code  特征码数组首地址
// len  特征码大小
// pFound 二级指针，传入传出参数
NTSTATUS SearchPattern(PVOID dllBase, int size, PUCHAR code, int len, PVOID * pFound)
{

    PUCHAR base = (PUCHAR)(dllBase);
    //798aad68   fffff806`798a7000
	for (int i = 0; i < size; i++)
	{
		BOOLEAN found = TRUE;
		for (int j = 0; j < len; j++)
		{
			if (code[j] != base[i + j])
			{
				found = FALSE;
				break;
			}

		}

		if (found)
		{
			*pFound = ((ULONGLONG)dllBase + i);
            DbgPrint("特征码匹配地址:%d [%p]\n", i, *pFound);
			return STATUS_SUCCESS;
		}
	}
    return 2;
}
