//========== Copyleft © 2011, Team Sandbox, Some rights reserved. ===========//
//
// Purpose:
//
//===========================================================================//

#include "stdafx.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

volatile bool g_bInitializedAsPlugin = false;
volatile bool g_bExit = false;
char g_PluginFolder[ MAX_PATH ];

HANDLE hThread = NULL;
HMODULE hmPlugin = NULL;
HWND hMainWindow = NULL;

void DllInit()
{
	Msg( "serverplugin_inject.dll loaded.\n" );
	Warning( "WARNING: Attempts to unload this plugin through plugin_unload will result in a crash!\n" );
}

void DllUnload()
{
	Msg( "Unloading serverplugin_inject...\n" );
}

void DllShutdown()
{
    if ( g_bInitializedAsPlugin )
    {
		Warning( "WARNING: serverplugin_inject.dll attempted to unload outside of plugin_unload!\n" );
        return;
    }

	//Andrew; call our unload code here when attempting to unload outside of the game.
	DllUnload();

    g_bExit = true;
}
