//========== Copyleft © 2011, Team Sandbox, Some rights reserved. ===========//
//
// Purpose:
//
//===========================================================================//

#include "cbase.h"
#include "ticketfix.h"

#if defined(_WIN32)
#pragma warning(disable : 4005)
#include <windows.h>
#endif 

bool GetEngineBinaryInfo(void** pEngineBase, unsigned int& cubEngineSize)
{
#if defined(_WIN32)
	HMODULE hEngine = GetModuleHandleA("engine.dll");
	ASSERT(hEngine != NULL);

	IMAGE_DOS_HEADER* pDOSHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(hEngine);
	if( pDOSHeader == NULL )
		return false;

	ASSERT(pDOSHeader->e_magic == IMAGE_DOS_SIGNATURE);

	IMAGE_NT_HEADERS* pNTHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(pDOSHeader->e_lfanew + (LONG)pDOSHeader);
	if( pNTHeaders == NULL )
		return false;

	ASSERT( pNTHeaders->Signature == IMAGE_NT_SIGNATURE );

	if( pEngineBase )
		*pEngineBase = reinterpret_cast<void*>(hEngine);

	cubEngineSize = pNTHeaders->OptionalHeader.SizeOfImage;

	return true;
#else
	// TODO: Implement me when necessary.
	return false;
#endif
}

bool PatchTicketValidation()
{
	const unsigned char *pEngineBase = NULL;
	unsigned int cubEngineSize = 0;

	if( !GetEngineBinaryInfo((void**)&pEngineBase, cubEngineSize) )
		return false;

	// 8B442404BA010000003991C8000000751566895068C7407000000000C740

	const unsigned char cubPattern[31] = 
		"\x8B\x44\x24\x04\xBA\x01\x00\x00\x00\x39\x91\xC8\x00\x00\x00\x75\x15\x66\x89\x50\x68\xC7\x40\x70\x00\x00\x00\x00\xC7\x40";

	for(unsigned int n = 0; n < cubEngineSize; n++)
	{
		if( memcmp((void*)(pEngineBase + n), cubPattern, 30) == 0 )
		{
#if defined(_DEBUG)
			DevMsg("Patching corrupted ticket validation: 0x%p\n", pEngineBase + n);
#endif

			DWORD dwOldProtect;
			VirtualProtect((LPVOID)(pEngineBase + n + 15), 2, PAGE_EXECUTE_READWRITE, &dwOldProtect);

			memcpy((void*)(pEngineBase + n + 15), "\x90\x90", 2);

			VirtualProtect((LPVOID)(pEngineBase + n + 15), 2, dwOldProtect, &dwOldProtect);

			return true;
		}
	}

	return false;
}

