#pragma once
#include <iostream>
#include <Windows.h>

//PrintUI("�ҵĳ���.exe", "MYWINDOW", "color 9", 800, 100, 700, 500);

#pragma warning(disable: 4996)

void PrintUI(const char* exeName, const char* uiName, const char* color, short x, short y, short UIWidth, short UIHeight)
{
    ///////// �ļ��ƶ���������
    //char oldPathName[MAX_PATH] = { 0 };
    //char newPathName[MAX_PATH] = { 0 };
    //GetModuleFileNameA(NULL, oldPathName, MAX_PATH); // ��ȡ��ǰ����·��
    //strcpy(newPathName, oldPathName);
    //for (size_t i = strlen(newPathName); i > 0; i--)
    //{
    //    if (newPathName[i - 1] == '\\')
    //    {
    //        newPathName[i] = 0;
    //        strcat(newPathName, exeName);
    //        break;
    //    }
    //}

    //char cmd[MAXBYTE] = { 0 };
    //strcat(cmd, "del/f/q \"");
    //strcat(cmd, newPathName);
    //strcat(cmd, "\">nul 2>nul");
    //std::cout << cmd << std::endl;

    //system(cmd);
    //rename(oldPathName, newPathName);


    AllocConsole(); // ��ӵ��Դ���
    freopen("conin$", "r+t", stdin); // ������������Ϊ��ǰ���Դ���
    freopen("conout$", "w+t", stdout); // �����������Ϊ��ǰ���Դ���
    SetConsoleTitleA(uiName);  // ���ô��ڱ���
    system(color);
    // 0=��ɫ #000000 8=��ɫ #808080
    // 1=����ɫ #000080 9=��ɫ #0000FF
    // 2=��ɫ #008000 A=����ɫ #00FF00
    // 3=����ɫ #008080 B=ǳ��ɫ #00FFFF
    // 4=���ɫ #800000 C=��ɫ #FF0000
    // 5=��ɫ #800080 D=��ɫ #FF00FF
    // 6=���ɫ #808000 E=��ɫ #FFFF00
    // 7=ǳ��ɫ #C0C0C0 F=��ɫ #FFFFFF
    HWND ui = GetConsoleWindow();
    SetWindowPos(ui, 0, 0, 0, UIWidth, UIHeight, SWP_NOMOVE);
    SetWindowPos(ui, 0, x, y, 0, 0, SWP_NOSIZE);
    std::wcout.imbue(std::locale("CHS")); // ֧������

}
