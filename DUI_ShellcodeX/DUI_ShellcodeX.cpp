// DUI_ShellcodeX.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "DUI_ShellcodeX.h"
#include "MainWnd.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	::OleInitialize(NULL);
	::CoInitialize(NULL);

	CPaintManagerUI::SetInstance(hInstance);





	CMainWnd *pMainWnd = CMainWnd::Instance();
	
	pMainWnd->ShowModal();


	//	delete pFrame;
	::CoUninitialize();
	::OleUninitialize();
	return 0;

	return 0;

}