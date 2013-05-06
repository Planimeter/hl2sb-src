//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Basic Lua handling.
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//

#include "filesystem.h"
#include "eiface.h"
#include "luamanager.h"
#include "lbasecombatweapon_shared.h"
#include "lbaseentity_shared.h"
#include "lbaseplayer_shared.h"
#include "lColor.h"
#include "lconvar.h"
#include "ldbg.h"
#include "leiface.h"
#include "lfilesystem.h"
#include "lglobalvars_base.h"
#include "licvar.h"
#include "engine/liserverplugin.h"
#include "livoiceserver.h"
#include "vstdlib/lrandom.h"
#include "lutil.h"
#include "mathlib/lvector.h"

const ConVar *lua_path;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern IVEngineServer	*engine;
extern IFileSystem *filesystem;


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
	  lua_pop(L, 2);
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
  char source[ 256 ];
  Q_StrRight( ar2.source, iLength-1, source, sizeof( source ) );
  Q_StripFilename( source );
  char filename[ 256 ];
  Q_snprintf( filename, sizeof( filename ), "%s\\%s", source, luaL_checkstring(L, 1) );
  luasrc_dofile(filename);
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
  lua_pushboolean(L, 0);
  lua_setglobal(L, "CLIENT");  /* set global CLIENT */
  lua_pushboolean(L, 1);
  lua_setglobal(L, "SERVER");  /* set global SERVER */
}


void luasrc_setmodulepaths(lua_State *L) {
  lua_getglobal(L, LUA_LOADLIBNAME);
  char gamePath[ 256 ];
  engine->GetGameDir( gamePath, 256 );

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

  luaopen_CBaseCombatWeapon(L);
  luaopen_CBaseEntity(L);
  luaopen_CBasePlayer(L);
  luaopen_Color(L);
  luaopen_ConCommand(L);
  luaopen_ConVar(L);
  luaopen_dbg(L);
  luaopen_engine(L);
  luaopen_filesystem(L);
  luaopen_gpGlobals(L);
  luaopen_cvar(L);
  luaopen_helpers(L);
  luaopen_g_pVoiceServer(L);
  luaopen_randomStr(L);
  luaopen_UTIL(L);
  luaopen_Vector(L);
  luaopen_QAngle(L);

  Msg( "Lua initialized (" LUA_VERSION ")\n" );
}

void luasrc_shutdown (void) {
  if (!g_bLuaInitialized)
	  return;

  g_bLuaInitialized = false;

  ResetConCommandDatabase();

  RemoveGlobalChangeCallbacks();
  ResetConVarDatabase();

  lua_close(L);
}

int luasrc_dostring (const char *string) {
  int iError = luaL_dostring(L, string);
  if (iError != 0) {
    Warning( "%s\n", lua_tostring(L, -1) );
    lua_pop(L, 1);
    return 1;
  }
  return 0;
}

int luasrc_dofile (const char *filename) {
  int iError = luaL_dofile(L, filename);
  if (iError != 0) {
    Warning( "%s\n", lua_tostring(L, -1) );
    lua_pop(L, 1);
    return 1;
  }
  return 0;
}

void luasrc_dofolder (const char *path) {
	FileFindHandle_t fh;
	lua_path = cvar->FindVar( "lua_path" );

	char searchPath[ 512 ];
	Q_snprintf( searchPath, sizeof( searchPath ), "%s\\%s\\*.lua", lua_path->GetString(), path );

	char gamePath[ 256 ];
	engine->GetGameDir( gamePath, 256 );

	char const *fn = g_pFullFileSystem->FindFirstEx( searchPath, "MOD", &fh );
	if ( fn )
	{
		do
		{
			if ( fn[0] != '.'  )
			{
				char ext[ 10 ];
				Q_ExtractFileExtension( fn, ext, sizeof( ext ) );

				if ( !Q_stricmp( ext, "lua" ) )
				{
					char loadname[ 512 ];
					Q_snprintf( loadname, sizeof( loadname ), "%s\\%s\\%s\\%s", gamePath, lua_path->GetString(), path, fn );
					luasrc_dofile( loadname );
				}
			}

			fn = g_pFullFileSystem->FindNext( fh );

		} while ( fn );

		g_pFullFileSystem->FindClose( fh );
	}
}

CON_COMMAND( lua_dostring, "Run a Lua string" )
{
	if ( !g_bLuaInitialized )
		return;

	if ( args.ArgC() == 1 )
	{
		Msg( "Usage: lua_dostring <string>\n" );
		return;
	}

	int status = luasrc_dostring( L, args.ArgS() );
	if (status == 0 && lua_gettop(L) > 0) {  /* any result to print? */
	  lua_getglobal(L, "print");
	  lua_insert(L, 1);
	  if (lua_pcall(L, lua_gettop(L)-1, 0, 0) != 0)
		Warning("%s", lua_pushfstring(L,
						  "error calling " LUA_QL("print") " (%s)",
						  lua_tostring(L, -1)));
	}
	lua_settop(L, 0);  /* clear stack */
}

static int DoFileCompletion( const char *partial, char commands[ COMMAND_COMPLETION_MAXITEMS ][ COMMAND_COMPLETION_ITEM_LENGTH ] )
{
	int current = 0;

	const char *cmdname = "lua_dofile";
	char *substring = NULL;
	int substringLen = 0;
	if ( Q_strstr( partial, cmdname ) && strlen(partial) > strlen(cmdname) + 1 )
	{
		substring = (char *)partial + strlen( cmdname ) + 1;
		substringLen = strlen(substring);
	}
	
	FileFindHandle_t fh;

	char WildCard[ MAX_PATH ] = { 0 };
	lua_path = cvar->FindVar( "lua_path" );
	if ( substring == NULL )
		substring = "";
	Q_snprintf( WildCard, sizeof( WildCard ), "%s\\" LUA_ROOT "\\%s*", lua_path->GetString(), substring );
	Q_FixSlashes( WildCard );
	char const *fn = g_pFullFileSystem->FindFirstEx( WildCard, "MOD", &fh );
	if ( fn )
	{
		do
		{
			if ( fn[0] != '.' )
			{
				char filename[ MAX_PATH ] = { 0 };
				Q_snprintf( filename, sizeof( filename ), "%s\\" LUA_ROOT "\\%s\\%s", lua_path->GetString(), substring, fn );
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

CON_COMMAND_F_COMPLETION( lua_dofile, "Load and run a Lua file", 0, DoFileCompletion )
{
	if ( !g_bLuaInitialized )
		return;

	if ( args.ArgC() == 1 )
	{
		Msg( "Usage: lua_dofile <filename>\n" );
		return;
	}

	char fullpath[ 512 ];
	// filename is local to game dir for Steam, so we need to prepend game dir for regular file load
	char gamePath[ 256 ];
	engine->GetGameDir( gamePath, 256 );
	Q_StripTrailingSlash( gamePath );
	lua_path = cvar->FindVar( "lua_path" );
	Q_snprintf( fullpath, sizeof( fullpath ), "%s\\%s\\lua\\%s", gamePath, lua_path->GetString(), args.ArgS() );
	Q_strlower( fullpath );
	Q_FixSlashes( fullpath );

	if ( Q_strstr( fullpath, ".." ) )
	{
		return;
	}
	Msg( "Running file %s...\n", args.ArgS() );
	luasrc_dofile( fullpath );
}

#if DEBUG
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
