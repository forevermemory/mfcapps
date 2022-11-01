// ConsoleAppPE.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "ui.h"
#include <stdio.h>

#include "PEUtils.h"

#define DLL_PATH "D:\\pojie\\projects\\pedll\\Release\\pedll.dll"

int main()
{
	PrintUI("我的程序.exe", "MYWINDOW", "color 9", 800, 100, 700, 500);

	PEUtils util;
	BOOL ret = util.LoadFile(DLL_PATH);
	if (!ret)
	{
		printf("加载dll文件失败");
	}

	util.InitPeInfo();
	util.PrintSectionHeaders();
}


