#pragma once

//#include <ntddk.h>

//PspCidTable 	UNICODE_STRING usPsLoadedModuleList = RTL_CONSTANT_STRING(L"PsLoadedModuleList");
//MmGetSystemRoutineAddress(&usPsLoadedModuleList);


//
//kd > dt _HANDLE_TABLE
//nt!_HANDLE_TABLE
//+ 0x000 NextHandleNeedingPool : Uint4B
//+ 0x004 ExtraInfoPages : Int4B
//+ 0x008 TableCode : Uint8B
//+ 0x010 QuotaProcess : Ptr64 _EPROCESS
//+ 0x018 HandleTableList : _LIST_ENTRY
//+ 0x028 UniqueProcessId : Uint4B
//+ 0x02c Flags : Uint4B
//+ 0x02c StrictFIFO : Pos 0, 1 Bit
//+ 0x02c EnableHandleExceptions : Pos 1, 1 Bit
//+ 0x02c Rundown : Pos 2, 1 Bit
//+ 0x02c Duplicated : Pos 3, 1 Bit
//+ 0x02c RaiseUMExceptionOnInvalidHandleClose : Pos 4, 1 Bit
//+ 0x030 HandleContentionEvent : _EX_PUSH_LOCK
//+ 0x038 HandleTableLock : _EX_PUSH_LOCK
//+ 0x040 FreeLists : [1] _HANDLE_TABLE_FREE_LIST
//+ 0x040 ActualEntry : [32] UChar
//+ 0x060 DebugInfo : Ptr64 _HANDLE_TRACE_DEBUG_INFO


typedef struct _HANDLE_TABLE 
{
	UINT32 NextHandleNeedingPool;
	UINT32 ExtraInfoPages;
	UINT64 TableCode;
	UINT64 QuotaProcess; // _EPROCESS
	LIST_ENTRY HandleTableList;
	UINT32 UniqueProcessId;
	union 
	{
		UINT32 Flags;
	};
	UINT64 HandleContentionEvent; // _EX_PUSH_LOCK
	UINT64 HandleTableLock; // _EX_PUSH_LOCK
} HANDLE_TABLE, *PHANDLE_TABLE;





typedef UINT32 SYSTEM_INFORMATION_CLASS;

NTSTATUS  ZwQuerySystemInformation(
    _In_      SYSTEM_INFORMATION_CLASS SystemInformationClass,
    _Inout_   PVOID                    SystemInformation,
    _In_      ULONG                    SystemInformationLength,
    _Out_opt_ PULONG                   ReturnLength
);


typedef enum _KTHREAD_STATE
{
    Initialized,
    Ready,
    Running,
    Standby,
    Terminated,
    Waiting,
    Transition,
    DeferredReady,
    GateWaitObsolete,
    WaitingForProcessInSwap,
    MaximumThreadState
} KTHREAD_STATE, * PKTHREAD_STATE;

typedef struct _SYSTEM_THREAD_INFORMATION
{
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER CreateTime;
    ULONG WaitTime;
    PVOID StartAddress;
    CLIENT_ID ClientId;
    KPRIORITY Priority;
    KPRIORITY BasePriority;
    ULONG ContextSwitches;
    KTHREAD_STATE ThreadState;
    KWAIT_REASON WaitReason;
} SYSTEM_THREAD_INFORMATION, * PSYSTEM_THREAD_INFORMATION;

// SystemProcessInformation = 5

#define SystemProcessInformation 5
typedef struct _SYSTEM_PROCESS_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG NumberOfThreads;
    LARGE_INTEGER WorkingSetPrivateSize; // Since Vista
    ULONG HardFaultCount; // Since Windows 7
    ULONG NumberOfThreadsHighWatermark; // Since Windows 7
    ULONGLONG CycleTime; // Since Windows 7
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
    UNICODE_STRING ImageName;
    KPRIORITY BasePriority;
    HANDLE UniqueProcessId;
    HANDLE InheritedFromUniqueProcessId;
    ULONG HandleCount;
    ULONG SessionId;
    ULONG_PTR UniqueProcessKey; // Since Vista (requires SystemExtendedProcessInformation)
    SIZE_T PeakVirtualSize;
    SIZE_T VirtualSize;
    ULONG PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
    SIZE_T PrivatePageCount;
    LARGE_INTEGER ReadOperationCount;
    LARGE_INTEGER WriteOperationCount;
    LARGE_INTEGER OtherOperationCount;
    LARGE_INTEGER ReadTransferCount;
    LARGE_INTEGER WriteTransferCount;
    LARGE_INTEGER OtherTransferCount;
    SYSTEM_THREAD_INFORMATION Threads[1]; // SystemProcessInformation
    // SYSTEM_EXTENDED_THREAD_INFORMATION Threads[1]; // SystemExtendedProcessinformation
    // SYSTEM_EXTENDED_THREAD_INFORMATION + SYSTEM_PROCESS_INFORMATION_EXTENSION // SystemFullProcessInformation
} SYSTEM_PROCESS_INFORMATION, * PSYSTEM_PROCESS_INFORMATION;


typedef struct _OBJECT_TYPE
{
    LIST_ENTRY TypeList;
    UNICODE_STRING Name;
    PVOID DefaultObject; 
    UINT32 Index;
    UINT32 TotalNumberOfObjects;
    UINT32 TotalNumberOfHandles;
    UINT32 HighWaterNumberOfObjects;
    UINT32 HighWaterNumberOfHandles;
    UINT64 TypeLock;
    UINT32 Key;
    LIST_ENTRY CallbackList;

} OBJECT_TYPE, *POBJECT_TYPE;

OBJECT_TYPE* ObGetObjectType(UINT_PTR obj);

//
//+0x000 TypeList         : _LIST_ENTRY[]
//+ 0x010 Name : _UNICODE_STRING "WindowStation"
//+ 0x020 DefaultObject : (null)
//+0x028 Index : 0x18 ''
//+ 0x02c TotalNumberOfObjects : 6
//+ 0x030 TotalNumberOfHandles : 0xb9
//+ 0x034 HighWaterNumberOfObjects : 7
//+ 0x038 HighWaterNumberOfHandles : 0xdf
//+ 0x040 TypeInfo : _OBJECT_TYPE_INITIALIZER
//+ 0x0b8 TypeLock : _EX_PUSH_LOCK
//+ 0x0c0 Key : 0x646e6957
//+ 0x0c8 CallbackList : _LIST_ENTRY[]

UINT64 FindPspCidTable();
void EnumPspCidTable();


UINT64 FindObpKernelHandleTable();
void EnumObpKernelHandleTable();
void EnumProcessHandleTable(PEPROCESS pEprocess);



char* PsGetProcessImageFileName(PEPROCESS);
void Test_ZwQuerySystemInformation();

void EnumDriverObjects(PDRIVER_OBJECT driverObject);