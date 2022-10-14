#pragma once
#include "afxwin.h"

class FormBase {


public:
	FormBase(void){}
	~FormBase(void){}
	virtual CWnd* GetWnd() = 0;
	virtual void DoExit() = 0;
};