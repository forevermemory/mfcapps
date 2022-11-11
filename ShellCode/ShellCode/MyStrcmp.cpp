
#include <stdio.h>

void __declspec(naked) MyStrcmpA(const char* str1, const char* str2)
{
	__asm
	{
		push ebp
		mov ebp, esp
		xor eax, eax
		mov ecx, -1
		mov edi, str1
		repnz scasb
		not ecx
		dec ecx // �õ��ַ�����ʵ����

		mov esi, str1
		mov edi, str2
		repe cmpsb // ������־ ecx==0 
		sete al   // ȡZF��־λ���� ZF=1 �ַ������ ZF=0�ַ��������

		cmp al, 1
		jnz notequal_flag
		mov eax, 0
		jmp end
	notequal_flag:
		mov eax, 1
	end:
		mov esp, ebp
		pop ebp
		retn
	}
}

int main_strcmp()
{

	const char* str1 = "hello";
	const char* str2 = "hello";

	int a = 1;
	if (a == 1)
	{
		a = 0;
	}

	MyStrcmpA(str1, str2);

	return 0;
}