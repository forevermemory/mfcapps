
#include<ntifs.h>
#include "EnumObHandleHooks.h"



static ULONG ObjectCallbackListOffset = 0;
extern PSHORT NtBuildNumber;
// 根据地址 判断所属驱动模块
BOOLEAN ObGetDriverNameByPoint(ULONG_PTR Point, WCHAR* szDriverName);
// 获取PsLoadedModuleList地址，方便判断地址所属模块
PVOID GetPsLoadedListModule();
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






BOOLEAN GetVersionAndHardCode()
{
	/*
	* https://www.gaijin.at/en/infos/windows-version-numbers
	Name / Description	Version	Build Number	Public Release
	Windows XP	5.1	2600	2001 - 10 - 25
	Windows XP, Service Pack 1	5.1	2600.1105 - 1106	2002 - 09 - 09
	Windows XP, Service Pack 2	5.1	2600.2180	2004 - 08 - 25
	Windows Vista	6.0	6000	2007 - 01 - 30
	Windows Vista, Service Pack 1	6.0	6001	2008 - 02 - 04
	Windows XP, Service Pack 3	5.1	2600	2008 - 04 - 21
	Windows Vista, Service Pack 2	6.0	6002	2009 - 05 - 26

	Windows 7	                 6.1	7600	2009 - 10 - 22
	Windows 7, Service Pack 1	 6.1	7601	2011 - 02 - 22
	Windows 8	                 6.2	9200	2012 - 10 - 26
	Windows 8.1	                 6.3	9600	2013 - 08 - 27
	Windows 10, Version 1507	10.0	10240	2015 - 07 - 29
	Windows 10, Version 1511	10.0	10586	2015 - 11 - 10
	Windows 10, Version 1607	10.0	14393	2016 - 08 - 02
	Windows 10, Version 1703	10.0	15063	2017 - 04 - 05
	Windows 10, Version 1709	10.0	16299	2017 - 10 - 17
	Windows 10, Version 1803	10.0	17134	2018 - 04 - 30
	Windows 10, Version 1809	10.0	17763	2018 - 11 - 13
	Windows 10, Version 1903	10.0	18362	2019 - 05 - 21
	Windows 10, Version 1909	10.0	18363	2019 - 11 - 12
	Windows 10, Version 2004	10.0	19041	2020 - 05 - 27
	Windows 10, Version 20H2	10.0	19042	2020 - 10 - 20
	Windows 10, Version 21H1	10.0	19043	2021 - 05 - 18
	Windows 11, Version 21H2	10.0	22000	2021 - 10 - 04
	Windows 10, Version 21H2	10.0	19044	2021 - 11 - 16
	Windows 11, Version 22H2	10.0	22621	2022 - 09 - 20
	Windows 10, Version 22H2	10.0	19045	2022 - 10 - 18

	*/


	BOOLEAN b = FALSE;
	switch (*NtBuildNumber)
	{
	case 7600:
	case 7601:
	{
		ObjectCallbackListOffset = 0xC0;
		b = TRUE;
		break;
	}
	case 9200:
	{
		ObjectCallbackListOffset = 0xC8;	//OBJECT_TYPE.CallbackList
		b = TRUE;
		break;
	}
	case 9600:
	{
		ObjectCallbackListOffset = 0xC8;	//OBJECT_TYPE.CallbackList
		b = TRUE;
		break;
	}
	default:
		if (*NtBuildNumber > 10000)
		{
			ObjectCallbackListOffset = 0xc8;
			b = TRUE;
		}
		break;
	}
	return b;
}

PVOID GetCallPoint(PVOID pCallPoint)
{
	ULONG dwOffset = 0;
	ULONG_PTR returnAddress = 0;
	LARGE_INTEGER returnAddressTemp = { 0 };
	PUCHAR pFunAddress = NULL;

	if (pCallPoint == NULL || !MmIsAddressValid(pCallPoint))
		return NULL;

	pFunAddress = pCallPoint;
	// 函数偏移  
	RtlCopyMemory(&dwOffset, (PVOID)(pFunAddress + 1), sizeof(ULONG));

	// JMP向上跳转  
	if ((dwOffset & 0x10000000) == 0x10000000)
	{
		dwOffset = dwOffset + 5 + pFunAddress;
		returnAddressTemp.QuadPart = (ULONG_PTR)pFunAddress & 0xFFFFFFFF00000000;
		returnAddressTemp.LowPart = dwOffset;
		returnAddress = returnAddressTemp.QuadPart;
		return (PVOID)returnAddress;
	}

	returnAddress = (ULONG_PTR)dwOffset + 5 + pFunAddress;
	return (PVOID)returnAddress;
}

ULONG EnumObRegisterCallBacks()
{
	ULONG c = 0;
	PLIST_ENTRY CurrEntry = NULL;
	POB_CALLBACK pObCallback;
	ULONG64 ObProcessCallbackListHead = 0;
	ULONG64 ObThreadCallbackListHead = 0;
	WCHAR* szDriverBaseName = NULL;
	szDriverBaseName = ExAllocatePool(NonPagedPool, 600);
	if (szDriverBaseName == NULL)
		return FALSE;

	RtlZeroMemory(szDriverBaseName, 600);

	GetVersionAndHardCode();


	//kd > dt _OBJECT_TYPE
	//nt!_OBJECT_TYPE
	//+ 0x000 TypeList         : _LIST_ENTRY
	//+ 0x010 Name : _UNICODE_STRING
	//+ 0x020 DefaultObject : Ptr64 Void
	//+ 0x028 Index : UChar
	//+ 0x02c TotalNumberOfObjects : Uint4B
	//+ 0x030 TotalNumberOfHandles : Uint4B
	//+ 0x034 HighWaterNumberOfObjects : Uint4B
	//+ 0x038 HighWaterNumberOfHandles : Uint4B
	//+ 0x040 TypeInfo : _OBJECT_TYPE_INITIALIZER
	//+ 0x0b8 TypeLock : _EX_PUSH_LOCK
	//+ 0x0c0 Key : Uint4B
	//+ 0x0c8 CallbackList : _LIST_ENTRY

	ObProcessCallbackListHead = *(ULONG64*)PsProcessType + ObjectCallbackListOffset;
	ObThreadCallbackListHead = *(ULONG64*)PsThreadType + ObjectCallbackListOffset;

	DbgPrint("sys: 进程对象回调==================================\n");
	CurrEntry = ((PLIST_ENTRY)ObProcessCallbackListHead)->Flink;
	if (CurrEntry == NULL || !MmIsAddressValid(CurrEntry))
	{
		ExFreePool(szDriverBaseName);
		return 0;
	}

	do
	{
		pObCallback = (POB_CALLBACK)CurrEntry;
		if (pObCallback->ObHandle != 0)
		{
			if (ObGetDriverNameByPoint(pObCallback->PreCall, szDriverBaseName))
			{
				DbgPrint("sys: DriverName=%S ObHandle=%p  Index=%wZ PreCall=%p PostCall=%p \n",
					szDriverBaseName,
					pObCallback->ObHandle,
					&pObCallback->ObHandle->AltitudeString,
					pObCallback->PreCall,
					pObCallback->PostCall);
			}

			c++;
		}

		CurrEntry = CurrEntry->Flink;

	} while (CurrEntry != (PLIST_ENTRY)ObProcessCallbackListHead);

	DbgPrint("sys: 线程对象回调==================================\n");
	CurrEntry = ((PLIST_ENTRY)ObThreadCallbackListHead)->Flink;
	if (CurrEntry == NULL || !MmIsAddressValid(CurrEntry))
	{
		ExFreePool(szDriverBaseName);
		return c;
	}

	do
	{
		pObCallback = (POB_CALLBACK)CurrEntry;
		if (pObCallback->ObHandle != 0)
		{
			if (ObGetDriverNameByPoint(pObCallback->PreCall, szDriverBaseName))
			{
				DbgPrint("sys: DriverName=%S ObHandle=%p  Index=%wZ PreCall=%p PostCall=%p \n",
					szDriverBaseName,
					pObCallback->ObHandle,
					&pObCallback->ObHandle->AltitudeString,
					pObCallback->PreCall,
					pObCallback->PostCall);
			}
			c++;
		}

		CurrEntry = CurrEntry->Flink;

	} while (CurrEntry != (PLIST_ENTRY)ObThreadCallbackListHead);

	DbgPrint("sys: ObCallback count: %ld\n", c);

	ExFreePool(szDriverBaseName);
	return c;
}



PVOID GetPsLoadedListModule()
{
	/*
	为什么不使用DriverObject去枚举呢 主要是win10这玩意导出了 追随步伐啊 哈哈
	RtlPcToFileHeader  // 这个函数偏移指定字节可以找到这个列表地址
	text:000000014015D6A3 48 8B 0D 86 50 0E 00           mov     rcx, qword ptr cs:PsLoadedModuleList
	.text:000000014015D6AA 48 85 C9                      test    rcx, rcx
	.text:000000014015D6AD 74 28                         jz      short loc_14015D6D7
	.text:000000014015D6AF 48 8D 15 7A 50 0E 00          lea     rdx, PsLoadedModuleList
	*/
	UNICODE_STRING usRtlPcToFileHeader = RTL_CONSTANT_STRING(L"RtlPcToFileHeader");
	UNICODE_STRING usPsLoadedModuleList = RTL_CONSTANT_STRING(L"PsLoadedModuleList");
	PVOID Point = NULL;
	static PVOID PsLoadedListModule = NULL;
	UCHAR shellcode[11] =
		"\x48\x8b\x0d\x60\x60\x60\x60"
		"\x48\x85\xc9";

	if (PsLoadedListModule)
		return PsLoadedListModule;

	// win10 PsLoadedModuleList导出
	if (*NtBuildNumber > 9600)
	{
		PsLoadedListModule = MmGetSystemRoutineAddress(&usPsLoadedModuleList);
		return PsLoadedListModule;
	}

	Point = GetUndocumentFunctionAddress(&usRtlPcToFileHeader, NULL, shellcode, 10, 0xff, 0x60, 0, TRUE);
	if (Point == NULL || !MmIsAddressValid(Point))
		return NULL;

	Point = GetMovPoint(Point);
	if (Point == NULL || !MmIsAddressValid(Point))
		return NULL;

	PsLoadedListModule = Point;
	return PsLoadedListModule;
}



BOOLEAN ObGetDriverNameByPoint(ULONG_PTR Point, WCHAR* szDriverName)
{
	PLDR_DATA_TABLE_ENTRY Begin = NULL;
	PLIST_ENTRY Head = NULL;
	PLIST_ENTRY Next = NULL;

	Begin = GetPsLoadedListModule();

	if (Begin == NULL)
		return FALSE;

	Head = (PLIST_ENTRY)Begin->InLoadOrderLinks.Flink;
	Next = Head->Flink;

	do
	{
		PLDR_DATA_TABLE_ENTRY Entry = CONTAINING_RECORD(Next, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		Next = Next->Flink;

		if ((ULONG_PTR)Entry->DllBase <= Point && Point <= ((ULONG_PTR)Entry->DllBase + Entry->SizeOfImage))
		{
			if (szDriverName == NULL)
				return FALSE;
			RtlZeroMemory(szDriverName, 600);
			RtlCopyMemory(szDriverName, Entry->BaseDllName.Buffer, Entry->BaseDllName.Length);
			return TRUE;
		}

	} while (Next != Head->Flink);

	return FALSE;
}


//获取未文档化的函数
PVOID GetUndocumentFunctionAddress(IN PUNICODE_STRING pFunName,
	IN PUCHAR pStartAddress,
	IN UCHAR* pFeatureCode,
	IN ULONG FeatureCodeNum,
	ULONG SerSize,
	UCHAR SegCode,
	ULONG AddNum,
	BOOLEAN ByName)
{
	ULONG dwIndex = 0;
	PUCHAR pFunAddress = NULL;
	ULONG dwCodeNum = 0;

	if (pFeatureCode == NULL)
		return NULL;

	if (FeatureCodeNum >= 15)
		return NULL;

	if (SerSize > 0x1024)
		return NULL;

	if (ByName)
	{
		if (pFunName == NULL || !MmIsAddressValid(pFunName->Buffer))
			return NULL;

		pFunAddress = (PUCHAR)MmGetSystemRoutineAddress(pFunName);
		if (pFunAddress == NULL)
			return NULL;
	}
	else
	{
		if (pStartAddress == NULL || !MmIsAddressValid(pStartAddress))
			return NULL;

		pFunAddress = pStartAddress;
	}

	for (dwIndex = 0; dwIndex < SerSize; dwIndex++)
	{
		__try
		{
			if (pFunAddress[dwIndex] == pFeatureCode[dwCodeNum] || pFeatureCode[dwCodeNum] == SegCode)
			{
				dwCodeNum++;

				if (dwCodeNum == FeatureCodeNum)
					return pFunAddress + dwIndex - dwCodeNum + 1 + AddNum;

				continue;
			}

			dwCodeNum = 0;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return 0;
		}
	}

	return 0;
}


PVOID GetMovPoint(PVOID pCallPoint)
{
	ULONG dwOffset = 0;
	ULONG_PTR returnAddress = 0;
	LARGE_INTEGER returnAddressTemp = { 0 };
	PUCHAR pFunAddress = NULL;

	if (pCallPoint == NULL || !MmIsAddressValid(pCallPoint))
		return NULL;

	pFunAddress = pCallPoint;
	// 函数偏移  
	RtlCopyMemory(&dwOffset, (PVOID)(pFunAddress + 3), sizeof(ULONG));

	// JMP向上跳转  
	if ((dwOffset & 0x10000000) == 0x10000000)
	{
		dwOffset = dwOffset + 7 + pFunAddress;
		returnAddressTemp.QuadPart = (ULONG_PTR)pFunAddress & 0xFFFFFFFF00000000;
		returnAddressTemp.LowPart = dwOffset;
		returnAddress = returnAddressTemp.QuadPart;
		return (PVOID)returnAddress;
	}

	returnAddress = (ULONG_PTR)dwOffset + 7 + pFunAddress;
	return (PVOID)returnAddress;
}