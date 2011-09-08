//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//
#define lc_baseentity_cpp

#include "cbase.h"
#include "lua.hpp"
#include "lbaseentity_shared.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


static int CBaseEntity_SetNextClientThink (lua_State *L) {
  luaL_checkentity(L, 1)->SetNextClientThink(luaL_checknumber(L, 2));
  return 0;
}


static const luaL_Reg CBaseEntitymeta[] = {
  {"SetNextClientThink", CBaseEntity_SetNextClientThink},
  {NULL, NULL}
};


/*
** Open CBaseEntity object
*/
int luaopen_CBaseEntity (lua_State *L) {
  luaL_getmetatable(L, "CBaseEntity");
  if (lua_isnoneornil(L, -1)) {
    lua_pop(L, 1);
    luaL_newmetatable(L, "CBaseEntity");
  }
  luaL_register(L, NULL, CBaseEntitymeta);
  lua_pop(L, 1);
  return 1;
}

