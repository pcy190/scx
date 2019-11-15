#include "stdafx.h"
#include "MainWnd.h"
#include "SCXHelper.h"
#include <shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")


extern "C" {
#include "GetProcAddressWithHash.h"
#include "scx.h"


}



CMainWnd* CMainWnd::g_pMainWnd = NULL;
MODULEINFO g_moduleInfos[] = { { L"ntdll.dll",0 },{ L"kernel32.dll",0 },{ L"advapi32.dll",0 },{ L"Crypt32.dll",0 },
{ L"shell32.dll",0 },{ L"user32.dll",0 },{ L"wsock32.dll",0 },{ L"ws2_32.dll",0 },{ L"ole32.dll",0 },{ L"secur32.dll",0 },
{ L"IPHLPAPI.dll",0 },{ L"winhttp.dll",0 },{ L"psapi.dll",0 },{ L"msvcrt.dll",0 },{ L"Shlwapi.dll",0 } };



CMainWnd::CMainWnd()
{
	
	Create(NULL, _T("ShellcodeX"), UI_WNDSTYLE_FRAME, WS_EX_ACCEPTFILES | WS_EX_WINDOWEDGE);
}


CMainWnd::~CMainWnd()
{
	
}

LPCTSTR CMainWnd::GetWindowClassName() const
{
	return _T("UI_SCX_MAIN_WND");
}

CDuiString CMainWnd::GetSkinFile()
{
	return _T("MainWnd.xml");
}

CDuiString CMainWnd::GetSkinFolder()
{
	return _T("");
}

CMainWnd* CMainWnd::Instance()
{
	if (g_pMainWnd==NULL)
	{
		g_pMainWnd = new CMainWnd();
	}

	return g_pMainWnd;
}

void CMainWnd::InitWindow()
{
	WindowImplBase::InitWindow();
	CenterWindow();

	m_pTabPanels		= static_cast<CTabLayoutUI*> (m_PaintManager.FindControl(_T("tabPanels")));

	// tools
	m_pTabTools			= static_cast<CTabLayoutUI*> (m_PaintManager.FindControl(_T("tabTools")));
	//API
	m_pTxtWinApi		= dynamic_cast<CEditUI*> (m_PaintManager.FindControl(_T("txtWinAPI")));
	m_pTxtModuleName	= dynamic_cast<CEditUI*> (m_PaintManager.FindControl(_T("txtModuleName")));
	m_pTxtApiHash		= dynamic_cast<CEditUI*> (m_PaintManager.FindControl(_T("txtApiHash")));
	// Strings
	m_pTxtStrSrc		= dynamic_cast<CEditUI*> (m_PaintManager.FindControl(_T("txtStrSrc")));
	m_pTxtStrDst		= dynamic_cast<CEditUI*> (m_PaintManager.FindControl(_T("txtStrDst")));
	// tests
	m_pTxtScFileName	= dynamic_cast<CEditUI*> (m_PaintManager.FindControl(_T("txtScFileName")));
	m_pTxtScFileSize	= dynamic_cast<CEditUI*> (m_PaintManager.FindControl(_T("txtScFileSize")));
	m_pTxtScEntryOffset	= dynamic_cast<CEditUI*> (m_PaintManager.FindControl(_T("txtScEntryOffset")));
	// Shellcode
	m_pTxtScSize		= dynamic_cast<CEditUI*> (m_PaintManager.FindControl(_T("txtScSize")));
	m_pTxtBinPath = dynamic_cast<CEditUI*> (m_PaintManager.FindControl(_T("txtBinPath")));
	m_pTxtMapPath = dynamic_cast<CEditUI*> (m_PaintManager.FindControl(_T("txtMapPath")));
	m_pTxtFilter = dynamic_cast<CEditUI*> (m_PaintManager.FindControl(_T("txtFilter")));
	m_pTxtFunPath = dynamic_cast<CEditUI*> (m_PaintManager.FindControl(_T("txtFunPath")));
	m_pLabFunNumb = dynamic_cast<CLabelUI*> (m_PaintManager.FindControl(_T("labFunNumb")));
	m_pListFuns = dynamic_cast<CListUI*> (m_PaintManager.FindControl(_T("listFuns")));

	
	m_PaintManager.FindControl(_T("btnLoadMapFunsPath"))->SetEnabled(IsDebuggerPresent());
	
	

	LoadModules();
	LoadConfigFromIni();

	CalcShellcodeSize();

}

void CMainWnd::Notify(TNotifyUI& msg)
{
	CDuiString strType = msg.sType;

	if (strType == DUI_MSGTYPE_SELECTCHANGED)
	{
		// tab , selectChanged 
		OnSelectChanged(msg);
		return;
	}else if (strType == DUI_MSGTYPE_TEXTCHANGED)
	{
		OnTextChanged(msg);
		return;
	}
	WindowImplBase::Notify(msg);
}

void CMainWnd::OnClick(TNotifyUI& msg)
{
	CDuiString strSender = msg.pSender->GetName();
	if (strSender == _T("btnCalcHash") )
	{
		OnCalcWinApiHash();
		return;
	}
	else if (strSender == _T("btnCopyHash"))
	{
		OnApiCopyHash();
		return;
	}
	else if (strSender == _T("btnWideStr2DwordArry"))
	{
		OnWideStr2DwordArry();
		return;
	}
	else if (strSender == _T("btnMultStr2DowrdArry"))
	{
		OnMultStr2DowrdArry();
		return;
	}else if (strSender==_T("btnCopyDest"))
	{
		OnStringCopyDest();
		return;
	}
	else if (strSender == _T("btnVsExec"))
	{
		OnShellcodeVsExec();
		return;
	}
	else if (strSender == _T("btnMemExec"))
	{
		OnShellcodeMemExec();
		return;
	}
	else if (strSender == _T("btnInjectExec"))
	{
		OnShellcodeInjectExec();
		return;
	}
	else if (strSender == _T("btnDump"))
	{
		OnShellcodeDump();
		return;
	}
	else if (strSender == _T("btnLoadMapFunsPath"))
	{
		OnShellcodeLoadMapFunsPath();
		return;
	}
	else if (strSender == _T("btnReadMap"))
	{
		OnShellcodeReadMap();
		return;
	}
	else if (strSender == _T("btnRandomFuns"))
	{
		OnShellcodeRandomFuns();
		return;
	}
	else if (strSender == _T("btnWriteFuns"))
	{
		OnShellcodeWriteFuns();
		return;
	}
	else if (strSender == _T("btnExecSc"))
	{
		OnTestExecSc();
		return;
	}
	else if (strSender == _T("btnBinPath"))
	{
		OnShellcodeBinPath();
		return;
	}
	else if (strSender == _T("btnMapPath"))
	{
		OnShellcodeMapPath();
		return;
	}
	else if (strSender == _T("btnFunPath"))
	{
		OnShellcodeFunPath();
		return;
	}
	WindowImplBase::OnClick(msg);
}



LRESULT CMainWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg==WM_DROPFILES && m_pTabTools->GetCurSel()==2 )
	{
		bHandled = TRUE;
		HDROP				hDrop = (HDROP)wParam;
		WORD                wNumFilesDropped = DragQueryFile(hDrop, -1, NULL, 0);
		WORD                wPathnameSize = 0;
		WCHAR				szShellcodeFile[MAX_PATH];
		


		// there may be many, but we'll only use the first
		if (wNumFilesDropped > 0)
		{
			wPathnameSize = DragQueryFile(hDrop, 0, NULL, 0);
			if (wPathnameSize)
			{
				::ZeroMemory(szShellcodeFile, wPathnameSize * sizeof(WCHAR));
				DragQueryFile(hDrop, 0, szShellcodeFile, ++wPathnameSize);

				m_pTxtScFileName->SetText(szShellcodeFile);
			}

		}

		DragFinish(hDrop);

		return 0;
	}
	return __super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}

void CMainWnd::OnFinalMessage(HWND hWnd)
{
	WriteConfig2Ini();
	__super::OnFinalMessage(hWnd);
}

void CMainWnd::Copy2Clipboard(LPBYTE lpBuffer, DWORD dwSize, DWORD dwType /*=CF_TEXT*/)
{
	if (::OpenClipboard(NULL)&& EmptyClipboard())
	{
		HGLOBAL clipBuffer;
		char* buffer;
		clipBuffer = GlobalAlloc(GMEM_DDESHARE, dwSize);
		buffer = (char*)GlobalLock(clipBuffer);
		CopyMemory(buffer, lpBuffer, dwSize);
		GlobalUnlock(clipBuffer);
		SetClipboardData(dwType, clipBuffer);
		CloseClipboard();


	}
}

void CMainWnd::OnSelectChanged(TNotifyUI& msg)
{
	CDuiString strSender = msg.pSender->GetName();
	if (strSender == _T("optTools") )
	{
		m_pTabPanels->SelectItem(0);
		return;
	}
	else if (strSender == _T("optShellcode"))
	{
		m_pTabPanels->SelectItem(1);
		return;
	}
	else if (strSender == _T("optApi"))
	{
		m_pTabTools->SelectItem(0);
		return;
	}
	else if (strSender == _T("optString"))
	{
		m_pTabTools->SelectItem(1);
		return;
	}
	else if (strSender == _T("optTest"))
	{
		m_pTabTools->SelectItem(2);
		return;
	}
}

void CMainWnd::OnTextChanged(TNotifyUI& msg)
{
	CDuiString strSender = msg.pSender->GetName();
	if (strSender == _T("txtWinAPI"))
	{
		OnCalcWinApiHash();
		return;
	}
	
}

void CMainWnd::OnCalcWinApiHash()
{
	DWORD i;
	BOOL bFind = FALSE;
	char szWinAPI[64];
	WCHAR wszHash[64];
	DWORD dwHash;

	WideCharToMultiByte(NULL, NULL, m_pTxtWinApi->GetText().GetData(), -1, szWinAPI, 64, NULL, NULL);
	for ( i=0;i<sizeof(g_moduleInfos) / sizeof(MODULEINFO);i++)
	{
		if (g_moduleInfos[i].hModule&&GetProcAddress(g_moduleInfos[i].hModule, szWinAPI))
		{
			bFind = TRUE;
			break;
		}
	}
	if (bFind)
	{
		
		m_pTxtModuleName->SetText(g_moduleInfos[i].wszModule);
		
		dwHash = CalcBufferHash((LPBYTE)g_moduleInfos[i].wszModule, (wcslen(g_moduleInfos[i].wszModule)+1) * sizeof(WCHAR));
		dwHash += CalcBufferHash((LPBYTE)szWinAPI, strlen(szWinAPI)+1);
		
		_snwprintf_s(wszHash, 64, _TRUNCATE, L"0x%08X", dwHash);
		m_pTxtApiHash->SetText(wszHash);
		//scx_GetProcAddressWithHash(dwHash);
	}
	else
	{
		m_pTxtApiHash->SetText(L"");
	}
	
}

void CMainWnd::LoadModules()
{

	
	for (DWORD i=0;i<sizeof(g_moduleInfos)/sizeof(MODULEINFO);i++)
	{
		g_moduleInfos[i].hModule = LoadLibraryW(g_moduleInfos[i].wszModule);
		wcsupr(g_moduleInfos[i].wszModule);
	}
}

void CMainWnd::OnWideStr2DwordArry()
{
	DWORD dwLen;
	DWORD dwAllocSize;
	PDWORD pArrayBuffer;
	CDuiString strDest;
	WCHAR wszTemp[16];
	if ((dwLen=m_pTxtStrSrc->GetText().GetLength())==0)
	{
		return;
	}
	dwLen++;
	dwAllocSize = dwLen * 2;
	dwAllocSize =  _Align(dwAllocSize, sizeof(DWORD));
	
	pArrayBuffer = (PDWORD)_HeapAlloc(dwAllocSize);
	CopyMemory(pArrayBuffer, m_pTxtStrSrc->GetText().GetData(), dwLen*sizeof(WCHAR));
	strDest = L"{";
	dwLen = dwAllocSize / 4;
	for (DWORD i=0;i<dwLen;i++)
	{
		_snwprintf_s(wszTemp, 12, _TRUNCATE, (i+1)==dwLen?L"0x%08X": L"0x%08X,", pArrayBuffer[i]);
		strDest += wszTemp;
	}
	strDest += L"};";
	m_pTxtStrDst->SetText(strDest);
	_HeapFree( pArrayBuffer);

}

void CMainWnd::OnMultStr2DowrdArry()
{
	DWORD dwLen;
	DWORD dwAllocSize;
	PSTR pszMultStr;
	PDWORD pArrayBuffer;
	CDuiString strDest;
	WCHAR wszTemp[16];
	if ((dwLen = m_pTxtStrSrc->GetText().GetLength()) == 0)
	{
		return;
	}
	dwLen++;
	pszMultStr = (LPSTR)_HeapAlloc(dwLen);
	WideCharToMultiByte(0, 0, m_pTxtStrSrc->GetText().GetData(), dwLen - 1, pszMultStr, dwLen, NULL, NULL);
	dwAllocSize = dwLen ;
	
	dwAllocSize = _Align(dwAllocSize, sizeof(DWORD));

	pArrayBuffer = (PDWORD)_HeapAlloc(dwAllocSize);
	CopyMemory(pArrayBuffer, pszMultStr, dwLen);
	_HeapFree(pszMultStr);
	strDest = L"{";
	dwLen = dwAllocSize / 4;
	for (DWORD i = 0; i < dwLen; i++)
	{
		_snwprintf_s(wszTemp, 12, _TRUNCATE, (i + 1) == dwLen ? L"0x%08X" : L"0x%08X,", pArrayBuffer[i]);
		strDest += wszTemp;
	}
	strDest += L"};";
	m_pTxtStrDst->SetText(strDest);
	_HeapFree(pArrayBuffer);
}

void CMainWnd::OnStringCopyDest()
{
	if (m_pTxtStrDst->GetText().GetLength()==0)
	{
		return;
	}
	
	LPSTR szText = (LPSTR)_HeapAlloc(m_pTxtStrDst->GetText().GetLength() +1);
	WideCharToMultiByte(0,0, m_pTxtStrDst->GetText().GetData(), m_pTxtStrDst->GetText().GetLength(),szText, m_pTxtStrDst->GetText().GetLength()+1,NULL,NULL);
	Copy2Clipboard((LPBYTE)szText, strlen(szText)+1 );
	_HeapFree((LPBYTE)szText);
}

void CMainWnd::OnApiCopyHash()
{
	int iFunlen,iLen;
	if ( m_pTxtApiHash->GetText().GetLength()!=10)
	{
		return;
	}
	iFunlen = m_pTxtWinApi->GetText().GetLength();
	iLen = 8 + 1 + 5 + iFunlen + 1 + 10 + 1;
	LPSTR szText = (LPSTR)_HeapAlloc(iLen);
	//WideCharToMultiByte(0, 0, m_pTxtStrDst->GetText().GetData(), m_pTxtStrDst->GetText().GetLength(), szText, m_pTxtStrDst->GetText().GetLength() + 1, NULL, NULL);
	sprintf_s(szText,iLen, "#define HASH_%S %S", m_pTxtWinApi->GetText().GetData(), m_pTxtApiHash->GetText().GetData());
	Copy2Clipboard((LPBYTE)szText, strlen(szText) + 1);
	_HeapFree((LPBYTE)szText);
}

void CMainWnd::OnShellcodeVsExec()
{
	Shellcode();
}

void CMainWnd::CalcShellcodeSize()
{
	DWORD dwShellcodeSize;
	CDuiString strSize;

	dwShellcodeSize = (ULONG_PTR)EndShellcode - (ULONG_PTR)Shellcode;
	strSize.SmallFormat(L"%d", dwShellcodeSize);
	m_pTxtScSize->SetText(strSize);


	
}

void CMainWnd::OnShellcodeMemExec()
{
	PVOID pMem;
	DWORD dwMemSize;
	FunPtr pfnShellcode;
	dwMemSize = (ULONG_PTR)EndShellcode - (ULONG_PTR)Shellcode;

	pMem = _VirtualAlloc(NULL, _Align(dwMemSize, 4096));
	CopyMemory(pMem, Shellcode, dwMemSize);
	pfnShellcode = (FunPtr)pMem;
	pfnShellcode();
	_VirtualFree(pMem);

}

void CMainWnd::OnShellcodeInjectExec()
{
	WCHAR wszProcess[] = L"c:\\windows\\system32\\svchost.exe";
	
	PVOID pMem;
	DWORD dwAllocSize, dwShellcodeSize, dwWriten;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	CONTEXT context;
	

	
	
	dwShellcodeSize = (ULONG_PTR)EndShellcode - (ULONG_PTR)Shellcode;
	dwAllocSize = _Align(dwShellcodeSize, 4096);




	try
	{

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		if (CreateProcessW(wszProcess, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED | DETACHED_PROCESS, NULL, NULL, &si, &pi))
		{
			context.ContextFlags = CONTEXT_FULL;
			if (GetThreadContext(pi.hThread, &context))
			{
				pMem = VirtualAllocEx(pi.hProcess, NULL, dwAllocSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

				WriteProcessMemory(pi.hProcess, pMem, Shellcode, dwShellcodeSize, (SIZE_T*)&dwWriten);


#ifdef _WIN64
				context.Rip = (ULONG_PTR)pMem;
#else
				context.Eip = (DWORD)pMem;
#endif


				SetThreadContext(pi.hThread, &context);

				ResumeThread(pi.hThread);
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);

			}
		}






	}
	catch (const std::exception&)
	{

	}
	
}

void CMainWnd::OnShellcodeDump()
{
	HANDLE hFile;
	DWORD dwShellcodeSize;
	DWORD dwWriten;
	if (m_pTxtBinPath->GetText().GetLength()==0)
	{
		return;
	}
	dwShellcodeSize = (ULONG_PTR)EndShellcode - (ULONG_PTR)Shellcode;
	hFile = CreateFileW(m_pTxtBinPath->GetText().GetData(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile!=INVALID_HANDLE_VALUE)
	{
		WriteFile(hFile, Shellcode, dwShellcodeSize, &dwWriten, NULL);
		CloseHandle(hFile);
	}


}

void CMainWnd::OnShellcodeLoadMapFunsPath()
{
	CDuiString strPath;
	WCHAR wszCurPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, wszCurPath);
	strPath.Format(L"%s\\map.txt", wszCurPath);
	m_pTxtMapPath->SetText(strPath);
	strPath.Format(L"%s\\funs.txt", wszCurPath);
	m_pTxtFunPath->SetText(strPath);
}

void CMainWnd::OnShellcodeReadMap()
{
	CDuiString strMapFile;
	CDuiString strFilter;
	LPBYTE lpBuffer;
	DWORD dwFileSize;
	LPWSTR strText;
	WCHAR wszOrder[32];
	strMapFile = m_pTxtMapPath->GetText();
	strFilter = m_pTxtFilter->GetText();
	if (strMapFile.IsEmpty() || strFilter.IsEmpty())
	{
	
		return;
	}
	lpBuffer = ReadBufferFromFile((LPWSTR)strMapFile.GetData(), &dwFileSize);


	if (!lpBuffer)
	{


		return;
	}
	strText = (LPWSTR)_HeapAlloc(dwFileSize * 2 + 2);

	MultiByteToWideChar(0, 0, (LPSTR)lpBuffer, dwFileSize, strText, dwFileSize + 1);
	_HeapFree(lpBuffer);

	LPWSTR strFun = StrStrW(strText, strFilter.GetData());
	//LPWSTR strEndShellcode = StrStrW(strText, L"EndShellcode");
	m_pListFuns->RemoveAll();

	DWORD dwIndex = 0;
#ifdef _WIN64
	while (strFun && *(strFun-1)==L' ' )
	{
#else
	while (strFun && *(strFun - 1) == L'_')
	{
#endif
	
		LPWSTR strSpace = StrStrW(strFun, L" ");
		*strSpace = 0;
		//m_lstFuns.InsertItem(dwIndex, strFun);
		CListTextElementUI* pTxtElem = new CListTextElementUI();
		_snwprintf_s(wszOrder, 32, _TRUNCATE, L"%d", dwIndex);
		m_pListFuns->Add(pTxtElem);
		pTxtElem->SetText(0, wszOrder);
		pTxtElem->SetText(1, strFun);

		strFun = StrStrW(strSpace + 1, strFilter.GetData());
		dwIndex++;
	}
	//m_dwFunsNumb = dwIndex;
	WCHAR szVal[10];
	_itow_s(dwIndex, szVal, 10, 10);
	m_pLabFunNumb->SetText(szVal);
	_HeapFree((LPBYTE)strText);

}

void CMainWnd::OnShellcodeRandomFuns()
{
	DWORD dwCount;
	DWORD dwFunStrMaxSize;
	LPBYTE strFuns;
	LPWSTR strFun, strFun2;
	CListTextElementUI* pTextEle;
	CDuiString strText;
	WCHAR wszOrder[32];
	dwFunStrMaxSize = 100 * sizeof(WCHAR);
	if (m_pListFuns->GetCount()<2)
	{
		
		return;
	}

	dwCount = m_pListFuns->GetCount();
	strFuns = (LPBYTE)_HeapAlloc(dwFunStrMaxSize*dwCount);
	for (DWORD i = 0; i < dwCount; i++)
	{
		strFun = (LPWSTR)(strFuns + dwFunStrMaxSize*i);
		pTextEle = (CListTextElementUI*) m_pListFuns->GetItemAt(i);
		strText =  pTextEle->GetText(1);
		wcscpy_s(strFun, 100, strText);


	}

	//LPWSTR strTemp;

	for (DWORD i = 0; i < dwCount; i++)
	{

		strFun = (LPWSTR)(strFuns + dwFunStrMaxSize*i);
		strText = strFun;
		strFun2 = (LPWSTR)(strFuns + dwFunStrMaxSize * GetRandomValue(0, dwCount - 1));
		wcscpy_s(strFun, 100, strFun2);
		wcscpy_s(strFun2, 100, strText);
	}

	
	m_pListFuns->RemoveAll();

	for (DWORD i = 0; i < dwCount; i++)
	{

		strFun = (LPWSTR)(strFuns + dwFunStrMaxSize*i);
		pTextEle = new CListTextElementUI();
		

		m_pListFuns->Add(pTextEle);
		_snwprintf_s(wszOrder, 32, _TRUNCATE, L"%d", i);
		pTextEle->SetText(0, wszOrder);
		pTextEle->SetText(1, strFun);
	}
	_HeapFree(strFuns);
}

void CMainWnd::OnShellcodeWriteFuns()
{
	LPBYTE strFuns;
	CListTextElementUI* pTextEle;
	CDuiString strText, strFileName;
	DWORD dwFunStrMaxSize, dwCount;
	char szFun[100];
	dwFunStrMaxSize = 100 * sizeof(WCHAR);
	strFileName = m_pTxtFunPath->GetText();
	
	dwCount = m_pListFuns->GetCount();
	if (!dwCount)
	{
		
		return;
	}
	if (strFileName.IsEmpty())
	{
		
		return;
	}
	

	strFuns = (LPBYTE)_HeapAlloc(dwFunStrMaxSize*dwCount);

	strcpy_s((char*)strFuns, dwFunStrMaxSize*dwCount, "Shellcode\r\n");

	for (DWORD i = 0; i < dwCount; i++)
	{
		pTextEle = (CListTextElementUI*)m_pListFuns->GetItemAt(i);
		strText = pTextEle->GetText(1);
		WideCharToMultiByte(0,0,(LPWSTR)strText.GetData(),-1, szFun,100,NULL,NULL);

		strcat_s(szFun, "\r\n");


		strcat_s((char*)strFuns, dwFunStrMaxSize*dwCount, szFun);
	}
	

	strcat_s((char*)strFuns, dwFunStrMaxSize*dwCount, "EndShellcode");
	if (WriteBuffer2File((LPWSTR)strFileName.GetData(), strFuns, strlen((char*)strFuns)))
	{
		MessageBoxW(NULL, L"write funs ok", L"scx", 0);

	}
	else
	{
		MessageBoxW(NULL, L"write funs failed", L"scx", 0);
	}
	_HeapFree(strFuns);
}

void CMainWnd::OnTestExecSc()
{
	CDuiString strScFileName;
	FunPtr pfnShellcode;
	LPBYTE pBuffer;
	DWORD dwFileSize;
	LPVOID pMem;
	DWORD dwOffset = 0;
	strScFileName = m_pTxtScFileName->GetText();
	if (strScFileName.GetLength()==0)
	{
		return;
	}
	pBuffer = ReadBufferFromFile((LPWSTR)strScFileName.GetData(), &dwFileSize);
	if (pBuffer)
	{
		strScFileName.SmallFormat(L"%d", dwFileSize);
		m_pTxtScFileSize->SetText(strScFileName);
		dwOffset = _ttoi(m_pTxtScEntryOffset->GetText());
		pMem = _VirtualAlloc(NULL, _Align(dwFileSize, 1024));
		CopyMemory(pMem, pBuffer, dwFileSize);
		pfnShellcode = (FunPtr)((PBYTE)pMem + dwOffset);
		pfnShellcode();
		_VirtualFree(pMem);
		_HeapFree(pBuffer);
	}

}

void CMainWnd::LoadConfigFromIni()
{
	WCHAR wszFileName[MAX_PATH];
	WCHAR wszTemp[MAX_PATH];
	GetModuleFileName(NULL, wszFileName, MAX_PATH);
	wcscat_s(wszFileName, MAX_PATH, L".ini");
	GetPrivateProfileStringW(L"SCX", L"BinPath", NULL, wszTemp, MAX_PATH, wszFileName);
	m_pTxtBinPath->SetText(wszTemp);
	GetPrivateProfileStringW(L"SCX", L"MapPath", NULL, wszTemp, MAX_PATH, wszFileName);
	m_pTxtMapPath->SetText(wszTemp);
	GetPrivateProfileStringW(L"SCX", L"FunsPath", NULL, wszTemp, MAX_PATH, wszFileName);
	m_pTxtFunPath->SetText(wszTemp);
	GetPrivateProfileStringW(L"SCX", L"ScFile", NULL, wszTemp, MAX_PATH, wszFileName);
	m_pTxtScFileName->SetText(wszTemp);

}

void CMainWnd::WriteConfig2Ini()
{
	WCHAR wszFileName[MAX_PATH];
	
	GetModuleFileName(NULL, wszFileName, MAX_PATH);
	wcscat_s(wszFileName, MAX_PATH, L".ini");
	
	WritePrivateProfileStringW(L"SCX", L"BinPath", m_pTxtBinPath->GetText().GetData(),wszFileName);
	WritePrivateProfileStringW(L"SCX", L"MapPath", m_pTxtMapPath->GetText().GetData(), wszFileName);
	WritePrivateProfileStringW(L"SCX", L"FunsPath", m_pTxtFunPath->GetText().GetData(), wszFileName);
	WritePrivateProfileStringW(L"SCX", L"ScFile", m_pTxtScFileName->GetText().GetData(), wszFileName);

}

void CMainWnd::OnShellcodeBinPath()
{
	WCHAR wszPath[MAX_PATH];
	if (BrowseFile(L"Bin Path", wszPath))
	{
		m_pTxtBinPath->SetText(wszPath);
	}
}

void CMainWnd::OnShellcodeMapPath()
{
	WCHAR wszPath[MAX_PATH];
	if (BrowseFile(L"Map Path", wszPath))
	{
		m_pTxtMapPath->SetText(wszPath);
	}
}

void CMainWnd::OnShellcodeFunPath()
{
	WCHAR wszPath[MAX_PATH];
	if (BrowseFile(L"Funs Path", wszPath))
	{
		m_pTxtFunPath->SetText(wszPath);
	}
}
