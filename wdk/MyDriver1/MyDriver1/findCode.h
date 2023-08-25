#pragma once

#include "define.h"


// dllBase 内核模块基址
// size  内核模块大小
// code  特征码数组首地址
// len  特征码大小
// pFound 二级指针，传入传出参数
NTSTATUS SearchPattern(PVOID dllBase, int size, PUCHAR code, int len, PVOID pFound);
