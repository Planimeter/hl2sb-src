//========== Copyleft © 2011, Team Sandbox, Some rights reserved. ===========//
//
// Purpose:
//
//===========================================================================//

#include "stdafx.h"
#include "serverplugin_inject.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

DWORD WINAPI InitThread( LPVOID param )
{
    GetModuleFileNameA( hmPlugin, g_PluginFolder, MAX_PATH );

    char* pChar = NULL; 
    for ( char* ptr = g_PluginFolder; *ptr; ++ptr ) 
    { 
        if ( *ptr == '\\' ) pChar = ptr + 1; 
    } 
    *pChar = '\0'; 

    DllInit();

	while ( !g_bExit )
    {
        Sleep( 250 );
        if ( GetAsyncKeyState( VK_DELETE ) && GetAsyncKeyState( VK_CONTROL ) )
			DllShutdown();
    }

    if ( g_bInitializedAsPlugin )
		return 0;
    else
		FreeLibraryAndExitThread( hmPlugin, 0 );
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD dwReason, LPVOID lpReserved )
{
    if ( dwReason == DLL_PROCESS_ATTACH )
    {
        hmPlugin = hModule;
        DisableThreadLibraryCalls( hModule );
        hThread = CreateThread( NULL, 0, &InitThread, NULL, 0, NULL );
    }
    return TRUE;
}
