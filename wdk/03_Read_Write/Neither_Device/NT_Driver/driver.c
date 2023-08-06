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
	//pDevObj->Flags |= DO_DIRECT_IO;
	// user address 模式



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

	//得到当前堆栈
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	//得到长度
	ULONG ulLength = stack->Parameters.Write.Length;
	//得到偏移量
	ULONG ulOffset = (ULONG)stack->Parameters.Write.ByteOffset.QuadPart;
	//得到用户模式地址
	PVOID user_address = pIrp->UserBuffer;

	DbgPrint("user_address:%p\n", user_address);

	__try
	{
		DbgPrint("Enter __try block\n");
		//判断空指针是否可读，显然会导致异常
		ProbeForRead(user_address, ulLength, 4);

		DbgPrint("user_address:%s ====\n", (char*)user_address);

		//由于在上面引发异常，所以以后语句不会被执行!
		DbgPrint("Leave __try block\n");
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("Catch the exception\n");
		DbgPrint("The program will keep going\n");
		status = STATUS_UNSUCCESSFUL;
	}
	//处理IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0x13;	// bytes xfered
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	DbgPrint("Leave HelloDDKWrite\n");
	return status;
}


NTSTATUS HelloDDKRead(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	DbgPrint("Enter HelloDDKRead\n");

	NTSTATUS status = STATUS_SUCCESS;
	//得到当前堆栈
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	//得到读的长度
	ULONG ulReadLength = stack->Parameters.Read.Length;
	//得到读的偏移量
	ULONG ulReadOffset = (ULONG)stack->Parameters.Read.ByteOffset.QuadPart;
	//得到用户模式地址
	PVOID user_address = pIrp->UserBuffer;

	DbgPrint("user_address:%p\n", user_address);

	__try
	{
		DbgPrint("Enter __try block\n");
		//判断空指针是否可写，显然会导致异常
		ProbeForWrite(user_address, ulReadLength, 4);

		memset(user_address, 0xAA, ulReadLength);
		pIrp->IoStatus.Information = ulReadLength;	// bytes xfered
		//由于在上面引发异常，所以以后语句不会被执行!
		DbgPrint("Leave __try block\n");
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("Catch the exception\n");
		DbgPrint("The program will keep going\n");
		status = STATUS_UNSUCCESSFUL;
	}

	// 完成IRP
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	DbgPrint("Leave HelloDDKRead\n");
	return status;
}