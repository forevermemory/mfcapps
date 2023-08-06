

#include <ntddk.h>

// dllBase �ں�ģ���ַ
// size  �ں�ģ���С
// code  �����������׵�ַ
// len  �������С
// pFound ����ָ�룬���봫������
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
            DbgPrint("������ƥ���ַ:%d [%p]\n", i, *pFound);
			return STATUS_SUCCESS;
		}
	}
    return 2;
}
