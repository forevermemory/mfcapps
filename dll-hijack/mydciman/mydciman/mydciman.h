#pragma once
#include "pch.h"
#include <Windows.h>

#define EXTERNC extern "C"
#define NAKED __declspec(naked)
#define EXPORT __declspec(dllexport)

#define ALCPP EXPORT NAKED
#define ALSTD EXTERNC EXPORT NAKED void __stdcall
#define ALCFAST EXTERNC EXPORT NAKED void __fastcall
#define ALCDECL EXTERNC  NAKED void __cdecl

void WINAPIV Init(LPVOID pParam); // Ҫע��Ĵ���ŵ�����
 BOOL WINAPI Load();
 BOOL WINAPI Free();