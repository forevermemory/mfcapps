/************************************************************************
* 完成日期:2023-07-29
*************************************************************************/

#include <ntddk.h>

#define PAGEDCODE code_seg("PAGE")
#define LOCKEDCODE code_seg()
#define INITCODE code_seg("INIT")

#define PAGEDDATA data_seg("PAGE")
#define LOCKEDDATA data_seg()
#define INITDATA data_seg("INIT")

#define arraysize(p) (sizeof(p)/sizeof((p)[0]))

#define DeviceName L"\\Device\\MyDDKDevice"
#define LinkName L"\\??\\MyDDKDevice"

NTSTATUS CreateDevice(IN PDRIVER_OBJECT pDriverObject);
VOID HelloDDKUnload(IN PDRIVER_OBJECT pDriverObject);
NTSTATUS HelloDDKDispatchRoutine(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp);
NTSTATUS HelloDDKDeviceIOControl(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp);


#define IOCTL_TEST_1 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)


#pragma INITCODE
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath)
{
	NTSTATUS status;
	DbgPrint("Enter DriverEntry\n");

	//注册其他驱动调用函数入口
	pDriverObject->DriverUnload = HelloDDKUnload;
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = HelloDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = HelloDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = HelloDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_READ] = HelloDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = HelloDDKDeviceIOControl;

	//创建驱动设备对象
	status = CreateDevice(pDriverObject);

	DbgPrint("DriverEntry end\n");
	return status;
}


#pragma INITCODE
NTSTATUS CreateDevice(
	IN PDRIVER_OBJECT	pDriverObject)
{
	NTSTATUS status;
	PDEVICE_OBJECT pDevObj;

	//创建设备名称
	UNICODE_STRING devName;
	RtlInitUnicodeString(&devName, DeviceName);

	//创建设备
	status = IoCreateDevice(pDriverObject,
		0,                  // 扩展数据
		&devName,           // 设备名称
		FILE_DEVICE_UNKNOWN,// 类型
		0, TRUE,
		&pDevObj);

	if (!NT_SUCCESS(status))
	{
		DbgPrint("CreateDevice error\n");
		return status;
	}

	pDevObj->Flags |= DO_BUFFERED_IO;

	//创建符号链接
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

	DbgPrint("Enter DriverUnload\n");

	UNICODE_STRING pLinkName;
	RtlInitUnicodeString(&pLinkName, LinkName);
	IoDeleteSymbolicLink(&pLinkName);

	DbgPrint("Enter DriverUnload IoDeleteSymbolicLink \n");
	IoDeleteDevice(pDriverObject->DeviceObject);
	DbgPrint("Enter DriverUnload IoDeleteDevice \n");


}

#pragma PAGEDCODE
NTSTATUS HelloDDKDispatchRoutine(IN PDEVICE_OBJECT pDevObj,
	IN PIRP pIrp)
{
	DbgPrint("Enter HelloDDKDispatchRoutine\n");
	NTSTATUS status = STATUS_SUCCESS;
	// 完成IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;	// bytes xfered
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	DbgPrint("Leave HelloDDKDispatchRoutine\n");
	return status;
}

typedef struct  _MY_STRUCT {
	ULONG64 Size;
	ULONG64 TYPE;
	char buf[32];
	wchar_t wbuf[64];
	char data[1024];
} MY_STRUCT, * PMY_STRUCT;

#pragma PAGEDCODE
NTSTATUS HelloDDKDeviceIOControl(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	NTSTATUS status = STATUS_SUCCESS;
	DbgPrint("Enter HelloDDKDeviceIOControl\n");

	//当前堆栈
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	//输入缓冲区大小
	ULONG cbin = stack->Parameters.DeviceIoControl.InputBufferLength;
	//输出缓冲区大小
	ULONG cbout = stack->Parameters.DeviceIoControl.OutputBufferLength;
	//IOCTL码
	ULONG code = stack->Parameters.DeviceIoControl.IoControlCode;

	DbgPrint("DeviceIOControl 输入缓冲区大小:%d \n", cbin);
	DbgPrint("DeviceIOControl 输出缓冲区大小:%d \n", cbout);
	DbgPrint("DeviceIOControl IOCTL:%d \n", code);

	ULONG info = 0;

	switch (code)
	{						// process request

	case IOCTL_TEST_1:
	{
		// 这种方式获取参数是在创建设备的时候指定了类型为
		// pDevObj->Flags |= DO_BUFFERED_IO;

		DbgPrint("IOCTL_TEST_SYSTEM_BUFFER\n");
		//显示输入缓冲区数据
		PMY_STRUCT param = (PMY_STRUCT)pIrp->AssociatedIrp.SystemBuffer;
		DbgPrint("r3 to r0:wbuf [%S]\n", param->wbuf);
		DbgPrint("r3 to r0:buf [%s]\n", param->buf);

		//操作输出缓冲区
		UNICODE_STRING tmp;
		RtlInitUnicodeString(&tmp, L"r0 to r3 string SystemBuffer");
		memcpy(param->data, tmp.Buffer, tmp.Length);
		param->Size = 0x300;
		param->TYPE = 0x666;
		//设置实际操作输出缓冲区长度
		info = cbout;
		break;
	}
	
	default:
		status = STATUS_INVALID_VARIANT;
	}

	// 完成IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = info;	// bytes xfered
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	DbgPrint("Leave HelloDDKDeviceIOControl\n");

	return status;
}