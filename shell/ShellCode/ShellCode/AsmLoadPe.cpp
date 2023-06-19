#include <stdio.h>

//"\x55\x8B\xEC\x60\x83\xEC\x20\x83\xC4\x18\x68\x6C\x64\x00\x00\x68\x6F\x77\x6F\x72\x68\x68\x65\x6C\x6C\x68\x6C\x6C\x00\x00\x68\x33\x32\x2E\x64\x68\x75\x73\x65\x72\x8B\xDC\x68\xB2\x1A\x87\x84\xE8\x9E\x00\x00\x00\x8B\xF0\x68\x87\x32\xD8\xC0\x50\xE8\x28\x00\x00\x00\x6A\x08\x6A\x00\x53\x8B\xD0\xFF\xD2\x68\x6A\x0A\x38\x1E\x50\xE8\x14\x00\x00\x00\x6A\x00\x6A\x00\x83\xC3\x0C\x53\x6A\x00\xFF\xD0\x83\xC4\x20\x61\x8B\xE5\x5D\xC3\x55\x8B\xEC\x83\xEC\x1C\x53\x51\x52\x56\x8B\x55\x08\x8B\x72\x3C\x8D\x34\x16\x8B\x76\x78\x8D\x34\x16\x8B\x7E\x1C\x8D\x3C\x17\x89\x7D\xFC\x8B\x7E\x20\x8D\x3C\x17\x89\x7D\xF8\x8B\x7E\x24\x8D\x3C\x17\x89\x7D\xF4\x33\xC9\x8B\x75\xF8\x8B\x34\x8E\x8D\x34\x32\x56\xE8\x5A\x00\x00\x00\x3B\x45\x0C\x74\x03\x41\xEB\xE9\x8B\x75\xF4\x33\xFF\x66\x8B\x3C\x4E\x8B\x5D\xFC\x8B\x34\xBB\x8D\x04\x16\x5E\x5A\x59\x5B\x8B\xE5\x5D\xC2\x08\x00\x55\x8B\xEC\x83\xEC\x04\x53\x51\x52\x56\x64\x8B\x35\x30\x00\x00\x00\x8B\x76\x0C\x8B\x76\x1C\x8B\x36\x8B\x5E\x20\x53\xE8\x5A\x00\x00\x00\x3B\x45\x08\x74\x02\xEB\xEE\x8B\x46\x08\x5E\x5A\x59\x5B\x8B\xE5\x5D\xC2\x04\x00\x55\x8B\xEC\x83\xEC\x04\xC7\x45\xFC\x00\x00\x00\x00\x53\x51\x52\x56\x8B\x75\x08\x33\xC9\x33\xD2\x33\xDB\x33\xC0\x8A\x04\x0E\x84\xC0\x74\x16\x8B\x5D\xFC\x8B\x55\xFC\xC1\xE2\x19\xC1\xEB\x07\x0B\xD3\x03\xD0\x41\x89\x55\xFC\xEB\xE1\x5E\x5A\x59\x5B\x8B\x45\xFC\x8B\xE5\x5D\xC2\x04\x00\x55\x8B\xEC\x51\x36\xC7\x45\xFC\x00\x00\x00\x00\x36\x8B\x45\x08\x3E\x0F\xB7\x08\x85\xC9\x74\x31\x36\x8B\x55\xFC\xC1\xE2\x19\x36\x8B\x45\xFC\xC1\xE8\x07\x0B\xD0\x36\x89\x55\xFC\x36\x8B\x4D\x08\x3E\x0F\xB7\x11\x36\x03\x55\xFC\x36\x89\x55\xFC\x36\x8B\x45\x08\x83\xC0\x02\x36\x89\x45\x08\xEB\xC3\x36\x8B\x45\xFC\x8B\xE5\x5D\xC2\x04\x00"

void __declspec(naked) helloShellcode()
{
	__asm
	{
		push ebp
		mov ebp, esp
		pushad
		sub esp, 0x20 // 分配参数空间 32个字节

		// 字符串入栈
		// user32.dll  75 73 65 72  33 32 2E 64  6C 6C 00    长度0x0B
		// helloworld  68 65 6C 6C  6F 77 6F 72  6C 64 00    长度0x0B
		add esp, 0x18
		push  0x646c
		push 0x726f776f
		push 0x6c6c6568

		push 0x6c6c
		push 0x642e3233
		push 0x72657375
		mov ebx, esp // ebx 就是字符串首地址


		// 找到kernelbase模块基址
		push 0x84871AB2 // KERNELBASE.dll  的wchar_t的hash值
		call func_getDll_BaseAddr
		mov esi, eax // 暂存KERNELBASE.dll模块基址

		// 找到 LoadLibraryExA地址
		push 0xC0D83287 // LoadLibraryExA  char C0D83287
		push eax
		call func_GetProcAddress

		push 0x8
		push 0
		push ebx // 需要加载的dll名称
		mov edx, eax
		call edx


		// 已经加载user32.dll 从user32.dll找到MessageBoxA
		push 0x1E380A6A // MessageBoxA  1E380A6A
		push eax
		call func_GetProcAddress

		// 调用 MessageBoxA
		push 0
		push 0
		add ebx, 0x0c
		push ebx
		push 0
		call eax

		//// 退出函数 ExitProcess
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


		// func_GetProcAddress 解析pe, 获取函数指针 start=====
		// 参数1 imageBase 模块基址 可以通过func_getDll_BaseAddr获取
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



				// func_getDll_BaseAddr 获取dll的模块基址 start=====
				//  参数1: dll名称的hash值 DWORD
		func_getDll_BaseAddr: //
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
			// 计算dll名称的hash值
			push ebx
			call func_compute_hash_wchar
			cmp eax, [ebp + 0x08] // 拿dll名称的hash和传入的第一个参数比较
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
	}

}

	//84871AB2 KERNELBASE.dll  的wchar_t的hash值
	// win10 kernelbase中有loadLibrary 和loadLibraryEx
	// win7 kernelbase中只有loadLibraryEx 

int mainA231()
{
	printf("helloworld\n");
	helloShellcode();
	printf("end\n");

	return 0;
}