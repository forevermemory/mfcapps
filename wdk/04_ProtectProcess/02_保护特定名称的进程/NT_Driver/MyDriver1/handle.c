#include "handle.h"

// 未文档函数
char* PsGetProcessImageFileName(PEPROCESS);

// 注意: 该函数通过r3传入pid获取进程名称是正常的
// 但是不可在ObRegisterCallbacks的回调中使用,否则会堆栈溢出
// NtOpenProcess 和ZwOpenProcess函数地址是一样的
const char* GetProcessNameByPid(ULONG dwPid)
{
	HANDLE ProcessHandle;
	NTSTATUS status;
	OBJECT_ATTRIBUTES  ObjectAttributes;
	CLIENT_ID myCid;
	PEPROCESS EProcess;
	char* PsName = 0;

	InitializeObjectAttributes(&ObjectAttributes, 0, 0, 0, 0);

	myCid.UniqueProcess = (HANDLE)dwPid;
	myCid.UniqueThread = 0;

	//打开进程，获取句柄  
	status = ZwOpenProcess(&ProcessHandle, PROCESS_ALL_ACCESS, &ObjectAttributes, &myCid);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("sys:打开进程出错\n");
		return;
	}

	//得到EPROCESS，结构中取进程名  
	status = ObReferenceObjectByHandle(ProcessHandle, FILE_READ_DATA, 0, KernelMode, &EProcess, 0);
	if (status == STATUS_SUCCESS)
	{
		//  char *ProcessName = (char*)EProcess + 0x174;  //ImageFileName[11]
		PsName = PsGetProcessImageFileName(EProcess);

		DbgPrint("sys:PsName is %s\n", PsName);
		ZwClose(ProcessHandle);
	}
	else
	{
		DbgPrint("sys:Get ProcessName error");
	}
	return PsName;
}

// 未文档化函数,需要声明
NTSTATUS PsLookupProcessByProcessId(
	__in HANDLE ProcessId,   //进程ID
	__deref_out PEPROCESS* Process //返回的EPROCESS
);

//功能:进程ID,进程名称
const char* GetProcessNameByPid2(HANDLE ProcessId)
{
	NTSTATUS st = STATUS_UNSUCCESSFUL;
	PEPROCESS ProcessObj = NULL;
	const char* string = NULL;
	st = PsLookupProcessByProcessId(ProcessId, &ProcessObj);
	if (NT_SUCCESS(st))
	{
		string = PsGetProcessImageFileName(ProcessObj);
		ObfDereferenceObject(ProcessObj);
	}
	return string;
}



void Handle_IOCTL_GETPROCESS_NAME(IN PIRP pIrp)
{
	//显示输入缓冲区数据
	UINT64* param = (UINT64*)pIrp->AssociatedIrp.SystemBuffer;
	DbgPrint("sys:r3 to r0:pid [%lld]\n", param[0]);

	UINT32 pid = (UINT32)param[0];




	char * processName = GetProcessNameByPid(pid);
	if (processName)
	{
		DbgPrint("sys: pid:%d process name:%s\n", pid, processName);
	}

	RtlCopyMemory(pIrp->AssociatedIrp.SystemBuffer,(PVOID)processName, strlen(processName)+1);
	DbgPrint("sys: RtlCopyMemory %d \n", strlen(processName));
	// 完成IRP
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = strlen(processName)+1;	// bytes return
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

}