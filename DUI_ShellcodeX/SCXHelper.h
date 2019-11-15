#pragma once
#ifndef _SCCHELPER_H_
#define _SCCHELPER_H_

#define _HeapAlloc(dwSize)  HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize)
#define _HeapFree(pBuffer) HeapFree(GetProcessHeap(), 0, pBuffer)
#define _VirtualAlloc(pAddr,dwSize) VirtualAlloc(pAddr, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE)
#define _VirtualFree(pMem)	VirtualFree(pMem, 0, MEM_RELEASE)
#define _Align(dwSize,dwAlign)  (dwSize / dwAlign + (dwSize %dwAlign ? 1 : 0))*dwAlign


DWORD CalcBufferHash(PBYTE pBuffer, DWORD dwBuffSize);
BOOL WriteBuffer2File(LPWSTR strFileName, LPBYTE pBuffer, DWORD dwBuffSize);
LPBYTE ReadBufferFromFile(LPWSTR strFileName, PDWORD pBuffSize);
void InitRandom();
DWORD GetRandomValue(DWORD dwMin, DWORD dwMax);
BOOL BrowseFile(LPWSTR strTitle, LPWSTR strPath);
#endif
