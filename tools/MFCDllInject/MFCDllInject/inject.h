#include "pch.h"
#include "define.h"
#include <bcrypt.h>

typedef
NTSTATUS(NTAPI* pfnRtlAdjustPrivilege)
(
	UINT32 Privilege,
	BOOLEAN Enable,
	BOOLEAN Client,
	PBOOLEAN WasEnabled
	);

typedef
NTSTATUS(NTAPI* pfnZwQuerySystemInformation)
(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	OUT PVOID SystemInformation,
	IN UINT32 SystemInformationLength,
	OUT PUINT32 ReturnLength OPTIONAL
	);

typedef
NTSTATUS(NTAPI* pfnNtQueryInformationProcess)
(
	__in HANDLE ProcessHandle,
	__in PROCESSINFOCLASS ProcessInformationClass,
	__out_bcount(ProcessInformationLength) PVOID ProcessInformation,
	__in UINT32 ProcessInformationLength,
	__out_opt PUINT32 ReturnLength
	);


BOOL GetProcessDllFunctionsByNameX64(IN DWORD processID, IN CString dllname, IN CString findFuncName, OUT ULONG64* funcAddr);
BOOL GetProcessDllFunctionsByNameX32(IN DWORD processID, IN CString dllname, IN CString findFuncName, OUT ULONG64* funcAddr);
