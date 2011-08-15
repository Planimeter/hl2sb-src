//========== Copyleft © 2011, Team Sandbox, Some rights reserved. ===========//
//
// Purpose:
//
//===========================================================================//

#include "stdafx.h"
#include "serverplugin_skeleton.h"

namespace Plugin
{
    void *CreateInterface( const char *pName, int *pReturnCode )
    {
        if ( strcmp( pName, INTERFACEVERSION_ISERVERPLUGINCALLBACKS ) == 0 )
			return &CEmptyServerPlugin::Plugin;
        else
			return NULL;
    }

    CEmptyServerPlugin CEmptyServerPlugin::Plugin;

	//---------------------------------------------------------------------------------
	// Purpose: called when the plugin is loaded, load the interface we need from the engine
	//---------------------------------------------------------------------------------
	bool CEmptyServerPlugin::Load(	CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory )
	{
        g_bInitializedAsPlugin = true;
        return true;
	}

	//---------------------------------------------------------------------------------
	// Purpose: called when the plugin is unloaded (turned off)
	//---------------------------------------------------------------------------------
	void CEmptyServerPlugin::Unload( void )
	{
        g_bInitializedAsPlugin = false;
        DllShutdown();
        WaitForSingleObject( hThread, 0 );
	}

	//---------------------------------------------------------------------------------
	// Purpose: called when the plugin is paused (i.e should stop running but isn't unloaded)
	//---------------------------------------------------------------------------------
	void CEmptyServerPlugin::Pause( void )
	{
	}

	//---------------------------------------------------------------------------------
	// Purpose: called when the plugin is unpaused (i.e should start executing again)
	//---------------------------------------------------------------------------------
	void CEmptyServerPlugin::UnPause( void )
	{
	}

	//---------------------------------------------------------------------------------
	// Purpose: the name of this plugin, returned in "plugin_print" command
	//---------------------------------------------------------------------------------
	const char *CEmptyServerPlugin::GetPluginDescription( void )
	{
		return "serverplugin_inject, Team Sandbox";
	}

	//---------------------------------------------------------------------------------
	// Purpose: called on level start
	//---------------------------------------------------------------------------------
	void CEmptyServerPlugin::LevelInit( char const *pMapName )
	{
	}

	//---------------------------------------------------------------------------------
	// Purpose: called on level start, when the server is ready to accept client connections
	//		edictCount is the number of entities in the level, clientMax is the max client count
	//---------------------------------------------------------------------------------
	void CEmptyServerPlugin::ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
	{
	}

	//---------------------------------------------------------------------------------
	// Purpose: called once per server frame, do recurring work here (like checking for timeouts)
	//---------------------------------------------------------------------------------
	void CEmptyServerPlugin::GameFrame( bool simulating )
	{
	}

	//---------------------------------------------------------------------------------
	// Purpose: called on level end (as the server is shutting down or going to a new map)
	//---------------------------------------------------------------------------------
	void CEmptyServerPlugin::LevelShutdown( void ) // !!!!this can get called multiple times per map change
	{
	}

	//---------------------------------------------------------------------------------
	// Purpose: called when a client spawns into a server (i.e as they begin to play)
	//---------------------------------------------------------------------------------
	void CEmptyServerPlugin::ClientActive( edict_t *pEntity )
	{
	}

	//---------------------------------------------------------------------------------
	// Purpose: called when a client leaves a server (or is timed out)
	//---------------------------------------------------------------------------------
	void CEmptyServerPlugin::ClientDisconnect( edict_t *pEntity )
	{
	}

	//---------------------------------------------------------------------------------
	// Purpose: called on 
	//---------------------------------------------------------------------------------
	void CEmptyServerPlugin::ClientPutInServer( edict_t *pEntity, char const *playername )
	{
	}

	//---------------------------------------------------------------------------------
	// Purpose: called on level start
	//---------------------------------------------------------------------------------
	void CEmptyServerPlugin::SetCommandClient( int index )
	{
		m_iClientCommandIndex = index;
	}

	//---------------------------------------------------------------------------------
	// Purpose: called on level start
	//---------------------------------------------------------------------------------
	void CEmptyServerPlugin::ClientSettingsChanged( edict_t *pEdict )
	{
	}

	//---------------------------------------------------------------------------------
	// Purpose: called when a client joins a server
	//---------------------------------------------------------------------------------
	PLUGIN_RESULT CEmptyServerPlugin::ClientConnect( bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen )
	{
		return PLUGIN_CONTINUE;
	}

	//---------------------------------------------------------------------------------
	// Purpose: called when a client types in a command (only a subset of commands however, not CON_COMMAND's)
	//---------------------------------------------------------------------------------
	PLUGIN_RESULT CEmptyServerPlugin::ClientCommand( edict_t *pEntity, const CCommand &args )
	{
		return PLUGIN_CONTINUE;
	}

	//---------------------------------------------------------------------------------
	// Purpose: called when a client is authenticated
	//---------------------------------------------------------------------------------
	PLUGIN_RESULT CEmptyServerPlugin::NetworkIDValidated( const char *pszUserName, const char *pszNetworkID )
	{
		return PLUGIN_CONTINUE;
	}

	//---------------------------------------------------------------------------------
	// Purpose: called when a cvar value query is finished
	//---------------------------------------------------------------------------------
	void CEmptyServerPlugin::OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue )
	{
	}
	void CEmptyServerPlugin::OnEdictAllocated( edict_t *edict )
	{
	}
	void CEmptyServerPlugin::OnEdictFreed( const edict_t *edict  )
	{
	}
};
