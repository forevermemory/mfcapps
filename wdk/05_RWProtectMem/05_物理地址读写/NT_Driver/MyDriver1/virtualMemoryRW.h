#pragma once
#include <ntddk.h>



UINT64 KReadProcessMemory(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN OUT PVOID Buffer);
UINT64 KWriteProcessMemory(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer);



UINT64 KReadProcessMemory_MDL(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN OUT PVOID Buffer);
UINT64 KWriteProcessMemory_MDL(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer);

UINT64 KWriteProcessMemory_MDL_只读(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer);
UINT64 KWriteProcessMemory_CR0_只读(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer);

// 通过物理地址读写内存
UINT64 KReadProcessMemory_Physical(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer);
UINT64 KWriteProcessMemory_Physical(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer);