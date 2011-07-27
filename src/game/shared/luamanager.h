//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef LUAMANAGER_H
#define LUAMANAGER_H
#ifdef _WIN32
#pragma once
#endif

#include "lua.hpp"

#define LUA_ROOT                "lua" // Can't be "LUA_PATH" because luaconf.h uses it.
#define LUA_PATH_CACHE          "cache\\lua"
#define LUA_PATH_ENUM           LUA_ROOT "\\includes\\enum"
#define LUA_PATH_EXTENSIONS     LUA_ROOT "\\includes\\extensions"
#define LUA_PATH_MODULES        LUA_ROOT "\\includes\\modules"
#define LUA_PATH_INCLUDES       LUA_ROOT "\\includes"
#define LUA_PATH_AUTORUN        LUA_ROOT "\\autorun"
#define LUA_PATH_AUTORUN_CLIENT LUA_ROOT "\\autorun\\client"
#define LUA_PATH_AUTORUN_SERVER LUA_ROOT "\\autorun\\server"
#define LUA_PATH_ENTITIES       LUA_ROOT "\\entities"
#define LUA_PATH_WEAPONS        LUA_ROOT "\\weapons"


#define LUA_BASE_WEAPON         "weapon_hl2mpbase_scriptedweapon"
#define LUA_BASE_GAMEMODE       "deathmatch"


#pragma warning( disable: 4800 )	// forcing value to bool 'true' or 'false' (performance warning)

//Andrew; while we're in development, let's just leave this at 1
#define DEBUG_LUA_STACK 1

#define BEGIN_LUA_CALL_HOOK(functionName) \
  lua_getglobal(L, "hook"); \
  if (lua_istable(L, -1)) { \
    lua_getfield(L, -1, "Call"); \
	if (lua_isfunction(L, -1)) { \
	  lua_remove(L, -2); \
	  int args = 0; \
	  lua_pushstring(L, functionName); \
	  lua_getglobal(L, "GAMEMODE"); \
	  args = 2;

#define END_LUA_CALL_HOOK(nArgs, nresults) \
	  args += nArgs; \
	  luasrc_pcall(L, args, nresults, 0); \
	} \
	else \
	  lua_pop(L, 2); \
  } \
  else \
    lua_pop(L, 1);

#define BEGIN_LUA_CALL_WEAPON_METHOD(functionName) \
  lua_getref(L, m_nRefCount); \
  lua_getfield(L, -1, functionName); \
  lua_remove(L, -2); \
  if (lua_isfunction(L, -1)) { \
    int args = 0; \
	lua_pushweapon(L, this); \
	++args;

#define END_LUA_CALL_WEAPON_METHOD(nArgs, nresults) \
	args += nArgs; \
	luasrc_pcall(L, args, nresults, 0); \
  } \
  else \
    lua_pop(L, 1);

#define BEGIN_LUA_CALL_WEAPON_HOOK(pWeapon, functionName) \
  if (pWeapon->IsScripted()) { \
    lua_getref(L, pWeapon->m_nRefCount); \
    lua_getfield(L, -1, functionName); \
    lua_remove(L, -2); \
    int args = 0; \
    lua_pushweapon(L, pWeapon); \
    ++args;

#define END_LUA_CALL_WEAPON_HOOK(nArgs, nresults) \
    args += nArgs; \
    luasrc_pcall(L, args, nresults, 0); \
  }

#define RETURN_LUA_NONE() \
  if (lua_gettop(L) == 1) { \
    if (lua_isboolean(L, -1)) { \
	  bool res = (bool)luaL_checkboolean(L, -1); \
	  lua_pop(L, 1); \
	  if (!res) \
	    return; \
	} \
    else \
	  lua_pop(L, 1); \
  }

#define RETURN_LUA_BOOLEAN() \
  if (lua_gettop(L) == 1) { \
    if (lua_isboolean(L, -1)) { \
	  bool res = (bool)luaL_checkboolean(L, -1); \
	  lua_pop(L, 1); \
	  return res; \
	} \
    else \
	  lua_pop(L, 1); \
  }

#define RETURN_LUA_NUMBER() \
  if (lua_gettop(L) == 1) { \
    if (lua_isnumber(L, -1)) { \
	  float res = luaL_checknumber(L, -1); \
	  lua_pop(L, 1); \
	  return res; \
	} \
    else \
	  lua_pop(L, 1); \
  }

#define RETURN_LUA_INTEGER() \
  if (lua_gettop(L) == 1) { \
    if (lua_isnumber(L, -1)) { \
	  int res = luaL_checkinteger(L, -1); \
	  lua_pop(L, 1); \
	  return res; \
	} \
    else \
	  lua_pop(L, 1); \
  }

#define RETURN_LUA_STRING() \
  if (lua_gettop(L) == 1) { \
    if (lua_isstring(L, -1)) { \
	  const char *res = luaL_checkstring(L, -1); \
	  lua_pop(L, 1); \
	  return res; \
	} \
    else \
	  lua_pop(L, 1); \
  }

#define RETURN_LUA_WEAPON() \
  if (lua_gettop(L) == 1) { \
    if (lua_isuserdata(L, -1) && luaL_checkudata(L, -1, "CBaseCombatWeapon")) { \
	  CBaseCombatWeapon *res = luaL_checkweapon(L, -1); \
	  lua_pop(L, 1); \
	  return res; \
	} \
    else \
	  lua_pop(L, 1); \
  }

#define RETURN_LUA_ENTITY() \
  if (lua_gettop(L) == 1) { \
    if (lua_isuserdata(L, -1) && luaL_checkudata(L, -1, "CBaseEntity")) { \
	  CBaseEntity *res = luaL_checkentity(L, -1); \
	  lua_pop(L, 1); \
	  return res; \
	} \
    else \
	  lua_pop(L, 1); \
  }

#define RETURN_LUA_PLAYER() \
  if (lua_gettop(L) == 1) { \
    if (lua_isuserdata(L, -1) && luaL_checkudata(L, -1, "CBasePlayer")) { \
	  CBasePlayer *res = luaL_checkplayer(L, -1); \
	  lua_pop(L, 1); \
	  return res; \
	} \
    else \
	  lua_pop(L, 1); \
  }

#define RETURN_LUA_VECTOR() \
  if (lua_gettop(L) == 1) { \
    if (lua_isuserdata(L, -1) && luaL_checkudata(L, -1, "Vector")) { \
	  Vector res = *(Vector *)luaL_checkvector(L, -1); \
	  lua_pop(L, 1); \
	  return res; \
	} \
    else \
	  lua_pop(L, 1); \
  }

#define RETURN_LUA_ANGLE() \
  if (lua_gettop(L) == 1) { \
    if (lua_isuserdata(L, -1) && luaL_checkudata(L, -1, "QAngle")) { \
	  QAngle res = *(QAngle *)luaL_checkangle(L, -1); \
	  lua_pop(L, 1); \
	  return res; \
	} \
    else \
	  lua_pop(L, 1); \
  }

extern ConVar sv_gamemode;

LUALIB_API int luaL_checkboolean (lua_State *L, int narg);
LUALIB_API int luaL_optboolean (lua_State *L, int narg,
                                              int def);

extern lua_State *L;


// Set to true between LevelInit and LevelShutdown.
extern bool	g_bLuaInitialized;

void       luasrc_init (void);
void       luasrc_shutdown (void);

LUA_API int   (luasrc_dostring) (lua_State *L, const char *string);
LUA_API int   (luasrc_dofile) (lua_State *L, const char *filename);
LUA_API void  (luasrc_dofolder) (lua_State *L, const char *path);

LUA_API int   (luasrc_pcall) (lua_State *L, int nargs, int nresults, int errfunc);

// void    luasrc_LoadEntities (void);
void       luasrc_LoadWeapons (void);

bool       luasrc_LoadGamemode (const char *gamemode);
bool       luasrc_SetGamemode (const char *gamemode);

#endif // LUAMANAGER_H
