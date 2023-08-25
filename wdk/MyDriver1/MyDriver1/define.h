#pragma once



#include <ntddk.h>




typedef struct _LDR_DATA
{
	/*0x000*/     struct _LIST_ENTRY InLoadOrderLinks;
	/*0x010*/     struct _LIST_ENTRY InMemoryOrderLinks;
	/*0x020*/     struct _LIST_ENTRY InInitializationOrderLinks;
	/*0x030*/     VOID* DllBase;
	/*0x038*/     VOID* EntryPoint;
	/*0x040*/     ULONG32      SizeOfImage;
	/*0x044*/     UINT8        _PADDING0_[0x4];
	/*0x048*/     struct _UNICODE_STRING FullDllName;
	/*0x058*/     struct _UNICODE_STRING BaseDllName;
	/*0x068*/     ULONG32      Flags;
	/*0x06C*/     UINT16       LoadCount;
	/*0x06E*/     UINT16       TlsIndex;
	union
	{
		/*0x070*/         struct _LIST_ENTRY HashLinks;
		struct
		{
			/*0x070*/             VOID* SectionPointer;
			/*0x078*/             ULONG32      CheckSum;
			/*0x07C*/             UINT8        _PADDING1_[0x4];
		};
	};
	union
	{
		/*0x080*/         ULONG32      TimeDateStamp;
		/*0x080*/         VOID* LoadedImports;
	};
	/*0x088*/     struct _ACTIVATION_CONTEXT* EntryPointActivationContext;
	/*0x090*/     VOID* PatchInformation;
	/*0x098*/     struct _LIST_ENTRY ForwarderLinks;
	/*0x0A8*/     struct _LIST_ENTRY ServiceTagLinks;
	/*0x0B8*/     struct _LIST_ENTRY StaticLinks;
	/*0x0C8*/     VOID* ContextInformation;
	/*0x0D0*/     UINT64       OriginalBase;
	/*0x0D8*/     union _LARGE_INTEGER LoadTime;
}LDR_DATA, * PLDR_DATA;

//这里字节对齐要采用默认，不要按1对齐，这样才符合32位和64位结构体
typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	union
	{
		LIST_ENTRY HashLinks;
		struct
		{
			PVOID SectionPointer;
			ULONG CheckSum;
		};
	};
	union
	{
		struct
		{
			ULONG TimeDateStamp;
		};
		struct
		{
			PVOID LoadedImports;
		};
	};
	struct _ACTIVATION_CONTEXT* EntryPointActivationContext;
	PVOID PatchInformation;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _OPERATION_INFO_ENTRY
{
	LIST_ENTRY    ListEntry;
	OB_OPERATION  Operation;
	ULONG         Flags;
	PVOID         Object;
	POBJECT_TYPE  ObjectType;
	ACCESS_MASK   AccessMask;
	ULONG32 time;
} OPERATION_INFO_ENTRY, * POPERATION_INFO_ENTRY;

typedef struct _CALL_BACK_INFO
{
	ULONG64 Unknow;
	ULONG64 Unknow1;
	UNICODE_STRING AltitudeString;
	LIST_ENTRY NextEntryItemList; //(callbacklist) 跟上面开头的那个一样 存储下一个callbacklist
	ULONG64 Operations;
	PVOID ObHandle; //存储详细的数据 版本号 POB_OPERATION_REGISTRATION AltitudeString 也就是本身节点CALL_BACK_INFO 注销时也使用这个 注意是指针 //CALL_BACK_INFO
	PVOID ObjectType;
	ULONG64 PreCallbackAddr;
	ULONG64 PostCallbackAddr;
}CALL_BACK_INFO, * PCALL_BACK_INFO;

typedef struct _OB_CALLBACK
{
	LIST_ENTRY	ListEntry;
	ULONG64		Operations;
	PCALL_BACK_INFO		ObHandle;
	ULONG64		ObjTypeAddr;
	ULONG64		PreCall;
	ULONG64		PostCall;
} OB_CALLBACK, * POB_CALLBACK;



//kd > dt _KTIMER_TABLE
//nt!_KTIMER_TABLE
//+ 0x000 TimerExpiry      : [64] Ptr64 _KTIMER
//+ 0x200 TimerEntries : [256] _KTIMER_TABLE_ENTRY




//kd > dt _KTIMER
//nt!_KTIMER
//+ 0x000 Header           : _DISPATCHER_HEADER
//+ 0x018 DueTime : _ULARGE_INTEGER
//+ 0x020 TimerListEntry : _LIST_ENTRY
//+ 0x030 Dpc : Ptr64 _KDPC
//+ 0x038 Processor : Uint4B
//+ 0x03c Period : Uint4B


typedef struct _KTIMER_TABLE_ENTRY
{
	ULONG_PTR   Lock;
	LIST_ENTRY  Entry;
	ULONG_PTR   Time;
}KTIMER_TABLE_ENTRY, * PKTIMER_TABLE_ENTRY;

typedef struct _KTIMER_TABLE
{
	ULONG_PTR           TimerExpiry[64];
	KTIMER_TABLE_ENTRY  TimerEntries[256];
}KTIMER_TABLE, * PKTIMER_TABLE;


NTKERNELAPI NTSTATUS ObReferenceObjectByName(
	__in PUNICODE_STRING ObjectName,
	__in ULONG Attributes,
	__in_opt PACCESS_STATE AccessState,
	__in_opt ACCESS_MASK DesiredAccess,
	__in POBJECT_TYPE ObjectType,
	__in KPROCESSOR_MODE AccessMode,
	__inout_opt PVOID ParseContext,
	__out PVOID* Object
);



extern POBJECT_TYPE* IoDriverObjectType;