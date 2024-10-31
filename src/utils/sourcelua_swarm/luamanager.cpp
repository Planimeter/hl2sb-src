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
#include "tier1/lconvar.h"
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


static lua_State *L;

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
	if (!lua_isstring(L, -1))
	  lua_pop(L, 2);
	else
	  return 1;
  }
  lua_pushstring(L, luaL_typename(L, 1));
  return 1;
}


static const luaL_Reg base_funcs[] = {
  {"print", luasrc_print},
  {"type", luasrc_type},
  {NULL, NULL}
};


static void base_open (lua_State *L) {
  /* set global _R */
  lua_pushvalue(L, LUA_REGISTRYINDEX);
  lua_setglobal(L, "_R");
  /* open lib into global table */
  luaL_register(L, "_G", base_funcs);
  lua_pushboolean(L, 0);
  lua_setglobal(L, "CLIENT");  /* set global CLIENT */
  lua_pushboolean(L, 1);
  lua_setglobal(L, "SERVER");  /* set global SERVER */
}


lua_State *lua_GetState (void) {
  return L;
}

void lua_init (void) {
  L = lua_open();

  luaL_openlibs(L);
  base_open(L);

  luaopen_CBaseCombatWeapon(L);
  luaopen_CBaseEntity(L);
  luaopen_CBasePlayer(L);
  luaopen_Color(L);
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

void lua_shutdown (void) {
  lua_close(L);
}

int lua_dostring (const char *string) {
  int iError = luaL_dostring(L, string);
  if (iError != 0) {
    Warning( "%s\n", lua_tostring(L, -1) );
    lua_pop(L, 1);
    return 1;
  }
  return 0;
}

int lua_dofile(const char *filename) {
  int iError = luaL_dofile(L, filename);
  if (iError != 0) {
    Warning( "%s\n", lua_tostring(L, -1) );
    lua_pop(L, 1);
    return 1;
  }
  return 0;
}

void lua_dofolder(const char *path) {
	FileFindHandle_t fh;
	lua_path = cvar->FindVar( "lua_path" );

	char searchPath[ 512 ];
	Q_snprintf( searchPath, sizeof( searchPath ), "%s\\%s\\*.lua", lua_path->GetString(), path );

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
			if ( fn[0] != '.'  )
			{
				char ext[ 10 ];
				Q_ExtractFileExtension( fn, ext, sizeof( ext ) );

				if ( !Q_stricmp( ext, "lua" ) )
				{
					char loadname[ 512 ];
					Q_snprintf( loadname, sizeof( loadname ), "%s\\%s\\%s\\%s", gamePath, lua_path->GetString(), path, fn );
					lua_dofile( loadname );
				}
			}

			fn = g_pFullFileSystem->FindNext( fh );

		} while ( fn );

		g_pFullFileSystem->FindClose( fh );
	}
}

CON_COMMAND( lua_dostring, "Run a Lua string" )
{
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

CON_COMMAND( lua_dofile, "Load and run a Lua file" )
{
	if ( args.ArgC() == 1 )
	{
		Msg( "Usage: lua_dofile <filename>\n" );
		return;
	}

	char fullpath[ 512 ];
	// filename is local to game dir for Steam, so we need to prepend game dir for regular file load
	char gamePath[ 256 ];
	engine->GetGameDir( gamePath, 256 );
	lua_path = cvar->FindVar( "lua_path" );
	Q_snprintf( fullpath, sizeof( fullpath ), "%s\\%s\\lua\\%s", gamePath, lua_path->GetString(), args.ArgS() );
	lua_dofile( fullpath );
}


