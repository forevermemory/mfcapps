#pragma once
#include <ntddk.h>



UINT64 KReadProcessMemory(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN OUT PVOID Buffer);
UINT64 KWriteProcessMemory(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer);



UINT64 KReadProcessMemory_MDL(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN OUT PVOID Buffer);
UINT64 KWriteProcessMemory_MDL(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer);

UINT64 KWriteProcessMemory_MDL_ֻ��(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer);
UINT64 KWriteProcessMemory_CR0_ֻ��(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer);

// ͨ�������ַ��д�ڴ�
UINT64 KReadProcessMemory_Physical(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer);
UINT64 KWriteProcessMemory_Physical(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer);