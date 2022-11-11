// ConsoleAppPE.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "ui.h"
#include <stdio.h>

#include "PEUtils.h"

//#define DLL_PATH "D:\\pojie\\projects\\pedll\\Release\\pedll.dll"
#define DLL_PATH "C:\\Users\\Administrator\\Desktop\\wmsj\\pedll\\Release\\pedll.dll"
#define EXE_PATH "C:\\Users\\Administrator\\source\\repos\\ConsoleApplication1\\Release\\ConsoleApplication1.exe"

int main()
{
	PrintUI("我的程序.exe", "MYWINDOW", "color d", 400, 100, 700, 500);

	PEUtils util;
	BOOL ret = util.LoadFile(EXE_PATH);
	if (!ret)
	{
		printf("加载dll文件失败");
	}

	util.InitPeInfo();
	//util.PrintSectionHeaders();
	//util.PrintDataDirectory_0();
	//util.PrintDataDirectory_1();
	//util.PrintDataDirectory_5();
	util.PrintDataDirectory_9();
}


