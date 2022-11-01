#pragma once


#ifdef DLL1_API
#else
#define DLL1_API _declspec(dllexport)
#endif

int add(int a, int b);
int subtract(int a, int b);
void myprint(int a);