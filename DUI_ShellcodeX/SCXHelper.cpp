#include "stdafx.h"
#include "SCXHelper.h"
#include <shtypes.h>
#include <shlobj.h>

#define ROTR32(value, shift)	(((DWORD) value >> (BYTE) shift) | ((DWORD) value << (32 - (BYTE) shift)))

DWORD CalcBufferHash(PBYTE pBuffer, DWORD dwBuffSize)
{
	DWORD dwHash = 0;
	LPCSTR pTempChar;
	for (DWORD i=0;i < dwBuffSize;i++)
	{
		dwHash = ROTR32(dwHash, 13);
		pTempChar = (LPCSTR)pBuffer + i;
		dwHash += *pTempChar;
	}
	return dwHash;
}

BOOL WriteBuffer2File(LPWSTR strFileName, LPBYTE pBuffer, DWORD dwBuffSize)
{
	DWORD dwWriten;
	BOOL bResult;
	HANDLE hFile = CreateFileW(strFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		bResult = WriteFile(hFile, pBuffer, dwBuffSize, &dwWriten, NULL);
		CloseHandle(hFile);
		return bResult;
	}
	return FALSE;
}

LPBYTE ReadBufferFromFile(LPWSTR strFileName, PDWORD pBuffSize)
{
	LPBYTE pBuffer;
	DWORD dwFileSize;

	HANDLE hFile = CreateFileW(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	*pBuffSize = 0;
	if (hFile != INVALID_HANDLE_VALUE)
	{
		dwFileSize = GetFileSize(hFile,NULL);
		if (dwFileSize)
		{
			pBuffer = (LPBYTE)_HeapAlloc(dwFileSize);
			if (ReadFile(hFile, pBuffer, dwFileSize, &dwFileSize, NULL))
			{
				*pBuffSize = dwFileSize;
				return pBuffer;
			}
			_HeapFree(pBuffer);
		}
	}
	return NULL;
}

void InitRandom()
{
	
	
	srand(GetTickCount());
}

DWORD GetRandomValue(DWORD dwMin, DWORD dwMax)
{
	DWORD dwInterval;

	
	dwInterval = dwMax - dwMin;

	return rand() % dwInterval + dwMin;
}

BOOL BrowseFile(LPWSTR strTitle, LPWSTR strPath)
{
	LPITEMIDLIST pItemIDList;
	//	IMalloc * imalloc ;
	BROWSEINFO bi;

	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.lpszTitle = strTitle;
	bi.pszDisplayName = NULL;
	bi.ulFlags = BIF_BROWSEINCLUDEFILES| BIF_NEWDIALOGSTYLE;
	bi.lpfn = NULL;
	bi.lParam = NULL;



	pItemIDList = SHBrowseForFolder(&bi);
	if (pItemIDList)
	{
		SHGetPathFromIDList(pItemIDList, strPath);

		
		return TRUE;
	}
	return FALSE;
}
