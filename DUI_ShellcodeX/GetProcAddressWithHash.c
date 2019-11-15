
#include "GetProcAddressWithHash.h"


PVOID scx_GetProcAddressWithHash( DWORD dwModuleFunctionHash)
{


	PPEB PebAddress;
	PMY_PEB_LDR_DATA pLdr;
	PMY_LDR_DATA_TABLE_ENTRY pDataTableEntry;
	PVOID pModuleBase;
	PIMAGE_NT_HEADERS pNTHeader;
	DWORD dwExportDirRVA;
	PIMAGE_EXPORT_DIRECTORY pExportDir;
	PLIST_ENTRY pNextModule;
	DWORD dwNumFunctions;
	USHORT usOrdinalTableIndex;
	PDWORD pdwFunctionNameBase;
	PCSTR pFunctionName;
	UNICODE_STRING BaseDllName;
	DWORD dwModuleHash;
	DWORD dwFunctionHash;
	PCSTR pTempChar;
	DWORD i, j,k;
	PVOID pFunction; // zhenwo
	DWORD dwExportSize;// zhenwo
	WCHAR szModuleName[64];
#define HASH_LoadLibraryW 0x07D6774C
	WinApiPtr pfnLoadLib;
	
	DWORD dwDotDll[] = { 0x0044002E,0x004C004C,0x00000000 };

#if defined(_WIN64)
	PebAddress = (PPEB)__readgsqword(0x60);
#elif defined(_M_ARM)
	// I can assure you that this is not a mistake. The C compiler improperly emits the proper opcodes
	// necessary to get the PEB.Ldr address
	PebAddress = (PPEB)((ULONG_PTR)_MoveFromCoprocessor(15, 0, 13, 0, 2) + 0);
	__emit(0x00006B1B);
#else
	PebAddress = (PPEB)__readfsdword(0x30);
#endif

	pLdr = (PMY_PEB_LDR_DATA)PebAddress->Ldr;
	pNextModule = pLdr->InLoadOrderModuleList.Flink;
	pDataTableEntry = (PMY_LDR_DATA_TABLE_ENTRY)pNextModule;

	while (pDataTableEntry->DllBase != NULL)
	{
		dwModuleHash = 0;
		pModuleBase = pDataTableEntry->DllBase;
		BaseDllName = pDataTableEntry->BaseDllName;
		pNTHeader = (PIMAGE_NT_HEADERS)((ULONG_PTR)pModuleBase + ((PIMAGE_DOS_HEADER)pModuleBase)->e_lfanew);
		dwExportDirRVA = pNTHeader->OptionalHeader.DataDirectory[0].VirtualAddress;
		dwExportSize = pNTHeader->OptionalHeader.DataDirectory[0].Size; //zhenwo
																		// Get the next loaded module entry
		pDataTableEntry = (PMY_LDR_DATA_TABLE_ENTRY)pDataTableEntry->InLoadOrderLinks.Flink;

		// If the current module does not export any functions, move on to the next module.
		if (dwExportDirRVA == 0)
		{
			continue;
		}

		// Calculate the module hash
		for (i = 0; i < BaseDllName.MaximumLength; i++)
		{
			pTempChar = ((PCSTR)BaseDllName.Buffer + i);

			dwModuleHash = ROTR32(dwModuleHash, 13);

			if (*pTempChar >= 0x61)
			{
				dwModuleHash += *pTempChar - 0x20;
			}
			else
			{
				dwModuleHash += *pTempChar;
			}
		}

		pExportDir = (PIMAGE_EXPORT_DIRECTORY)((ULONG_PTR)pModuleBase + dwExportDirRVA);

		dwNumFunctions = pExportDir->NumberOfNames;
		pdwFunctionNameBase = (PDWORD)((PCHAR)pModuleBase + pExportDir->AddressOfNames);

		for (i = 0; i < dwNumFunctions; i++)
		{
			dwFunctionHash = 0;
			pFunctionName = (PCSTR)(*pdwFunctionNameBase + (ULONG_PTR)pModuleBase);
			pdwFunctionNameBase++;

			pTempChar = pFunctionName;

			do
			{
				dwFunctionHash = ROTR32(dwFunctionHash, 13);
				dwFunctionHash += *pTempChar;
				pTempChar++;
			} while (*(pTempChar - 1) != 0);

			dwFunctionHash += dwModuleHash;

			if (dwFunctionHash == dwModuleFunctionHash)
			{
				usOrdinalTableIndex = *(PUSHORT)(((ULONG_PTR)pModuleBase + pExportDir->AddressOfNameOrdinals) + (2 * i));
				pFunction = (PVOID)((ULONG_PTR)pModuleBase + *(PDWORD)(((ULONG_PTR)pModuleBase + pExportDir->AddressOfFunctions) + (4 * usOrdinalTableIndex)));
				if (pFunction < pExportDir || pFunction >((PBYTE)pExportDir + dwExportSize))
				{
					return pFunction;
				}
				else
				{
					k = 0;
					// copy
					while (((LPSTR)pFunction)[k] != '.')
					{
						szModuleName[k] = ((LPSTR)pFunction)[k];
						k++;
					}

					// calc Function Hash
					dwFunctionHash = 0;
					pTempChar = &((LPSTR)pFunction)[k + 1];
					do
					{
						dwFunctionHash = ROTR32(dwFunctionHash, 13);
						dwFunctionHash += *pTempChar;
						pTempChar++;
					} while (*(pTempChar - 1) != 0);
					// wcscat Module name
					j = 0;
					while (((LPWSTR)dwDotDll)[j])
					{
						szModuleName[k] = ((LPWSTR)dwDotDll)[j];
						k++;
						j++;
					}
					szModuleName[k] = 0;
					// calc Module hash
					dwModuleHash = 0;
					for (j = 0; j < (k + 1) * 2; j++)
					{
						pTempChar = ((PCSTR)szModuleName + j);

						dwModuleHash = ROTR32(dwModuleHash, 13);

						if (*pTempChar >= 0x61)
						{
							dwModuleHash += *pTempChar - 0x20;
						}
						else
						{
							dwModuleHash += *pTempChar;
						}
					}
					// Calc Hash
					dwModuleFunctionHash = dwModuleHash + dwFunctionHash;
					
					pfnLoadLib = scx_GetProcAddressWithHash(HASH_LoadLibraryW);
					pfnLoadLib(szModuleName);
					return scx_GetProcAddressWithHash(dwModuleFunctionHash);


				}
			}
		}
	}

	// All modules have been exhausted and the function was not found.
	return NULL;
}