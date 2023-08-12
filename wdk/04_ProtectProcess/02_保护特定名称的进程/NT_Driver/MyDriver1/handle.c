#include "handle.h"

// δ�ĵ�����
char* PsGetProcessImageFileName(PEPROCESS);

// ע��: �ú���ͨ��r3����pid��ȡ����������������
// ���ǲ�����ObRegisterCallbacks�Ļص���ʹ��,������ջ���
// NtOpenProcess ��ZwOpenProcess������ַ��һ����
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

	//�򿪽��̣���ȡ���  
	status = ZwOpenProcess(&ProcessHandle, PROCESS_ALL_ACCESS, &ObjectAttributes, &myCid);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("sys:�򿪽��̳���\n");
		return;
	}

	//�õ�EPROCESS���ṹ��ȡ������  
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

// δ�ĵ�������,��Ҫ����
NTSTATUS PsLookupProcessByProcessId(
	__in HANDLE ProcessId,   //����ID
	__deref_out PEPROCESS* Process //���ص�EPROCESS
);

//����:����ID,��������
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
	//��ʾ���뻺��������
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
	// ���IRP
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = strlen(processName)+1;	// bytes return
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

}