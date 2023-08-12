#pragma once

#include <ntddk.h>

NTSTATUS ObRegisterCallbacks(
	_In_ POB_CALLBACK_REGISTRATION CallbackRegistration,
	_Outptr_ PVOID* RegistrationHandle
);



#define PROCESS_TERMINATE                  (0x0001)  
#define PROCESS_CREATE_THREAD              (0x0002)  
#define PROCESS_SET_SESSIONID              (0x0004)  
#define PROCESS_VM_OPERATION               (0x0008)  
#define PROCESS_VM_READ                    (0x0010)  
#define PROCESS_VM_WRITE                   (0x0020)  
#define PROCESS_DUP_HANDLE                 (0x0040)  
#define PROCESS_CREATE_PROCESS             (0x0080)  
#define PROCESS_SET_QUOTA                  (0x0100)  
#define PROCESS_SET_INFORMATION            (0x0200)  
#define PROCESS_QUERY_INFORMATION          (0x0400)  
#define PROCESS_SUSPEND_RESUME             (0x0800)  
#define PROCESS_QUERY_LIMITED_INFORMATION  (0x1000)  
#define PROCESS_SET_LIMITED_INFORMATION    (0x2000)  

void InstallProcessProtect();
void UnInstallProcessProtect();


typedef struct _KLDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY listEntry;
    ULONG64 __Undefined1;
    ULONG64 __Undefined2;
    ULONG64 __Undefined3;
    ULONG64 NonPagedDebugInfo;
    ULONG64 DllBase;
    ULONG64 EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING path;
    UNICODE_STRING name;
    ULONG   Flags;
    USHORT  LoadCount;
    USHORT  __Undefined5;
    ULONG64 __Undefined6;
    ULONG   CheckSum;
    ULONG   __padding1;
    ULONG   TimeDateStamp;
    ULONG   __padding2;
} KLDR_DATA_TABLE_ENTRY, * PKLDR_DATA_TABLE_ENTRY;


BOOLEAN BypassCheckSign(PDRIVER_OBJECT pDriverObject);


