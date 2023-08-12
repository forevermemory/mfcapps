#include <ntifs.h>
#include <ntddk.h>

#include "processCallback.h"
#include "processProtect.h"


void sCreateProcessNotifyRoutine(HANDLE ppid, HANDLE pid, BOOLEAN create)
{

	PEPROCESS process = NULL;
	PUNICODE_STRING parentProcessName = NULL, processName = NULL;

	PsLookupProcessByProcessId(pid, &process);
	SeLocateProcessImageName(process, &processName);
	
	if (create)
	{
		DbgPrint("sys: PROCESS: create  %d  %d  %wZ\n", ppid, pid, processName);
	}
	else
	{
		DbgPrint("sys: PROCESS: exit  %d lost child  %d  %wZ\n", ppid, pid, processName);
		// 判断是否存在保护中，如果存在则移除保护
		if (IsPidToProtect(pid))
		{
			DbgPrint("sys: PROCESS: 移除保护 pid:%d\n", pid);
			RemovePidToProtect(pid);
		}
	}
}