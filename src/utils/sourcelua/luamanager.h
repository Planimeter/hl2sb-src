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

#ifndef SERVERPLUGIN_LUA_H
#define SERVERPLUGIN_LUA_H
#ifdef _WIN32
#pragma once
#endif

#include "lua.hpp"

#define LUA_ROOT                "lua" // Can't be "LUA_PATH" because luaconf.h uses it.
#define LUA_PATH_ENUM           LUA_ROOT "\\includes\\enum"
#define LUA_PATH_EXTENSIONS     LUA_ROOT "\\includes\\extensions"
#define LUA_PATH_MODULES        LUA_ROOT "\\includes\\modules"
#define LUA_PATH_INCLUDES       LUA_ROOT "\\includes"
#define LUA_PATH_AUTORUN        LUA_ROOT "\\autorun"
#define LUA_PATH_AUTORUN_CLIENT LUA_ROOT "\\autorun\\client"
#define LUA_PATH_AUTORUN_SERVER LUA_ROOT "\\autorun\\server"


#pragma warning( disable: 4800 )	// forcing value to bool 'true' or 'false' (performance warning)

//Andrew; while we're in development, let's just leave this at 1
#define DEBUG_LUA_STACK 1

#define BEGIN_LUA_CALL_HOOK(functionName) \
  if (g_bLuaInitialized) { \
    lua_getglobal(L, "hook"); \
	if (lua_istable(L, -1)) { \
	  lua_getfield(L, -1, "Call"); \
	  if (lua_isfunction(L, -1)) { \
	    lua_remove(L, -2); \
		int args = 0; \
		lua_pushstring(L, functionName); \
		args = 1;

#define END_LUA_CALL_HOOK(nArgs, nresults) \
	    args += nArgs; \
		lua_call(L, args, nresults); \
	  } \
      else \
	    lua_pop(L, 2); \
	} \
    else \
	  lua_pop(L, 1); \
  }

LUALIB_API int luaL_checkboolean (lua_State *L, int narg);
LUALIB_API int luaL_optboolean (lua_State *L, int narg,
                                              int def);

extern lua_State *L;


// Set to true between LevelInit and LevelShutdown.
extern bool	g_bLuaInitialized;

void       luasrc_init (void);
void       luasrc_shutdown (void);
int        luasrc_dostring (const char *string);
int        luasrc_dofile (const char *filename);
void       luasrc_dofolder (const char *path);

#endif // SERVERPLUGIN_LUA_H
