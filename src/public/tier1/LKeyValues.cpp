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


LUA_API lua_KeyValues *lua_tokeyvalues (lua_State *L, int idx) {
  lua_KeyValues *kv = *(lua_KeyValues **)lua_touserdata(L, idx);
  return kv;
}



/*
** push functions (C -> stack)
*/


LUA_API void lua_pushkeyvalues (lua_State *L, lua_KeyValues *pKV) {
  lua_KeyValues **ppKV = (lua_KeyValues **)lua_newuserdata(L, sizeof(lua_KeyValues *));
  *ppKV = pKV;
  luaL_getmetatable(L, "KeyValues");
  lua_setmetatable(L, -2);
}


LUALIB_API lua_KeyValues *luaL_checkkeyvalues (lua_State *L, int narg) {
  lua_KeyValues **d = (lua_KeyValues **)luaL_checkudata(L, narg, "KeyValues");
  if (*d == 0)  /* avoid extra test when d is not 0 */
    luaL_argerror(L, narg, "KeyValues expected, got NULL keyvalues");
  return *d;
}


static int KeyValues_AddSubKey (lua_State *L) {
  luaL_checkkeyvalues(L, 1)->AddSubKey(luaL_checkkeyvalues(L, 2));
  return 0;
}

static int KeyValues_ChainKeyValue (lua_State *L) {
  luaL_checkkeyvalues(L, 1)->ChainKeyValue(luaL_checkkeyvalues(L, 2));
  return 0;
}

static int KeyValues_Clear (lua_State *L) {
  luaL_checkkeyvalues(L, 1)->Clear();
  return 0;
}

static int KeyValues_CopySubkeys (lua_State *L) {
  luaL_checkkeyvalues(L, 1)->CopySubkeys(luaL_checkkeyvalues(L, 2));
  return 0;
}

static int KeyValues_CreateNewKey (lua_State *L) {
  lua_pushkeyvalues(L, luaL_checkkeyvalues(L, 1)->CreateNewKey());
  return 1;
}

static int KeyValues_deleteThis (lua_State *L) {
  KeyValues *pKV = luaL_checkkeyvalues(L, 1);
  pKV->deleteThis();
  // Andrew; this isn't standard behavior or usage, but we do this for the sake
  // of things being safe in Lua
  *(void **)lua_touserdata(L, 1) = (void **)NULL;
  return 0;
}

static int KeyValues___tostring (lua_State *L) {
  lua_pushfstring(L, "KeyValues: %p", *(void **)luaL_checkudata(L, 1, "KeyValues"));
  return 1;
}


static const luaL_Reg KeyValuesmeta[] = {
  {"AddSubKey", KeyValues_AddSubKey},
  {"ChainKeyValue", KeyValues_ChainKeyValue},
  {"Clear", KeyValues_Clear},
  {"CopySubkeys", KeyValues_CopySubkeys},
  {"CreateNewKey", KeyValues_CreateNewKey},
  {"deleteThis", KeyValues_deleteThis},
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
