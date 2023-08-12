/************************************************************************
* �������:2023-07-29
*************************************************************************/

#include <ntddk.h>
#include "processProtect.h"

#define PAGEDCODE code_seg("PAGE")
#define LOCKEDCODE code_seg()
#define INITCODE code_seg("INIT")

#define PAGEDDATA data_seg("PAGE")
#define LOCKEDDATA data_seg()
#define INITDATA data_seg("INIT")

#define arraysize(p) (sizeof(p)/sizeof((p)[0]))

#define DeviceName L"\\Device\\MyDriver"
#define LinkName L"\\??\\MyDriver"

NTSTATUS CreateDevice(IN PDRIVER_OBJECT pDriverObject);
VOID HelloDDKUnload(IN PDRIVER_OBJECT pDriverObject);
NTSTATUS HelloDDKDispatchRoutine(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp);
NTSTATUS HelloDDKDeviceIOControl(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp);


#define IOCTL_TEST_1 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)


#pragma INITCODE
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath)
{
	NTSTATUS status;
	DbgPrint("sys:Enter DriverEntry\n");
	pRegistryPath;
	BypassCheckSign(pDriverObject);
	//ע�������������ú������
	pDriverObject->DriverUnload = HelloDDKUnload;
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = HelloDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = HelloDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = HelloDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_READ] = HelloDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = HelloDDKDeviceIOControl;

	//���������豸����
	status = CreateDevice(pDriverObject);
	InstallProcessProtect();
	DbgPrint("sys:DriverEntry end\n");
	return status;
}


#pragma INITCODE
NTSTATUS CreateDevice(
	IN PDRIVER_OBJECT	pDriverObject)
{
	NTSTATUS status;
	PDEVICE_OBJECT pDevObj;

	//�����豸����
	UNICODE_STRING devName;
	RtlInitUnicodeString(&devName, DeviceName);

	//�����豸
	status = IoCreateDevice(pDriverObject,
		0,                  // ��չ����
		&devName,           // �豸����
		FILE_DEVICE_UNKNOWN,// ����
		0, TRUE,
		&pDevObj);

	if (!NT_SUCCESS(status))
	{
		DbgPrint("sys:CreateDevice error\n");
		return status;
	}

	pDevObj->Flags |= DO_BUFFERED_IO;

	//������������
	UNICODE_STRING symLinkName;
	RtlInitUnicodeString(&symLinkName, LinkName);

	status = IoCreateSymbolicLink(&symLinkName, &devName);
	if (!NT_SUCCESS(status))
	{
		IoDeleteDevice(pDevObj);
		return status;
	}
	return STATUS_SUCCESS;
}


#pragma PAGEDCODE
VOID HelloDDKUnload(IN PDRIVER_OBJECT pDriverObject)
{

	DbgPrint("sys:Enter DriverUnload\n");

	UNICODE_STRING pLinkName;
	RtlInitUnicodeString(&pLinkName, LinkName);
	IoDeleteSymbolicLink(&pLinkName);

	
	UnInstallProcessProtect();
	DbgPrint("sys:Enter DriverUnload IoDeleteSymbolicLink \n");
	IoDeleteDevice(pDriverObject->DeviceObject);
	DbgPrint("sys:Enter DriverUnload IoDeleteDevice \n");


}

#pragma PAGEDCODE
NTSTATUS HelloDDKDispatchRoutine(IN PDEVICE_OBJECT pDevObj,
	IN PIRP pIrp)
{
	DbgPrint("sys:Enter HelloDDKDispatchRoutine\n");
	NTSTATUS status = STATUS_SUCCESS;
	// ���IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;	// bytes xfered
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	DbgPrint("sys:Leave HelloDDKDispatchRoutine\n");
	return status;
}



#pragma PAGEDCODE
NTSTATUS HelloDDKDeviceIOControl(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	NTSTATUS status = STATUS_SUCCESS;
	DbgPrint("sys:Enter HelloDDKDeviceIOControl\n");

	//��ǰ��ջ
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	//���뻺������С
	ULONG cbin = stack->Parameters.DeviceIoControl.InputBufferLength;
	//�����������С
	ULONG cbout = stack->Parameters.DeviceIoControl.OutputBufferLength;
	//IOCTL��
	ULONG code = stack->Parameters.DeviceIoControl.IoControlCode;

	DbgPrint("sys:DeviceIOControl ���뻺������С:%d \n", cbin);
	DbgPrint("sys:DeviceIOControl �����������С:%d \n", cbout);
	DbgPrint("sys:DeviceIOControl IOCTL:%d \n", code);

	ULONG info = 0;

	switch (code)
	{						// process request

	case IOCTL_TEST_1:
	{
		// ���ַ�ʽ��ȡ�������ڴ����豸��ʱ��ָ��������Ϊ
		// pDevObj->Flags |= DO_BUFFERED_IO;
		DbgBreakPoint();
		DbgPrint("sys:IOCTL_TEST_SYSTEM_BUFFER\n");
		//��ʾ���뻺��������
		UINT64* param = (UINT64*)pIrp->AssociatedIrp.SystemBuffer;
		DbgPrint("r3 to r0:wbuf [%lld]\n", param[0]);

		//�������������
		param[0] = 0x3;

		//����ʵ�ʲ����������������
		info = cbout;
		break;
	}

	default:
		status = STATUS_INVALID_VARIANT;
	}

	// ���IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = info;	// bytes xfered
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	DbgPrint("sys:Leave HelloDDKDeviceIOControl\n");

	return status;
}

