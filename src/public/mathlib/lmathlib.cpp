//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
//===========================================================================//

#define lmathlib_cpp

#include "cbase.h"
#include "mathlib.h"
#include "lua.hpp"
#include "luasrclib.h"
#include "lvector.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static int mathlib_clamp (lua_State *L) {
  lua_pushnumber(L, clamp(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)));
  return 1;
}

static int mathlib_AngleVectors (lua_State *L) {
  if (lua_gettop(L) > 2)
    AngleVectors(luaL_checkangle(L, 1), &luaL_checkvector(L, 2), &luaL_checkvector(L, 3), &luaL_checkvector(L, 4));
  else
    AngleVectors(luaL_checkangle(L, 1), &luaL_checkvector(L, 2));
  return 0;
}

static int mathlib_VectorAngles (lua_State *L) {
  if (lua_gettop(L) > 2)
    VectorAngles(luaL_checkvector(L, 1), luaL_checkvector(L, 2), luaL_checkangle(L, 3));
  else
    VectorAngles(luaL_checkvector(L, 1), luaL_checkangle(L, 2));
  return 0;
}


static const luaL_Reg mathliblib[] = {
  {"clamp",   mathlib_clamp},
  {"AngleVectors",   mathlib_AngleVectors},
  {"VectorAngles",   mathlib_VectorAngles},
  {NULL, NULL}
};


/*
** Open mathlib library
*/
LUALIB_API int luaopen_mathlib (lua_State *L) {
  luaL_register(L, LUA_MATHLIBLIBNAME, mathliblib);
  return 1;
}

