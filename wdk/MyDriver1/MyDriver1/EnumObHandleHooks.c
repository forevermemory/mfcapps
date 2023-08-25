
#include<ntifs.h>
#include "EnumObHandleHooks.h"
#include "define.h"
#include "utils.h"





// ��ȡPsLoadedModuleList��ַ�������жϵ�ַ����ģ��
PVOID GetPsLoadedListModule();







PVOID GetCallPoint(PVOID pCallPoint)
{
	ULONG dwOffset = 0;
	ULONG_PTR returnAddress = 0;
	LARGE_INTEGER returnAddressTemp = { 0 };
	PUCHAR pFunAddress = NULL;

	if (pCallPoint == NULL || !MmIsAddressValid(pCallPoint))
		return NULL;

	pFunAddress = pCallPoint;
	// ����ƫ��  
	RtlCopyMemory(&dwOffset, (PVOID)(pFunAddress + 1), sizeof(ULONG));

	// JMP������ת  
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

	ULONG ObjectCallbackListOffset =  GetVersionAndHardCode();


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

	DbgPrint("sys: ���̶���ص�==================================\n");
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

	DbgPrint("sys: �̶߳���ص�==================================\n");
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






