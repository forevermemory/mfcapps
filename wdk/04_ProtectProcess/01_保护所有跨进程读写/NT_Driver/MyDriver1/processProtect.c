#include "processProtect.h"


OB_PREOP_CALLBACK_STATUS my_pre_callback(
	PVOID RegistrationContext,
	POB_PRE_OPERATION_INFORMATION OperationInformation
)
{

	if (OperationInformation->KernelHandle)
	{
		//内核创建
	}
	else
	{
		////用户层调用了 OpenProcess NtOpenProcess 的进程名 OpenProcess
		//UINT32 curPID = PsGetCurrentProcessId(); //获取当前进程PID

		//用户层
		ACCESS_MASK old权限 = OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess;
		ACCESS_MASK raw = old权限;
		// 操作类别
		switch (OperationInformation->Operation)
		{
		case OB_OPERATION_HANDLE_CREATE:

			//排除 被终止权限
			old权限 &= ~PROCESS_TERMINATE;
			//排除 PROCESS_VM_READ 权限
			old权限 &= ~PROCESS_VM_READ;
			//排除掉 PROCESS_VM_WRITE
			old权限 &= ~PROCESS_VM_WRITE;

			//返回我们修改过的权限 OpenProcess
			OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = old权限;
			DbgPrint("sys:old权限=%X 新权限=%X\n", raw, old权限);

		case OB_OPERATION_HANDLE_DUPLICATE:
			break;
		default:
			break;
		}

	}

	return OB_PREOP_SUCCESS;
};

HANDLE gs_HandleCallback = NULL;//用来存放返回的句柄  以方便卸载对应功能
void InstallProcessProtect()
{
	OB_CALLBACK_REGISTRATION ob1_callback_reg = { 0 };
	OB_OPERATION_REGISTRATION ob2_operation = { 0 };
	//ob1 5项
	RtlInitUnicodeString(&ob1_callback_reg.Altitude, L"666666");
	ob1_callback_reg.RegistrationContext = NULL;
	ob1_callback_reg.Version = OB_FLT_REGISTRATION_VERSION;// ObGetFilterVersion(); //OB_FLT_REGISTRATION_VERSION
	ob1_callback_reg.OperationRegistrationCount = 1;//只注册了一个回调pre
	ob1_callback_reg.OperationRegistration = &ob2_operation;
	//接下来 初始化 ob2_operation
	ob2_operation.ObjectType = PsProcessType; //OpenProcess OpenThread PsThreadType
	ob2_operation.Operations = OB_OPERATION_HANDLE_CREATE;  // 只处理创建句柄的操作
	ob2_operation.PostOperation = NULL;
	ob2_operation.PreOperation = my_pre_callback;
	ObRegisterCallbacks(&ob1_callback_reg, &gs_HandleCallback); // /INTEGRITYCHECK 
	DbgPrint("sys:安装内存保护 gs_HandleCallback=%p\n", gs_HandleCallback);
}

void UnInstallProcessProtect()
{
	if (gs_HandleCallback)
	{
		ObUnRegisterCallbacks(gs_HandleCallback);
	}
	DbgPrint("sys:卸载内存保护\n");
}




BOOLEAN BypassCheckSign(PDRIVER_OBJECT pDriverObject)
{
#ifdef _WIN64
	typedef struct _KLDR_DATA_TABLE_ENTRY
	{
		LIST_ENTRY listEntry;
		ULONG64 __Undefined1;
		ULONG64 __Undefined2;
		ULONG64 __Undefined3;
		ULONG64 NonPagedDebugInfo;
		ULONG64 DllBase;
		ULONG64 EntryPoint;
		ULONG SizeOfImage;
		UNICODE_STRING path;
		UNICODE_STRING name;
		ULONG   Flags;
		USHORT  LoadCount;
		USHORT  __Undefined5;
		ULONG64 __Undefined6;
		ULONG   CheckSum;
		ULONG   __padding1;
		ULONG   TimeDateStamp;
		ULONG   __padding2;
	} KLDR_DATA_TABLE_ENTRY, * PKLDR_DATA_TABLE_ENTRY;
#else
	typedef struct _KLDR_DATA_TABLE_ENTRY
	{
		LIST_ENTRY listEntry;
		ULONG unknown1;
		ULONG unknown2;
		ULONG unknown3;
		ULONG unknown4;
		ULONG unknown5;
		ULONG unknown6;
		ULONG unknown7;
		UNICODE_STRING path;
		UNICODE_STRING name;
		ULONG   Flags;
	} KLDR_DATA_TABLE_ENTRY, * PKLDR_DATA_TABLE_ENTRY;
#endif
	PKLDR_DATA_TABLE_ENTRY pLdrData = (PKLDR_DATA_TABLE_ENTRY)pDriverObject->DriverSection;
	pLdrData->Flags = pLdrData->Flags | 0x20;
	return TRUE;
}