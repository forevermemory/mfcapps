

#include <Windows.h>

// dllBase �ں�ģ���ַ
// size  �ں�ģ���С
// code  �����������׵�ַ
// len  �������С
// pFound ����ָ�룬���봫������
DWORD SearchPattern(PVOID dllBase, int size, PUCHAR code, int len, ULONGLONG* pFound)
{

	PUCHAR base = (PUCHAR)(dllBase);

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
			OutputDebugStringA("������������");
			return 0;
		}
	}
	return 2;
}
