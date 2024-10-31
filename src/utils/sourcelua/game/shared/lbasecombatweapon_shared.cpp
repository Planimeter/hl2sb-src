//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#define lbasecombatweapon_shared_cpp

#include "../../luamanager.h"
#include "lbasecombatweapon_shared.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

/*
** access functions (stack -> C)
*/


LUA_API lua_CBaseCombatWeapon *lua_toweapon (lua_State *L, int idx) {
  lua_CBaseCombatWeapon **ppWeapon = (lua_CBaseCombatWeapon **)luaL_checkudata(L, idx, "CBaseCombatWeapon");
  return *ppWeapon;
}



/*
** push functions (C -> stack)
*/


LUA_API void lua_pushweapon (lua_State *L, lua_CBaseCombatWeapon *pWeapon) {
  if (pWeapon == NULL)
    lua_pushnil(L);
  else {
    lua_CBaseCombatWeapon **ppWeapon = (lua_CBaseCombatWeapon **)lua_newuserdata(L, sizeof(pWeapon));
    *ppWeapon = pWeapon;
    luaL_getmetatable(L, "CBaseCombatWeapon");
    lua_setmetatable(L, -2);
  }
}


LUALIB_API lua_CBaseCombatWeapon *luaL_checkweapon (lua_State *L, int narg) {
  lua_CBaseCombatWeapon *d = lua_toweapon(L, narg);
  if (d == NULL)  /* avoid extra test when d is not 0 */
    luaL_typerror(L, narg, "CBaseCombatWeapon");
  return d;
}


static int CBaseCombatWeapon___index (lua_State *L) {
  lua_getmetatable(L, 1);
  lua_pushvalue(L, 2);
  lua_gettable(L, -2);
  if (lua_isnil(L, -1)) {
    luaL_getmetatable(L, "CBaseEntity");
    lua_pushvalue(L, 2);
    lua_gettable(L, -2);
  }
  return 1;
}

static int CBaseCombatWeapon___tostring (lua_State *L) {
  lua_pushfstring(L, "CBaseCombatWeapon: %p", luaL_checkudata(L, 1, "CBaseCombatWeapon"));
  return 1;
}


static const luaL_Reg CBaseCombatWeaponmeta[] = {
  {"__index", CBaseCombatWeapon___index},
  {"__tostring", CBaseCombatWeapon___tostring},
  {NULL, NULL}
};


/*
** Open CBaseCombatWeapon object
*/
int luaopen_CBaseCombatWeapon (lua_State *L) {
  luaL_newmetatable(L, "CBaseCombatWeapon");
  luaL_register(L, NULL, CBaseCombatWeaponmeta);
  lua_pushstring(L, "entity");
  lua_setfield(L, -2, "__type");  /* metatable.__type = "entity" */
  lua_pop(L, 1);
  return 1;
}

