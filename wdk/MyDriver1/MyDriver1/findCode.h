#pragma once

#include "define.h"


// dllBase �ں�ģ���ַ
// size  �ں�ģ���С
// code  �����������׵�ַ
// len  �������С
// pFound ����ָ�룬���봫������
NTSTATUS SearchPattern(PVOID dllBase, int size, PUCHAR code, int len, PVOID pFound);
