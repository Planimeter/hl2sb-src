//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "KeyValues.h"
#include "lua.hpp"
#include "luasrclib.h"
#include "LKeyValues.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


/*
** access functions (stack -> C)
*/


LUA_API lua_KeyValues &lua_tokeyvalues (lua_State *L, int idx) {
  lua_KeyValues *kv = (lua_KeyValues *)lua_touserdata(L, idx);
  return *kv;
}



/*
** push functions (C -> stack)
*/


LUA_API void lua_pushkeyvalues (lua_State *L, lua_KeyValues *kv) {
  lua_KeyValues *pKV = (lua_KeyValues *)lua_newuserdata(L, sizeof(lua_KeyValues));
  *pKV = *kv;
  luaL_getmetatable(L, "KeyValues");
  lua_setmetatable(L, -2);
}


LUALIB_API lua_KeyValues &luaL_checkkeyvalues (lua_State *L, int narg) {
  lua_KeyValues *d = (lua_KeyValues *)luaL_checkudata(L, narg, "KeyValues");
  return *d;
}


static int KeyValues_AddSubKey (lua_State *L) {
  luaL_checkkeyvalues(L, 1).AddSubKey(&luaL_checkkeyvalues(L, 2));
  return 0;
}

static int KeyValues___gc (lua_State *L) {
  lua_tokeyvalues(L, 1).deleteThis();
  return 0;
}

static int KeyValues___tostring (lua_State *L) {
  lua_pushfstring(L, "KeyValues: %p", luaL_checkudata(L, 1, "KeyValues"));
  return 1;
}


static const luaL_Reg KeyValuesmeta[] = {
  {"AddSubKey", KeyValues_AddSubKey},
  {"__gc", KeyValues___gc},
  {"__tostring", KeyValues___tostring},
  {NULL, NULL}
};


static int luasrc_KeyValues (lua_State *L) {
  KeyValues *pKV = new KeyValues(luaL_checkstring(L, 1));
  lua_pushkeyvalues(L, pKV);
  return 1;
}


static const luaL_Reg KeyValues_funcs[] = {
  {"KeyValues", luasrc_KeyValues},
  {NULL, NULL}
};


/*
** Open KeyValues object
*/
LUALIB_API int luaopen_KeyValues (lua_State *L) {
  luaL_newmetatable(L, LUA_KEYVALUESLIBNAME);
  luaL_register(L, NULL, KeyValuesmeta);
  lua_pushvalue(L, -1);  /* push metatable */
  lua_setfield(L, -2, "__index");  /* metatable.__index = metatable */
  lua_pushstring(L, "keyvalues");
  lua_setfield(L, -2, "__type");  /* metatable.__type = "keyvalues" */
  luaL_register(L, "_G", KeyValues_funcs);
  lua_pop(L, 1);
  return 1;
}
