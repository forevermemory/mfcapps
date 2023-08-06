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
NTSTATUS HelloDDKRead(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp);
NTSTATUS HelloDDKWrite(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp);


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
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = HelloDDKWrite;
	pDriverObject->MajorFunction[IRP_MJ_READ] = HelloDDKRead;

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

	//pDevObj->Flags |= DO_BUFFERED_IO;
	pDevObj->Flags |= DO_DIRECT_IO;



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


NTSTATUS HelloDDKWrite(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	DbgPrint("Enter HelloDDKWrite\n");

	//对一般IRP的简单操作，后面会介绍对IRP更复杂的操作
	NTSTATUS status = STATUS_SUCCESS;

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	ULONG ulWriteLength = stack->Parameters.Write.Length;

	// 完成IRP
	//设置IRP完成状态
	pIrp->IoStatus.Status = status;

	// 获取从r3传进来的buff
	PVOID kenelP = MmGetSystemAddressForMdlSafe(pIrp->MdlAddress, NormalPagePriority);
	DbgPrint("HelloDDKWrite SystemBuffer:%p  content %s", kenelP, (char*)kenelP);

	//处理IRP
	pIrp->IoStatus.Information = 0x13;	// bytes xfered
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	DbgPrint("Leave HelloDDKWrite\n");
	return status;
}


NTSTATUS HelloDDKRead(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	DbgPrint("Enter HelloDDKRead\n");

	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);

	ULONG ulReadLength = stack->Parameters.Read.Length;
	DbgPrint("ulReadLength:%d\n", ulReadLength);
	DbgPrint("pIrp->MdlAddress:%p\n", pIrp->MdlAddress);

	ULONG mdl_length = MmGetMdlByteCount(pIrp->MdlAddress);
	PVOID mdl_address = MmGetMdlVirtualAddress(pIrp->MdlAddress);
	ULONG mdl_offset = MmGetMdlByteOffset(pIrp->MdlAddress);

	DbgPrint("mdl_address:%p\n", mdl_address);
	DbgPrint("mdl_length:%d\n", mdl_length);
	DbgPrint("mdl_offset:%d\n", mdl_offset);

	if (mdl_length != ulReadLength)
	{
		//MDL的长度应该和读长度相等，否则该操作应该设为不成功
		pIrp->IoStatus.Information = 0;
		status = STATUS_UNSUCCESSFUL;
	}
	else
	{
		//用MmGetSystemAddressForMdlSafe得到MDL在内核模式下的映射
		PVOID kernel_address = MmGetSystemAddressForMdlSafe(pIrp->MdlAddress, NormalPagePriority);
		DbgPrint("kernel_address:%p\n", kernel_address);
		memset(kernel_address, 0XAA, ulReadLength);
		pIrp->IoStatus.Information = ulReadLength;	// bytes xfered
	}

	// 完成IRP
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	DbgPrint("Leave HelloDDKRead\n");
	return status;
}