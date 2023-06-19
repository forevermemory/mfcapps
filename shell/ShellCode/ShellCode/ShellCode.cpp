

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>

int  checkPassword(const char* password, int len) {
	int result = 1;
	char buff[7]{};
	result = strcmp(password, "51hook");
	memcpy(buff, password, len); // 这里会发生缓冲区溢出 从而覆盖堆栈
	return result;
}

int mainS12() {
	int flag = 0;
	char password[0x1000] = { 0 };
	char buff[0x100] = { 0 };
	FILE* fp;
	if (NULL == (fp = fopen("password.txt", "rb"))) {
		return 0;
	}
	fread(password, sizeof(password), 1, fp);
	//IMAGE_EXPORT_DIRECTORY

	//LoadLibraryExA()
	//	LOAD_WITH_ALTERED_SEARCH_PATH
	__asm
	{
		mov eax, eax
		mov eax, eax
		mov eax, eax
		mov eax,eax
	}
	flag = checkPassword(password, 0x1000);
	if (flag) {
		MessageBoxA(NULL, "密码错误！", "提示", MB_OK);
	}
	else {
		MessageBoxA(NULL, "密码正确！", "提示", MB_OK);
	}
	fclose(fp);
	system("pause");
	return 0;
}
