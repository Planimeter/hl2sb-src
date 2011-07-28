//===== Copyright © 1996-2008, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//
//===========================================================================//

#include <stdio.h>


#include <stdio.h>
#include "interface.h"
#include "filesystem.h"
#include "engine/iserverplugin.h"
#include "eiface.h"
#include "igameevents.h"
#include "convar.h"
#include "Color.h"
#include "vstdlib/random.h"
#include "engine/IEngineTrace.h"
#include "tier2/tier2.h"
#include "game/server/iplayerinfo.h"
#include "ivoiceserver.h"
#include "networkstringtabledefs.h"
#include "engine/IStaticPropMgr.h"
#include "engine/IEngineSound.h"
#include "ispatialpartition.h"
#include "engine/ivmodelinfo.h"
#include "datacache/idatacache.h"
#include "engine/ivdebugoverlay.h"
#include "SoundEmitterSystem/isoundemittersystembase.h"
#include "toolframework/iserverenginetools.h"
#include "scenefilecache/iscenefilecache.h"
#include "sigscan.h"
#include "basecombatcharacter.h"
#include "baseentity.h"
#include "baseentity_shared.h"
#include "mapentities.h"
#include "player.h"
#include "util.h"
#include "luamanager.h"
#include "lbaseentity_shared.h"
#include "lbaseplayer_shared.h"
#include "lColor.h"

// Uncomment this to compile the sample TF2 plugin code, note: most of this is duplicated in serverplugin_tony, but kept here for reference!
//#define SAMPLE_TF2_PLUGIN
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// Interfaces from the engine
IVEngineServer	*engine = NULL; // helper functions (messaging clients, loading content, making entities, running commands, etc)
IGameEventManager *gameeventmanager = NULL; // game events interface
IPlayerInfoManager *playerinfomanager = NULL; // game dll interface to interact with players
IServerPluginHelpers *helpers = NULL; // special 3rd party plugin helpers from the engine
IUniformRandomStream *randomStr = NULL;
IEngineTrace *enginetrace = NULL;
IVoiceServer	*g_pVoiceServer = NULL;
#if !defined(_STATIC_LINKED)
IFileSystem		*filesystem = NULL;
#else
extern IFileSystem *filesystem;
#endif
INetworkStringTableContainer *networkstringtable = NULL;
IStaticPropMgrServer *staticpropmgr = NULL;
IEngineSound *enginesound = NULL;
ISpatialPartition *partition = NULL;
IVModelInfo *modelinfo = NULL;
IDataCache *datacache = NULL;
IVDebugOverlay * debugoverlay = NULL;
ISoundEmitterSystemBase *soundemitterbase = NULL;
IServerEngineTools *serverenginetools = NULL;
ISceneFileCache *scenefilecache = NULL;



CGlobalVars *gpGlobals = NULL;

//---------------------------------------------------------------------------------
// Purpose: a sample 3rd party plugin class
//---------------------------------------------------------------------------------
class CSourceLua: public IServerPluginCallbacks, public IGameEventListener
{
public:
	CSourceLua();
	~CSourceLua();

	// IServerPluginCallbacks methods
	virtual bool			Load(	CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory );
	virtual void			Unload( void );
	virtual void			Pause( void );
	virtual void			UnPause( void );
	virtual const char     *GetPluginDescription( void );      
	virtual void			LevelInit( char const *pMapName );
	virtual void			ServerActivate( edict_t *pEdictList, int edictCount, int clientMax );
	virtual void			GameFrame( bool simulating );
	virtual void			LevelShutdown( void );
	virtual void			ClientActive( edict_t *pEntity );
	virtual void			ClientDisconnect( edict_t *pEntity );
	virtual void			ClientPutInServer( edict_t *pEntity, char const *playername );
	virtual void			SetCommandClient( int index );
	virtual void			ClientSettingsChanged( edict_t *pEdict );
	virtual PLUGIN_RESULT	ClientConnect( bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen );
	virtual PLUGIN_RESULT	ClientCommand( edict_t *pEntity, const CCommand &args );
	virtual PLUGIN_RESULT	NetworkIDValidated( const char *pszUserName, const char *pszNetworkID );
	virtual void			OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue );

	// added with version 3 of the interface.
	virtual void			OnEdictAllocated( edict_t *edict );
	virtual void			OnEdictFreed( const edict_t *edict  );	

	// IGameEventListener Interface
	virtual void FireGameEvent( KeyValues * event );

	virtual int GetCommandIndex() { return m_iClientCommandIndex; }
private:
	int m_iClientCommandIndex;
};


// 
// The plugin is a static singleton that is exported as an interface
//
CSourceLua g_SourceLua;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CSourceLua, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_SourceLua );

//---------------------------------------------------------------------------------
// Purpose: constructor/destructor
//---------------------------------------------------------------------------------
CSourceLua::CSourceLua()
{
	m_iClientCommandIndex = 0;
}

CSourceLua::~CSourceLua()
{
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is loaded, load the interface we need from the engine
//---------------------------------------------------------------------------------
bool CSourceLua::Load(	CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory )
{
	ConnectTier1Libraries( &interfaceFactory, 1 );
	ConnectTier2Libraries( &interfaceFactory, 1 );

	playerinfomanager = (IPlayerInfoManager *)gameServerFactory(INTERFACEVERSION_PLAYERINFOMANAGER,NULL);
	if ( !playerinfomanager )
	{
		Warning( "Unable to load playerinfomanager, ignoring\n" ); // this isn't fatal, we just won't be able to access specific player data
	}

	// init each (seperated for ease of debugging)
	engine = (IVEngineServer*)interfaceFactory(INTERFACEVERSION_VENGINESERVER, NULL);
	gameeventmanager = (IGameEventManager *)interfaceFactory(INTERFACEVERSION_GAMEEVENTSMANAGER,NULL);
	helpers = (IServerPluginHelpers*)interfaceFactory(INTERFACEVERSION_ISERVERPLUGINHELPERS, NULL);
	enginetrace = (IEngineTrace *)interfaceFactory(INTERFACEVERSION_ENGINETRACE_SERVER,NULL);
	randomStr = (IUniformRandomStream *)interfaceFactory(VENGINE_SERVER_RANDOM_INTERFACE_VERSION, NULL);
	if ( (g_pVoiceServer = (IVoiceServer*)interfaceFactory(INTERFACEVERSION_VOICESERVER, NULL)) == NULL )
		Warning( "Unable to load g_pVoiceServer, ignoring\n" );
	if ( (networkstringtable = (INetworkStringTableContainer *)interfaceFactory(INTERFACENAME_NETWORKSTRINGTABLESERVER,NULL)) == NULL )
		Warning( "Unable to load networkstringtable, ignoring\n" );
	if ( (staticpropmgr = (IStaticPropMgrServer *)interfaceFactory(INTERFACEVERSION_STATICPROPMGR_SERVER,NULL)) == NULL )
		Warning( "Unable to load staticpropmgr, ignoring\n" );
	if ( (enginesound = (IEngineSound *)interfaceFactory(IENGINESOUND_SERVER_INTERFACE_VERSION, NULL)) == NULL )
		Warning( "Unable to load enginesound, ignoring\n" );
	if ( (partition = (ISpatialPartition *)interfaceFactory(INTERFACEVERSION_SPATIALPARTITION, NULL)) == NULL )
		Warning( "Unable to load partition, ignoring\n" );
	if ( (modelinfo = (IVModelInfo *)interfaceFactory(VMODELINFO_SERVER_INTERFACE_VERSION, NULL)) == NULL )
		Warning( "Unable to load modelinfo, ignoring\n" );
	if ( (filesystem = (IFileSystem *)interfaceFactory(FILESYSTEM_INTERFACE_VERSION,NULL)) == NULL )
		Warning( "Unable to load filesystem, ignoring\n" );
	if ( (datacache = (IDataCache*)interfaceFactory(DATACACHE_INTERFACE_VERSION, NULL )) == NULL )
		Warning( "Unable to load datacache, ignoring\n" );
	if ( (soundemitterbase = (ISoundEmitterSystemBase *)interfaceFactory(SOUNDEMITTERSYSTEM_INTERFACE_VERSION, NULL)) == NULL )
		Warning( "Unable to load soundemitterbase, ignoring\n" );
	if ( (scenefilecache = (ISceneFileCache *)interfaceFactory( SCENE_FILE_CACHE_INTERFACE_VERSION, NULL )) == NULL )
		Warning( "Unable to load scenefilecache, ignoring\n" );

	// try to get debug overlay, may be NULL if on HLDS
	if ( (debugoverlay = (IVDebugOverlay *)interfaceFactory( VDEBUG_OVERLAY_INTERFACE_VERSION, NULL )) == NULL )
		Warning( "Unable to load debugoverlay, ignoring\n" );

	// get the interfaces we want to use
	if(	! ( engine && gameeventmanager && g_pFullFileSystem && helpers && enginetrace && randomStr ) )
	{
		return false; // we require all these interface to function
	}

	// If not running dedicated, grab the engine vgui interface
	if ( !engine->IsDedicatedServer() )
	{
#ifdef _WIN32
		// This interface is optional, and is only valid when running with -tools
		if ( (serverenginetools = ( IServerEngineTools * )interfaceFactory( VSERVERENGINETOOLS_INTERFACE_VERSION, NULL )) == NULL )
			Warning( "Unable to load serverenginetools, ignoring\n" );
#endif
	}

	if ( playerinfomanager )
	{
		gpGlobals = playerinfomanager->GetGlobalVars();
	}

#ifndef DISABLE_SIGNATURE_SCANNING
	CSigScan::sigscan_dllfunc = gameServerFactory;
	if ( !CSigScan::GetDllMemInfo() )
	{
		return false; // we require this information to function
	}

	siginit_CBaseCombatCharacter();
	siginit_CBaseEntity();
	siginit_CBaseEntity_shared();
	siginit_mapentities();
	siginit_CBasePlayer();
	siginit_UTIL();
#endif

	MathLib_Init( 2.2f, 2.2f, 0.0f, 2.0f );
	ConVar_Register( 0 );
	gameeventmanager->AddListener( this, true );

	return true;
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is unloaded (turned off)
//---------------------------------------------------------------------------------
void CSourceLua::Unload( void )
{
	BEGIN_LUA_CALL_HOOK( "Unload" );
	END_LUA_CALL_HOOK( 0, 0 );

	gameeventmanager->RemoveListener( this ); // make sure we are unloaded from the event system

	ConVar_Unregister( );
	DisconnectTier2Libraries( );
	DisconnectTier1Libraries( );
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is paused (i.e should stop running but isn't unloaded)
//---------------------------------------------------------------------------------
void CSourceLua::Pause( void )
{
	BEGIN_LUA_CALL_HOOK( "Pause" );
	END_LUA_CALL_HOOK( 0, 0 );
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is unpaused (i.e should start executing again)
//---------------------------------------------------------------------------------
void CSourceLua::UnPause( void )
{
	BEGIN_LUA_CALL_HOOK( "UnPause" );
	END_LUA_CALL_HOOK( 0, 0 );
}

//---------------------------------------------------------------------------------
// Purpose: the name of this plugin, returned in "plugin_print" command
//---------------------------------------------------------------------------------
const char *CSourceLua::GetPluginDescription( void )
{
	return "SourceLua, Team Sandbox";
}

//---------------------------------------------------------------------------------
// Purpose: called on level start
//---------------------------------------------------------------------------------
void CSourceLua::LevelInit( char const *pMapName )
{
	luasrc_init();

	luasrc_dofolder( LUA_PATH_ENUM );
	luasrc_dofolder( LUA_PATH_MODULES );
	luasrc_dofolder( LUA_PATH_INCLUDES );
	luasrc_dofolder( LUA_PATH_AUTORUN );
	luasrc_dofolder( LUA_PATH_AUTORUN_SERVER );

	BEGIN_LUA_CALL_HOOK( "LevelInit" );
		lua_pushstring( L, pMapName );
	END_LUA_CALL_HOOK( 1, 0 );
}

//---------------------------------------------------------------------------------
// Purpose: called on level start, when the server is ready to accept client connections
//		edictCount is the number of entities in the level, clientMax is the max client count
//---------------------------------------------------------------------------------
void CSourceLua::ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
}

//---------------------------------------------------------------------------------
// Purpose: called once per server frame, do recurring work here (like checking for timeouts)
//---------------------------------------------------------------------------------
void CSourceLua::GameFrame( bool simulating )
{
	BEGIN_LUA_CALL_HOOK( "GameFrame" );
		lua_pushboolean( L, simulating );
	END_LUA_CALL_HOOK( 1, 0 );
}

//---------------------------------------------------------------------------------
// Purpose: called on level end (as the server is shutting down or going to a new map)
//---------------------------------------------------------------------------------
void CSourceLua::LevelShutdown( void ) // !!!!this can get called multiple times per map change
{
	BEGIN_LUA_CALL_HOOK( "LevelShutdown" );
	END_LUA_CALL_HOOK( 0, 0 );

	luasrc_shutdown( );
}

//---------------------------------------------------------------------------------
// Purpose: called when a client spawns into a server (i.e as they begin to play)
//---------------------------------------------------------------------------------
void CSourceLua::ClientActive( edict_t *pEntity )
{
	BEGIN_LUA_CALL_HOOK( "ClientActive" );
		CBasePlayer *pPlayer = (CBasePlayer *)CBaseEntity_Instance( pEntity );
		lua_pushplayer( L, pPlayer );
	END_LUA_CALL_HOOK( 1, 0 );
}

//---------------------------------------------------------------------------------
// Purpose: called when a client leaves a server (or is timed out)
//---------------------------------------------------------------------------------
void CSourceLua::ClientDisconnect( edict_t *pEntity )
{
	BEGIN_LUA_CALL_HOOK( "ClientDisconnect" );
		CBasePlayer *pPlayer = (CBasePlayer *)CBaseEntity_Instance( pEntity );
		lua_pushplayer( L, pPlayer );
	END_LUA_CALL_HOOK( 1, 0 );
}

//---------------------------------------------------------------------------------
// Purpose: called on 
//---------------------------------------------------------------------------------
void CSourceLua::ClientPutInServer( edict_t *pEntity, char const *playername )
{
	BEGIN_LUA_CALL_HOOK( "ClientPutInServer" );
		CBasePlayer *pPlayer = (CBasePlayer *)CBaseEntity_Instance( pEntity );
		lua_pushplayer( L, pPlayer );
		lua_pushstring( L, playername );
	END_LUA_CALL_HOOK( 2, 0 );
}

//---------------------------------------------------------------------------------
// Purpose: called on level start
//---------------------------------------------------------------------------------
void CSourceLua::SetCommandClient( int index )
{
	m_iClientCommandIndex = index;
}

//---------------------------------------------------------------------------------
// Purpose: called on level start
//---------------------------------------------------------------------------------
void CSourceLua::ClientSettingsChanged( edict_t *pEdict )
{
	BEGIN_LUA_CALL_HOOK( "ClientSettingsChanged" );
		CBasePlayer *pPlayer = (CBasePlayer *)CBaseEntity_Instance( pEdict );
		lua_pushplayer( L, pPlayer );
	END_LUA_CALL_HOOK( 1, 0 );
}

//---------------------------------------------------------------------------------
// Purpose: called when a client joins a server
//---------------------------------------------------------------------------------
PLUGIN_RESULT CSourceLua::ClientConnect( bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen )
{
	BEGIN_LUA_CALL_HOOK( "ClientConnect" );
		lua_pushboolean( L, (int)bAllowConnect );
		CBasePlayer *pPlayer = (CBasePlayer *)CBaseEntity_Instance( pEntity );
		lua_pushplayer( L, pPlayer );
		lua_pushstring( L, pszName );
		lua_pushstring( L, pszAddress );
		lua_pushstring( L, reject );
		lua_pushinteger( L, maxrejectlen );
	END_LUA_CALL_HOOK( 6, 0 );

	return PLUGIN_CONTINUE;
}

//---------------------------------------------------------------------------------
// Purpose: called when a client types in a command (only a subset of commands however, not CON_COMMAND's)
//---------------------------------------------------------------------------------
PLUGIN_RESULT CSourceLua::ClientCommand( edict_t *pEntity, const CCommand &args )
{
	return PLUGIN_CONTINUE;
}

//---------------------------------------------------------------------------------
// Purpose: called when a client is authenticated
//---------------------------------------------------------------------------------
PLUGIN_RESULT CSourceLua::NetworkIDValidated( const char *pszUserName, const char *pszNetworkID )
{
	BEGIN_LUA_CALL_HOOK( "NetworkIDValidated" );
		lua_pushstring( L, pszUserName );
		lua_pushstring( L, pszNetworkID );
	END_LUA_CALL_HOOK( 2, 0 );

	return PLUGIN_CONTINUE;
}

//---------------------------------------------------------------------------------
// Purpose: called when a cvar value query is finished
//---------------------------------------------------------------------------------
void CSourceLua::OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue )
{
	BEGIN_LUA_CALL_HOOK( "OnQueryCvarValueFinished" );
		lua_pushinteger( L, iCookie );
		CBasePlayer *pPlayer = (CBasePlayer *)CBaseEntity_Instance( pPlayerEntity );
		lua_pushplayer( L, pPlayer );
		lua_pushinteger( L, eStatus );
		lua_pushstring( L, pCvarName );
		lua_pushstring( L, pCvarValue );
	END_LUA_CALL_HOOK( 5, 0 );
}
void CSourceLua::OnEdictAllocated( edict_t *edict )
{
	BEGIN_LUA_CALL_HOOK( "OnEdictAllocated" );
		CBaseEntity *pEntity = CBaseEntity_Instance( edict );
		lua_pushentity( L, pEntity );
	END_LUA_CALL_HOOK( 1, 0 );
}
void CSourceLua::OnEdictFreed( const edict_t *edict  )
{
}

//---------------------------------------------------------------------------------
// Purpose: called when an event is fired
//---------------------------------------------------------------------------------
void CSourceLua::FireGameEvent( KeyValues * event )
{
	BEGIN_LUA_CALL_HOOK( event->GetName() );
	for ( KeyValues *pValue = event->GetFirstValue(); pValue; pValue = pValue->GetNextValue() )
	{
		switch( pValue->GetDataType() )
		{
		case KeyValues::TYPE_NONE:
			break;
		case KeyValues::TYPE_STRING:
			lua_pushstring( L, pValue->GetString() );
			break;
		case KeyValues::TYPE_INT:
			lua_pushinteger( L, pValue->GetInt() );
			break;
		case KeyValues::TYPE_FLOAT:
			lua_pushnumber( L, pValue->GetFloat() );
			break;
		case KeyValues::TYPE_PTR:
			break;
		case KeyValues::TYPE_UINT64:
			lua_pushinteger( L, pValue->GetUint64() );
			break;
		case KeyValues::TYPE_COLOR:
			{
				Color clr = pValue->GetColor();
				lua_pushcolor( L, &clr );
			}
			break;
			
		default:
			{
				// do nothing . .what the heck is this?
				Assert( 0 );
			}
			break;
		}

		args++;
	}
	END_LUA_CALL_HOOK( 0, 0 );
}

//---------------------------------------------------------------------------------
// Purpose: prints the version of SourceLua
//---------------------------------------------------------------------------------
CON_COMMAND( lua_version, "prints the version of SourceLua" )
{
	Msg( "Version:2.0.0.0\n" );
}

CON_COMMAND( lua_log, "logs the version of SourceLua" )
{
	engine->LogPrint( "Version:2.0.0.0\n" );
}

//---------------------------------------------------------------------------------
// Purpose: sets the SourceLua path
//---------------------------------------------------------------------------------
static ConVar lua_path("lua_path", "addons\\sourcelua", 0, "SourceLua path");
