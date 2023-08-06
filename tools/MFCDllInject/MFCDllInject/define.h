#pragma once
#include <Windows.h>
#include <bcrypt.h>

#define SE_DEBUG_PRIVILEGE                (20L)

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)


typedef struct _CLIENT_ID64 {
	UINT64 UniqueProcess;       // +0x000
	UINT64 UniqueThread;        // +0x008
} CLIENT_ID64, * PCLIENT_ID64;

typedef struct _CLIENT_ID32 {
	UINT32 UniqueProcess;       // +0x000
	UINT32 UniqueThread;        // +0x004
} CLIENT_ID32, * PCLIENT_ID32;


typedef struct _GDI_TEB_BATCH64 {
	union 
	{
		UINT64 flag;
		struct 
		{
			UINT64 Offset: 31;               // Pos 0, 31 Bits
			UINT64 HasRenderingCommand : 1; // Pos 31, 1 Bit
		};
	};       // +0x000
	UINT64 HDC;        // +0x008
	UINT32 Buffer[310];        // +0x0010
} GDI_TEB_BATCH64, * PGDI_TEB_BATCH64;



typedef struct _GDI_TEB_BATCH32 {
	union
	{
		UINT32 flag;
		struct
		{
			UINT32 Offset : 31;               // Pos 0, 31 Bits
			UINT32 HasRenderingCommand : 1; // Pos 31, 1 Bit
		};
	};       // +0x000
	UINT32 HDC;        // +0x004
	UINT32 Buffer[310];        // +0x008
} GDI_TEB_BATCH32, * PGDI_TEB_BATCH32;




typedef DWORD(WINAPI* pRtlCreateUserThread32)(   
	IN HANDLE                     ProcessHandle,
	IN PSECURITY_DESCRIPTOR     SecurityDescriptor,
	IN BOOL                     CreateSuspended,
	IN ULONG                    StackZeroBits,
	IN OUT PULONG                StackReserved,
	IN OUT PULONG                StackCommit,
	IN LPVOID                    StartAddress,
	IN LPVOID                    StartParameter,
	OUT HANDLE                     ThreadHandle,
	OUT LPVOID                    ClientID
	);

typedef DWORD(WINAPI* pRtlCreateUserThread64)(
	IN HANDLE                     ProcessHandle,
	IN PSECURITY_DESCRIPTOR     SecurityDescriptor,
	IN BOOL                     CreateSuspended,
	IN ULONG                    StackZeroBits,
	OUT HANDLE                     ThreadHandle,
	OUT LPVOID                    ClientID
	);
// x64
//__int64 __fastcall RtlCreateUserThread(int a1, int a2, char a3, int a4, __int64 a5, __int64 a6)
//{
//	//return RtlpCreateUserThreadEx(a1, a2, a3 == 1, a4, a5, a6);
//	return 0;
//}

typedef NTSTATUS(NTAPI* P_NtCreateThreadEx64)(
	OUT PHANDLE hThread,
	IN ACCESS_MASK DesiredAccess,
	IN PVOID ObjectAttributes,
	IN HANDLE ProcessHandle,
	IN PVOID lpStartAddress,
	IN PVOID lpParameter,
	IN ULONG Flags,
	IN SIZE_T StackZeroBits,
	IN SIZE_T SizeOfStackCommit,
	IN SIZE_T SizeOfStackReserve,
	OUT PVOID lpBytesBuffer);


typedef DWORD(WINAPI* P_NtCreateThreadEx32)(
	PHANDLE ThreadHandle,
	ACCESS_MASK DesiredAccess,
	LPVOID ObjectAttributes,
	HANDLE ProcessHandle,
	LPTHREAD_START_ROUTINE lpStartAddress,
	LPVOID lpParameter,
	BOOL CreateThreadFlags,
	DWORD  ZeroBits,
	DWORD  StackSize,
	DWORD  MaximumStackSize,
	LPVOID pUnkown);

typedef LONG KPRIORITY;

typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemBasicInformation = 0x0,
	SystemProcessorInformation = 0x1,
	SystemPerformanceInformation = 0x2,
	SystemTimeOfDayInformation = 0x3,
	SystemPathInformation = 0x4,
	SystemProcessInformation = 0x5,
	SystemCallCountInformation = 0x6,
	SystemDeviceInformation = 0x7,
	SystemProcessorPerformanceInformation = 0x8,
	SystemFlagsInformation = 0x9,
	SystemCallTimeInformation = 0xa,
	SystemModuleInformation = 0xb,
	SystemLocksInformation = 0xc,
	SystemStackTraceInformation = 0xd,
	SystemPagedPoolInformation = 0xe,
	SystemNonPagedPoolInformation = 0xf,
	SystemHandleInformation = 0x10,
	SystemObjectInformation = 0x11,
	SystemPageFileInformation = 0x12,
	SystemVdmInstemulInformation = 0x13,
	SystemVdmBopInformation = 0x14,
	SystemFileCacheInformation = 0x15,
	SystemPoolTagInformation = 0x16,
	SystemInterruptInformation = 0x17,
	SystemDpcBehaviorInformation = 0x18,
	SystemFullMemoryInformation = 0x19,
	SystemLoadGdiDriverInformation = 0x1a,
	SystemUnloadGdiDriverInformation = 0x1b,
	SystemTimeAdjustmentInformation = 0x1c,
	SystemSummaryMemoryInformation = 0x1d,
	SystemMirrorMemoryInformation = 0x1e,
	SystemPerformanceTraceInformation = 0x1f,
	SystemObsolete0 = 0x20,
	SystemExceptionInformation = 0x21,
	SystemCrashDumpStateInformation = 0x22,
	SystemKernelDebuggerInformation = 0x23,
	SystemContextSwitchInformation = 0x24,
	SystemRegistryQuotaInformation = 0x25,
	SystemExtendServiceTableInformation = 0x26,
	SystemPrioritySeperation = 0x27,
	SystemVerifierAddDriverInformation = 0x28,
	SystemVerifierRemoveDriverInformation = 0x29,
	SystemProcessorIdleInformation = 0x2a,
	SystemLegacyDriverInformation = 0x2b,
	SystemCurrentTimeZoneInformation = 0x2c,
	SystemLookasideInformation = 0x2d,
	SystemTimeSlipNotification = 0x2e,
	SystemSessionCreate = 0x2f,
	SystemSessionDetach = 0x30,
	SystemSessionInformation = 0x31,
	SystemRangeStartInformation = 0x32,
	SystemVerifierInformation = 0x33,
	SystemVerifierThunkExtend = 0x34,
	SystemSessionProcessInformation = 0x35,
	SystemLoadGdiDriverInSystemSpace = 0x36,
	SystemNumaProcessorMap = 0x37,
	SystemPrefetcherInformation = 0x38,
	SystemExtendedProcessInformation = 0x39,
	SystemRecommendedSharedDataAlignment = 0x3a,
	SystemComPlusPackage = 0x3b,
	SystemNumaAvailableMemory = 0x3c,
	SystemProcessorPowerInformation = 0x3d,
	SystemEmulationBasicInformation = 0x3e,
	SystemEmulationProcessorInformation = 0x3f,
	SystemExtendedHandleInformation = 0x40,
	SystemLostDelayedWriteInformation = 0x41,
	SystemBigPoolInformation = 0x42,
	SystemSessionPoolTagInformation = 0x43,
	SystemSessionMappedViewInformation = 0x44,
	SystemHotpatchInformation = 0x45,
	SystemObjectSecurityMode = 0x46,
	SystemWatchdogTimerHandler = 0x47,
	SystemWatchdogTimerInformation = 0x48,
	SystemLogicalProcessorInformation = 0x49,
	SystemWow64SharedInformationObsolete = 0x4a,
	SystemRegisterFirmwareTableInformationHandler = 0x4b,
	SystemFirmwareTableInformation = 0x4c,
	SystemModuleInformationEx = 0x4d,
	SystemVerifierTriageInformation = 0x4e,
	SystemSuperfetchInformation = 0x4f,
	SystemMemoryListInformation = 0x50,
	SystemFileCacheInformationEx = 0x51,
	SystemThreadPriorityClientIdInformation = 0x52,
	SystemProcessorIdleCycleTimeInformation = 0x53,
	SystemVerifierCancellationInformation = 0x54,
	SystemProcessorPowerInformationEx = 0x55,
	SystemRefTraceInformation = 0x56,
	SystemSpecialPoolInformation = 0x57,
	SystemProcessIdInformation = 0x58,
	SystemErrorPortInformation = 0x59,
	SystemBootEnvironmentInformation = 0x5a,
	SystemHypervisorInformation = 0x5b,
	SystemVerifierInformationEx = 0x5c,
	SystemTimeZoneInformation = 0x5d,
	SystemImageFileExecutionOptionsInformation = 0x5e,
	SystemCoverageInformation = 0x5f,
	SystemPrefetchPatchInformation = 0x60,
	SystemVerifierFaultsInformation = 0x61,
	SystemSystemPartitionInformation = 0x62,
	SystemSystemDiskInformation = 0x63,
	SystemProcessorPerformanceDistribution = 0x64,
	SystemNumaProximityNodeInformation = 0x65,
	SystemDynamicTimeZoneInformation = 0x66,
	SystemCodeIntegrityInformation = 0x67,
	SystemProcessorMicrocodeUpdateInformation = 0x68,
	SystemProcessorBrandString = 0x69,
	SystemVirtualAddressInformation = 0x6a,
	SystemLogicalProcessorAndGroupInformation = 0x6b,
	SystemProcessorCycleTimeInformation = 0x6c,
	SystemStoreInformation = 0x6d,
	SystemRegistryAppendString = 0x6e,
	SystemAitSamplingValue = 0x6f,
	SystemVhdBootInformation = 0x70,
	SystemCpuQuotaInformation = 0x71,
	SystemNativeBasicInformation = 0x72,
	SystemErrorPortTimeouts = 0x73,
	SystemLowPriorityIoInformation = 0x74,
	SystemBootEntropyInformation = 0x75,
	SystemVerifierCountersInformation = 0x76,
	SystemPagedPoolInformationEx = 0x77,
	SystemSystemPtesInformationEx = 0x78,
	SystemNodeDistanceInformation = 0x79,
	SystemAcpiAuditInformation = 0x7a,
	SystemBasicPerformanceInformation = 0x7b,
	SystemQueryPerformanceCounterInformation = 0x7c,
	SystemSessionBigPoolInformation = 0x7d,
	SystemBootGraphicsInformation = 0x7e,
	SystemScrubPhysicalMemoryInformation = 0x7f,
	SystemBadPageInformation = 0x80,
	SystemProcessorProfileControlArea = 0x81,
	SystemCombinePhysicalMemoryInformation = 0x82,
	SystemEntropyInterruptTimingInformation = 0x83,
	SystemConsoleInformation = 0x84,
	SystemPlatformBinaryInformation = 0x85,
	SystemThrottleNotificationInformation = 0x86,
	SystemHypervisorProcessorCountInformation = 0x87,
	SystemDeviceDataInformation = 0x88,
	SystemDeviceDataEnumerationInformation = 0x89,
	SystemMemoryTopologyInformation = 0x8a,
	SystemMemoryChannelInformation = 0x8b,
	SystemBootLogoInformation = 0x8c,
	SystemProcessorPerformanceInformationEx = 0x8d,
	SystemSpare0 = 0x8e,
	SystemSecureBootPolicyInformation = 0x8f,
	SystemPageFileInformationEx = 0x90,
	SystemSecureBootInformation = 0x91,
	SystemEntropyInterruptTimingRawInformation = 0x92,
	SystemPortableWorkspaceEfiLauncherInformation = 0x93,
	SystemFullProcessInformation = 0x94,
	SystemKernelDebuggerInformationEx = 0x95,
	SystemBootMetadataInformation = 0x96,
	SystemSoftRebootInformation = 0x97,
	SystemElamCertificateInformation = 0x98,
	SystemOfflineDumpConfigInformation = 0x99,
	SystemProcessorFeaturesInformation = 0x9a,
	SystemRegistryReconciliationInformation = 0x9b,
	MaxSystemInfoClass = 0x9c,
} SYSTEM_INFORMATION_CLASS;

typedef enum _KWAIT_REASON {
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
	MaximumWaitReason
} KWAIT_REASON;

typedef enum _PROCESSINFOCLASS {
	ProcessBasicInformation,
	ProcessQuotaLimits,
	ProcessIoCounters,
	ProcessVmCounters,
	ProcessTimes,
	ProcessBasePriority,
	ProcessRaisePriority,
	ProcessDebugPort,
	ProcessExceptionPort,
	ProcessAccessToken,
	ProcessLdtInformation,
	ProcessLdtSize,
	ProcessDefaultHardErrorMode,
	ProcessIoPortHandlers,          // Note: this is kernel mode only
	ProcessPooledUsageAndLimits,
	ProcessWorkingSetWatch,
	ProcessUserModeIOPL,
	ProcessEnableAlignmentFaultFixup,
	ProcessPriorityClass,
	ProcessWx86Information,
	ProcessHandleCount,
	ProcessAffinityMask,
	ProcessPriorityBoost,
	ProcessDeviceMap,
	ProcessSessionInformation,
	ProcessForegroundInformation,
	ProcessWow64Information,
	ProcessImageFileName,
	ProcessLUIDDeviceMapsEnabled,
	ProcessBreakOnTermination,
	ProcessDebugObjectHandle,
	ProcessDebugFlags,
	ProcessHandleTracing,
	ProcessIoPriority,
	ProcessExecuteFlags,
	ProcessResourceManagement,
	ProcessCookie,
	ProcessImageInformation,
	MaxProcessInfoClass             // MaxProcessInfoClass should always be the last enum
} PROCESSINFOCLASS;

// 系统线程信息
typedef struct _SYSTEM_THREAD_INFORMATION
{
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	UINT32 WaitTime;
	PVOID StartAddress;
	CLIENT_ID32 ClientId;
	KPRIORITY Priority;
	INT32 BasePriority;
	UINT32 ContextSwitches;
	UINT32 ThreadState;
	KWAIT_REASON WaitReason;
}SYSTEM_THREAD_INFORMATION, * PSYSTEM_THREAD_INFORMATION;

typedef struct _UNICODE_STRING
{
	USHORT					Length;
	USHORT					MaximumLength;
	UINT64					Buffer; // wchar *
} UNICODE_STRING, * PUNICODE_STRING;

// SystemProcessInformation
typedef struct _SYSTEM_PROCESS_INFO
{
	UINT32 NextEntryOffset;
	UINT32 NumberOfThreads;
	LARGE_INTEGER WorkingSetPrivateSize;
	UINT32 HardFaultCount;
	UINT32 NumberOfThreadsHighWatermark;
	UINT64 CycleTime;
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ImageName;
	KPRIORITY BasePriority;
	HANDLE UniqueProcessId;
	HANDLE InheritedFromUniqueProcessId;
	UINT32 HandleCount;
	UINT32 SessionId;
	UINT_PTR UniqueProcessKey;
	SIZE_T PeakVirtualSize;
	SIZE_T VirtualSize;
	UINT32 PageFaultCount;
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
	SYSTEM_THREAD_INFORMATION Threads[1];
} SYSTEM_PROCESS_INFO, * PSYSTEM_PROCESS_INFO;


#define FIELD_OFFSET(type, field)    ((LONG)(LONG_PTR)&(((type *)0)->field))
//////////////////////////////////////////////////////////////////////////
// 定义 PEB 相关结构  PROCESS_BASIC_INFORMATION 相关结构



typedef struct _CURDIR
{
	UNICODE_STRING DosPath;
	HANDLE Handle;
} CURDIR, * PCURDIR;

typedef struct _RTL_USER_PROCESS_PARAMETERS_WINXP_X86 {
	UINT32 MaximumLength;
	UINT32 Length;
	UINT32 Flags;
	UINT32 DebugFlags;
	HANDLE ConsoleHandle;
	UINT32 ConsoleFlags;
	HANDLE StandardInput;
	HANDLE StandardOutput;
	HANDLE StandardError;
	CURDIR CurrentDirectory;        // ProcessParameters
	UNICODE_STRING DllPath;         // ProcessParameters
	UNICODE_STRING ImagePathName;   // ProcessParameters
	UNICODE_STRING CommandLine;     // ProcessParameters
	PVOID   Environment;
	UINT32	StartingX;
	UINT32	StartingY;
	UINT32	CountX;
	UINT32	CountY;
	UINT32	CountCharsX;
	UINT32	CountCharsY;
	UINT32	FillAttribute;
	UINT32	WindowFlags;
	UINT32	ShowWindowFlags;
	UNICODE_STRING	WindowTitle;
	UNICODE_STRING	DesktopInfo;
	UNICODE_STRING	ShellInfo;
	UNICODE_STRING	RuntimeData;
	UINT32	CurrentDirectores[8];
}RTL_USER_PROCESS_PARAMETERS_WINXP_X86, * PRTL_USER_PROCESS_PARAMETERS_WINXP_X86;


typedef struct _RTL_USER_PROCESS_PARAMETERS_WIN7_X64 {
	UINT32 MaximumLength;
	UINT32 Length;
	UINT32 Flags;
	UINT32 DebugFlags;
	HANDLE ConsoleHandle;
	UINT32  ConsoleFlags;
	HANDLE StandardInput;
	HANDLE StandardOutput;
	HANDLE StandardError;
	CURDIR CurrentDirectory;        // ProcessParameters
	UNICODE_STRING DllPath;         // ProcessParameters
	UNICODE_STRING ImagePathName;   // ProcessParameters
	UNICODE_STRING CommandLine;     // ProcessParameters
	PVOID   Environment;
	UINT32	StartingX;
	UINT32	StartingY;
	UINT32	CountX;
	UINT32	CountY;
	UINT32	CountCharsX;
	UINT32	CountCharsY;
	UINT32	FillAttribute;
	UINT32	WindowFlags;
	UINT32	ShowWindowFlags;
	UNICODE_STRING	WindowTitle;
	UNICODE_STRING	DesktopInfo;
	UNICODE_STRING	ShellInfo;
	UNICODE_STRING	RuntimeData;
	UINT32	CurrentDirectores[8];
	UINT64  EnvironmentSize;
	UINT64  EnvironmentVersion;
}RTL_USER_PROCESS_PARAMETERS_WIN7_X64, * PRTL_USER_PROCESS_PARAMETERS_WIN7_X64;


#ifdef _WIN64
#define PRTL_USER_PROCESS_PARAMETERS	PRTL_USER_PROCESS_PARAMETERS_WIN7_X64
#define RTL_USER_PROCESS_PARAMETERS		RTL_USER_PROCESS_PARAMETERS_WIN7_X64
#else   
#define PRTL_USER_PROCESS_PARAMETERS	PRTL_USER_PROCESS_PARAMETERS_WINXP_X86
#define RTL_USER_PROCESS_PARAMETERS		RTL_USER_PROCESS_PARAMETERS_WINXP_X86
#endif


#define GDI_HANDLE_BUFFER_SIZE32 34
#define GDI_HANDLE_BUFFER_SIZE64 60
#ifndef _WIN64
#define GDI_HANDLE_BUFFER_SIZE GDI_HANDLE_BUFFER_SIZE32
#else
#define GDI_HANDLE_BUFFER_SIZE GDI_HANDLE_BUFFER_SIZE64
#endif

typedef UINT32 GDI_HANDLE_BUFFER[GDI_HANDLE_BUFFER_SIZE];


////////////////////////////////////////

typedef struct _UNICODE_STRING32
{
	USHORT					Length;
	USHORT					MaximumLength;
	UINT32					Buffer;         // 定义为UINT32 然后到进程中读取
} UNICODE_STRING32, * PUNICODE_STRING32;

typedef struct _PEB_LDR_DATA32
{
	UINT32	Length;
	UINT32   Initialized;
	UINT32   SsHandle;
	LIST_ENTRY32 InLoadOrderModuleList;            // +0x0c 
	LIST_ENTRY32 InMemoryOrderModuleList;          // +0x14
	LIST_ENTRY32 InInitializationOrderModuleList;  // +0x1c 
	PVOID	EntryInProgress;                     // +0x24
}PEB_LDR_DATA32, * PPEB_LDR_DATA32;

typedef struct _PEB_LDR_DATA64
{
	UINT32	Length;								   // +0x000
	UINT32   Initialized;						   // +0x004
	UINT64   SsHandle;							   // +0x008
	LIST_ENTRY64 InLoadOrderLinks;            // +0x010
	LIST_ENTRY64 InMemoryOrderModuleList;          // +0x020
	LIST_ENTRY64 InInitializationOrderModuleList;  // +0x030
	UINT64	EntryInProgress;                       // +0x040
	UINT64	ShutdownInProgress;                    // +0x048
	UINT64	ShutdownThreadId;                      // +0x050
}PEB_LDR_DATA64, * PPEB_LDR_DATA64;


//_RTL_BALANCED_NODE
//+ 0x000 Children         : [2] Ptr64 _RTL_BALANCED_NODE
//+ 0x000 Left : Ptr64 _RTL_BALANCED_NODE
//+ 0x008 Right : Ptr64 _RTL_BALANCED_NODE
//+ 0x010 Red : Pos 0, 1 Bit
//+ 0x010 Balance : Pos 0, 2 Bits
//+ 0x010 ParentValue : Uint8B


typedef struct _RTL_BALANCED_NODE64
{
	union
	{
		UINT64 Children[2];
		struct
		{
			/* 0x0000 */ UINT64 Left;
			/* 0x0008 */ UINT64 Right;
		}; /* size: 0x0010 */
	}; /* size: 0x0010 */
	union
	{
		/* 0x0010 */ UCHAR Red : 1; /* bit position: 0 */
		/* 0x0010 */ UCHAR Balance : 2; /* bit position: 0 */
		/* 0x0010 */ UINT64 ParentValue;
	}; /* size: 0x0008 */
} RTL_BALANCED_NODE64, * PRTL_BALANCED_NODE64; /* size: 0x0018 */


typedef struct _LDR_DATA_TABLE_ENTRY32
{
	LIST_ENTRY32 InLoadOrderModuleList;
	LIST_ENTRY32 InMemoryOrderModuleList;
	LIST_ENTRY32 InInitializationOrderModuleList;
	UINT32 DllBase;
	UINT32 EntryPoint;
	UINT32 SizeOfImage;
	UNICODE_STRING32 FullDllName; 
	UNICODE_STRING32 BaseDllName; 
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	union
	{
		LIST_ENTRY32 HashLinks;
		ULONG SectionPointer;
	};
	ULONG CheckSum;
	union
	{
		ULONG TimeDateStamp;
		ULONG LoadedImports;
	};

	ULONG EntryPointActivationContext;
	ULONG PatchInformation;
} LDR_DATA_TABLE_ENTRY32, * PLDR_DATA_TABLE_ENTRY32;

enum _LDR_DLL_LOAD_REASON
{
	LoadReasonStaticDependency,
	LoadReasonStaticForwarderDependency,
	LoadReasonDynamicForwarderDependency ,
	LoadReasonDelayloadDependency ,
	LoadReasonDynamicLoad ,
	LoadReasonAsImageLoad,
	LoadReasonAsDataLoad,
	LoadReasonEnclavePrimary ,
	LoadReasonEnclaveDependency ,
	LoadReasonUnknown = -1,
};

typedef struct _LDR_DATA_TABLE_ENTRY64
{
	LIST_ENTRY64	InLoadOrderLinks;                               // 0x0000 
	LIST_ENTRY64	InMemoryOrderLinks;                             // 0x0010 
	union
	{
		LIST_ENTRY64	InInitializationOrderLinks;             // 0x0020 
		LIST_ENTRY64	InProgressLinks;                        // 0x0020 
	};                                                              // size: 0x0010 
	UINT64 DllBase;                              // 0x0030 
	UINT64 EntryPoint;                           // 0x0038 
	UINT32		SizeOfImage;                            // 0x0040 
	UNICODE_STRING	FullDllName;                            // 0x0048 
	UNICODE_STRING	BaseDllName;                            // 0x0058 
	union
	{
		UCHAR	FlagGroup[4];                           // 0x0068 
		UINT32	Flags;                                  // 0x0068 
		struct                                                  // bitfield 
		{
			UINT32	PackagedBinary : 1;             // 0x0068  bit position: 0 
			UINT32	MarkedForRemoval : 1;           // 0x0068  bit position: 1 
			UINT32	ImageDll : 1;                   // 0x0068  bit position: 2 
			UINT32	LoadNotificationsSent : 1;      // 0x0068  bit position: 3 
			UINT32	TelemetryEntryProcessed : 1;    // 0x0068  bit position: 4 
			UINT32	ProcessStaticImport : 1;        // 0x0068  bit position: 5 
			UINT32	InLegacyLists : 1;              // 0x0068  bit position: 6 
			UINT32	InIndexes : 1;                  // 0x0068  bit position: 7 
			UINT32	ShimDll : 1;                    // 0x0068  bit position: 8 
			UINT32	InExceptionTable : 1;           // 0x0068  bit position: 9 
			UINT32	ReservedFlags1 : 2;             // 0x0068  bit position: 10 
			UINT32	LoadInProgress : 1;             // 0x0068  bit position: 12 
			UINT32	ReservedFlags2 : 1;             // 0x0068  bit position: 13 
			UINT32	EntryProcessed : 1;             // 0x0068  bit position: 14 
			UINT32	ReservedFlags3 : 3;             // 0x0068  bit position: 15 
			UINT32	DontCallForThreads : 1;         // 0x0068  bit position: 18 
			UINT32	ProcessAttachCalled : 1;        // 0x0068  bit position: 19 
			UINT32	ProcessAttachFailed : 1;        // 0x0068  bit position: 20 
			UINT32	CorDeferredValidate : 1;        // 0x0068  bit position: 21 
			UINT32	CorImage : 1;                   // 0x0068  bit position: 22 
			UINT32	DontRelocate : 1;               // 0x0068  bit position: 23 
			UINT32	CorILOnly : 1;                  // 0x0068  bit position: 24 
			UINT32	ReservedFlags5 : 3;             // 0x0068  bit position: 25 
			UINT32	Redirected : 1;                 // 0x0068  bit position: 28 
			UINT32	ReservedFlags6 : 2;             // 0x0068  bit position: 29 
			UINT32	CompatDatabaseProcessed : 1;    // 0x0068  bit position: 31 
		};                                                      // bitfield 
	};                                                              // size: 0x0004 
	UINT16			        ObsoleteLoadCount;              // 0x006c 
	UINT16			        TlsIndex;                       // 0x006e 
	LIST_ENTRY64	        HashLinks;                      // 0x0070 
	UINT32			        TimeDateStamp;                  // 0x0080 
	UINT64                  EntryPointActivationContext;  // 0x0088 
	UINT64                  PatchInformation;             // 0x0090 
	UINT64                  DdagNode;                     // 0x0098 
	LIST_ENTRY64			NodeModuleLink;                 // 0x00a0 
	UINT64                  SnapContext;                  // 0x00b0 
	UINT64                  ParentDllBase;                // 0x00b8 
	UINT64                  SwitchBackContext;            // 0x00c0 
	RTL_BALANCED_NODE64	    BaseAddressIndexNode;           // 0x00c8 
	RTL_BALANCED_NODE64	    MappingInfoIndexNode;           // 0x00e0 
	UINT64		            OriginalBase;                   // 0x00f8 
	LARGE_INTEGER		    LoadTime;                       // 0x0100 
	UINT32			        BaseNameHashValue;              // 0x0108 
	enum LDR_DLL_LOAD_REASON  	LoadReason;                     // 0x010c 
	UINT32                  ImplicitPathOptions;	         // 0x110
	UINT32                  ReferenceCount;			         // 0x114
	UINT32                  DependentLoadFlags;		         // 0x118
	UCHAR                   SigningLevel;			         // 0x11c

} LDR_DATA_TABLE_ENTRY64, * PLDR_DATA_TABLE_ENTRY64;                         // size: 0x0110 


//////////////////////////////////////////////////////////////////////////
// Ldr 三根链表结构
typedef struct _LDR_DATA_TABLE_ENTRY {
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	UINT32 SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	UINT32 Flags;
	UINT16 LoadCount;
	UINT16 TlsIndex;
	union {
		LIST_ENTRY HashLinks;
		struct {
			PVOID SectionPointer;
			UINT32 CheckSum;
		};
	};
	union {
		struct {
			UINT32 TimeDateStamp;
		};
		struct {
			PVOID LoadedImports;
		};
	};
	struct _ACTIVATION_CONTEXT* EntryPointActivationContext;

	PVOID PatchInformation;

} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef const struct _LDR_DATA_TABLE_ENTRY* PCLDR_DATA_TABLE_ENTRY;

//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////




//typedef struct _GDI_TEB_BATCH32
//{
//	// 0x000 
//	union {
//		UINT32 HEADER; 
//		struct 
//		{
//			UINT32	Offset : 31;                    // Pos 0, 31 Bits 
//			UINT32	HasRenderingCommand : 1;        // Pos 31, 1 Bit 
//		};
//
//	};
//	UINT32	HDC;                                    // +0x004 
//	UINT32	Buffer[310];                            // +0x008 
//}GDI_TEB_BATCH32, * PGDI_TEB_BATCH32;

typedef struct _STRING32
{
	UINT16	Length;                                 // +0x000 
	UINT16	MaximumLength;                          // +0x002 
	UINT32	Buffer;                                 // +0x004 
}STRING32, * PSTRING32;


typedef struct _ACTIVATION_CONTEXT_STACK32
{
	UINT32	        ActiveFrame;                    // 0x000
	LIST_ENTRY32	ActiFrameListCacheveFrame;      // 0x004
	UINT32          Flags;                          // 0x00c
	UINT32          NextCookieSequenceNumber;       // 0x010
	UINT32          StackId;                        // 0x014
}ACTIVATION_CONTEXT_STACK32, * PACTIVATION_CONTEXT_STACK32;


typedef struct _ACTIVATION_CONTEXT_STACK64
{
	UINT64	        ActiveFrame;                    // 0x000
	LIST_ENTRY64	FrameListCache;                 // 0x008
	UINT32          Flags;                          // 0x018
	UINT32          NextCookieSequenceNumber;       // 0x01c
	UINT32          StackId;                        // 0x020
}ACTIVATION_CONTEXT_STACK64, * PACTIVATION_CONTEXT_STACK64;




// 大小 0x47C
typedef struct _PEB32
{
	BYTE	InheritedAddressSpace;                                  // +0x000 
	BYTE	ReadImageFileExecOptions;                               // +0x001 
	BYTE	BeingDebugged;                                          // +0x002 
	union
	{
		BYTE BitField;                                          // +0x003 
		struct
		{
			BYTE	ImageUsesLargePages : 1;                // Pos 0, 1 Bit 
			BYTE	IsProtectedProcess : 1;                 // Pos 1, 1 Bit 
			BYTE	IsLegacyProcess : 1;                    // Pos 2, 1 Bit 
			BYTE	IsImageDynamicallyRelocated : 1;        // Pos 3, 1 Bit 
			BYTE	SkipPatchingUser32Forwarders : 1;       // Pos 4, 1 Bit 
			BYTE	IsPackagedProcess : 1;                  // Pos 5, 1 Bit 
			BYTE	IsAppContainer : 1;                     // Pos 6, 1 Bit 
			BYTE	SpareBits : 1;                          //  Pos 7, 1 Bit 
		};
	};                                                              // +0x004 
	UINT32	Mutant;                                                 // +0x008 
	UINT32	ImageBaseAddress;                                       // +0x00c 
	UINT32	Ldr;                                                    // +0x00c 
	UINT32	ProcessParameters;                                      // +0x010 
	UINT32	SubSystemData;                                          // +0x014 
	UINT32	ProcessHeap;                                            // +0x018 
	UINT32	FastPebLock;                                            // +0x01c 
	UINT32	AtlThunkSListPtr;                                       // +0x020 
	UINT32	IFEOKey;                                                // +0x024 
	union                                                           //  
	{                                                               //  
		UINT32 CrossProcessFlags;                               // +0x028 
		struct                                                  //  
		{                                                       //  
			UINT32	ProcessInJob : 1;                       // Pos 0, 1 Bit 
			UINT32	ProcessInitializing : 1;                // Pos 1, 1 Bit 
			UINT32	ProcessUsingVEH : 1;                    //  Pos 2, 1 Bit 
			UINT32	ProcessUsingVCH : 1;                    // Pos 3, 1 Bit 
			UINT32	ProcessUsingFTH : 1;                    // Pos 4, 1 Bit 
			UINT32	ProcessPreviouslyThrottled : 1;         // Pos 5, 1 Bit 
			UINT32	ProcessCurrentlyThrottled : 1;          // Pos 6, 1 Bit 
			UINT32	ProcessImagesHotPatched : 1;            // Pos 7, 1 Bit 
			UINT32	ReservedBits0 : 24;                     // Pos 8, 24 Bits 
		};                                                      //  
	};
	union
	{
		UINT32	KernelCallbackTable;                            // +0x02c 
		UINT32	UserSharedInfoPtr;                              // +0x02c 
	};


	UINT32		SystemReserved;                                 // +0x030  : Uint4B 
	UINT32		AtlThunkSListPtr32;                             // +0x034  : Uint4B 
	UINT32		ApiSetMap;                                      // +0x038  : Uint4B 
	UINT32		TlsExpansionCounter;                            // +0x03c  : Uint4B 
	UINT32		TlsBitmap;                                      // +0x040  : Uint4B 
	UINT32		TlsBitmapBits[2];                               // +0x044  : [2] Uint4B 
	UINT32		ReadOnlySharedMemoryBase;                       // +0x04c  : Uint4B 
	UINT32		SharedData;                                     // +0x050  : Uint4B 
	UINT32		ReadOnlyStaticServerData;                       // +0x054  : Uint4B 
	UINT32		AnsiCodePageData;                               // +0x058  : Uint4B 
	UINT32		OemCodePageData;                                // +0x05c  : Uint4B 
	UINT32		UnicodeCaseTableData;                           // +0x060  : Uint4B 
	UINT32		NumberOfProcessors;                             // +0x064  : Uint4B 
	UINT32		NtGlobalFlag;                                   // +0x068  : Uint4B 
	LARGE_INTEGER	CriticalSectionTimeout;                         // +0x070  : _LARGE_INTEGER 
	UINT32		HeapSegmentReserve;                             // +0x078  : Uint4B 
	UINT32		HeapSegmentCommit;                              // +0x07c  : Uint4B 
	UINT32		HeapDeCommitTotalFreeThreshold;                 // +0x080  : Uint4B 
	UINT32		HeapDeCommitFreeBlockThreshold;                 // +0x084  : Uint4B 
	UINT32		NumberOfHeaps;                                  // +0x088  : Uint4B 
	UINT32		MaximumNumberOfHeaps;                           // +0x08c  : Uint4B 
	UINT32		ProcessHeaps;                                   // +0x090  : Uint4B 
	UINT32		GdiSharedHandleTable;                           // +0x094  : Uint4B 
	UINT32		ProcessStarterHelper;                           // +0x098  : Uint4B 
	UINT32		GdiDCAttributeList;                             // +0x09c  : Uint4B 
	UINT32		LoaderLock;                                     // +0x0a0  : Uint4B 
	UINT32		OSMajorVersion;                                 // +0x0a4  : Uint4B 
	UINT32		OSMinorVersion;                                 // +0x0a8  : Uint4B 
	UINT16		OSBuildNumber;                                  // +0x0ac  : Uint2B 
	UINT16		OSCSDVersion;                                   // +0x0ae  : Uint2B 
	UINT32		OSPlatformId;                                   // +0x0b0  : Uint4B 
	UINT32		ImageSubsystem;                                 // +0x0b4  : Uint4B 
	UINT32		ImageSubsystemMajorVersion;                     // +0x0b8  : Uint4B 
	UINT32		ImageSubsystemMinorVersion;                     // +0x0bc  : Uint4B 
	UINT32		ActiveProcessAffinityMask;                      // +0x0c0  : Uint4B 
	UINT32		GdiHandleBuffer[34];                            // +0x0c4  : [34] Uint4B 
	UINT32		PostProcessInitRoutine;                         // +0x14c  : Uint4B 
	UINT32		TlsExpansionBitmap;                             // +0x150  : Uint4B 
	UINT32		TlsExpansionBitmapBits[32];                     // +0x154  : [32] Uint4B 
	UINT32		SessionId;                                      // +0x1d4  : Uint4B 
	ULARGE_INTEGER	AppCompatFlags;                                 // +0x1d8  : _ULARGE_INTEGER 
	ULARGE_INTEGER	AppCompatFlagsUser;                             // +0x1e0  : _ULARGE_INTEGER 
	UINT32		pShimData;                                      // +0x1e8  : Uint4B 
	UINT32		AppCompatInfo;                                  // +0x1ec  : Uint4B 
	STRING32	CSDVersion;                                     // +0x1f0  : _STRING32 
	UINT32		ActivationContextData;                          // +0x1f8  : Uint4B 
	UINT32		ProcessAssemblyStorageMap;                      // +0x1fc  : Uint4B 
	UINT32		SystemDefaultActivationContextData;             // +0x200  : Uint4B 
	UINT32		SystemAssemblyStorageMap;                       // +0x204  : Uint4B 
	UINT32		MinimumStackCommit;                             // +0x208  : Uint4B 
	UINT32		SparePointers[4];                               // +0x20c  : [4] Uint4B 
	UINT32		SpareUlongs[5];                                 // +0x21c  : [5] Uint4B 
	UINT32		WerRegistrationData;                            // +0x230  : Uint4B 
	UINT32		WerShipAssertPtr;                               // +0x234  : Uint4B 
	UINT32		pUnused;                                        // +0x238  : Uint4B 
	UINT32		pImageHeaderHash;                               // +0x23c  : Uint4B 

	union
	{
		UINT32 TracingFlags;                                    // +0x240  : Uint4B 
		struct
		{
			UINT32	HeapTracingEnabled : 1;                 // +0x240  : Pos 0, 1 Bit 
			UINT32	CritSecTracingEnabled : 1;              // +0x240  : Pos 1, 1 Bit 
			UINT32	LibLoaderTracingEnabled : 1;            // +0x240  : Pos 2, 1 Bit 
			UINT32	SpareTracingBits : 29;                  // +0x240  : Pos 3, 29 Bits 
		};
	};

	UINT64		CsrServerReadOnlySharedMemoryBase;              // +0x248  : Uint8B 
	UINT32		TppWorkerpListLock;                             // +0x250  : Uint4B 
	LIST_ENTRY32	TppWorkerpList;                                 // +0x254  : 
	UINT32		WaitOnAddressHashTable[128];                    // +0x25c  : [128] Uint4B 
	UINT32		TelemetryCoverageHeader;                        // +0x45c  : Uint4B 
	UINT32		CloudFileFlags;                                 // +0x460  : Uint4B 
	UINT32		CloudFileDiagFlags;                             // +0x464  : Uint4B 
	BYTE		PlaceholderCompatibilityMode;                   // +0x468  : Char 
	BYTE		PlaceholderCompatibilityModeReserved[7];        // +0x469  : [7] Char 
	UINT32		LeapSecondData;                                 // +0x470  : Uint4B 

	union
	{
		UINT32 LeapSecondFlags;                                 // +0x474  : Uint4B 
		struct
		{
			UINT32	SixtySecondEnabled : 1;                 // Pos 0, 1 Bit 
			UINT32	Reserved : 31;                          //  Pos 1, 31 Bits 
		};
	};
	UINT32 NtGlobalFlag2;                                           // +0x478  : Uint4B 
}PEB32, * PPEB32;


// size: 0x1000字节大小
typedef struct _TEB32
{
	NT_TIB32			NtTib;                                  // +0x000
	UINT32				EnvironmentPointer;                     // +0x01c
	CLIENT_ID32			ClientId;                               // +0x020
	UINT32				ActiveRpcHandle;                        // +0x028
	UINT32				ThreadLocalStoragePointer;              // +0x02c
	UINT32				ProcessEnvironmentBlock;                // +0x030
	UINT32				LastErrorValue;                         // +0x034
	UINT32				CountOfOwnedCriticalSections;           // +0x038
	UINT32				CsrClientThread;                        // +0x03c
	UINT32				Win32ThreadInfo;                        // +0x040
	UINT32				User32Reserved[26];                     // +0x044
	UINT32				UserReserved[5];                        // +0x0ac
	UINT32				WOW32Reserved;                          // +0x0c0
	UINT32				CurrentLocale;                          // +0x0c4
	UINT32				FpSoftwareStatusRegister;               // +0x0c8
	UINT32				ReservedForDebuggerInstrumentation[16]; // +0x0cc
	UINT32				SystemReserved1[26];                    // +0x10c
	CHAR				PlaceholderCompatibilityMode;           // +0x174
	UCHAR				PlaceholderHydrationAlwaysExplicit;     // +0x175
	CHAR				PlaceholderReserved[10];                //  +0x176
	UINT32				ProxiedProcessId;                       // +0x180
	ACTIVATION_CONTEXT_STACK32	_ActivationStack;                       // +0x184
	UCHAR				WorkingOnBehalfTicket[8];               // +0x19c
	INT32				ExceptionCode;                          // +0x1a4
	UINT32				ActivationContextStackPointer;          // +0x1a8
	UINT32				InstrumentationCallbackSp;              // +0x1ac
	UINT32				InstrumentationCallbackPreviousPc;      // +0x1b0
	UINT32				InstrumentationCallbackPreviousSp;      // +0x1b4
	UCHAR				InstrumentationCallbackDisabled;        // +0x1b8
	UCHAR				SpareBytes[23];                         // +0x1b9
	UINT32				TxFsContext;                            // +0x1d0
	GDI_TEB_BATCH32		GdiTebBatch;                            // +0x1d4
	CLIENT_ID32			RealClientId;                           // +0x6b4
	UINT32				GdiCachedProcessHandle;                 // +0x6bc
	UINT32				GdiClientPID;                           // +0x6c0
	UINT32				GdiClientTID;                           // +0x6c4
	UINT32				GdiThreadLocalInfo;                     // +0x6c8
	UINT32				Win32ClientInfo[62];                    // +0x6cc
	UINT32				glDispatchTable[233];                   // +0x7c4
	UINT32				glReserved1[29];                        // +0xb68
	UINT32				glReserved2;                            // +0xbdc
	UINT32				glSectionInfo;                          // +0xbe0
	UINT32				glSection;                              // +0xbe4
	UINT32				glTable;                                // +0xbe8
	UINT32				glCurrentRC;                            // +0xbec
	UINT32				glContext;                              // +0xbf0
	UINT32				LastStatusValue;                        // +0xbf4
	STRING32			StaticUnicodeString;                    // +0xbf8
	WCHAR				StaticUnicodeBuffer[261];               // +0xc00
	UINT32				DeallocationStack;                      // +0xe0c
	UINT32				TlsSlots[64];                           // +0xe10
	LIST_ENTRY32		TlsLinks;                               // +0xf10
	UINT32				Vdm;                                    // +0xf18
	UINT32				ReservedForNtRpc;                       // +0xf1c
	UINT32				DbgSsReserved[2];                       // +0xf20
	UINT32				HardErrorMode;                          // +0xf28
	UINT32				Instrumentation[9];                     // +0xf2c
	GUID				ActivityId;                             // +0xf50
	UINT32				SubProcessTag;                          // +0xf60
	UINT32				PerflibData;                            // +0xf64
	UINT32				EtwTraceData;                           // +0xf68
	UINT32				WinSockData;                            // +0xf6c
	UINT32				GdiBatchCount;                          // +0xf70
	union 
	{
		PROCESSOR_NUMBER	CurrentIdealProcessor;                  // +0xf74
		UINT32				IdealProcessorValue;                    // +0xf74
		struct 
		{
			UCHAR				ReservedPad0;                           // +0xf74
			UCHAR				ReservedPad1;                           // +0xf75
			UCHAR				ReservedPad2;                           // +0xf76
			UCHAR				IdealProcessor;                         // +0xf77
		};
	};
	
	UINT32				GuaranteedStackBytes;                   // +0xf78
	UINT32				ReservedForPerf;                        // +0xf7c
	UINT32				ReservedForOle;                         // +0xf80
	UINT32				WaitingOnLoaderLock;                    // +0xf84
	UINT32				SavedPriorityState;                     // +0xf88
	UINT32				ReservedForCodeCoverage;                // +0xf8c
	UINT32				ThreadPoolData;                         // +0xf90
	UINT32				TlsExpansionSlots;                      // +0xf94
	UINT32				MuiGeneration;                          // +0xf98
	UINT32				IsImpersonating;                        // +0xf9c
	UINT32				NlsCache;                               // +0xfa0
	UINT32				pShimData;                              // +0xfa4
	UINT32				HeapData;                               // +0xfa8
	UINT32				CurrentTransactionHandle;               // +0xfac
	UINT32				ActiveFrame;                            // +0xfb0
	UINT32				FlsData;                                // +0xfb4
	UINT32				PreferredLanguages;                     // +0xfb8
	UINT32				UserPrefLanguages;                      // +0xfbc
	UINT32				MergedPrefLanguages;                    // +0xfc0
	UINT32				MuiImpersonation;                       // +0xfc4

	union
	{
		UINT16 CrossTebFlags;                                           // +0xfc8
		UINT16 SpareCrossTebBits;                                       // +0xfc8
	};


	union
	{
		UINT16 SameTebFlags;                                            // +0xfca
		struct
		{
			UINT16	SafeThunkCall : 1;                              // +0xfca
			UINT16	InDebugPrint : 1;                               // +0xfca
			UINT16	HasFiberData : 1;                               // +0xfca
			UINT16	SkipThreadAttach : 1;                           // +0xfca
			UINT16	WerInShipAssertCode : 1;                        // +0xfca
			UINT16	RanProcessInit : 1;                             // +0xfca
			UINT16	ClonedThread : 1;                               // +0xfca
			UINT16	SuppressDebugMsg : 1;                           // +0xfca
			UINT16	DisableUserStackWalk : 1;                       // +0xfca
			UINT16	RtlExceptionAttached : 1;                       // +0xfca
			UINT16	InitialThread : 1;                              // +0xfca
			UINT16	SessionAware : 1;                               // +0xfca
			UINT16	LoadOwner : 1;                                  // +0xfca
			UINT16	LoaderWorker : 1;                               // +0xfca
			UINT16	SkipLoaderInit : 1;                             // +0xfca
			UINT16	SpareSameTebBits : 1;                           // +0xfca
		};
	};


	UINT32	TxnScopeEnterCallback;                                          // +0xfcc
	UINT32	TxnScopeExitCallback;                                           // +0xfd0
	UINT32	TxnScopeContext;                                                // +0xfd4
	UINT32	LockCount;                                                      // +0xfd8
	INT32	WowTebOffset;                                                   // +0xfdc
	UINT32	ResourceRetValue;                                               // +0xfe0
	UINT32	ReservedForWdf;                                                 // +0xfe4
	UINT64	ReservedForCrt;                                                 // +0xfe8
	GUID	EffectiveContainerId;                                           // +0xff0
} TEB32, * PTEB32;



typedef struct _TEB64
{
	struct _NT_TIB64	NtTib;                                  // 0x0000 
	UINT64			EnvironmentPointer;                     // 0x0038 
	CLIENT_ID64	    ClientId;                               // 0x0040 
	UINT64			ActiveRpcHandle;                        // 0x0050 
	UINT64			ThreadLocalStoragePointer;              // 0x0058 
	UINT64			ProcessEnvironmentBlock;                // 0x0060 
	UINT32			LastErrorValue;                         // 0x0068 
	UINT32			CountOfOwnedCriticalSections;           // 0x006c 
	UINT64			CsrClientThread;                        // 0x0070 
	UINT64			Win32ThreadInfo;                        // 0x0078 
	UINT32			User32Reserved[26];                     // 0x0080 
	UINT32			UserReserved[5];                        // 0x00e8 
	UINT64			WOW32Reserved;                          // 0x0100 
	UINT32			CurrentLocale;                          // 0x0108 
	UINT32			FpSoftwareStatusRegister;               // 0x010c 
	UINT64			SystemReserved1[54];                    // 0x0110 
	INT32			ExceptionCode;                          // 0x02c0 
	UINT64			ActivationContextStackPointer;          // 0x02c8 
	UCHAR		    SpareBytes[24];                         // 0x02d0 
	UINT32			TxFsContext;                            // 0x02e8 
	GDI_TEB_BATCH64 GdiTebBatch;                            // 0x02f0 
	CLIENT_ID64	    RealClientId;                           // 0x07d8 
	UINT64			GdiCachedProcessHandle;                 // 0x07e8 
	UINT32			GdiClientPID;                           // 0x07f0 
	UINT32			GdiClientTID;                           // 0x07f4 
	UINT64			GdiThreadLocalInfo;                     // 0x07f8 
	UINT64			Win32ClientInfo[62];                    // 0x0800 
	UINT64			glDispatchTable[233];                   // 0x09f0 
	UINT64			glReserved1[29];                        // 0x1138 
	UINT64			glReserved2;                            // 0x1220 
	UINT64			glSectionInfo;                          // 0x1228 
	UINT64			glSection;                              // 0x1230 
	UINT64			glTable;                                // 0x1238 
	UINT64			glCurrentRC;                            // 0x1240 
	UINT64			glContext;                              // 0x1248 
	UINT32			LastStatusValue;                        // 0x1250 
	UNICODE_STRING	StaticUnicodeString;                    // 0x1258 
	wchar_t			StaticUnicodeBuffer[261];               // 0x1268 
	UINT64			DeallocationStack;                      // 0x1478 
	UINT64			TlsSlots[64];                           // 0x1480 
	LIST_ENTRY64	TlsLinks;                               // 0x1680 
	UINT64			Vdm;                                    // 0x1690 
	UINT64			ReservedForNtRpc;                       // 0x1698 
	UINT64			DbgSsReserved[2];                       // 0x16a0 
	UINT32			HardErrorMode;                          // 0x16b0 
	UINT64			Instrumentation[11];                    // 0x16b8 
	GUID		ActivityId;                             // 0x1710 
	UINT64			SubProcessTag;                          // 0x1720 
	UINT64			PerflibData;                            // 0x1728 
	UINT64			EtwTraceData;                           // 0x1730 
	UINT64			WinSockData;                            // 0x1738 
	UINT32			GdiBatchCount;                          // 0x1740 
	union
	{
		struct _PROCESSOR_NUMBER	CurrentIdealProcessor;  // 0x1744 
		UINT32				IdealProcessorValue;    // 0x1744 
		struct
		{
			UCHAR	ReservedPad0;                   // 0x1744 
			UCHAR	ReservedPad1;                   // 0x1745 
			UCHAR	ReservedPad2;                   // 0x1746 
			UCHAR	IdealProcessor;                 // 0x1747 
		};                                                      // size: 0x0004 
	};                                                              // size: 0x0004 
	UINT32	GuaranteedStackBytes;                                   // 0x1748 
	UINT64	ReservedForPerf;                                        // 0x1750 
	UINT64	ReservedForOle;                                         // 0x1758 
	UINT32	WaitingOnLoaderLock;                                    // 0x1760 
	UINT64	SavedPriorityState;                                     // 0x1768 
	UINT64	ReservedForCodeCoverage;                                // 0x1770 
	UINT64	ThreadPoolData;                                         // 0x1778 
	UINT64	TlsExpansionSlots;                                      // 0x1780 
	UINT64	DeallocationBStore;                                     // 0x1788 
	UINT64	BStoreLimit;                                            // 0x1790 
	UINT32	MuiGeneration;                                          // 0x1798 
	UINT32	IsImpersonating;                                        // 0x179c 
	UINT64	NlsCache;                                               // 0x17a0 
	UINT64	pShimData;                                              // 0x17a8 
	UINT16	HeapVirtualAffinity;                                    // 0x17b0 
	UINT16	LowFragHeapDataSlot;                                    // 0x17b2 
	UINT64	CurrentTransactionHandle;                               // 0x17b8 
	UINT64	ActiveFrame;                                            // 0x17c0 
	UINT64	FlsData;                                                // 0x17c8 
	UINT64	PreferredLanguages;                                     // 0x17d0 
	UINT64	UserPrefLanguages;                                      // 0x17d8 
	UINT64	MergedPrefLanguages;                                    // 0x17e0 
	UINT32	MuiImpersonation;                                       // 0x17e8 
	union
	{
		UINT16	CrossTebFlags;                                  // 0x17ec 
		UINT16	SpareCrossTebBits : 16;                         // bit position: 0  // 0x17ec 
	};                                                              // size: 0x0002 
	union
	{
		UINT16 SameTebFlags;                                    // 0x17ee 
		struct                                                  // bitfield 
		{
			UINT16	SafeThunkCall : 1;                      // bit position: 0  // 0x17ee 
			UINT16	InDebugPrint : 1;                       // bit position: 1  // 0x17ee 
			UINT16	HasFiberData : 1;                       // bit position: 2  // 0x17ee 
			UINT16	SkipThreadAttach : 1;                   // bit position: 3  // 0x17ee 
			UINT16	WerInShipAssertCode : 1;                // bit position: 4  // 0x17ee 
			UINT16	RanProcessInit : 1;                     // bit position: 5  // 0x17ee 
			UINT16	ClonedThread : 1;                       // bit position: 6  // 0x17ee 
			UINT16	SuppressDebugMsg : 1;                   // bit position: 7  // 0x17ee 
			UINT16	DisableUserStackWalk : 1;               // bit position: 8  // 0x17ee 
			UINT16	RtlExceptionAttached : 1;               // bit position: 9  // 0x17ee 
			UINT16	InitialThread : 1;                      // bit position: 10  // 0x17ee 
			UINT16	SessionAware : 1;                       // bit position: 11  // 0x17ee 
			UINT16	SpareSameTebBits : 4;                   // bit position: 12  // 0x17ee 
		};                                                      // bitfield 
	};                                                              // size: 0x0002 
	UINT64	TxnScopeEnterCallback;                                  // 0x17f0 
	UINT64	TxnScopeExitCallback;                                   // 0x17f8 
	UINT64	TxnScopeContext;                                        // 0x1800 
	UINT32	LockCount;                                              // 0x1808 
	UINT32	SpareUlong0;                                            // 0x180c 
	UINT64	ResourceRetValue;                                       // 0x1810 
	UINT64	ReservedForWdf;                                         // 0x1818 
} TEB64, * PTEB64;                                                       // size: 0x1820 

typedef struct _PEB64
{
	UCHAR	InheritedAddressSpace;                                                  //  0x0000
	UCHAR	ReadImageFileExecOptions;                                               //  0x0001
	UCHAR	BeingDebugged;                                                          //  0x0002
	union
	{
		UCHAR BitField;                                                         //  0x0003
		struct                                                                  //  bitfield
		{
			UCHAR	ImageUsesLargePages : 1;             // 0x0003 // bit position: 0 
			UCHAR	IsProtectedProcess : 1;              // 0x0003 // bit position: 1 
			UCHAR	IsLegacyProcess : 1;                 // 0x0003 // bit position: 2 
			UCHAR	IsImageDynamicallyRelocated : 1;     // 0x0003 // bit position: 3 
			UCHAR	SkipPatchingUser32Forwarders : 1;    // 0x0003 // bit position: 4 
			UCHAR	IsPackagedProcess : 1;               // 0x0003 // bit position: 5 
			UCHAR	IsAppContainer : 1;                  // 0x0003 // bit position: 6 
			UCHAR	SpareBits : 1;                       // 0x0003 // bit position: 7 
		};                                                                          //  bitfield
	};                                                                              //  size: 0x0001
	UINT64	Mutant;                                                                 //  0x0008
	UINT64	ImageBaseAddress;                                                       //  0x0010
	UINT64	Ldr;                                                                    //  0x0018
	UINT64	ProcessParameters;                                                      //  0x0020
	UINT64	SubSystemData;                                                          //  0x0028
	UINT64	ProcessHeap;                                                            //  0x0030
	UINT64	FastPebLock;                                                            //  0x0038
	UINT64	AtlThunkSListPtr;                                                       //  0x0040
	UINT64	IFEOKey;                                                                //  0x0048
	union
	{
		UINT32 CrossProcessFlags;                                               //  0x0050
		struct                                                                  //  bitfield
		{
			UINT32	ProcessInJob : 1;                      //  0x0050  //  bit position: 0
			UINT32	ProcessInitializing : 1;               //  0x0050  //  bit position: 1
			UINT32	ProcessUsingVEH : 1;                   //  0x0050  //  bit position: 2
			UINT32	ProcessUsingVCH : 1;                   //  0x0050  //  bit position: 3
			UINT32	ProcessUsingFTH : 1;                   //  0x0050  //  bit position: 4
			UINT32	ReservedBits0 : 27;                    //  0x0050  //  bit position: 5
		};                                                                      //  bitfield
	};                                                                              //  size: 0x0004
	union
	{
		UINT64	KernelCallbackTable;                                            //  0x0058
		UINT64	UserSharedInfoPtr;                                              //  0x0058
	};                                                                              //  size: 0x0008
	UINT32			SystemReserved[1];                                      //  0x0060
	UINT32			AtlThunkSListPtr32;                                     //  0x0064
	UINT64			ApiSetMap;                                              //  0x0068
	UINT32			TlsExpansionCounter;                                    //  0x0070
	UINT64			TlsBitmap;                                              //  0x0078
	UINT32			TlsBitmapBits[2];                                       //  0x0080
	UINT64			ReadOnlySharedMemoryBase;                               //  0x0088
	UINT64			HotpatchInformation;                                    //  0x0090
	UINT64			ReadOnlyStaticServerData;                               //  0x0098
	UINT64			AnsiCodePageData;                                       //  0x00a0
	UINT64			OemCodePageData;                                        //  0x00a8
	UINT64			UnicodeCaseTableData;                                   //  0x00b0
	UINT32			NumberOfProcessors;                                     //  0x00b8
	UINT32			NtGlobalFlag;                                           //  0x00bc
	LARGE_INTEGER	CriticalSectionTimeout;                                 //  0x00c0
	UINT64			HeapSegmentReserve;                                     //  0x00c8
	UINT64			HeapSegmentCommit;                                      //  0x00d0
	UINT64			HeapDeCommitTotalFreeThreshold;                         //  0x00d8
	UINT64			HeapDeCommitFreeBlockThreshold;                         //  0x00e0
	UINT32			NumberOfHeaps;                                          //  0x00e8
	UINT32			MaximumNumberOfHeaps;                                   //  0x00ec
	UINT64			ProcessHeaps;                                           //  0x00f0
	UINT64			GdiSharedHandleTable;                                   //  0x00f8
	UINT64			ProcessStarterHelper;                                   //  0x0100
	UINT32			GdiDCAttributeList;                                     //  0x0108
	UINT64			LoaderLock;                                             //  0x0110
	UINT32			OSMajorVersion;                                         //  0x0118
	UINT32			OSMinorVersion;                                         //  0x011c
	UINT16		    OSBuildNumber;                                          //  0x0120
	UINT16   		OSCSDVersion;                                           //  0x0122
	UINT32			OSPlatformId;                                           //  0x0124
	UINT32			ImageSubsystem;                                         //  0x0128
	UINT32			ImageSubsystemMajorVersion;                             //  0x012c
	UINT32			ImageSubsystemMinorVersion;                             //  0x0130
	UINT64			ActiveProcessAffinityMask;                              //  0x0138
	UINT32			GdiHandleBuffer[60];                                    //  0x0140
	UINT64			PostProcessInitRoutine;                                 //  0x0230
	UINT64			TlsExpansionBitmap;                                     //  0x0238
	UINT32			TlsExpansionBitmapBits[32];                             //  0x0240
	UINT32			SessionId;                                              //  0x02c0
	ULARGE_INTEGER	AppCompatFlags;                                         //  0x02c8
	ULARGE_INTEGER	AppCompatFlagsUser;                                     //  0x02d0
	UINT64			pShimData;                                              //  0x02d8
	UINT64			AppCompatInfo;                                          //  0x02e0
	UNICODE_STRING	CSDVersion;                                             //  0x02e8
	UINT64			ActivationContextData;                                  //  0x02f8
	UINT64			ProcessAssemblyStorageMap;                              //  0x0300
	UINT64			SystemDefaultActivationContextData;                     //  0x0308
	UINT64			SystemAssemblyStorageMap;                               //  0x0310
	UINT64			MinimumStackCommit;                                     //  0x0318
	UINT64			FlsCallback;                                            //  0x0320
	LIST_ENTRY64	FlsListHead;                                            //  0x0328
	UINT64			FlsBitmap;                                              //  0x0338
	UINT32			FlsBitmapBits[4];                                       //  0x0340
	UINT32			FlsHighIndex;                                           //  0x0350
	UINT64			WerRegistrationData;                                    //  0x0358
	UINT64			WerShipAssertPtr;                                       //  0x0360
	UINT64			pUnused;                                                //  0x0368
	UINT64			pImageHeaderHash;                                       //  0x0370
	union
	{
		UINT32 TracingFlags;                                                    //  0x0378
		struct                                                                  //  bitfield
		{
			UINT32	HeapTracingEnabled : 1;               //  0x0378   //  bit position: 0
			UINT32	CritSecTracingEnabled : 1;            //  0x0378   //  bit position: 1
			UINT32	LibLoaderTracingEnabled : 1;          //  0x0378   //  bit position: 2
			UINT32	SpareTracingBits : 29;                //  0x0378   //  bit position: 3
		};                                                                      //  bitfield
	};                                                                              //  size: 0x0004
	UINT64 CsrServerReadOnlySharedMemoryBase;                                       //  0x0380
} PEB64, * PPEB64;                                                                       //  size: 0x0388



typedef struct _PROCESS_BASIC_INFORMATION
{
	ULONG64 ExitStatus;
	ULONG64 PebBaseAddress;
	ULONG64 AffinityMask;
	ULONG64 BasePriority;
	ULONG64 UniqueProcessId;
	ULONG64 InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION, * PPROCESS_BASIC_INFORMATION;