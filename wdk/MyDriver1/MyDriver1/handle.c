
#include "handle.h"
#include "processProtect.h"
#include "virtualMemoryRW.h"

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



void Handle_IOCTL_ADD_PID(IN PIRP pIrp)
{
	//��ʾ���뻺��������
	UINT64* param = (UINT64*)pIrp->AssociatedIrp.SystemBuffer;
	DbgPrint("sys:r3 to r0:pid [%lld]\n", param[0]);

	UINT32 pid = (UINT32)param[0];

	char* processName = GetProcessNameByPid2(pid);
	if (processName)
	{
		DbgPrint("sys: pid:%d process name:%s\n", pid, processName);
	}

	AddPidToProtect(pid);

	// ���IRP
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;	// bytes return
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

}




void Handle_IOCTL_REMOVE_PID(IN PIRP pIrp)
{
	//��ʾ���뻺��������
	UINT64* param = (UINT64*)pIrp->AssociatedIrp.SystemBuffer;
	DbgPrint("sys:r3 to r0:pid [%lld]\n", param[0]);

	UINT32 pid = (UINT32)param[0];

	char* processName = GetProcessNameByPid2(pid);
	if (processName)
	{
		DbgPrint("sys: pid:%d process name:%s\n", pid, processName);
	}

	RemovePidToProtect(pid);

	// ���IRP
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;	// bytes return
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

}



// ��ĳ������pid�������ڴ�
void Handle_IOCTL_������������(IN PIRP pIrp)
{
	UINT64* param = (UINT64*)pIrp->AssociatedIrp.SystemBuffer;

	// �������
	// UINT64 handle;      // Ŀ�����pid / ���
	// PVOID virtualAddr;  // Ŀ����������ַ
	// UINT64 nSize;       // ��С
	UINT64 pid = (UINT64)param[0];
	PVOID virtualAddr =(PVOID)(UINT64)param[1];
	UINT64 nSize = (UINT64)param[2];
	DbgPrint("sys: ReadVirtualMemory pid=%d pBase=%p\n", pid, virtualAddr);


	UINT64 retLen = 0;
	// ��ȡĿ����̵������ڴ�
	PEPROCESS pEPROCESS = NULL;
	if (STATUS_SUCCESS == PsLookupProcessByProcessId((HANDLE)pid, &pEPROCESS))
	{
		retLen = KReadProcessMemory(pEPROCESS, virtualAddr, nSize, param);
		ObDereferenceObject(pEPROCESS);
	}
	else
	{
		DbgPrint("sys: PsLookupProcessByProcessId Fail...\n");
	}

	DbgPrint("sys: ������������,���IRP,retLen:%lld\n", retLen);
	// ���IRP
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = retLen;	// bytes return
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
}

// дĳ������pid�������ڴ�
void Handle_IOCTL_������д����(IN PIRP pIrp)
{
	UINT64* param = (UINT64*)pIrp->AssociatedIrp.SystemBuffer;

	// �������
	// UINT64 handle;      // Ŀ�����pid / ���
	// PVOID virtualAddr;  // Ŀ����������ַ
	// UINT64 nSize;       // ��С
	// UINT64 value;       // ��д��ֵ
	UINT64 pid = (UINT64)param[0];
	PVOID virtualAddr = (PVOID)(UINT64)param[1];
	UINT64 nSize = (UINT64)param[2];
	UINT64 value = (UINT64)param[3];
	DbgPrint("sys: WriteVirtualMemory pid=%d pBase=%p value:%llX\n", pid, virtualAddr, value);


	UINT64 retLen = 0;
	// ��ȡĿ����̵������ڴ�
	PEPROCESS pEPROCESS = NULL;
	if (STATUS_SUCCESS == PsLookupProcessByProcessId((HANDLE)pid, &pEPROCESS))
	{
		retLen = KWriteProcessMemory(pEPROCESS, virtualAddr, nSize, &value);
		ObDereferenceObject(pEPROCESS);
	}
	else
	{
		DbgPrint("sys: PsLookupProcessByProcessId Fail...\n");
	}

	DbgPrint("sys: ������������,���IRP,retLen:%lld\n", retLen);
	// ���IRP
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = retLen;	// bytes return
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
}



void Handle_IOCTL_MDL������(IN PIRP pIrp)
{
	UINT64* param = (UINT64*)pIrp->AssociatedIrp.SystemBuffer;

	// �������
	// UINT64 handle;      // Ŀ�����pid / ���
	// PVOID virtualAddr;  // Ŀ����������ַ
	// UINT64 nSize;       // ��С
	UINT64 pid = (UINT64)param[0];
	PVOID virtualAddr = (PVOID)(UINT64)param[1];
	UINT64 nSize = (UINT64)param[2];
	DbgPrint("sys: MDL ReadVirtualMemory pid=%d pBase=%p\n", pid, virtualAddr);


	UINT64 retLen = 0;
	// ��ȡĿ����̵������ڴ�
	PEPROCESS pEPROCESS = NULL;
	if (STATUS_SUCCESS == PsLookupProcessByProcessId((HANDLE)pid, &pEPROCESS))
	{
		retLen = KReadProcessMemory_MDL(pEPROCESS, virtualAddr, nSize, param);
		ObDereferenceObject(pEPROCESS);
	}
	else
	{
		DbgPrint("sys: PsLookupProcessByProcessId Fail...\n");
	}

	DbgPrint("sys: ������������,���IRP,retLen:%lld\n", retLen);
	// ���IRP
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = retLen;	// bytes return
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
}


void Handle_IOCTL_MDLд����(IN PIRP pIrp)
{
	UINT64* param = (UINT64*)pIrp->AssociatedIrp.SystemBuffer;

	// �������
	// UINT64 handle;      // Ŀ�����pid / ���
	// PVOID virtualAddr;  // Ŀ����������ַ
	// UINT64 nSize;       // ��С
	// UINT64 value;       // ��д��ֵ
	UINT64 pid = (UINT64)param[0];
	PVOID virtualAddr = (PVOID)(UINT64)param[1];
	UINT64 nSize = (UINT64)param[2];
	UINT64 value = (UINT64)param[3];
	DbgPrint("sys: WriteVirtualMemory pid=%d pBase=%p value:%llX\n", pid, virtualAddr, value);


	UINT64 retLen = 0;
	// ��ȡĿ����̵������ڴ�
	PEPROCESS pEPROCESS = NULL;
	if (STATUS_SUCCESS == PsLookupProcessByProcessId((HANDLE)pid, &pEPROCESS))
	{
		retLen = KWriteProcessMemory_MDL(pEPROCESS, virtualAddr, nSize, &value);
		ObDereferenceObject(pEPROCESS);
	}
	else
	{
		DbgPrint("sys: PsLookupProcessByProcessId Fail...\n");
	}

	DbgPrint("sys: ������������,���IRP,retLen:%lld\n", retLen);
	// ���IRP
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = retLen;	// bytes return
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
}



void Handle_IOCTL_MDLдֻ������(IN PIRP pIrp)
{
	UINT64* param = (UINT64*)pIrp->AssociatedIrp.SystemBuffer;

	// �������
	// UINT64 handle;      // Ŀ�����pid / ���
	// PVOID virtualAddr;  // Ŀ����������ַ
	// UINT64 nSize;       // ��С
	// UINT64 value;       // ��д��ֵ
	UINT64 pid = (UINT64)param[0];
	PVOID virtualAddr = (PVOID)(UINT64)param[1];
	UINT64 nSize = (UINT64)param[2];
	UINT64 value = (UINT64)param[3];
	DbgPrint("sys: WriteVirtualMemory pid=%d pBase=%p value:%llX\n", pid, virtualAddr, value);


	UINT64 retLen = 0;
	// ��ȡĿ����̵������ڴ�
	PEPROCESS pEPROCESS = NULL;
	if (STATUS_SUCCESS == PsLookupProcessByProcessId((HANDLE)pid, &pEPROCESS))
	{
		retLen = KWriteProcessMemory_MDL_ֻ��(pEPROCESS, virtualAddr, nSize, &value);
		ObDereferenceObject(pEPROCESS);
	}
	else
	{
		DbgPrint("sys: PsLookupProcessByProcessId Fail...\n");
	}

	DbgPrint("sys: ������������,���IRP,retLen:%lld\n", retLen);
	// ���IRP
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = retLen;	// bytes return
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
}


void Handle_IOCTL_CR0дֻ������(IN PIRP pIrp)
{
	UINT64* param = (UINT64*)pIrp->AssociatedIrp.SystemBuffer;

	// �������
	// UINT64 handle;      // Ŀ�����pid / ���
	// PVOID virtualAddr;  // Ŀ����������ַ
	// UINT64 nSize;       // ��С
	// UINT64 value;       // ��д��ֵ
	UINT64 pid = (UINT64)param[0];
	PVOID virtualAddr = (PVOID)(UINT64)param[1];
	UINT64 nSize = (UINT64)param[2];
	UINT64 value = (UINT64)param[3];
	DbgPrint("sys: WriteVirtualMemory pid=%d pBase=%p value:%llX\n", pid, virtualAddr, value);


	UINT64 retLen = 0;
	// ��ȡĿ����̵������ڴ�
	PEPROCESS pEPROCESS = NULL;
	if (STATUS_SUCCESS == PsLookupProcessByProcessId((HANDLE)pid, &pEPROCESS))
	{
		retLen = KWriteProcessMemory_CR0_ֻ��(pEPROCESS, virtualAddr, nSize, &value);
		ObDereferenceObject(pEPROCESS);
	}
	else
	{
		DbgPrint("sys: PsLookupProcessByProcessId Fail...\n");
	}

	DbgPrint("sys: ������������,���IRP,retLen:%lld\n", retLen);
	// ���IRP
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = retLen;	// bytes return
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
}


void Handle_IOCTL_�����ַӳ�������(IN PIRP pIrp)
{
	UINT64* param = (UINT64*)pIrp->AssociatedIrp.SystemBuffer;

	// �������
	// UINT64 handle;      // Ŀ�����pid / ���
	// PVOID virtualAddr;  // Ŀ����������ַ
	// UINT64 nSize;       // ��С
	UINT64 pid = (UINT64)param[0];
	PVOID virtualAddr = (PVOID)(UINT64)param[1];
	UINT64 nSize = (UINT64)param[2];
	DbgPrint("sys: MDL ReadVirtualMemory pid=%d pBase=%p\n", pid, virtualAddr);


	UINT64 retLen = 0;
	// ��ȡĿ����̵������ڴ�
	PEPROCESS pEPROCESS = NULL;
	if (STATUS_SUCCESS == PsLookupProcessByProcessId((HANDLE)pid, &pEPROCESS))
	{
		retLen = KReadProcessMemory_Physical(pEPROCESS, virtualAddr, nSize, param);
		ObDereferenceObject(pEPROCESS);
	}
	else
	{
		DbgPrint("sys: PsLookupProcessByProcessId Fail...\n");
	}

	DbgPrint("sys: ������������,���IRP,retLen:%lld\n", retLen);
	// ���IRP
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = retLen;	// bytes return
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
}

void Handle_IOCTL_�����ַӳ��д����(IN PIRP pIrp)
{
	UINT64* param = (UINT64*)pIrp->AssociatedIrp.SystemBuffer;

	// �������
	// UINT64 handle;      // Ŀ�����pid / ���
	// PVOID virtualAddr;  // Ŀ����������ַ
	// UINT64 nSize;       // ��С
	UINT64 pid = param[0];
	PVOID virtualAddr = (PVOID)param[1];
	UINT64 nSize = param[2];
	PVOID buffer = (PVOID)param[3];
	DbgPrint("sys: MDL Handle_IOCTL_�����ַӳ��д���� pid=%d pBase=%p \n",pid, virtualAddr);
	DbgPrint("sys: nSize:%lld, Buffer:%llX\n", nSize, buffer);

	UINT64 retLen = 0;
	// ��ȡĿ����̵������ڴ�
	PEPROCESS pEPROCESS = NULL;
	if (STATUS_SUCCESS == PsLookupProcessByProcessId((HANDLE)pid, &pEPROCESS))
	{
		retLen = KWriteProcessMemory_Physical(pEPROCESS, virtualAddr, nSize, &buffer);
		ObDereferenceObject(pEPROCESS);
	}
	else
	{
		DbgPrint("sys: PsLookupProcessByProcessId Fail...\n");
	}

	DbgPrint("sys: ������������,���IRP,retLen:%lld\n", retLen);
	// ���IRP
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = retLen;	// bytes return
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
}