//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: Random number generator
//
// $Workfile: $
// $NoKeywords: $
//===========================================================================//

#define lrandom_cpp

#include "vstdlib/random.h"
#include "lua.hpp"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"



extern IUniformRandomStream	*randomStr;

static int randomStr_RandomFloat (lua_State *L) {
  lua_pushnumber(L, randomStr->RandomFloat(luaL_optnumber(L, 1, 0.000000), luaL_optnumber(L, 2, 0.000000)));
  return 1;
}

static int randomStr_RandomFloatExp (lua_State *L) {
  lua_pushnumber(L, randomStr->RandomFloatExp(luaL_optnumber(L, 1, 0.000000), luaL_optnumber(L, 2, 0.000000), luaL_optnumber(L, 3, 1.000000)));
  return 1;
}

static int randomStr_RandomInt (lua_State *L) {
  lua_pushinteger(L, randomStr->RandomInt(luaL_checkint(L, 1), luaL_checkint(L, 2)));
  return 1;
}

static int randomStr_SetSeed (lua_State *L) {
  randomStr->SetSeed(luaL_checkint(L, 1));
  return 0;
}


static const luaL_Reg randomStrlib[] = {
  {"RandomFloat",   randomStr_RandomFloat},
  {"RandomFloatExp",   randomStr_RandomFloatExp},
  {"RandomInt",   randomStr_RandomInt},
  {"SetSeed",   randomStr_SetSeed},
  {NULL, NULL}
};


/*
** Open randomStr library
*/
int luaopen_randomStr (lua_State *L) {
  luaL_register(L, "randomStr", randomStrlib);
  return 1;
}

