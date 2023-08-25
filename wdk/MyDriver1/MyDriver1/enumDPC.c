

#include "define.h"
#include "enumDpc.h"
//#include "enumSystemInfo.h"
#include "findCode.h"
#include "utils.h"



BOOLEAN FindKiWaitNeverAndKiWaitAlways(UINT64* never, UINT64* always)
{
	// 通过PsLoadedModuleList找到内核模块
	UNICODE_STRING usPsLoadedModuleList = RTL_CONSTANT_STRING(L"PsLoadedModuleList");
	PVOID PsLoadedListModule = MmGetSystemRoutineAddress(&usPsLoadedModuleList);
	if (NULL == PsLoadedListModule)
	{
		DbgPrint("获取PsLoadedListModule失败\n");
		return FALSE;
	}

	PLDR_DATA_TABLE_ENTRY Begin = (PLDR_DATA_TABLE_ENTRY)PsLoadedListModule;
	PLIST_ENTRY Head = NULL;
	PLIST_ENTRY Next = NULL;


	if (Begin == NULL)
		return FALSE;

	Head = (PLIST_ENTRY)Begin->InLoadOrderLinks.Flink;
	Next = Head->Flink;

	UNICODE_STRING ntoskrnl = RTL_CONSTANT_STRING(L"ntoskrnl.exe");

	do
	{
		PLDR_DATA_TABLE_ENTRY Entry = CONTAINING_RECORD(Next, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		Next = Next->Flink;

		// 找到内核模块基址
		LONG isEqual = RtlCompareUnicodeString(&(Entry->BaseDllName), &ntoskrnl, TRUE);
		DbgPrint("内核模块名称:%wZ, base:%llX [%d]\n", Entry->BaseDllName, Entry->DllBase, isEqual);
		if (isEqual == 0)
		{

			/*
			nt!KiSetTimerEx:
			fffff806`798bd280 48895c2408      mov     qword ptr [rsp+8],rbx
			fffff806`798bd285 48896c2410      mov     qword ptr [rsp+10h],rbp
			fffff806`798bd28a 4889742418      mov     qword ptr [rsp+18h],rsi
			fffff806`798bd28f 57              push    rdi
			fffff806`798bd290 4154            push    r12
			fffff806`798bd292 4155            push    r13
			fffff806`798bd294 4156            push    r14
			fffff806`798bd296 4157            push    r15
			fffff806`798bd298 4883ec50        sub     rsp,50h
			fffff806`798bd29c 488b0555945500  mov     rax,qword ptr [nt!KiWaitNever (fffff806`79e166f8)]
			fffff806`798bd2a3 488bf9          mov     rdi,rcx
			fffff806`798bd2a6 488b3533965500  mov     rsi,qword ptr [nt!KiWaitAlways (fffff806`79e168e0)]


			*/

			// 根据特征码进行搜索
			UCHAR code[] = { 0x48, 0x89, 0x5c, 0x24, 0x08, 0x48, 0x89, 0x6c, 0x24, 0x10, 0x48,
				0x89, 0x74, 0x24, 0x18
				, 0x57 , 0x41, 0x54 , 0x41, 0x55 , 0x41, 0x56 , 0x41, 0x57 , 
				0x48, 0x83, 0xec, 0x50 , 0x48, 0x8b, 0x05 };
			UINT64 startAddr = 0;
			NTSTATUS ret = SearchPattern(Entry->DllBase, Entry->SizeOfImage, code, sizeof(code), &startAddr);
			if (ret != STATUS_SUCCESS)
			{
				DbgPrint("SearchPattern失败\n");
				return 0;
			}
			//addr & 0xffffffff00000000 + (int*)(header + 0x23 + 0x3) + 7 + UINT32(addr)

			UINT32 offset = *((UINT32*)(startAddr + 0x1C + 0x3));
			*never = (startAddr & 0xffffffff00000000) | (offset + 0x7 + 0x1C + (UINT32)startAddr);

			offset = *((UINT32*)(startAddr + 0x26 + 0x3));
			*always = (startAddr & 0xffffffff00000000) | (offset + 0x7 + 0x26 + (UINT32)startAddr);

			return TRUE;
		}

	} while (Next != Head->Flink);

	return FALSE;
}


void EnumKernelDPC()
{

	UINT64 nerver = 0xfffff80679e166f8;
	UINT64 always = 0xfffff80679e168e0;
	// 搜索到地址 KiWaitNever KiWaitAlways
	// FindKiWaitNeverAndKiWaitAlways(&nerver, &always);

	//fffff806`798bd29c 488b0555945500  mov     rax, qword ptr[nt!KiWaitNever(fffff806`79e166f8)]
	//	fffff806`798bd2a3 488bf9          mov     rdi, rcx
	//	fffff806`798bd2a6 488b3533965500  mov     rsi, qword ptr[nt!KiWaitAlways(fffff806`79e168e0)]
		
	DbgPrint("sys: val: nerver:%llx, always:%llx\n", nerver, always);
	WCHAR* szDriverBaseName = ExAllocatePool(NonPagedPool, 600);
	if (szDriverBaseName == NULL)
		return ;
	RtlZeroMemory(szDriverBaseName, 600);

	int cpuNum = KeNumberProcessors;
	DbgPrint("sys: CPU核心数: %d \n", cpuNum);

	for (int i = 0; i < cpuNum; i++)
	{
		DbgPrint("sys: 正在遍历CPU: %d \n", cpuNum);

		// 线程绑定特定CPU
		KeSetSystemAffinityThread(i + 1);

		// 获得KPRCB的地址
		ULONG64 kPRCB = (ULONG64)__readmsr(0xC0000101) + 0x20; // fffff806`79333180
		DbgPrint("sys: kPRCB Addr: %llX \n", kPRCB);
		if (!MmIsAddressValid((PVOID64)kPRCB))
		{
			DbgPrint("sys: KPRCB地址非法: %llx \n", kPRCB);
			return ;
		}


		// 取消绑定CPU
		KeRevertToUserAffinityThread();
		//  dt _KPRCB fffff806`79333180 fffff806`79333180
		// Windows10 计算TimerTable在_KPRCB 结构中的偏移+0x3680
		
		PKTIMER_TABLE p_TimeTable = (PKTIMER_TABLE)(*(PULONG64)(kPRCB) + 0x3680);
		DbgPrint("sys: PKTIMER_TABLE: %llX \n", p_TimeTable); // fffff806`79336800
		// DbgBreakPoint();
		if (!MmIsAddressValid((PVOID64)p_TimeTable))
		{
			continue;
		}
		//DbgBreakPoint();

		KTIMER_TABLE_ENTRY timeTableEntry;
		// 遍历 TimerEntries[] 数组（大小 256）
		for (INT j = 0; j < 256; j++)
		{
			DbgPrint("sys: TimerEntries: %d \n", j);
			// 获取 Entry 双向链表地址
			
			timeTableEntry = p_TimeTable->TimerEntries[j];
			PLIST_ENTRY Head = &(timeTableEntry.Entry);
			PLIST_ENTRY Next = Head->Flink;

			do
			{
				PKTIMER p_Timer = CONTAINING_RECORD(Next, KTIMER, TimerListEntry);

				Next = Next->Flink;
				if (!MmIsAddressValid((PVOID64)Next))
				{
					continue;
				}

				// 获取解密前的Dpc对象
				if (!MmIsAddressValid((PVOID64)p_Timer))
				{
					continue;
				}

				ULONG64 ul_Dpc = (ULONG64)p_Timer->Dpc;
				INT i_Shift = (*((PULONG64)nerver) & 0xFF);

				// 解密 Dpc 对象
				ul_Dpc ^= *((ULONG_PTR*)nerver);			// 异或
				ul_Dpc = _rotl64(ul_Dpc, i_Shift);		// 循环左移
				ul_Dpc ^= (ULONG_PTR)p_Timer;			// 异或
				ul_Dpc = _byteswap_uint64(ul_Dpc);		// 颠倒顺序
				ul_Dpc ^= *((ULONG_PTR*)always);		// 异或

				// 对象类型转换
				PKDPC p_Dpc = (PKDPC)ul_Dpc;

				// 打印验证
				if (!MmIsAddressValid((PVOID64)p_Dpc))
				{
					continue;
				}

				ObGetDriverNameByPoint(p_Dpc->DeferredRoutine, szDriverBaseName);
				DbgPrint("sys:定时器对象：0x%p | 函数入口：0x%p | 触发周期: %d ,所属模块:%ws\n ",
					p_Timer, p_Dpc->DeferredRoutine, p_Timer->Period, szDriverBaseName);

			
			} while (Next != Head->Flink);
			}

	}
	
	ExFreePool(szDriverBaseName);
}



ULONG dwTimerCnt = 0;
KDPC stTimerDPC = { 0 };
KTIMER stTimerObj = { 0 };

VOID OnTimer(KDPC* Dpc, PVOID DeferredContext, PVOID SystemArgument1, PVOID SystemArgument2)
{
	dwTimerCnt += 1;
	DbgPrint("DpcTimer: %d\n", dwTimerCnt);
}



void InstallKernelDPC(PDEVICE_OBJECT pOurDevice)
{

	KeInitializeTimer(&stTimerObj);
	KeInitializeDpc(&stTimerDPC, OnTimer, pOurDevice);

	DbgPrint("IOCTL_START");
	LARGE_INTEGER stTimeCnt;
	dwTimerCnt = 0;
	stTimeCnt.HighPart |= -1;
	stTimeCnt.LowPart = -10000000;
	KeSetTimerEx(&stTimerObj, stTimeCnt, 1000, &stTimerDPC);
}