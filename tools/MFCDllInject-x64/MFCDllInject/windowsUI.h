#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <process.h>
#include <stdio.h>

#pragma warning(disable:4996)

struct UIParamStruct
{
	HWND hwnd;
	HDC hdc;
	HDC memoryDC;
	WORD x;
	WORD y;
	WORD width;
	WORD height;

	WORD minWindowWidth;
	WORD minWindowHeight;
	WORD maxWindowWidth;
	WORD maxWindowHeight;
	COLORREF bkColor;
	const char* title;
	const char* className;

	// 界面循环的线程
	HANDLE threadHandle;
};

UIParamStruct* uiList = 0; 
DWORD uiNum = 0;

class UIClass
{
public:
	UIClass() {};
	~UIClass() {};


	static LRESULT CALLBACK Window_Message(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		
		switch (msg)
		{
		case WM_MOUSEMOVE:
		{
			HDC dc = GetDC(hwnd);
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			//printf("鼠标移动: x:%d, y:%d\n", xPos, yPos);

			char buf[64] = { 0 };
			sprintf(buf, "x:%d, y%d", xPos + 40, yPos);
			TextOutA(dc, xPos, yPos, buf, strlen(buf));
			break;
		}
		default:
			break;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	static unsigned  __stdcall uiThreadProc(void* arg)
	{
		DWORD index = (DWORD)arg;
		UIParamStruct* param = (UIParamStruct*)&(uiList[index]);

		HINSTANCE hInstance = GetModuleHandle(NULL);
		WNDCLASSEXA cls; 
		cls.cbClsExtra = 0;
		cls.cbWndExtra = 0;
		cls.cbSize = sizeof(WNDCLASSEXA);
		cls.hCursor = LoadCursor(hInstance, IDC_ARROW);
		cls.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
		cls.hIconSm = NULL;
		cls.style = CS_HREDRAW | CS_VREDRAW;
		cls.hbrBackground = (HBRUSH)CreateSolidBrush(param->bkColor);
		cls.hInstance = hInstance;
		cls.lpszMenuName = 0;
		cls.lpszClassName = param->className;

		cls.lpfnWndProc = (WNDPROC)Window_Message;

		RegisterClassExA(&cls);


		param->hwnd = CreateWindowA( param->className, param->title, WS_OVERLAPPEDWINDOW, param->x, param->y,
			param->width, param->height, NULL, NULL, hInstance, 0);

		ShowWindow(param->hwnd, SW_SHOW);
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return 0;
	}

	void NewUI(WORD x, WORD y, WORD width, WORD height, COLORREF bkColor,
		const char * title, const char * className, 
		WORD minWindowWidth,WORD minWindowHeight,WORD maxWindowWidth,WORD maxWindowHeight)
	{
		uiList = (UIParamStruct*)realloc(uiList, (uiNum + 1) * sizeof(UIParamStruct));
		uiList[uiNum].x = x;
		uiList[uiNum].y = y;
		uiList[uiNum].width = width;
		uiList[uiNum].height = height;
		uiList[uiNum].bkColor = bkColor;
		uiList[uiNum].title = title;
		uiList[uiNum].className = className;
		uiList[uiNum].minWindowWidth = minWindowWidth;
		uiList[uiNum].minWindowHeight = minWindowHeight;
		uiList[uiNum].maxWindowWidth = maxWindowWidth;
		uiList[uiNum].maxWindowHeight = maxWindowHeight;
		uiList[uiNum].threadHandle = (HANDLE)_beginthreadex(NULL, 0, uiThreadProc, (void *)uiNum, 0,0);
		printf("NewUI\n");
		uiNum++;
	}

private:

};

