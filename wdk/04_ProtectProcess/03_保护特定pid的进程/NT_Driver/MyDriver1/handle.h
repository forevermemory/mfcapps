#pragma once

#include <ntddk.h>

const char* GetProcessNameByPid(ULONG dwPid);
const char* GetProcessNameByPid2(HANDLE ProcessId);

void Handle_IOCTL_GETPROCESS_NAME(IN PIRP pIrp);
void Handle_IOCTL_ADD_PID(IN PIRP pIrp);
void Handle_IOCTL_REMOVE_PID(IN PIRP pIrp);


