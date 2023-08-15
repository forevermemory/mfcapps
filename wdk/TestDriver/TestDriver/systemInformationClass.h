
#pragma once
#include <Windows.h>
//#include "define.h"

typedef LONG KPRIORITY, * PKPRIORITY;



typedef struct _PEB
{
    BOOLEAN InheritedAddressSpace;
    BOOLEAN ReadImageFileExecOptions;
    BOOLEAN BeingDebugged;
    // ........
} PEB, * PPEB;

// ProcessBasicInformation = 0
typedef struct _PROCESS_BASIC_INFORMATION
{
    LONG ExitStatus;
    PPEB PebBaseAddress;
    KAFFINITY AffinityMask;
    KPRIORITY BasePriority;
    HANDLE UniqueProcessId;
    HANDLE InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION, * PPROCESS_BASIC_INFORMATION;


// SystemProcessorInformation = 1
typedef struct _SYSTEM_PROCESSOR_INFORMATION
{
    USHORT ProcessorArchitecture;
    USHORT ProcessorLevel;
    USHORT ProcessorRevision;
    USHORT MaximumProcessors;
    ULONG ProcessorFeatureBits;
} SYSTEM_PROCESSOR_INFORMATION, * PSYSTEM_PROCESSOR_INFORMATION;




// SYSTEM_PERFORMANCE_INFORMATION = 2

/* Documented in "Windows NT/2000 Native API Reference" by Gary Nebbett. */
typedef struct _SYSTEM_PERFORMANCE_INFORMATION {
    LARGE_INTEGER IdleTime;
    LARGE_INTEGER ReadTransferCount;
    LARGE_INTEGER WriteTransferCount;
    LARGE_INTEGER OtherTransferCount;
    ULONG ReadOperationCount;
    ULONG WriteOperationCount;
    ULONG OtherOperationCount;
    ULONG AvailablePages;
    ULONG TotalCommittedPages;
    ULONG TotalCommitLimit;
    ULONG PeakCommitment;
    ULONG PageFaults;
    ULONG WriteCopyFaults;
    ULONG TransitionFaults;
    ULONG Reserved1;
    ULONG DemandZeroFaults;
    ULONG PagesRead;
    ULONG PageReadIos;
    ULONG Reserved2[2];
    ULONG PagefilePagesWritten;
    ULONG PagefilePageWriteIos;
    ULONG MappedFilePagesWritten;
    ULONG MappedFilePageWriteIos;
    ULONG PagedPoolUsage;
    ULONG NonPagedPoolUsage;
    ULONG PagedPoolAllocs;
    ULONG PagedPoolFrees;
    ULONG NonPagedPoolAllocs;
    ULONG NonPagedPoolFrees;
    ULONG TotalFreeSystemPtes;
    ULONG SystemCodePage;
    ULONG TotalSystemDriverPages;
    ULONG TotalSystemCodePages;
    ULONG SmallNonPagedLookasideListAllocateHits;
    ULONG SmallPagedLookasideListAllocateHits;
    ULONG Reserved3;
    ULONG MmSystemCachePage;
    ULONG PagedPoolPage;
    ULONG SystemDriverPage;
    ULONG FastReadNoWait;
    ULONG FastReadWait;
    ULONG FastReadResourceMiss;
    ULONG FastReadNotPossible;
    ULONG FastMdlReadNoWait;
    ULONG FastMdlReadWait;
    ULONG FastMdlReadResourceMiss;
    ULONG FastMdlReadNotPossible;
    ULONG MapDataNoWait;
    ULONG MapDataWait;
    ULONG MapDataNoWaitMiss;
    ULONG MapDataWaitMiss;
    ULONG PinMappedDataCount;
    ULONG PinReadNoWait;
    ULONG PinReadWait;
    ULONG PinReadNoWaitMiss;
    ULONG PinReadWaitMiss;
    ULONG CopyReadNoWait;
    ULONG CopyReadWait;
    ULONG CopyReadNoWaitMiss;
    ULONG CopyReadWaitMiss;
    ULONG MdlReadNoWait;
    ULONG MdlReadWait;
    ULONG MdlReadNoWaitMiss;
    ULONG MdlReadWaitMiss;
    ULONG ReadAheadIos;
    ULONG LazyWriteIos;
    ULONG LazyWritePages;
    ULONG DataFlushes;
    ULONG DataPages;
    ULONG ContextSwitches;
    ULONG FirstLevelTbFills;
    ULONG SecondLevelTbFills;
    ULONG SystemCalls;
} SYSTEM_PERFORMANCE_INFORMATION, * PSYSTEM_PERFORMANCE_INFORMATION;


// SystemTimeOfDayInformation = 3
typedef struct _SYSTEM_TIMEOFDAY_INFORMATION
{
    LARGE_INTEGER BootTime;
    LARGE_INTEGER CurrentTime;
    LARGE_INTEGER TimeZoneBias;
    ULONG TimeZoneId;
    ULONG Reserved;
    ULONGLONG BootTimeBias;
    ULONGLONG SleepTimeBias;
} SYSTEM_TIMEOFDAY_INFORMATION, * PSYSTEM_TIMEOFDAY_INFORMATION;


//


typedef struct _CLIENT_ID
{
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID, * PCLIENT_ID;

typedef struct _CLIENT_ID32
{
    ULONG UniqueProcess;
    ULONG UniqueThread;
} CLIENT_ID32, * PCLIENT_ID32;

typedef struct _CLIENT_ID64
{
    ULONGLONG UniqueProcess;
    ULONGLONG UniqueThread;
} CLIENT_ID64, * PCLIENT_ID64;


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


typedef enum _KWAIT_REASON
{
    Executive,
    FreePage,
    PageIn,
    PoolAllocation,
    DelayExecution,
    Suspended,
    UserRequest,
    WrExecutive,
    WrFreePage,
    WrPageIn,
    WrPoolAllocation,
    WrDelayExecution,
    WrSuspended,
    WrUserRequest,
    WrEventPair,
    WrQueue,
    WrLpcReceive,
    WrLpcReply,
    WrVirtualMemory,
    WrPageOut,
    WrRendezvous,
    WrKeyedEvent,
    WrTerminated,
    WrProcessInSwap,
    WrCpuRateControl,
    WrCalloutStack,
    WrKernel,
    WrResource,
    WrPushLock,
    WrMutex,
    WrQuantumEnd,
    WrDispatchInt,
    WrPreempted,
    WrYieldExecution,
    WrFastMutex,
    WrGuardedMutex,
    WrRundown,
    WrAlertByThreadId,
    WrDeferredPreempt,
    WrPhysicalFault,
    WrIoRing,
    WrMdlCache,
    MaximumWaitReason
} KWAIT_REASON, * PKWAIT_REASON;


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

typedef struct _UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    _Field_size_bytes_part_opt_(MaximumLength, Length) PWCH Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

// SystemProcessInformation = 5
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




// SystemDeviceInformation = 7
typedef struct _SYSTEM_DEVICE_INFORMATION
{
    ULONG NumberOfDisks;
    ULONG NumberOfFloppies;
    ULONG NumberOfCdRoms;
    ULONG NumberOfTapes;
    ULONG NumberOfSerialPorts;
    ULONG NumberOfParallelPorts;
} SYSTEM_DEVICE_INFORMATION, * PSYSTEM_DEVICE_INFORMATION;


// SystemModuleInformation = 11 驱动模块信息
typedef struct _RTL_PROCESS_MODULE_INFORMATION
{
    HANDLE Section;
    PVOID MappedBase;
    PVOID ImageBase;
    ULONG ImageSize;
    ULONG Flags;
    USHORT LoadOrderIndex;
    USHORT InitOrderIndex;
    USHORT LoadCount;
    USHORT OffsetToFileName;
    UCHAR FullPathName[256];
} RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES
{
    ULONG NumberOfModules;
    RTL_PROCESS_MODULE_INFORMATION Modules[1];
} RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;



// SystemProcessIdInformation 88  maybe call from r0
typedef struct _SYSTEM_PROCESS_ID_INFORMATION
{
    HANDLE ProcessId;
    UNICODE_STRING ImageName;
} SYSTEM_PROCESS_ID_INFORMATION, * PSYSTEM_PROCESS_ID_INFORMATION;













