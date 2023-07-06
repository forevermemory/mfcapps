
//===============================================================================================//
#ifndef _REFLECTIVEDLLINJECTION_LOADLIBRARYR_H
#define _REFLECTIVEDLLINJECTION_LOADLIBRARYR_H
//===============================================================================================//
#include "ReflectiveDLLInjection.h"

DWORD GetReflectiveLoaderOffset(VOID* lpReflectiveDllBuffer);
DWORD GetReflectiveLoaderOffset32(VOID * lpReflectiveDllBuffer);

HMODULE WINAPI LoadLibraryR( LPVOID lpBuffer, DWORD dwLength );

HANDLE WINAPI LoadRemoteLibraryR(HANDLE hProcess, LPVOID lpBuffer, DWORD dwLength, LPVOID lpParameter);
HANDLE WINAPI LoadRemoteLibraryR32( HANDLE hProcess, LPVOID lpBuffer, DWORD dwLength, LPVOID lpParameter );

DWORD Rva2Offset(DWORD dwRva, UINT_PTR uiBaseAddress);
DWORD Rva2Offset32(DWORD dwRva, UINT_PTR uiBaseAddress);

//===============================================================================================//
#endif
//===============================================================================================//
