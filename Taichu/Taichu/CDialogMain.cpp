// CDialogMain.cpp: 实现文件
//

#include "pch.h"
#include <tlhelp32.h>
#include "Taichu.h"
#include "afxdialogex.h"
#include "CDialogMain.h"
#include "GlobalInfo.h"
#include "find.h"
#pragma warning(disable:4996)

// CDialogMain 对话框

UCHAR secret[27] = { 0x8B,0x45,0xD4,0x4C,0x8D,0x9C,0x24,0x80,
0x00,0x00,0x00,0x49,0x8B,0x5B,0x20,0x49,
0x8B,0x73,0x28,0x49,0x8B,0xE3,0x41,0x5E,
0x5F,0x5D,0xC3,
};
ULONGLONG secretLength = 27;
ULONGLONG pFound = 0;

extern "C"  ULONGLONG pFound_2 = 0;
extern "C"  ULONGLONG tmpRAX = 0;
extern "C"  ULONGLONG tmpRCX = 0;
extern "C"  ULONGLONG tmpRDX = 0;
extern "C"  ULONGLONG tmpRSI = 0;
extern "C"  ULONGLONG tmpR8 = 0;
extern "C"  ULONGLONG tmpR9 = 0;
extern "C"  ULONGLONG tmpR14 = 0;

//extern "C" UCHAR buff[10240] = { 0 };
//extern "C" ULONGLONG buf_length = 0;

extern "C" void  __stdcall asm_func();
extern "C" void  __stdcall hook_recv();


extern "C" void   my_printf(void * bf, ULONGLONG len)
{
    OutputDebugStringA("=====================enter ");

    CString str;
    str.Format("LENGTH: %lld ", len);
    if (len < 10)
    {
        return;
    }
    OutputDebugStringA(str);

    char* bfA = (char*)bf;
    OutputDebugStringA("=====================A: ");
    OutputDebugStringA(bfA);

  
    wchar_t* bfw = (wchar_t*)(bf);
    OutputDebugStringA("=====================W");
    OutputDebugStringW(bfw);

    OutputDebugStringA("=====================end");

}

extern "C" ULONGLONG C_VAR = 0;


IMPLEMENT_DYNAMIC(CDialogMain, CDialogEx)

CDialogMain::CDialogMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MAIN, pParent)
{

}

CDialogMain::~CDialogMain()
{
}

void CDialogMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogMain, CDialogEx)
END_MESSAGE_MAP()


// CDialogMain 消息处理程序


BOOL CDialogMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GlobalInfo::GetInstance()->m_hWndDlgMain = m_hWnd;


    HANDLE snapHandele = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
    if (INVALID_HANDLE_VALUE == snapHandele)
    {
        MessageBoxA("snapHandele");
        return TRUE;
    }
    
    MODULEENTRY32 entry = { 0 };
    entry.dwSize = sizeof(entry);// 长度必须赋值
    BOOL ret = Module32First(snapHandele, &entry);
 
    CString str;
    CString w3232 = "WS2_32.dll";
    while (ret) {
        str.Format("%x-%s-size:%x",entry.hModule, entry.szModule, entry.modBaseSize);

        if (strcmp(w3232, entry.szModule) == 0)
        {
            OutputDebugStringA("=======================");
            OutputDebugStringA(str);
            SearchPattern(entry.hModule, entry.modBaseSize, secret, secretLength, &pFound);
            str.Format("SearchPattern: %llX", pFound);
            OutputDebugStringA(str);
            OutputDebugStringA("=======================");
            break;
        }
        ret = Module32Next(snapHandele, &entry);
    }
    CloseHandle(snapHandele);

    // HOOK 
    // 00007FFB247B1E74 | 8B45 D4   | mov eax,dword ptr ss:[rbp-2C]                     |
    //00007FFB247B1E77 | 4C:8D9C24 80000000 | lea r11, qword ptr ss : [rsp + 80] | [rsp + 80]
    //00007FFB247B1E7F | 49 : 8B5B 20 | mov rbx, qword ptr ds : [r11 + 20] |
    //00007FFB247B1E83 | 49 : 8B73 28 | mov rsi, qword ptr ds : [r11 + 28] |
    //00007FFB247B1E87 | 49 : 8BE3 | mov rsp, r11 |
    //00007FFB247B1E8A | 41 : 5E | pop r14 |
    //00007FFB247B1E8C | 5F | pop rdi |
    //00007FFB247B1E8D | 5D | pop rbp |
    //00007FFB247B1E8E | C3 | ret |
    // asm_func();
    // 
    // 
    // 
    //00007FFD1A020951 | 50 | push rax |
    //    ; | 48:B8 8967452301000000 | mov rax, 123456789 |
    //    00007FFD1A020957 | 50 | push rax |
    //    00007FFD1A020958 | C3 | ret              |
    //    | 58 | pop rax |

    UCHAR code[27] = {0x50,
        0x48, 0xB8,
        0, 0 ,0 ,0 ,0,0,0,0,
        0x50, 0xC3,
        0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc,
        0xcc, 0xcc, 0xcc, 0xcc,
        //0x58,
    };
    pFound_2 = pFound + 28;


    *(ULONGLONG*)((ULONGLONG)code + 3) = (ULONGLONG)hook_recv;
    str.Format("hook_send: %llx", hook_recv);
    OutputDebugStringA(str);

    // 拷贝代码到目标地址 pFound
    DWORD old;
    VirtualProtect((LPVOID)pFound, secretLength, PAGE_EXECUTE_READWRITE, &old);
    memcpy((PVOID)pFound, code, secretLength);
    VirtualProtect((LPVOID)pFound, secretLength, old, &old);



    str.Format("C_VAR: %llX", C_VAR);
    OutputDebugStringA(str);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
