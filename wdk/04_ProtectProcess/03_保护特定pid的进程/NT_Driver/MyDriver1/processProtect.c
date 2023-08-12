

#include "processProtect.h"
#include "handle.h"


OB_PREOP_CALLBACK_STATUS my_pre_callback(
	PVOID RegistrationContext,
	POB_PRE_OPERATION_INFORMATION OperationInformation
)
{

	if (OperationInformation->KernelHandle)
	{
		//�ں˴���
	}
	else
	{
		////�û�������� OpenProcess NtOpenProcess �Ľ����� OpenProcess
		UINT32 curPID = PsGetCurrentProcessId(); //��ȡ��ǰ����PID
		const char* ������ = GetProcessNameByPid2(curPID); //11����Ч���ַ�

		///Ŀ�����PID ��Ҫ������PID
		HANDLE dwProcessId = PsGetProcessId((PEPROCESS)OperationInformation->Object);
		const char* Ŀ������� = GetProcessNameByPid2(dwProcessId); //Ҫ���������������

		//�û���
		ACCESS_MASK oldȨ�� = OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess;
		ACCESS_MASK raw = oldȨ��;

		// �������
		switch (OperationInformation->Operation)
		{
		case OB_OPERATION_HANDLE_CREATE:
			// �����ض�PID�Ľ��̲�������̲���
			if (IsPidToProtect(dwProcessId))
			{
				DbgPrint("sys: curPID=%d ������=%s  Ŀ�����PID =%d  Ŀ�������=%s \n",
					curPID, ������, dwProcessId, Ŀ�������);
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

HANDLE gs_HandleCallback = NULL;//������ŷ��صľ��  �Է���ж�ض�Ӧ����
void InstallProcessProtect()
{
	OB_CALLBACK_REGISTRATION ob1_callback_reg = { 0 };
	OB_OPERATION_REGISTRATION ob2_operation = { 0 };
	//ob1 5��
	RtlInitUnicodeString(&ob1_callback_reg.Altitude, L"666666");
	ob1_callback_reg.RegistrationContext = NULL;
	ob1_callback_reg.Version = OB_FLT_REGISTRATION_VERSION;// ObGetFilterVersion(); //OB_FLT_REGISTRATION_VERSION
	ob1_callback_reg.OperationRegistrationCount = 1;//ֻע����һ���ص�pre
	ob1_callback_reg.OperationRegistration = &ob2_operation;
	//������ ��ʼ�� ob2_operation
	ob2_operation.ObjectType = PsProcessType; //OpenProcess OpenThread PsThreadType
	ob2_operation.Operations = OB_OPERATION_HANDLE_CREATE;  // ֻ����������Ĳ���
	ob2_operation.PostOperation = NULL;
	ob2_operation.PreOperation = my_pre_callback;
	ObRegisterCallbacks(&ob1_callback_reg, &gs_HandleCallback); // /INTEGRITYCHECK 
	DbgPrint("sys:��װ�ڴ汣�� gs_HandleCallback=%p\n", gs_HandleCallback);
}

void UnInstallProcessProtect()
{
	DbgPrint("sys:ж���ڴ汣��,gs_HandleCallback:%p \n", gs_HandleCallback);
	if (gs_HandleCallback)
	{
		ObUnRegisterCallbacks(gs_HandleCallback);
	}
	DbgPrint("sys:ж���ڴ汣������\n");
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
		if (�ܱ����Ľ���PID[i] == 0 || �ܱ����Ľ���PID[i] == pid)
		{
			//�ǿ�λ��
			�ܱ����Ľ���PID[i] = pid;
			DbgPrint("sys: ����ܱ�����PID =%d ", pid);
			break;
		}
	}
}

void RemovePidToProtect(UINT32 pid)
{
	for (size_t i = 0; i < 256; i++)
	{
		if (�ܱ����Ľ���PID[i] == pid) //��ȱ�ʾ�ҵ���
		{
			�ܱ����Ľ���PID[i] = 0;
			DbgPrint("sys: �Ƴ��ܱ�����PID =%d ", pid);
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
		if (�ܱ����Ľ���PID[i] == pid) // ��ȱ�ʾ�ҵ���
		{
			return TRUE;
		}
	}
	return FALSE;
}
