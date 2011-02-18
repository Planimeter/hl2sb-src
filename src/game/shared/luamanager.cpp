//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: Contains the implementation of Lua for scripting.
//
//===========================================================================//

#include "cbase.h"
#include "filesystem.h"
#ifndef CLIENT_DLL
#include "gameinterface.h"
#endif
#include "steam/isteamfriends.h"
#include "networkstringtabledefs.h"
#include "weapon_hl2mpbase_scriptedweapon.h"
#include "luamanager.h"
#ifdef CLIENT_DLL
#include "lc_baseanimating.h"
#include "lcdll_int.h"
#include "lcdll_util.h"
#include "vgui/LISurface.h"
#include "vgui_controls/lPanel.h"
#else
#include "lbaseanimating.h"
#include "lbaseentity.h"
#include "leiface.h"
#include "linetchannelinfo.h"
#include "lutil.h"
#endif
#include "lbasecombatweapon_shared.h"
#include "lbaseentity_shared.h"
#include "lbaseplayer_shared.h"
#include "lColor.h"
#include "lconvar.h"
#include "ldbg.h"
#include "leffect_dispatch_data.h"
#include "lfilesystem.h"
#include "lgametrace.h"
#include "lglobalvars_base.h"
#include "licvar.h"
#include "materialsystem/limaterial.h"
#include "steam/listeamfriends.h"
#include "engine/livdebugoverlay.h"
#include "lnetworkstringtabledefs.h"
#include "vstdlib/lrandom.h"
#include "steam/lsteam_api.h"
#include "ltakedamageinfo.h"
#include "lutil_shared.h"
#include "mathlib/lvector.h"
#include "lvphysics_interface.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar sv_gamemode( "sv_gamemode", "sandbox", FCVAR_ARCHIVE | FCVAR_REPLICATED );

static void tag_error (lua_State *L, int narg, int tag) {
  luaL_typerror(L, narg, lua_typename(L, tag));
}


LUALIB_API int luaL_checkboolean (lua_State *L, int narg) {
  int d = lua_toboolean(L, narg);
  if (d == 0 && !lua_isboolean(L, narg))  /* avoid extra test when d is not 0 */
    tag_error(L, narg, LUA_TBOOLEAN);
  return d;
}


LUALIB_API int luaL_optboolean (lua_State *L, int narg,
                                              int def) {
  return luaL_opt(L, luaL_checkboolean, narg, def);
}


lua_State *L;

// Lua system
bool g_bLuaInitialized;

static int luasrc_print (lua_State *L) {
  int n = lua_gettop(L);  /* number of arguments */
  int i;
  lua_getglobal(L, "tostring");
  for (i=1; i<=n; i++) {
    const char *s;
    lua_pushvalue(L, -1);  /* function to be called */
    lua_pushvalue(L, i);   /* value to print */
    lua_call(L, 1, 1);
    s = lua_tostring(L, -1);  /* get result */
    if (s == NULL)
      return luaL_error(L, LUA_QL("tostring") " must return a string to "
                           LUA_QL("print"));
    if (i>1) Msg("\t");
    Msg(s);
    lua_pop(L, 1);  /* pop result */
  }
  Msg("\n");
  return 0;
}


static int luasrc_type (lua_State *L) {
  luaL_checkany(L, 1);
  if (lua_getmetatable(L, 1)) {
    lua_pushstring(L, "__type");
	lua_rawget(L, -2);
	lua_remove(L, -2);
	if (!lua_isstring(L, -1))
	  lua_pop(L, 1);
	else
	  return 1;
  }
  lua_pushstring(L, luaL_typename(L, 1));
  return 1;
}


static int luasrc_include (lua_State *L) {
  lua_Debug ar1;
  lua_getstack(L, 1, &ar1);
  lua_getinfo(L, "f", &ar1);
  lua_Debug ar2;
  lua_getinfo(L, ">S", &ar2);
  int iLength = Q_strlen( ar2.source );
  char source[MAX_PATH];
  Q_StrRight( ar2.source, iLength-1, source, sizeof( source ) );
  Q_StripFilename( source );
  char filename[MAX_PATH];
  Q_snprintf( filename, sizeof( filename ), "%s\\%s", source, luaL_checkstring(L, 1) );
  luasrc_dofile(L, filename);
  return 0;
}


static const luaL_Reg base_funcs[] = {
  {"print", luasrc_print},
  {"type", luasrc_type},
  {"include", luasrc_include},
  {NULL, NULL}
};


static void base_open (lua_State *L) {
  /* set global _R */
  lua_pushvalue(L, LUA_REGISTRYINDEX);
  lua_setglobal(L, "_R");
  /* open lib into global table */
  luaL_register(L, "_G", base_funcs);
  lua_pop(L, 1);
#ifdef CLIENT_DLL
  lua_pushboolean(L, 1);
  lua_setglobal(L, "CLIENT_LUA");  /* set global CLIENT */
  lua_pushboolean(L, 0);
  lua_setglobal(L, "GAME_LUA");  /* set global SERVER */
#else
  lua_pushboolean(L, 0);
  lua_setglobal(L, "CLIENT_LUA");  /* set global CLIENT */
  lua_pushboolean(L, 1);
  lua_setglobal(L, "GAME_LUA");  /* set global SERVER */
#endif
}


void luasrc_setmodulepaths(lua_State *L) {
  lua_getglobal(L, LUA_LOADLIBNAME);
#ifdef CLIENT_DLL
	const char *gamePath = engine->GetGameDirectory();
#else
	char gamePath[ 256 ];
	engine->GetGameDir( gamePath, 256 );
#endif

  //Andrew; set package.cpath.
  lua_getfield(L, -1, "cpath");
  char lookupCPath[MAX_PATH];
  Q_snprintf( lookupCPath, sizeof( lookupCPath ), "%s\\%s;%s", gamePath,
#ifdef _WIN32
    LUA_PATH_MODULES "\\?.dll",
#elif _LINUX
    LUA_PATH_MODULES "\\?.so",
#endif
	luaL_checkstring(L, -1) );
  Q_strlower( lookupCPath );
  Q_FixSlashes( lookupCPath );
  lua_pop(L, 1);  /* pop result */
  lua_pushstring(L, lookupCPath);
  lua_setfield(L, -2, "cpath");

  //Andrew; set package.path.
  lua_getfield(L, -1, "path");
  char lookupPath[MAX_PATH];
  Q_snprintf( lookupPath, sizeof( lookupPath ), "%s\\%s;%s", gamePath, LUA_PATH_MODULES "\\?.lua", luaL_checkstring(L, -1) );
  Q_strlower( lookupPath );
  Q_FixSlashes( lookupPath );
  lua_pop(L, 1);  /* pop result */
  lua_pushstring(L, lookupPath);
  lua_setfield(L, -2, "path");

  lua_pop(L, 1);  /* pop result */
}

void luasrc_init (void) {
  if (g_bLuaInitialized)
	  return;
  g_bLuaInitialized = true;

  L = lua_open();

  luaL_openlibs(L);
  base_open(L);

  // Andrew; Someone set us up the path for great justice
  luasrc_setmodulepaths(L);

  luaopen_CBaseAnimating(L);
  luaopen_engine(L);
  luaopen_CBaseCombatWeapon(L);
#ifdef GAME_DLL
  luaopen_CBaseEntity(L);
#endif
  luaopen_CBaseEntity_shared(L);
  luaopen_CBasePlayer(L);
  luaopen_Color(L);
  luaopen_ConCommand(L);
  luaopen_ConVar(L);
  luaopen_dbg(L);
  luaopen_CEffectData(L);
  luaopen_filesystem(L);
  luaopen_CGameTrace(L);
  luaopen_gpGlobals(L);
  luaopen_cvar(L);
  luaopen_IMaterial(L);
  luaopen_ISteamFriends(L);
#ifdef GAME_DLL
  luaopen_INetChannelInfo(L);
#endif
  luaopen_debugoverlay(L);
  luaopen_INetworkStringTable(L);
  luaopen_networkstringtable(L);
  luaopen_random(L);
  luaopen_steamapicontext(L);
  luaopen_CTakeDamageInfo(L);
#ifdef CLIENT_DLL
  luaopen_surface(L);
  luaopen_Panel(L);
#endif
  luaopen_UTIL(L);
  luaopen_UTIL_shared(L);
  luaopen_Vector(L);
  luaopen_QAngle(L);
  luaopen_physenv(L);
  luaopen_IPhysicsObject(L);

  Msg( "Lua initialized (" LUA_VERSION ")\n" );
}

void luasrc_shutdown (void) {
  if (!g_bLuaInitialized)
	  return;

  g_bLuaInitialized = false;

  ResetConCommandDatabase();

  RemoveGlobalChangeCallbacks();
  ResetConVarDatabase();

#ifndef CLIENT_DLL
  ResetEntityFactoryDatabase();
#endif

  lua_close(L);
}

LUA_API int luasrc_dostring (lua_State *L, const char *string) {
  int iError = luaL_dostring(L, string);
  if (iError != 0) {
    Warning( "%s\n", lua_tostring(L, -1) );
    lua_pop(L, 1);
  }
  return iError;
}

LUA_API int luasrc_dofile (lua_State *L, const char *filename) {
  int iError = luaL_dofile(L, filename);
  if (iError != 0) {
    Warning( "%s\n", lua_tostring(L, -1) );
    lua_pop(L, 1);
  }
  return iError;
}

LUA_API void luasrc_dofolder (lua_State *L, const char *path)
{
	FileFindHandle_t fh;

	char searchPath[ 512 ];
	Q_snprintf( searchPath, sizeof( searchPath ), "%s\\*.lua", path );

#ifdef CLIENT_DLL
	const char *gamePath = engine->GetGameDirectory();
#else
	char gamePath[ 256 ];
	engine->GetGameDir( gamePath, 256 );
#endif

	char const *fn = g_pFullFileSystem->FindFirstEx( searchPath, "MOD", &fh );
	if ( fn )
	{
		do
		{
			if ( fn[0] != '.' )
			{
				char ext[ 10 ];
				Q_ExtractFileExtension( fn, ext, sizeof( ext ) );

				if ( !Q_stricmp( ext, "lua" ) )
				{
					char loadname[ 512 ];
					Q_snprintf( loadname, sizeof( loadname ), "%s\\%s\\%s", gamePath, path, fn );
					luasrc_dofile( L, loadname );
				}
			}

			fn = g_pFullFileSystem->FindNext( fh );

		} while ( fn );

		g_pFullFileSystem->FindClose( fh );
	}
}

LUA_API int luasrc_pcall (lua_State *L, int nargs, int nresults, int errfunc) {
  int iError = lua_pcall(L, nargs, nresults, errfunc);
  if (iError != 0) {
	Warning( "%s\n", lua_tostring(L, -1) );
	lua_pop(L, 1);
  }
  return iError;
}

void luasrc_LoadWeapons (void)
{
	FileFindHandle_t fh;

	char filename[ MAX_PATH ] = { 0 };
	char fullpath[ MAX_PATH ] = { 0 };
	char className[ MAX_WEAPON_STRING ] = { 0 };

	char const *fn = g_pFullFileSystem->FindFirstEx( LUA_PATH_WEAPONS "\\*", "MOD", &fh );
	if ( fn )
	{
		do
		{
			Q_strcpy( className, fn );
			Q_strlower( className );
			if ( fn[0] != '.' )
			{
				if ( g_pFullFileSystem->FindIsDirectory( fh ) )
				{
#ifdef CLIENT_DLL
					Q_snprintf( filename, sizeof( filename ), LUA_PATH_WEAPONS "\\%s\\cl_init.lua", className );
#else
					Q_snprintf( filename, sizeof( filename ), LUA_PATH_WEAPONS "\\%s\\init.lua", className );
#endif
					if ( filesystem->FileExists( filename, "MOD" ) )
					{
						filesystem->RelativePathToFullPath( filename, "MOD", fullpath, sizeof( fullpath ) );
						lua_newtable( L );
						char path[ MAX_PATH ];
						Q_snprintf( path, sizeof( path ), "weapons\\%s", className );
						lua_pushstring( L, path );
						lua_setfield( L, -2, "Folder" );
						lua_pushstring( L, LUA_BASE_WEAPON );
						lua_setfield( L, -2, "Base" );
						lua_newtable( L );
						lua_setfield( L, -2, "Primary" );
						lua_newtable( L );
						lua_setfield( L, -2, "Secondary" );
						lua_setglobal( L, "SWEP" );
						if ( luasrc_dofile( L, fullpath ) == 0 )
						{
							lua_getglobal( L, "weapon" );
							if ( lua_istable( L, -1 ) )
							{
								lua_getfield( L, -1, "Register" );
								if ( lua_isfunction( L, -1 ) )
								{
									lua_remove( L, -2 );
									lua_getglobal( L, "SWEP" );
									lua_pushstring( L, className );
									luasrc_pcall( L, 2, 0, 0 );
									RegisterScriptedWeapon( className );
								}
								else
								{
									lua_pop( L, 2 );
								}
							}
							else
							{
								lua_pop( L, 1 );
							}
						}
						lua_pushnil( L );
						lua_setglobal( L, "SWEP" );
					}
				}
			}

			fn = g_pFullFileSystem->FindNext( fh );

		} while ( fn );

		g_pFullFileSystem->FindClose( fh );
	}
}

bool luasrc_LoadGamemode (const char *gamemode) {
  lua_newtable(L);
  lua_pushstring(L, "Folder");
  char gamemodepath[MAX_PATH];
  Q_snprintf( gamemodepath, sizeof( gamemodepath ), "gamemodes\\%s", gamemode );
  lua_pushstring(L, gamemodepath);
  lua_settable(L, -3);
  lua_setglobal(L, "GM");
  char filename[MAX_PATH];
  char fullpath[MAX_PATH];
#ifdef CLIENT_DLL
  Q_snprintf( filename, sizeof( filename ), "%s\\gamemode\\cl_init.lua", gamemodepath );
#else
  Q_snprintf( filename, sizeof( filename ), "%s\\gamemode\\init.lua", gamemodepath );
#endif
  if ( filesystem->FileExists( filename, "MOD" ) )
  {
    filesystem->RelativePathToFullPath( filename, "MOD", fullpath, sizeof( fullpath ) );
	if (luasrc_dofile(L, fullpath) == 0) {
	  lua_getglobal(L, "gamemode");
	  lua_getfield(L, -1, "Register");
	  lua_remove(L, -2);
	  lua_getglobal(L, "GM");
	  lua_pushstring(L, gamemode);
	  lua_getfield(L, -2, "Base");
	  if (lua_isnoneornil(L, -1) && Q_strcmp(gamemode, LUA_BASE_GAMEMODE) != 0) {
	    lua_pop(L, 1);
		lua_pushstring(L, LUA_BASE_GAMEMODE);
	  }
	  luasrc_pcall(L, 3, 0, 0);
	  lua_pushnil(L);
	  lua_setglobal(L, "GM");
	  return true;
	}
	else
	{
	  lua_pushnil(L);
	  lua_setglobal(L, "GM");
	  Warning( "ERROR: Attempted to load an invalid gamemode!\n" );
	  return false;
	}
  }
  else
  {
    lua_pushnil(L);
	lua_setglobal(L, "GM");
	Warning( "ERROR: Attempted to load an invalid gamemode!\n" );
    return false;
  }
}

bool luasrc_SetGamemode (const char *gamemode) {
  lua_getglobal(L, "gamemode");
  if (lua_istable(L, -1)) {
    lua_getfield(L, -1, "Get");
	if (lua_isfunction(L, -1)) {
	  lua_remove(L, -2);
	  lua_pushstring(L, gamemode);
	  luasrc_pcall(L, 1, 1, 0);
	  lua_setglobal(L, "GAMEMODE");
	  BEGIN_LUA_CALL_HOOK("Initialize");
	  END_LUA_CALL_HOOK(0,0);
	  return true;
	}
	else
	{
	  lua_pop(L, 2);
	  Warning( "ERROR: Attempted to load an invalid gamemode!\n" );
	  return false;
	}
  }
  else
  {
    lua_pop(L, 1);
	Warning( "ERROR: Attempted to load an invalid gamemode!\n" );
	return false;
  }
}

#ifdef CLIENT_DLL
	CON_COMMAND( lua_dostring_cl, "Run a Lua string" )
	{
		if ( !g_bLuaInitialized )
			return;

		if ( args.ArgC() == 1 )
		{
			Msg( "Usage: lua_dostring_cl <string>\n" );
			return;
		}

		luasrc_dostring( L, args.ArgS() );
	}
#else
	CON_COMMAND( lua_dostring, "Run a Lua string" )
	{
		if ( !g_bLuaInitialized )
			return;

		if ( !UTIL_IsCommandIssuedByServerAdmin() )
			return;

		if ( args.ArgC() == 1 )
		{
			Msg( "Usage: lua_dostring <string>\n" );
			return;
		}

		luasrc_dostring( L, args.ArgS() );
	}
#endif

static int DoFileCompletion( const char *partial, char commands[ COMMAND_COMPLETION_MAXITEMS ][ COMMAND_COMPLETION_ITEM_LENGTH ] )
{
	int current = 0;

#ifdef CLIENT_DLL
	const char *cmdname = "lua_dofile_cl";
#else
	const char *cmdname = "lua_dofile";
#endif
	char *substring = NULL;
	int substringLen = 0;
	if ( Q_strstr( partial, cmdname ) && strlen(partial) > strlen(cmdname) + 1 )
	{
		substring = (char *)partial + strlen( cmdname ) + 1;
		substringLen = strlen(substring);
	}
	
	FileFindHandle_t fh;

	char WildCard[ MAX_PATH ] = { 0 };
	if ( substring == NULL )
		substring = "";
	Q_snprintf( WildCard, sizeof( WildCard ), LUA_ROOT "\\%s*", substring );
	Q_FixSlashes( WildCard );
	char const *fn = g_pFullFileSystem->FindFirstEx( WildCard, "MOD", &fh );
	if ( fn )
	{
		do
		{
			if ( fn[0] != '.' )
			{
				char filename[ MAX_PATH ] = { 0 };
				Q_snprintf( filename, sizeof( filename ), LUA_ROOT "\\%s\\%s", substring, fn );
				Q_FixSlashes( filename );
				if ( filesystem->FileExists( filename, "MOD" ) )
				{
					Q_snprintf( commands[ current ], sizeof( commands[ current ] ), "%s %s%s", cmdname, substring, fn );
					current++;
				}
			}

			fn = g_pFullFileSystem->FindNext( fh );

		} while ( fn && current < COMMAND_COMPLETION_MAXITEMS );

		g_pFullFileSystem->FindClose( fh );
	}

	return current;
}

#ifdef CLIENT_DLL
	CON_COMMAND_F_COMPLETION( lua_dofile_cl, "Load and run a Lua file", 0, DoFileCompletion )
	{
		if ( !g_bLuaInitialized )
			return;

		if ( args.ArgC() == 1 )
		{
			Msg( "Usage: lua_dofile_cl <filename>\n" );
			return;
		}

		char fullpath[ 512 ] = { 0 };
		char filename[ 256 ] = { 0 };
		Q_snprintf( filename, sizeof( filename ), LUA_ROOT "\\%s", args.ArgS() );
		Q_strlower( filename );
		Q_FixSlashes( filename );
		if ( filesystem->FileExists( filename, "MOD" ) )
		{
			filesystem->RelativePathToFullPath( filename, "MOD", fullpath, sizeof( fullpath ) );
		}
		else
		{
			Q_snprintf( fullpath, sizeof( fullpath ), "%s\\" LUA_ROOT "\\%s", engine->GetGameDirectory(), args.ArgS() );
			Q_strlower( fullpath );
			Q_FixSlashes( fullpath );
		}

		if ( Q_strstr( fullpath, ".." ) )
		{
			return;
		}
		Msg( "Running file %s...\n", args.ArgS() );
		luasrc_dofile( L, fullpath );
	}
#else
	CON_COMMAND_F_COMPLETION( lua_dofile, "Load and run a Lua file", 0, DoFileCompletion )
	{
		if ( !g_bLuaInitialized )
			return;

		if ( !UTIL_IsCommandIssuedByServerAdmin() )
			return;

		if ( args.ArgC() == 1 )
		{
			Msg( "Usage: lua_dofile <filename>\n" );
			return;
		}

		char fullpath[ 512 ] = { 0 };
		char filename[ 256 ] = { 0 };
		Q_snprintf( filename, sizeof( filename ), LUA_ROOT "lua\\%s", args.ArgS() );
		Q_strlower( filename );
		Q_FixSlashes( filename );
		if ( filesystem->FileExists( filename, "MOD" ) )
		{
			filesystem->RelativePathToFullPath( filename, "MOD", fullpath, sizeof( fullpath ) );
		}
		else
		{
			// filename is local to game dir for Steam, so we need to prepend game dir for regular file load
			char gamePath[256];
			engine->GetGameDir( gamePath, 256 );
			Q_StripTrailingSlash( gamePath );
			Q_snprintf( fullpath, sizeof( fullpath ), "%s\\" LUA_ROOT "\\%s", gamePath, args.ArgS() );
			Q_strlower( fullpath );
			Q_FixSlashes( fullpath );
		}

		if ( Q_strstr( fullpath, ".." ) )
		{
			return;
		}
		Msg( "Running file %s...\n", args.ArgS() );
		luasrc_dofile( L, fullpath );
	}
#endif

#if DEBUG_LUA_STACK
#ifdef CLIENT_DLL
	CON_COMMAND( lua_dumpstack_cl, "Prints the Lua stack" )
	{
	  if (!g_bLuaInitialized)
	    return;
	  int n = lua_gettop(L);  /* number of objects */
	  int i;
	  lua_getglobal(L, "tostring");
	  for (i=1; i<=n; i++) {
		const char *s;
		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);  /* get result */
		Warning( " %d:\t%s\n", i, s );
		lua_pop(L, 1);  /* pop result */
	  }
	  lua_pop(L, 1);  /* pop function */
	  if (n>0)
	    Warning( "Warning: %d object(s) left on the stack!\n", n );
	}
#else
	CON_COMMAND( lua_dumpstack, "Prints the Lua stack" )
	{
	  if (!g_bLuaInitialized)
	    return;
	  int n = lua_gettop(L);  /* number of objects */
	  int i;
	  lua_getglobal(L, "tostring");
	  for (i=1; i<=n; i++) {
		const char *s;
		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);  /* get result */
		Warning( " %d:\t%s\n", i, s );
		lua_pop(L, 1);  /* pop result */
	  }
	  lua_pop(L, 1);  /* pop function */
	  if (n>0)
	    Warning( "Warning: %d object(s) left on the stack!\n", n );
	}
#endif
#endif
