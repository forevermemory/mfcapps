#include "pch.h"
#include "addr.h"
#include <afxstr.h>
#include <vector>





DWORD WINAPI FindAllObjectsThread(LPVOID pParam)
{
	
	PCodeThread param = (PCodeThread)pParam;
	CString str;
	str.Format("start find=======%d", param->length);
	OutputDebugStringA(str);

	//for (int i = 0; i < 8; i++)
	//{
	//	str.Format("%02x,", param->code[i]);
	//	OutputDebugStringA(str);
	//}



	//FindAllObjectsWithCharCode(param->code, 8);
	FindAllObjectsULONG64(*(PULONG64)(param->code));

	OutputDebugStringA("stop find=======");

	return 0;


}



void FindAllObjectsULONG64(ULONG64 code)
{
	std::vector<ULONG64> results ;
	ULONG64 total = 0;

	CString str;
	str.Format("start find=======%x", code);
	OutputDebugStringA(str);



	BYTE* dataBuffer = NULL;
	BOOL readReturn = 0;

	// ��ȡ PageSize �͵�ַ����
	SYSTEM_INFO sysInfo = { 0 };
	GetSystemInfo(&sysInfo);
	char* p = (char*)sysInfo.lpMinimumApplicationAddress;
	MEMORY_BASIC_INFORMATION  memInfo = { 0 };

	//	for (int i = 0; i < 8; i++)
	//{
	//	str.Format("%02x,", code[i]);
	//	OutputDebugStringA(str);
	//}


	while (p < sysInfo.lpMaximumApplicationAddress) {
		size_t size = VirtualQuery(
			p,										// Ҫ��ѯ�ڴ��Ļ���ַָ��
			&memInfo,								// �����ڴ����Ϣ�� MEMORY_BASIC_INFORMATION ����
			sizeof(MEMORY_BASIC_INFORMATION64)		// ��������С
		);
		if (size != sizeof(MEMORY_BASIC_INFORMATION64)) { break; }

		if (memInfo.Type == MEM_PRIVATE && !(memInfo.Protect & PAGE_NOACCESS))
		{
			//str.Format("find addr: %p, length:%x", p, memInfo.RegionSize);
			//OutputDebugStringA(str);

			//// ���� memInfo.RegionSize
			dataBuffer = (BYTE*)malloc(memInfo.RegionSize);
			memset(dataBuffer, 0, memInfo.RegionSize);

			//DWORD old;
			//VirtualProtect((LPVOID)p, memInfo.RegionSize, PAGE_EXECUTE_READWRITE, &old);
			SIZE_T rd;
			ReadProcessMemory(GetCurrentProcess(), p, dataBuffer, memInfo.RegionSize, &rd);
			//VirtualProtect((LPVOID)p, memInfo.RegionSize, old, &old);

			size_t length = memInfo.RegionSize;
			//str.Format("find addr: %p, length:%x, rd:%X", p, length, rd);
			//OutputDebugStringA(str);

			PULONG64 ptr = (PULONG64)dataBuffer;

			for (int i = 0; i < length / 8; i++)
			{
				if(ptr == 0)
				{
					continue;
				}
				if (*ptr == code)
				{
					str.Format("\t\t find addr: %llX", ((ULONG64)p + i * 8));
					OutputDebugStringA(str);
					total++;
					results.push_back(((ULONG64)p + i * 8));

				}
				ptr++;
			}


			free(dataBuffer);
		}

		p += memInfo.RegionSize;
	}
	str.Format("ɨ����ɹ��ҵ�%d����¼, results:%d", total, results.size());
	OutputDebugStringA(str);

	// ��ʼ����
	
	std::vector<ULONG64>::iterator it1;
	for (it1 = results.begin(); it1 != results.end(); ++it1) {
		PBigObject o = (PBigObject)*it1 ;
		str.Format("addr:%p code:%llx, arr:%llx, code2:%llX size:%llX",
			o, o->code, o->arr, ((o->size) >> 32) , (o->size) & 0xffffffff);
		OutputDebugStringA(str);

		ULONG64 _size = (o->size) & 0xffffffff;
		if (_size > 0x1000 || _size< 0x3 )
		{
			continue;
		}
		str.Format("==================CODE:%llX========================start", o->code);
		OutputDebugStringA(str);
		OutputDebugStringA(str);
		OutputDebugStringA(str);
		PrintRamdoms(o->arr, _size, o->code);
		str.Format("==================CODE:%llX========================end", o->code);
		OutputDebugStringA(str);
		OutputDebugStringA(str);
		OutputDebugStringA(str);

	}
}


void FindAllObjectsWithCharCode(PUCHAR code, ULONG64 codeLength)
{
	CString str;


	BYTE* dataBuffer = NULL;
	BOOL readReturn = 0;

	// ��ȡ PageSize �͵�ַ����
	SYSTEM_INFO sysInfo = { 0 };
	GetSystemInfo(&sysInfo);
	char* p = (char*)sysInfo.lpMinimumApplicationAddress;
	MEMORY_BASIC_INFORMATION  memInfo = { 0 };

//	for (int i = 0; i < 8; i++)
//{
//	str.Format("%02x,", code[i]);
//	OutputDebugStringA(str);
//}


	while (p < sysInfo.lpMaximumApplicationAddress) {
		size_t size = VirtualQuery(
			p,										// Ҫ��ѯ�ڴ��Ļ���ַָ��
			&memInfo,								// �����ڴ����Ϣ�� MEMORY_BASIC_INFORMATION ����
			sizeof(MEMORY_BASIC_INFORMATION64)		// ��������С
		);
		if (size != sizeof(MEMORY_BASIC_INFORMATION64)) { break; }

		if (memInfo.Type == MEM_PRIVATE && !(memInfo.Protect & PAGE_NOACCESS))
		{
			

			//// ���� memInfo.RegionSize
			dataBuffer = (BYTE*)malloc(memInfo.RegionSize);
			memset(dataBuffer, 0, memInfo.RegionSize);

			DWORD old;
			VirtualProtect((LPVOID)p, memInfo.RegionSize, PAGE_EXECUTE_READWRITE, &old);
			SIZE_T rd;
			ReadProcessMemory(GetCurrentProcess(), p, dataBuffer, memInfo.RegionSize, &rd);
			VirtualProtect((LPVOID)p, memInfo.RegionSize, old, &old);

			size_t length = memInfo.RegionSize;
			//str.Format("find addr: %p, length:%x, rd:%X", p, length, rd);
			//OutputDebugStringA(str);

			/*
			for (int i = 0; i < length; i++)
			{
				BOOLEAN found = TRUE;
				for (int j = 0; j < codeLength; j++)
				{
					if (code[j] != dataBuffer[i + j])
					{
						found = FALSE;
						break;
					}

				}

				if (found)
				{
					str.Format("\t\t find addr: %llX", ((ULONG64)p + i));
					OutputDebugStringA(str);
					
				}
			}
			*/

			free(dataBuffer);
		}

		p += memInfo.RegionSize;
	}

}

void FindAllObjects(ULONG64 num)
{
	CString str;
	str.Format("���������Ϊ:%llX", num);
	OutputDebugStringA(str);

	UCHAR code[8] = { 0 };
	for (int i = 7; i >= 0; i--) {
		code[i] = ((num >> (8 * i)) & 0xff);
	}

	CodeThread * data =(CodeThread*) malloc(sizeof(CodeThread));
	memcpy(data->code, code, 8);
	data->length = 8;


	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FindAllObjectsThread,
		(LPVOID)data, NULL, NULL);
	
}

void PrintStructs(ULONG64 base)
{
	OutputDebugStringA("ִ�п�ʼ.......");

	// ������Ʒ�����׵�ַ
	ULONG64* goodsAddr = (ULONG64*)(base + 0x30);
	ULONG32  goodsNumVal = *(PULONG32)(base + 0x38);

	OutputDebugStringA("ת�����.......");
	// ������Ʒ����ֵ
	ULONG64 goodsVal = *goodsAddr;



	CString str;
	str.Format("goodsAddr:%llX, val:%llX,  numval:%x", goodsAddr, goodsVal, goodsNumVal);
	OutputDebugString(str);



	wchar_t buf[1024] = { 0 };

	GoodBase* gb = NULL;
	gb = (GoodBase*)goodsVal;
	gb++;

	str.Format("gb:%llX", gb);
	OutputDebugString(str);

	int total = 1;
	PStructDetail detail;
	while (total < goodsNumVal)
	{

		detail = (PStructDetail)gb->detail;

		if (detail->name && detail->desc)
		{
			wsprintfW(buf, L"%d ���:%d, ����: %ls , ����: %ls ",
				total, gb->number, (PWCHAR)(*(ULONG64*)(detail->name)), (PWCHAR)(*(ULONG64*)(detail->desc)));
		}
		else
		{
			wsprintfW(buf, L"%d =========================================================", total);
		}


		OutputDebugStringW(buf);
		memset(buf, 0, 1024);

		gb++;
		total++;
	}

	OutputDebugStringA("ִ�����.......");
}

void PrintPackages(ULONG64 base)
{
	OutputDebugStringA("ִ�п�ʼ.......");

	// ������Ʒ�����׵�ַ
	ULONG64* goodsAddr = (ULONG64*)(base+0x30);
	ULONG32  goodsNumVal = *(PULONG32)(base+0x38);

	OutputDebugStringA("ת�����.......");
	// ������Ʒ����ֵ
	ULONG64 goodsVal = *goodsAddr;


	/*
		[
			{
				���
				��Ʒ��ַ
				δ֪
			}
		]
	*/

	CString str;
	str.Format("goodsAddr:%llX, val:%llX,  numval:%x", goodsAddr, goodsVal, goodsNumVal);
	OutputDebugString(str);



	wchar_t buf[1024] = { 0 };

	GoodBase* gb = NULL;
	gb = (GoodBase*)goodsVal;
	gb++;

	str.Format("gb:%llX", gb);
	OutputDebugString(str);

	int total = 1;
	PGoodDetail detail;
	while (total< goodsNumVal)
	{
		
		detail = (PGoodDetail)gb->detail;

		if (detail->name && detail->desc)
		{
			wsprintfW(buf, L"%d ���:%d, ����: %ls , ����: %ls ",
				total, gb->number, (PWCHAR)(*(ULONG64*)(detail->name)), (PWCHAR)(*(ULONG64*)(detail->desc)));
		}
		else
		{
			wsprintfW(buf, L"%d =========================================================",total);
		}


		OutputDebugStringW(buf);
		memset(buf, 0, 1024);

		gb++;
		total++;
	}

	OutputDebugStringA("ִ�����.......");


}


// ������Ʒ�����׵�ַ
// ������Ʒ���鳤��
void PrintRamdoms(ULONG64 arr, ULONG64 size, ULONG64 code)
{
	
	ULONG64* goodsAddr = (ULONG64*)arr;


	CString str;
	str.Format("arr=%llX goodsAddr:%llX ", arr, goodsAddr);
	OutputDebugStringA(str);



	wchar_t buf[1024] = { 0 };

	GoodBase* gb = NULL;
	gb = (GoodBase*)goodsAddr;
	gb++;

	int total = 1;
	//PStructDetail detail = NULL;

	PVOID detail = NULL;
	ULONG64 name64 = 0;
	ULONG64 desc64 = 0;
	str.Format(" gb=%p" ,  gb );
	OutputDebugStringA(str);

	while (total < size)
	{
		if ((ULONG64)gb < 0xffffffff || (ULONG64)gb > 0x3ffffffffff)
		{
			goto FAIL;
		}
		if ((ULONG64)gb->detail <0xffffffff || (ULONG64)gb->detail > 0x3ffffffffff)
		{
			goto FAIL;
		}

		ULONG64* tmpName = 0; 
		ULONG64* tmpDesc = 0; 
		PWCHAR _name = 0;
		PWCHAR _desc = 0;

		switch (code)
		{
		case 0xA4947:
			detail = (PGoodDetail_A4947_ͼ��)gb->detail;
			
			if (detail == 0)
			{
				goto FAIL;
			}
			name64 =((PGoodDetail_A4947_ͼ��)detail)->name;
			desc64 = ((PGoodDetail_A4947_ͼ��)detail)->desc;

			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"NAME";
			}
			else
			{
				tmpName = (ULONG64*)(name64);
				if (tmpName && *tmpName)
				{
					_name = (PWCHAR)((*(ULONG64*)(name64)));
				}
				
			}
			if (desc64 < 0xffffffffff || desc64 > 0x3ffffffffff)
			{
				_desc = L"DESC";
			}
			else
			{
				tmpDesc = (ULONG64*)(desc64);
				if (tmpDesc && *tmpDesc)
				{
					_desc = (PWCHAR)((*(ULONG64*)(desc64)));
				}
			}

			
			break;
		case 0xa4394:
			detail = (PGoodDetail_A44394_����)gb->detail;
		
			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_A44394_����)detail)->name;
			desc64 = ((PGoodDetail_A44394_����)detail)->desc;
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"NAME";
			}
			else
			{
				tmpName = (ULONG64*)(name64);
				if (tmpName && *tmpName)
				{
					_name = (PWCHAR)((*(ULONG64*)(name64)));
				}
				
			}
			if (desc64 < 0xffffffffff || desc64 > 0x3ffffffffff)
			{
				_desc = L"DESC";
			}
			else
			{
				tmpDesc = (ULONG64*)(desc64);
				if (tmpDesc && *tmpDesc)
				{
					_desc = (PWCHAR)((*(ULONG64*)(desc64)));
				}
			}
			break;
		case 0x15C50E:
			detail = (PGoodDetail_15C50E_�����)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_15C50E_�����)detail)->name;
			desc64 = name64;

			// �ַ���ֻȡ��һ��ָ�� ������ȡ������ָ��
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"NAME";
				_desc = L"DESC";
			}
			else
			{
				tmpName = (ULONG64*)(name64);
				if (tmpName)
				{
					_name = (PWCHAR)(name64);
				}
				_desc = L"DESC";

			}
			

			break;
		case 0x15C0AE:
			detail = (PGoodDetail_15C0AE_������)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_15C0AE_������)detail)->name;
			desc64 = name64;

			// �ַ���ֻȡ��һ��ָ�� ������ȡ������ָ��
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"NAME";
				_desc = L"DESC";
			}
			else
			{
				tmpName = (ULONG64*)(name64);
				if (tmpName)
				{
					_name = (PWCHAR)(name64);
				}
				_desc = L"DESC";

			}


			break;

		case 0x15C102:
			detail = (PGoodDetail_15C102_������)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_15C102_������)detail)->name;
			desc64 = name64;

			// �ַ���ֻȡ��һ��ָ�� ������ȡ������ָ��
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"NAME";
				_desc = L"DESC";
			}
			else
			{
				tmpName = (ULONG64*)(name64);
				if (tmpName)
				{
					_name = (PWCHAR)(name64);
				}
				_desc = L"DESC";

			}

			break;
		case 0xA4B16:
			detail = (PGoodDetail_A4B16_����)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_A4B16_����)detail)->name;
			desc64 = name64;

			// �ַ���ֻȡ��һ��ָ�� ������ȡ������ָ��
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"NAME";
				_desc = L"DESC";
			}
			else
			{
				tmpName = (ULONG64*)(name64);
				if (tmpName)
				{
					_name = (PWCHAR)(name64);
				}
				_desc = L"DESC";

			}

			break;
			
		case 0x26E292:
			detail = (PGoodDetail_26E292_������)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_26E292_������)detail)->name;
			desc64 = ((PGoodDetail_26E292_������)detail)->desc;
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"NAME";
			}
			else
			{
				tmpName = (ULONG64*)(name64);
				if (tmpName && *tmpName)
				{
					_name = (PWCHAR)((*(ULONG64*)(name64)));
				}
				
			}
			if (desc64 < 0xffffffffff || desc64 > 0x3ffffffffff)
			{
				_desc = L"DESC";
			}
			else
			{
				
				tmpDesc = (ULONG64*)(desc64);
				if (tmpDesc && *tmpDesc)
				{
					_desc = (PWCHAR)((*(ULONG64*)(desc64)));
				}
			}
			break;
		case 0x272CA3:
			detail = (PGoodDetail_272CA3_�Ҿ�)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_272CA3_�Ҿ�)detail)->name;
			_desc = L"DESC";
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"NAME";
			;
			}
			else
			{
				tmpName = (ULONG64*)(name64);
				if (tmpName && *tmpName)
				{
					_name = (PWCHAR)((*(ULONG64*)(name64)));
				}

			}
			
			break;
			
		case 0x9C31E:
			detail = (PGoodDetail_C31E_����)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_C31E_����)detail)->name;
			desc64 = ((PGoodDetail_C31E_����)detail)->desc;
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"NAME";
			}
			else
			{

				if (tmpName && *tmpName)
				{
					_name = (PWCHAR)((*(ULONG64*)(name64)));
				}
				tmpDesc = (ULONG64*)(desc64);
			}
			if (desc64 < 0xffffffffff || desc64 > 0x3ffffffffff)
			{
				_desc = L"DESC";
			}
			else
			{
				tmpName = (ULONG64*)(name64);
				if (tmpDesc && *tmpDesc)
				{
					_desc = (PWCHAR)((*(ULONG64*)(desc64)));
				}
			}
			break;

		case 0x9FB86:

			detail = (PGoodDetail_9FB86_����)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_9FB86_����)detail)->name;
			
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"NAME";
				_desc = L"DESC";
			}
			else
			{

				if (tmpName && *tmpName)
				{
					_name = (PWCHAR)((*(ULONG64*)(name64)));
				}
				tmpDesc = (ULONG64*)(desc64);
				_desc = L"DESC";

			}
			
			
			break;
		case 0xA4718:
			detail = (PGoodDetail_A4718_������Ʒ)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_A4718_������Ʒ)detail)->name;
			desc64 = ((PGoodDetail_A4718_������Ʒ)detail)->desc;
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"NAME";
			}
			else
			{
				tmpName = (ULONG64*)(name64);
				if (tmpName && *tmpName)
				{
					
					_name = (PWCHAR)((*(ULONG64*)(name64)));
				}
				
			}
			if (desc64 < 0xffffffffff || desc64 > 0x3ffffffffff)
			{
				_desc = L"DESC";
			}
			else
			{
				tmpDesc = (ULONG64*)(desc64);
				if (tmpDesc && *tmpDesc)
				{
					_desc = (PWCHAR)((*(ULONG64*)(desc64)));
				}
			}
			break;
		case 0xA4798:
			_name = (PWCHAR)(gb->detail);
			_desc = L"0xA4798";
		case 0xA4D64:
			_name = L"A4D64";
			_desc = L"A4D64";
			break;
		case 0xA4D27:
			_name = L"A4D27";
			_desc = L"A4D27";
			break;

		case 0xA4458:
			_name = L"A4458";
			_desc = L"A4458";
			break;
		case 0x15C1B7:
			_name = L"15C1B7";
			_desc = L"15C1B7";
			break;
		case 0xA4511:
			_name = L"0xA4511";
			_desc = L"0xA4511";
			break;
		case 0xA42E9:
			_name = L"A42E9";
			_desc = L"A42E9";
			break;
		case 0x2786E3:
			_name = L"2786E3";
			_desc = L"2786E3";
			break;
		case 0x286A71:
			_name = L"0x286A71";
			_desc = L"0x286A71";
			break;
		case 0xA4984:
			_name = L"A4984";
			_desc = L"A4984";
			break;
		case 0xA4843:
			_name = L"A4843";
			_desc = L"A4843";
			break;
		case 0x15C4C7:
			// Ӧ���Ƕ�ȡ�������ļ��б�
			_name = L"15C4C7";
			_desc = L"15C4C7";
			break;
			
		default:
			break;
		}
		

		str.Format("enter: ARR=%llX  gb=%p detail=%p, name:%llX, desc:%llX",
			arr, gb, detail, name64, desc64);
		OutputDebugStringA(str);

		wsprintfW(buf, L"%d ���:%X, ����: %ls , ����: %ls ",
			total, gb->number, _name, _desc);
		OutputDebugStringW(buf);

	FAIL:
		//OutputDebugStringA("faillll_111111111");
		memset(buf, 0, 1024);
		gb++;
		total++;
		name64 = 0;
		desc64 = 0;
	
	}

	OutputDebugStringA("ִ�����.......");
}
