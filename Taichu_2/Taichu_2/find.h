

#include <Windows.h>

// dllBase 内核模块基址
// size  内核模块大小
// code  特征码数组首地址
// len  特征码大小
// pFound 二级指针，传入传出参数
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
			OutputDebugStringA("搜索到特征码");
			return 0;
		}
	}
	return 2;
}
