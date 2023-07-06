
#include <Windows.h>

//===============================================================================================//
extern "C"  __declspec(dllexport) int addNumbers(int a, int b)
{
	return a + b + 1;
}

DWORD WINAPI Func_callback(LPVOID lpThreadParameter)
{
	OutputDebugStringA("Func_callback==================");
	for (size_t i = 0; i < 10; i++)
	{
		Sleep(500);
		OutputDebugStringA("Func_callback===eqw===============");
	}
	OutputDebugStringA("Func_callback end ==================");
	return 0;
}

HINSTANCE  hAppInstance;
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved)
{
	OutputDebugStringA("dll main .........");
	BOOL bReturnValue = TRUE;
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		hAppInstance = hinstDLL;
		MessageBoxA(NULL, "Hello from DllMain!", "Reflective Dll Injection", MB_OK);
		CreateThread(NULL, 0, Func_callback, NULL, 0, NULL);
		break;
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return bReturnValue;
}

//===============================================================================================//
