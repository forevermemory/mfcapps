/************************************************************************
* 完成日期:2023-07-29
*************************************************************************/

#include <ntddk.h>
#include "processProtect.h"
#include "handle.h"
#include "processCallback.h"

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
#define IOCTL_GETPROCESS_NAME CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ADD_PID CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REMOVE_PID CTL_CODE(FILE_DEVICE_UNKNOWN, 0x804, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_缓冲区读数据 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x810, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_缓冲区写数据 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x811, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_MDL读数据 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x812, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MDL写数据 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x813, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_MDL写只读数据 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x814, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_CR0写只读数据 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x815, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_物理地址映射读数据 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x816, METHOD_BUFFERED, FILE_ANY_ACCESS)  
#define IOCTL_物理地址映射写数据 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x817, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_Enum对象回调 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x820, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_给指定pid提权 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x821, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_移除指定pid提权 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x822, METHOD_BUFFERED, FILE_ANY_ACCESS)

 
#pragma INITCODE
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath)
{
	NTSTATUS status;
	DbgPrint("sys:Enter DriverEntry\n");
	pRegistryPath;
	BypassCheckSign(pDriverObject);
	//注册其他驱动调用函数入口
	pDriverObject->DriverUnload = HelloDDKUnload;
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = HelloDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = HelloDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = HelloDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_READ] = HelloDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = HelloDDKDeviceIOControl;

	//创建驱动设备对象
	status = CreateDevice(pDriverObject);

	// 注册OpenProcess回调
	InstallProcessProtect();
	// 提权
	InstallProcessPrivilege();

	// 注册进程普通回调
	PsSetCreateProcessNotifyRoutine(sCreateProcessNotifyRoutine, FALSE);

	DbgPrint("sys:DriverEntry end\n");
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
		DbgPrint("sys:CreateDevice error\n");
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

	DbgPrint("sys:Enter DriverUnload\n");

	UNICODE_STRING pLinkName;
	RtlInitUnicodeString(&pLinkName, LinkName);
	IoDeleteSymbolicLink(&pLinkName);

	// 卸载进程保护
	UnInstallProcessProtect();
	UnInstallProcessPrivilege();
	// 卸载普通进程回调
	PsSetCreateProcessNotifyRoutine(sCreateProcessNotifyRoutine, TRUE);


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
	// 完成IRP
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

	//当前堆栈
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	//输入缓冲区大小
	ULONG cbin = stack->Parameters.DeviceIoControl.InputBufferLength;
	//输出缓冲区大小
	ULONG cbout = stack->Parameters.DeviceIoControl.OutputBufferLength;
	//IOCTL码
	ULONG code = stack->Parameters.DeviceIoControl.IoControlCode;

	DbgPrint("sys:DeviceIOControl 输入缓冲区大小:%d \n", cbin);
	DbgPrint("sys:DeviceIOControl 输出缓冲区大小:%d \n", cbout);
	DbgPrint("sys:DeviceIOControl IOCTL:%d \n", code);

	ULONG info = 0;

	switch (code)
	{						// process request

	case IOCTL_TEST_1:
	{
		// 这种方式获取参数是在创建设备的时候指定了类型为
		// pDevObj->Flags |= DO_BUFFERED_IO;
		//DbgBreakPoint();
		DbgPrint("sys:IOCTL_TEST_SYSTEM_BUFFER\n");
		//显示输入缓冲区数据
		UINT64* param = (UINT64*)pIrp->AssociatedIrp.SystemBuffer;
		DbgPrint("r3 to r0:wbuf [%lld]\n", param[0]);

		//操作输出缓冲区
		param[0] = 0x3;

		//设置实际操作输出缓冲区长度
		info = cbout;
		break;
	}
	case IOCTL_GETPROCESS_NAME:
	{
		Handle_IOCTL_GETPROCESS_NAME(pIrp);
		return status;
	}


	case IOCTL_ADD_PID:
	{
		Handle_IOCTL_ADD_PID(pIrp);
		return status;
	}

	case IOCTL_REMOVE_PID:
	{
		Handle_IOCTL_REMOVE_PID(pIrp);
		return status;
	}
	case IOCTL_缓冲区读数据:
	{
		Handle_IOCTL_缓冲区读数据(pIrp);
		return status;
	}
	case IOCTL_缓冲区写数据:
	{
		Handle_IOCTL_缓冲区写数据(pIrp);
		return status;
	}
	case IOCTL_MDL读数据:
	{
		Handle_IOCTL_MDL读数据(pIrp);
		return status;
	}
	case IOCTL_MDL写数据:
	{
		Handle_IOCTL_MDL写数据(pIrp);
		return status;
	}

	case IOCTL_MDL写只读数据:
	{
		Handle_IOCTL_MDL写只读数据(pIrp);
		return status;
	}
	case IOCTL_CR0写只读数据:
	{
		Handle_IOCTL_CR0写只读数据(pIrp);
		return status;
	}
	case IOCTL_物理地址映射读数据:  
	{
		Handle_IOCTL_物理地址映射读数据(pIrp);
		return status;
	}
	case IOCTL_物理地址映射写数据:
	{
		Handle_IOCTL_物理地址映射写数据(pIrp);
		return status;
	}
	case IOCTL_Enum对象回调:
	{
		Handle_IOCTL_Enum对象回调(pIrp);
		return status;
	}
	case IOCTL_给指定pid提权:
	{
		Handle_IOCTL_给指定pid提权(pIrp);
		return status;
	}
	case IOCTL_移除指定pid提权:
	{
		Handle_IOCTL_移除指定pid提权(pIrp);
		return status;
	}
	
	default:
		status = STATUS_INVALID_VARIANT;
	}

	// 完成IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = info;	// bytes xfered
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	DbgPrint("sys:Leave HelloDDKDeviceIOControl\n");

	return status;
}


