#pragma once

#include <ntddk.h>
//ObReferenceObjectByHandle(
//    _In_ HANDLE Handle,
//    _In_ ACCESS_MASK DesiredAccess,
//    _In_opt_ POBJECT_TYPE ObjectType,
//    _In_ KPROCESSOR_MODE AccessMode,
//    _Out_ PVOID* Object,
//    _Out_opt_ POBJECT_HANDLE_INFORMATION HandleInformation
//);

//typedef struct _R3R0_Param {
//    UINT64 handle;      // Ŀ�����pid / ���
//    PVOID virtualAddr;  // Ŀ����������ַ
//    UINT64 nSize;       // ��С
//
//} R3R0_Param, * PR3R0_Param;

const char* GetProcessNameByPid(ULONG dwPid);
const char* GetProcessNameByPid2(HANDLE ProcessId);

void Handle_IOCTL_GETPROCESS_NAME(IN PIRP pIrp);
void Handle_IOCTL_ADD_PID(IN PIRP pIrp);
void Handle_IOCTL_REMOVE_PID(IN PIRP pIrp);

void Handle_IOCTL_������������(IN PIRP pIrp);
void Handle_IOCTL_������д����(IN PIRP pIrp);

void Handle_IOCTL_MDL������(IN PIRP pIrp);
void Handle_IOCTL_MDLд����(IN PIRP pIrp);

void Handle_IOCTL_MDLдֻ������(IN PIRP pIrp);
void Handle_IOCTL_CR0дֻ������(IN PIRP pIrp);

void Handle_IOCTL_�����ַӳ�������(IN PIRP pIrp);
void Handle_IOCTL_�����ַӳ��д����(IN PIRP pIrp);

void Handle_IOCTL_Enum����ص�(IN PIRP pIrp);
void Handle_IOCTL_��ָ��pid��Ȩ(IN PIRP pIrp);
void Handle_IOCTL_�Ƴ�ָ��pid��Ȩ(IN PIRP pIrp);
