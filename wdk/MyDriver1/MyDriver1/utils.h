#pragma once

#include <ntddk.h>


ULONG GetVersionAndHardCode();

// ���ݵ�ַ �ж���������ģ��
BOOLEAN ObGetDriverNameByPoint(ULONG_PTR Point, WCHAR* szDriverName);


PVOID GetMovPoint(PVOID pCallPoint);

// ��ȡδ�ĵ����ĺ���
PVOID GetUndocumentFunctionAddress(IN PUNICODE_STRING pFunName,
	IN PUCHAR pStartAddress,
	IN UCHAR* pFeatureCode,
	IN ULONG FeatureCodeNum,
	ULONG SerSize,
	UCHAR SegCode,
	ULONG AddNum,
	BOOLEAN ByName);