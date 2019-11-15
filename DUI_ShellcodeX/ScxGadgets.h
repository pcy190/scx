#pragma once
#ifndef _SCXGADGETS_H_
#define _SCXGADGETS_H_
// shellcode small parts
#include <windows.h>
//#define SCXAPI __declspec(dllexport)
#define  SCXAPI
#define _GetProcByHash(API) pfn##API = scx_GetProcAddressWithHash(HASH_##API)
SCXAPI void scx_mset(PVOID ptr, SIZE_T cnt, char c);
SCXAPI ULONG_PTR scx_GetCPUPtr();
SCXAPI ULONG_PTR scx_GetProcAbsoluteAddr(ULONG_PTR ulProc);
#endif

