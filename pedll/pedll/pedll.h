#pragma once


#ifdef DLL1_API
#else
#define DLL1_API _declspec(dllexport)
#endif

_declspec(dllexport) int __stdcall  add(int a, int b);
_declspec(dllexport) int __stdcall substract(int a, int b);
_declspec(dllexport) void __stdcall myprint(int a); 
_declspec(dllexport) void __stdcall myprint2();