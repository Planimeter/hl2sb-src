//===== Copy	right © 1996-2005, Valve Corporation, All rights reserved. ==//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//
#define lc_baseanimating_cpp

#include "cbase.h"
#include "lua.hpp"
#include "lc_baseanimating.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

/*
** access functions (stack -> C)
*/


LUA_API lua_CBaseAnimating *lua_toanimating (lua_State *L, int idx) {
  if (!lua_isuserdata(L, idx))
    luaL_typerror(L, idx, "CBaseAnimating");
  if (lua_getmetatable(L, idx)) {
    lua_pushstring(L, "__type");
	lua_rawget(L, -2);
	if (!lua_isstring(L, -1))
	  lua_pop(L, 2);
	else if (Q_strcmp(luaL_checkstring(L, -1), "entity") != 0)
	  luaL_typerror(L, idx, "CBaseAnimating");
  }
  CBaseHandle *hEntity = (CBaseHandle *)lua_touserdata(L, idx);
  return (lua_CBaseAnimating *)hEntity->Get();
}



/*
** push functions (C -> stack)
*/


LUA_API void lua_pushanimating (lua_State *L, CBaseAnimating *pEntity) {
  CBaseHandle *hEntity = (CBaseHandle *)lua_newuserdata(L, sizeof(CBaseHandle));
  hEntity->Set(pEntity);
  luaL_getmetatable(L, "CBaseAnimating");
  lua_setmetatable(L, -2);
}


LUALIB_API lua_CBaseAnimating *luaL_checkanimating (lua_State *L, int narg) {
  lua_CBaseAnimating *d = lua_toanimating(L, narg);
  if (d == NULL)  /* avoid extra test when d is not 0 */
    luaL_argerror(L, narg, "attempt to index a NULL entity");
  return d;
}


static int CBaseAnimating___index (lua_State *L) {
  CBaseAnimating *pEntity = lua_toanimating(L, 1);
  if (pEntity == NULL) {  /* avoid extra test when d is not 0 */
    lua_Debug ar1;
    lua_getstack(L, 1, &ar1);
    lua_getinfo(L, "fl", &ar1);
    lua_Debug ar2;
    lua_getinfo(L, ">S", &ar2);
	lua_pushfstring(L, "%s:%d: attempt to index a NULL entity", ar2.short_src, ar1.currentline);
	return lua_error(L);
  }
  lua_getmetatable(L, 1);
  lua_pushvalue(L, 2);
  lua_gettable(L, -2);
  if (lua_isnil(L, -1)) {
    lua_pop(L, 1);
    luaL_getmetatable(L, "CBaseEntity");
    lua_pushvalue(L, 2);
    lua_gettable(L, -2);
  }
  return 1;
}

static int CBaseAnimating___eq (lua_State *L) {
  lua_pushboolean(L, lua_toanimating(L, 1) == lua_toanimating(L, 2));
  return 1;
}

static int CBaseAnimating___tostring (lua_State *L) {
  CBaseAnimating *pEntity = lua_toanimating(L, 1);
  if (pEntity == NULL)
    lua_pushstring(L, "NULL");
  else
    lua_pushfstring(L, "CBaseAnimating: %d \"%s\"", pEntity->entindex(), pEntity->GetClassname());
  return 1;
}


static const luaL_Reg CBaseAnimatingmeta[] = {
  {"__index", CBaseAnimating___index},
  {"__eq", CBaseAnimating___eq},
  {"__tostring", CBaseAnimating___tostring},
  {NULL, NULL}
};


/*
** Open CBaseAnimating object
*/
int luaopen_CBaseAnimating (lua_State *L) {
  luaL_newmetatable(L, "CBaseAnimating");
  luaL_register(L, NULL, CBaseAnimatingmeta);
  lua_pushstring(L, "entity");
  lua_setfield(L, -2, "__type");  /* metatable.__type = "entity" */
  lua_pop(L, 1);
  return 1;
}

