

#include <Windows.h>
#include <stdio.h>


void __declspec(naked) helloworld()
{
	__asm {
		// 0x68 0x65 0x6c 0x6c 0x6f 0x77 0x6f 0x72 0x6c 0x64
		//   h    e    l    l    o    w   o    r    l    d
		// 尽量不要在shellcode出现0
		push 0x646c
		push 0x726f776f
		push 0x6c6c6568
		mov eax,esp
		xor edi,edi
		push edi
		push edi
		push eax
		push edi
		mov eax,0x75B21300 // MessageBoxA
		call eax
		mov eax, 0x777C4100 // ExitProcess
		push edi
		call eax
	}
}

int main67895() {
	
	LoadLibraryA("user32.dll");

	_TEB* teb = NtCurrentTeb();
	helloworld();
	printf("helloworld\n");

	//00491030 > |  55            push ebp
	//00491031 | .  8BEC          mov ebp, esp
	//00491033 | .  64:A1 1800000 mov eax, dword ptr fs : [0x18]
	//00491039 | .  5D            pop ebp;  KERNEL32.777BFA29
	//0049103A  .   C3            retn
	//"\x68\x6C\x64\x00\x00\x68\x6F\x77\x6F\x72\x68\x68\x65\x6C\x6C\x8B\xC4\x33\xFF\x57\x57\x50\x57\xB8\x00\x13\xB2\x75\xFF\xD0\xB8\x00\x41\x7C\x77\x57\xFF\xD0"


	
	return 0;
}
