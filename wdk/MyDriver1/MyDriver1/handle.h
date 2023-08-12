#pragma once

#include <ntddk.h>


//typedef struct _R3R0_Param {
//    UINT64 handle;      // 目标进程pid / 句柄
//    PVOID virtualAddr;  // 目标进程虚拟地址
//    UINT64 nSize;       // 大小
//
//} R3R0_Param, * PR3R0_Param;

const char* GetProcessNameByPid(ULONG dwPid);
const char* GetProcessNameByPid2(HANDLE ProcessId);

void Handle_IOCTL_GETPROCESS_NAME(IN PIRP pIrp);
void Handle_IOCTL_ADD_PID(IN PIRP pIrp);
void Handle_IOCTL_REMOVE_PID(IN PIRP pIrp);

void Handle_IOCTL_缓冲区读数据(IN PIRP pIrp);
void Handle_IOCTL_缓冲区写数据(IN PIRP pIrp);

void Handle_IOCTL_MDL读数据(IN PIRP pIrp);
void Handle_IOCTL_MDL写数据(IN PIRP pIrp);

void Handle_IOCTL_MDL写只读数据(IN PIRP pIrp);
void Handle_IOCTL_CR0写只读数据(IN PIRP pIrp);

void Handle_IOCTL_物理地址映射读数据(IN PIRP pIrp);
void Handle_IOCTL_物理地址映射写数据(IN PIRP pIrp);

