#include <stdio.h>

//"\x55\x8B\xEC\x60\x83\xEC\x20\x83\xC4\x18\x68\x6C\x64\x00\x00\x68\x6F\x77\x6F\x72\x68\x68\x65\x6C\x6C\x68\x6C\x6C\x00\x00\x68\x33\x32\x2E\x64\x68\x75\x73\x65\x72\x8B\xDC\x68\xB2\x1A\x87\x84\xE8\x9E\x00\x00\x00\x8B\xF0\x68\x87\x32\xD8\xC0\x50\xE8\x28\x00\x00\x00\x6A\x08\x6A\x00\x53\x8B\xD0\xFF\xD2\x68\x6A\x0A\x38\x1E\x50\xE8\x14\x00\x00\x00\x6A\x00\x6A\x00\x83\xC3\x0C\x53\x6A\x00\xFF\xD0\x83\xC4\x20\x61\x8B\xE5\x5D\xC3\x55\x8B\xEC\x83\xEC\x1C\x53\x51\x52\x56\x8B\x55\x08\x8B\x72\x3C\x8D\x34\x16\x8B\x76\x78\x8D\x34\x16\x8B\x7E\x1C\x8D\x3C\x17\x89\x7D\xFC\x8B\x7E\x20\x8D\x3C\x17\x89\x7D\xF8\x8B\x7E\x24\x8D\x3C\x17\x89\x7D\xF4\x33\xC9\x8B\x75\xF8\x8B\x34\x8E\x8D\x34\x32\x56\xE8\x5A\x00\x00\x00\x3B\x45\x0C\x74\x03\x41\xEB\xE9\x8B\x75\xF4\x33\xFF\x66\x8B\x3C\x4E\x8B\x5D\xFC\x8B\x34\xBB\x8D\x04\x16\x5E\x5A\x59\x5B\x8B\xE5\x5D\xC2\x08\x00\x55\x8B\xEC\x83\xEC\x04\x53\x51\x52\x56\x64\x8B\x35\x30\x00\x00\x00\x8B\x76\x0C\x8B\x76\x1C\x8B\x36\x8B\x5E\x20\x53\xE8\x5A\x00\x00\x00\x3B\x45\x08\x74\x02\xEB\xEE\x8B\x46\x08\x5E\x5A\x59\x5B\x8B\xE5\x5D\xC2\x04\x00\x55\x8B\xEC\x83\xEC\x04\xC7\x45\xFC\x00\x00\x00\x00\x53\x51\x52\x56\x8B\x75\x08\x33\xC9\x33\xD2\x33\xDB\x33\xC0\x8A\x04\x0E\x84\xC0\x74\x16\x8B\x5D\xFC\x8B\x55\xFC\xC1\xE2\x19\xC1\xEB\x07\x0B\xD3\x03\xD0\x41\x89\x55\xFC\xEB\xE1\x5E\x5A\x59\x5B\x8B\x45\xFC\x8B\xE5\x5D\xC2\x04\x00\x55\x8B\xEC\x51\x36\xC7\x45\xFC\x00\x00\x00\x00\x36\x8B\x45\x08\x3E\x0F\xB7\x08\x85\xC9\x74\x31\x36\x8B\x55\xFC\xC1\xE2\x19\x36\x8B\x45\xFC\xC1\xE8\x07\x0B\xD0\x36\x89\x55\xFC\x36\x8B\x4D\x08\x3E\x0F\xB7\x11\x36\x03\x55\xFC\x36\x89\x55\xFC\x36\x8B\x45\x08\x83\xC0\x02\x36\x89\x45\x08\xEB\xC3\x36\x8B\x45\xFC\x8B\xE5\x5D\xC2\x04\x00"

void __declspec(naked) helloShellcode()
{
	__asm
	{
		push ebp
		mov ebp, esp
		pushad
		sub esp, 0x20 // ��������ռ� 32���ֽ�

		// �ַ�����ջ
		// user32.dll  75 73 65 72  33 32 2E 64  6C 6C 00    ����0x0B
		// helloworld  68 65 6C 6C  6F 77 6F 72  6C 64 00    ����0x0B
		add esp, 0x18
		push  0x646c
		push 0x726f776f
		push 0x6c6c6568

		push 0x6c6c
		push 0x642e3233
		push 0x72657375
		mov ebx, esp // ebx �����ַ����׵�ַ


		// �ҵ�kernelbaseģ���ַ
		push 0x84871AB2 // KERNELBASE.dll  ��wchar_t��hashֵ
		call func_getDll_BaseAddr
		mov esi, eax // �ݴ�KERNELBASE.dllģ���ַ

		// �ҵ� LoadLibraryExA��ַ
		push 0xC0D83287 // LoadLibraryExA  char C0D83287
		push eax
		call func_GetProcAddress

		push 0x8
		push 0
		push ebx // ��Ҫ���ص�dll����
		mov edx, eax
		call edx


		// �Ѿ�����user32.dll ��user32.dll�ҵ�MessageBoxA
		push 0x1E380A6A // MessageBoxA  1E380A6A
		push eax
		call func_GetProcAddress

		// ���� MessageBoxA
		push 0
		push 0
		add ebx, 0x0c
		push ebx
		push 0
		call eax

		//// �˳����� ExitProcess
		//push 0x4FD18963 // ExitProcess	4FD18963
		//push esi
		//call func_GetProcAddress
		//push 0
		//call eax

		//////////////// return 
		add esp, 0x20
		popad
		mov esp, ebp
		pop ebp
		retn


		// func_GetProcAddress ����pe, ��ȡ����ָ�� start=====
		// ����1 imageBase ģ���ַ ����ͨ��func_getDll_BaseAddr��ȡ
		// ����2 funcName  �������� hash�������
	func_GetProcAddress:
		push ebp
			mov ebp, esp
			sub esp, 0x1c // ��������ռ�

			push ebx
			push ecx
			push edx
			push esi

			// ����pe -- �ҵ�����Ŀ¼��λ�� -- ��һ��Ϊ������
			mov edx, [ebp + 0x8] // ��һ������ ģ���ַ
			mov esi, [edx + 0x3c] // if_new
			lea esi, [esi + edx] // ntͷ
			mov esi, [esi + 0x78] // ����Ŀ¼��rva �� DataDirectory�ֶ�����λ��
			lea esi, [esi + edx] // ����Ŀ¼��va
			// ������������
			mov edi, [esi + 0x1c] // EAT ������ַ�� rva
			lea edi, [edi + edx] // EAT va
			mov[ebp - 0x4], edi // �浽�ֲ�����
			mov edi, [esi + 0x20] // ENT �������Ʊ� rva
			lea edi, [edi + edx] // ENT va
			mov[ebp - 0x8], edi // �浽�ֲ�����
			mov edi, [esi + 0x24] // EOT ������ű� rva
			lea edi, [edi + edx] // EOT va
			mov[ebp - 0x0C], edi // �浽�ֲ�����

			// ѭ���ʹ����������Ƚ�
			xor ecx, ecx
			func_GetProcAddress_getAddr_loop :
		mov esi, [ebp - 0x8] // ENT
			mov esi, [esi + ecx * 4] // rva
			lea esi, [edx + esi] // ���������ַ�����ַ
			// ����hash char
			push esi
			call func_compute_hash
			cmp eax, [ebp + 0x0c] // �ʹ���ĺ������ƱȽ� 
			je func_GetProcAddress_getAddr_loop_end
			inc ecx
			jmp func_GetProcAddress_getAddr_loop
			func_GetProcAddress_getAddr_loop_end :
		// ����ű�͵�ַ���õ���ַ
		mov esi, [ebp - 0x0c] // EOT
			xor edi, edi
			mov di, [esi + ecx * 2] // EAT ������
			mov ebx, [ebp - 0x4] // EAT
			mov esi, [ebx + edi * 4] // ������ַrva
			lea eax, [esi + edx]  // func addr

			pop esi
			pop edx
			pop ecx
			pop ebx

			mov esp, ebp
			pop ebp
			retn 0x8
			// func_GetProcAddress ����pe ��ȡ����ָ�� end=====



				// func_getDll_BaseAddr ��ȡdll��ģ���ַ start=====
				//  ����1: dll���Ƶ�hashֵ DWORD
		func_getDll_BaseAddr: //
		push ebp
			mov ebp, esp
			sub esp, 0x4 // ��������ռ�

			push ebx
			push ecx
			push edx
			push esi

			mov esi, fs: [0x30] // PEB
			mov esi, [esi + 0x0c] // LDR�ṹ��ָ��
			mov esi, [esi + 0x1c] // list
			// ��ʼ�����б� �ӵڶ��ʼ ��һ����ntdll
			func_getDll_BaseAddr_getmodule :
			mov esi, [esi]
			mov ebx, [esi + 0x20] // dll������ wchar
			// ����dll���Ƶ�hashֵ
			push ebx
			call func_compute_hash_wchar
			cmp eax, [ebp + 0x08] // ��dll���Ƶ�hash�ʹ���ĵ�һ�������Ƚ�
			je func_getDll_BaseAddr_end
			jmp func_getDll_BaseAddr_getmodule
			func_getDll_BaseAddr_end :
		mov eax, [esi + 0x08] // dll��ģ���ַ

			pop esi
			pop edx
			pop ecx
			pop ebx

			mov esp, ebp
			pop ebp
			retn 0x4
			// func_getDll_BaseAddr ��ȡdll��ģ���ַ end=====

			// func_compute_hash ����char*�ַ�����hash���� start====stdcall
			// ����1: �ַ����׵�ַ
		func_compute_hash: // ����һ���ַ�����ַ stdcall
		push ebp
			mov ebp, esp

			sub esp, 0x4 // �������ֽڿռ�
			mov dword ptr[ebp - 0x4], 0

			push ebx
			push ecx
			push edx
			push esi

			mov esi, [ebp + 0x08] // �����ĵ�ַ, ���ַ�����ַ
			// ��[ebp-0x4]���digest  ecx ���ѭ��index 
			xor ecx, ecx
			xor edx, edx
			xor ebx, ebx
			tag_loop_start : // eax ��al����ֽ�  ebx edx����м����
		xor eax, eax
			mov al, [esi + ecx]
			test al, al
			jz end_func_compute_hash    // �Ƚ��Ƿ��ַ�����β
			mov ebx, dword ptr[ebp - 0x4]
			mov edx, dword ptr[ebp - 0x4]
			shl edx, 25
			shr ebx, 7
			or edx, ebx
			add edx, eax
			inc ecx
			mov dword ptr[ebp - 0x4], edx
			jmp tag_loop_start
			end_func_compute_hash :
		pop esi
			pop edx
			pop ecx
			pop ebx

			mov eax, [ebp - 0x4]
			mov esp, ebp
			pop ebp
			retn 0x4
			// func_compute_hash ����char*�ַ�����hash���� end====stdcall
			// 
			// func_compute_hash_wchar ����wchar_t�ַ�����hash���� start====stdcall
			// ����1: �ַ����׵�ַ
		func_compute_hash_wchar:
		push ebp
			mov ebp, esp
			push ecx
			mov dword ptr ss : [ebp - 0x4] , 0x0
			func_compute_hash_wchar_tag1 :
			mov eax, dword ptr ss : [ebp + 0x8]
			movzx ecx, word ptr ds : [eax]
			test ecx, ecx
			je func_compute_hash_wchar_tag_end
			mov edx, dword ptr ss : [ebp - 0x4]
			shl edx, 0x19
			mov eax, dword ptr ss : [ebp - 0x4]
			shr eax, 0x7
			or edx, eax
			mov dword ptr ss : [ebp - 0x4] , edx
			mov ecx, dword ptr ss : [ebp + 0x8]
			movzx edx, word ptr ds : [ecx]
			add edx, dword ptr ss : [ebp - 0x4]
			mov dword ptr ss : [ebp - 0x4] , edx
			mov eax, dword ptr ss : [ebp + 0x8]
			add eax, 0x2
			mov dword ptr ss : [ebp + 0x8] , eax
			jmp func_compute_hash_wchar_tag1
			func_compute_hash_wchar_tag_end :
		mov eax, dword ptr ss : [ebp - 0x4]
			mov esp, ebp
			pop ebp
			retn 0x4
			// func_compute_hash_wchar ����wchar_t�ַ�����hash���� end======stdcall
	}

}

	//84871AB2 KERNELBASE.dll  ��wchar_t��hashֵ
	// win10 kernelbase����loadLibrary ��loadLibraryEx
	// win7 kernelbase��ֻ��loadLibraryEx 

int mainA231()
{
	printf("helloworld\n");
	helloShellcode();
	printf("end\n");

	return 0;
}