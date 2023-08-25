#pragma once

#include <ntddk.h>


ULONG GetVersionAndHardCode();

// 根据地址 判断所属驱动模块
BOOLEAN ObGetDriverNameByPoint(ULONG_PTR Point, WCHAR* szDriverName);


PVOID GetMovPoint(PVOID pCallPoint);

// 获取未文档化的函数
PVOID GetUndocumentFunctionAddress(IN PUNICODE_STRING pFunName,
	IN PUCHAR pStartAddress,
	IN UCHAR* pFeatureCode,
	IN ULONG FeatureCodeNum,
	ULONG SerSize,
	UCHAR SegCode,
	ULONG AddNum,
	BOOLEAN ByName);