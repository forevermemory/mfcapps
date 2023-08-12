

#include "processProtect.h"
#include "handle.h"


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
		UINT32 curPID = PsGetCurrentProcessId(); //获取当前进程PID
		const char* 进程名 = GetProcessNameByPid2(curPID); //11个有效的字符

		///目标进程PID 想要保护的PID
		HANDLE dwProcessId = PsGetProcessId((PEPROCESS)OperationInformation->Object);
		const char* 目标进程名 = GetProcessNameByPid2(dwProcessId); //要保护是这个进程名

		//用户层
		ACCESS_MASK old权限 = OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess;
		ACCESS_MASK raw = old权限;

		// 操作类别
		switch (OperationInformation->Operation)
		{
		case OB_OPERATION_HANDLE_CREATE:
			// 保护特定PID的进程不被跨进程操作
			if (IsPidToProtect(dwProcessId))
			{
				DbgPrint("sys: curPID=%d 进程名=%s  目标进程PID =%d  目标进程名=%s \n",
					curPID, 进程名, dwProcessId, 目标进程名);
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = 0;
			}

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
	DbgPrint("sys:卸载内存保护,gs_HandleCallback:%p \n", gs_HandleCallback);
	if (gs_HandleCallback)
	{
		ObUnRegisterCallbacks(gs_HandleCallback);
	}
	DbgPrint("sys:卸载内存保护结束\n");
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






void AddPidToProtect(UINT32 pid)
{
	for (size_t i = 0; i < 256; i++)
	{
		if (受保护的进程PID[i] == 0 || 受保护的进程PID[i] == pid)
		{
			//是空位置
			受保护的进程PID[i] = pid;
			DbgPrint("sys: 添加受保护的PID =%d ", pid);
			break;
		}
	}
}

void RemovePidToProtect(UINT32 pid)
{
	for (size_t i = 0; i < 256; i++)
	{
		if (受保护的进程PID[i] == pid) //相等表示找到了
		{
			受保护的进程PID[i] = 0;
			DbgPrint("sys: 移出受保护的PID =%d ", pid);
			//break;
		}
	}
	return;
}

BOOLEAN IsPidToProtect(UINT32 pid)
{
	if (pid == 0) return 0;

	for (size_t i = 0; i < 256; i++)
	{
		if (受保护的进程PID[i] == pid) // 相等表示找到了
		{
			return TRUE;
		}
	}
	return FALSE;
}
