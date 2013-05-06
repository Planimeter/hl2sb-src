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

#pragma warning( disable: 4800 )	// forcing value to bool 'true' or 'false' (performance warning)

#define BEGIN_LUA_CALL_HOOK(functionName) \
  lua_State *L = lua_GetState(); \
  lua_getfield(L, LUA_GLOBALSINDEX, "hook"); \
  bool bHookInitialized = true; \
  int args = 0; \
  if (!lua_istable(L, -1)) { \
    lua_pop(L, 1); \
    bHookInitialized = false; \
  } \
  if (bHookInitialized) { \
    lua_getfield(L, -1, "Call"); \
    if (lua_isfunction(L, -1)) { \
      lua_pushstring(L, functionName); \
      args = 1;

#define END_LUA_CALL_HOOK(nArgs, nresults) \
      args += nArgs; \
      lua_call(L, args, nresults); \
	} \
	else \
	{ \
	  lua_pop(L, 2); \
	} \
  }

#define LUA_PATH_ENUM           "lua\\includes\\enum"
#define LUA_PATH_MODULES        "lua\\includes\\modules"
#define LUA_PATH_INCLUDES       "lua\\includes"
#define LUA_PATH_AUTORUN        "lua\\autorun"
#define LUA_PATH_AUTORUN_CLIENT "lua\\autorun\\client"
#define LUA_PATH_AUTORUN_SERVER "lua\\autorun\\server"


LUALIB_API int luaL_checkboolean (lua_State *L, int narg);
LUALIB_API int luaL_optboolean (lua_State *L, int narg,
                                              int def);

lua_State *lua_GetState (void);
void       lua_init (void);
void       lua_shutdown (void);
int        lua_dostring (const char *string);
int        lua_dofile (const char *filename);
void       lua_dofolder (const char *path);

#endif // SERVERPLUGIN_LUA_H
