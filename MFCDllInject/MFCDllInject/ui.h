#pragma once
#include <iostream>
#include <Windows.h>

//PrintUI("我的程序.exe", "MYWINDOW", "color 9", 800, 100, 700, 500);

#pragma warning(disable: 4996)

void PrintUI(const char* exeName, const char* uiName, const char* color, short x, short y, short UIWidth, short UIHeight)
{
    ///////// 文件移动和重命名
    //char oldPathName[MAX_PATH] = { 0 };
    //char newPathName[MAX_PATH] = { 0 };
    //GetModuleFileNameA(NULL, oldPathName, MAX_PATH); // 获取当前程序路径
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


    AllocConsole(); // 添加调试窗口
    freopen("conin$", "r+t", stdin); // 将输入流设置为当前调试窗口
    freopen("conout$", "w+t", stdout); // 将输出流设置为当前调试窗口
    SetConsoleTitleA(uiName);  // 设置窗口标题
    system(color);
    // 0=黑色 #000000 8=灰色 #808080
    // 1=深蓝色 #000080 9=蓝色 #0000FF
    // 2=绿色 #008000 A=草绿色 #00FF00
    // 3=蓝绿色 #008080 B=浅蓝色 #00FFFF
    // 4=枣红色 #800000 C=红色 #FF0000
    // 5=紫色 #800080 D=粉色 #FF00FF
    // 6=橄榄色 #808000 E=黄色 #FFFF00
    // 7=浅灰色 #C0C0C0 F=白色 #FFFFFF
    HWND ui = GetConsoleWindow();
    SetWindowPos(ui, 0, 0, 0, UIWidth, UIHeight, SWP_NOMOVE);
    SetWindowPos(ui, 0, x, y, 0, 0, SWP_NOSIZE);
    std::wcout.imbue(std::locale("CHS")); // 支持中文

}
