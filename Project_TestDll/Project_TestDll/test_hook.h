#pragma once


#include <Windows.h>
#include <stdio.h>
#pragma warning(disable:4996)

//HANDLE hThread = NULL;
//HMODULE g_hInstance = NULL;
//HHOOK g_hook = 0;


LRESULT CALLBACK MyKeyboardProc(
	 int    code,
	 WPARAM wParam,
	 LPARAM lParam
);

extern "C"  _declspec(dllexport) BOOL  RegisterHook();
extern "C"  _declspec(dllexport) BOOL UnHook();