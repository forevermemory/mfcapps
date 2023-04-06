#include "pch.h"
#include "addr.h"

std::map<CStringW, PGoodDetail_554_������Ʒ> g_AllGoods;
std::vector<PPackageGood> g_Packages;
HANDLE h_Thread = 0;

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
		//OutputDebugStringA(str);
		//OutputDebugStringA(str);
		//OutputDebugStringA(str);
		// PrintRamdoms(o->arr, _size, ((o->size) >> 32));
		PrintRamdoms_����(o->arr, _size, ((o->size) >> 32) );
		
		str.Format("==================CODE:%llX========================end", o->code);
		//OutputDebugStringA(str);
		//OutputDebugStringA(str);
		//OutputDebugStringA(str);
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


	h_Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FindAllObjectsThread,
		(LPVOID)data, NULL, NULL);
	
}



// ������Ʒ�����׵�ַ
// ������Ʒ���鳤��
void PrintRamdoms(ULONG64 arr, ULONG64 size, ULONG64 code2)
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

		switch (code2)
		{
		case 0x61:
			detail = (PGoodDetail_0x61_ͼ��)gb->detail;
			
			if (detail == 0)
			{
				goto FAIL;
			}
			name64 =((PGoodDetail_0x61_ͼ��)detail)->name;
			desc64 = ((PGoodDetail_0x61_ͼ��)detail)->desc;

			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"-";
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
				_desc = L"-";
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
		case 0xf8:
			detail = (PGoodDetail_0xF8_����)gb->detail;
		
			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_0xF8_����)detail)->name;
			desc64 = ((PGoodDetail_0xF8_����)detail)->desc;
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"-";
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
				_desc = L"-";
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
		case 0x44:
			detail = (PGoodDetail_0x44_�����)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_0x44_�����)detail)->name;

			// �ַ���ֻȡ��һ��ָ�� ������ȡ������ָ��
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"-";
				_desc = L"-";
			}
			else
			{
				tmpName = (ULONG64*)(name64);
				if (tmpName)
				{
					_name = (PWCHAR)(name64);
				}
				_desc = L"";

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

		case 0x222:
			detail = (PGoodDetail_0x222_������)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_0x222_������)detail)->name;
			desc64 = name64;

			// �ַ���ֻȡ��һ��ָ�� ������ȡ������ָ��
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"-";
				_desc = L"-";
			}
			else
			{
				tmpName = (ULONG64*)(name64);
				if (tmpName)
				{
					_name = (PWCHAR)(name64);
				}
				_desc = L"-";

			}

			break;
		
			
		case 0x1c6:
			detail = (PGoodDetail_0x1c6_������)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_0x1c6_������)detail)->name;
			desc64 = ((PGoodDetail_0x1c6_������)detail)->desc;
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"-";
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
				_desc = L"-";
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

		case 0x15:
			
			detail = (PGoodDetail_0x15_���ͱ�)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_0x15_���ͱ�)detail)->name;
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"-";
			}
			else
			{
				tmpName = (ULONG64*)(name64);
				if (tmpName && *tmpName)
				{
					_name = (PWCHAR)((*(ULONG64*)(name64)));
				}

			}
			_desc = L"-";
			break;

		case 0x2aa:
			detail = (PGoodDetail_2aa_װ��2)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_2aa_װ��2)detail)->name;
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"-";
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
			
			if (desc64 < 0xffffffffff || desc64 > 0x3ffffffffff)
			{
				_desc = L"-";
			}
			else
			{
				tmpDesc = (ULONG64*)(desc64);
				if (tmpDesc)
				{
					_desc = (PWCHAR)(desc64); 			// �ַ���ֻȡ��һ��ָ�� ������ȡ������ָ��
				}
			}

			break;
			
		case 0xA:
			detail = (PGoodDetail_0xa_����)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_0xa_����)detail)->name;
			desc64 = ((PGoodDetail_0xa_����)detail)->desc;
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"-";
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
				_desc = L"-";
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
		case 0x554:
			detail = (PGoodDetail_554_������Ʒ)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_554_������Ʒ)detail)->name;
			desc64 = ((PGoodDetail_554_������Ʒ)detail)->desc;
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"-";
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
				_desc = L"-";
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
		
		default:
			_name = L"";
			_desc = L"";
			break;
		}
		

		//str.Format("enter: ARR=%llX  gb=%p detail=%p, name:%llX, desc:%llX",
		//	arr, gb, detail, name64, desc64);
		//OutputDebugStringA(str);

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








// ������Ʒ�����׵�ַ
// ������Ʒ���鳤��
void PrintRamdoms_����(ULONG64 arr, ULONG64 size, ULONG64 code2)
{

	ULONG64* goodsAddr = (ULONG64*)arr;
	CString str;
	str.Format("arr=%llX goodsAddr:%llX ", arr, goodsAddr);
	OutputDebugStringA(str);


	wchar_t buf[1024] = { 0 };

	GoodBase* gb = NULL;
	gb = (GoodBase*)goodsAddr;
	//gb++;

	int total = 1;

	PVOID detail = NULL;
	ULONG64 name64 = 0;
	ULONG64 desc64 = 0;
	ULONG64 id64 = 0;

	
	str.Format("gb=%p", gb);
	OutputDebugStringA(str);

	while (total < size)
	{
		if ((ULONG64)gb < 0xffffffff || (ULONG64)gb > 0x3ffffffffff)
		{
			goto FAIL;
		}
		if ((ULONG64)gb->detail < 0xffffffff || (ULONG64)gb->detail > 0x3ffffffffff)
		{
			goto FAIL;
		}

		ULONG64* tmpName = 0;
		ULONG64* tmpDesc = 0;
		ULONG64* tmpID = 0;
		PWCHAR _name = 0;
		PWCHAR _desc = 0;
		PWCHAR _id = 0;

		switch (code2)
		{
		
		case 0xf8:
			detail = (PGoodDetail_0xF8_����)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_0xF8_����)detail)->name;
			desc64 = ((PGoodDetail_0xF8_����)detail)->desc;
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"-";
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
				_desc = L"-";
			}
			else
			{
				tmpDesc = (ULONG64*)(desc64);
				if (tmpDesc && *tmpDesc)
				{
					_desc = (PWCHAR)((*(ULONG64*)(desc64)));
				}
			}
			_id = L"-";
			break;
		

		case 0x2aa:
			detail = (PGoodDetail_2aa_װ��2)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_2aa_װ��2)detail)->name;
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"-";
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

			if (desc64 < 0xffffffffff || desc64 > 0x3ffffffffff)
			{
				_desc = L"-";
			}
			else
			{
				tmpDesc = (ULONG64*)(desc64);
				if (tmpDesc)
				{
					_desc = (PWCHAR)(desc64); 			// �ַ���ֻȡ��һ��ָ�� ������ȡ������ָ��
				}
			}
			_id = L"-";
			break;

		case 0xA:
			detail = (PGoodDetail_0xa_����)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			name64 = ((PGoodDetail_0xa_����)detail)->name;
			desc64 = ((PGoodDetail_0xa_����)detail)->desc;
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"-";
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
				_desc = L"-";
			}
			else
			{
				tmpName = (ULONG64*)(name64);
				if (tmpDesc && *tmpDesc)
				{
					_desc = (PWCHAR)((*(ULONG64*)(desc64)));
				}
			}
			_id = L"-";
			break;

		
		case 0x554:
			detail = (PGoodDetail_554_������Ʒ)gb->detail;

			if (detail == 0)
			{
				goto FAIL;
			}
			id64 = ((PGoodDetail_554_������Ʒ)detail)->id;
			name64 = ((PGoodDetail_554_������Ʒ)detail)->name;
			desc64 = ((PGoodDetail_554_������Ʒ)detail)->desc;
			if (name64 < 0xffffffffff || name64 > 0x3ffffffffff)
			{
				_name = L"-";
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
				_desc = L"-";
			}
			else
			{
				tmpDesc = (ULONG64*)(desc64);
				if (tmpDesc && *tmpDesc)
				{
					_desc = (PWCHAR)((*(ULONG64*)(desc64)));
				}
			}
			// id
			if (id64 < 0xffffffffff || id64 > 0x3ffffffffff)
			{
				_id = L"-";
			}
			else
			{
				tmpID = (ULONG64*)(id64);
				if (tmpID)
				{
					_id = (PWCHAR)(id64);
				}
			}

			g_AllGoods[CStringW(_id)] = (PGoodDetail_554_������Ʒ)detail;
			
			break;

		default:
			goto FAIL;
			_name = L"-";
			_desc = L"-";
			break;
		}


		//str.Format("enter: ARR=%llX  gb=%p detail=%p, name:%llX, desc:%llX",
		//	arr, gb, detail, name64, desc64);
		//OutputDebugStringA(str);

		//wsprintfW(buf, L"���:%d,���1:%X, ��Ʒ����:%ls, ����: %ls, ����: %ls",
		//	total, gb->number,_id, _name, _desc);
		//OutputDebugStringW(buf);

	FAIL:

		memset(buf, 0, 1024);
		gb++;
		total++;
		name64 = 0;
		desc64 = 0;

	}

	//OutputDebugStringA("ִ�����.......");
}


void PrintAllGoods()
{

	OutputDebugStringA("xxxxxxxxxxxxxxxSTART WAITINGxxxxxxxxxxxxxxxxxxx");

	WaitForSingleObject(h_Thread, INFINITE);

	CStringW str;
	str.Format(L"g_AllGoods total len: %lld", g_AllGoods.size());
	OutputDebugStringW(str);



	CStringW _name;
	for (std::map<CStringW, PGoodDetail_554_������Ʒ>::iterator it = g_AllGoods.begin(); it != g_AllGoods.end(); it++)
	{
		if (it->second->name < 0xffffffffff || it->second->name > 0x3ffffffffff)
		{
			_name = L"-";
		}
		else
		{
			_name = (PWCHAR)((*(ULONG64*)(it->second->name)));
		}

		//str.Format(L"����: %ls\t, %ls", it->first, _name);
		//OutputDebugStringW(str);
	}


	OutputDebugStringA("xxxxxxxxxxxxxxxENDxxxxxxxxxxxxxxxxxxx");
}