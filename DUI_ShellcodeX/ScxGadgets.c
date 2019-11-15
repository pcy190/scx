
#include "ScxGadgets.h"
#include <intrin.h>

SCXAPI void scx_mset(PVOID ptr, SIZE_T cnt,char c)
{
	volatile char *vptr = (volatile char *)ptr;

#if defined(_M_AMD64)

	__stosb((PBYTE)((DWORD64)vptr), c, cnt);

#else

	while (cnt) {
		*vptr = c;
		vptr++;
		cnt--;
	}

#endif

	return ptr;
}

#pragma intrinsic( _ReturnAddress )

SCXAPI __declspec(noinline) ULONG_PTR scx_caller(VOID) { return (ULONG_PTR)_ReturnAddress(); }
//////////////////////////////////////////////////////////////////////////

SCXAPI __declspec(noinline) ULONG_PTR scx_GetCPUPtr()
{
	ULONG_PTR rax;
	rax = scx_caller();
	rax -= 9;
	return rax;
}

SCXAPI ULONG_PTR scx_GetProcAbsoluteAddr(ULONG_PTR ulProc)
{
	ULONG_PTR ulCpuPtr;
	ulCpuPtr = scx_GetCPUPtr();
	return (ULONG_PTR)ulProc + ulCpuPtr - (ULONG_PTR)scx_GetCPUPtr;
}
