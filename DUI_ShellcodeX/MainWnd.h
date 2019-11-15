#pragma once
#include "Resource.h"

typedef struct _MODULE_INFO_
{
	WCHAR wszModule[64];
	HMODULE hModule;
}MODULEINFO, *PMODULEINFO;

class CMainWnd :public WindowImplBase
{
public:





#ifdef _ZIPRES_
	LPCTSTR GetResourceID() const
	{
		return MAKEINTRESOURCE(IDR_ZIPRES1);
	};
	UILIB_RESOURCETYPE GetResourceType() const
	{
		return UILIB_ZIPRESOURCE;
	};
#endif
	CMainWnd();
	~CMainWnd();
	LPCTSTR GetWindowClassName() const;
	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();
	static CMainWnd* Instance();
	void InitWindow();
	void Notify(TNotifyUI& msg);
	void OnClick(TNotifyUI& msg);
	UINT GetClassStyle() const
	{
		return __super::GetClassStyle() | CS_DROPSHADOW;
	}
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	static CMainWnd* g_pMainWnd;
	// Control
	CTabLayoutUI* m_pTabPanels;
	// Tools
	CTabLayoutUI* m_pTabTools;
	// API
	CEditUI*	m_pTxtWinApi;
	CEditUI*	m_pTxtModuleName;
	CEditUI*	m_pTxtApiHash;
	// Strings
	CEditUI*	m_pTxtStrSrc;
	CEditUI*	m_pTxtStrDst;
	// test
	CEditUI*	m_pTxtScFileName;
	CEditUI*	m_pTxtScFileSize;
	CEditUI*	m_pTxtScEntryOffset;
	// shellcode
	CEditUI* m_pTxtScSize;
	CEditUI* m_pTxtBinPath;
	CEditUI* m_pTxtMapPath;
	CEditUI* m_pTxtFilter;
	CEditUI* m_pTxtFunPath;
	CLabelUI*	m_pLabFunNumb;
	CListUI*	m_pListFuns;
	void OnFinalMessage(HWND hWnd);

	void Copy2Clipboard(LPBYTE lpBuffer, DWORD dwSize, DWORD dwType =CF_TEXT);
	


	void OnSelectChanged(TNotifyUI& msg);
	void OnTextChanged(TNotifyUI& msg);
	void OnCalcWinApiHash();
	void LoadModules();
	void OnWideStr2DwordArry();
	void OnMultStr2DowrdArry();

	void OnStringCopyDest();
	void OnApiCopyHash();
	void OnShellcodeVsExec();
	void CalcShellcodeSize();
	void OnShellcodeMemExec();
	void OnShellcodeInjectExec();
	void OnShellcodeDump();
	void OnShellcodeLoadMapFunsPath();
	void OnShellcodeReadMap();
	void OnShellcodeRandomFuns();
	void OnShellcodeWriteFuns();
	void OnTestExecSc();
	void LoadConfigFromIni();
	void WriteConfig2Ini();
	void OnShellcodeBinPath();
	void OnShellcodeMapPath();
	void OnShellcodeFunPath();
};

