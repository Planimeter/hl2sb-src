//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $NoKeywords: $
//===========================================================================//

#define lconvar_cpp

#include "convar.h"
#include "../LuaManager.h"
#include "lconvar.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

/*
** access functions (stack -> C)
*/


LUA_API const lua_ConVar *lua_toconvar (lua_State *L, int idx) {
  const lua_ConVar **ppConVar = (const lua_ConVar **)luaL_checkudata(L, idx, "ConVar");
  return *ppConVar;
}



/*
** push functions (C -> stack)
*/


LUA_API void lua_pushconvar (lua_State *L, const lua_ConVar *pConVar) {
  const lua_ConVar **ppConVar = (const lua_ConVar **)lua_newuserdata(L, sizeof(lua_ConVar));
  *ppConVar = pConVar;
  luaL_getmetatable(L, "ConVar");
  lua_setmetatable(L, -2);
}


LUALIB_API const lua_ConVar *luaL_checkconvar (lua_State *L, int narg) {
  const lua_ConVar *d = lua_toconvar(L, narg);
  if (&d == NULL)  /* avoid extra test when d is not 0 */
    luaL_typerror(L, narg, "ConVar");
  return d;
}


static int ConVar_AddFlags (lua_State *L) {
  const_cast<ConVar *>(luaL_checkconvar(L, 1))->AddFlags(luaL_checkint(L, 2));
  return 0;
}

static int ConVar_GetBool (lua_State *L) {
  lua_pushboolean(L, luaL_checkconvar(L, 1)->GetBool());
  return 1;
}

static int ConVar_GetDefault (lua_State *L) {
  lua_pushstring(L, luaL_checkconvar(L, 1)->GetDefault());
  return 1;
}

static int ConVar_GetFloat (lua_State *L) {
  lua_pushnumber(L, luaL_checkconvar(L, 1)->GetFloat());
  return 1;
}

static int ConVar_GetHelpText (lua_State *L) {
  lua_pushstring(L, luaL_checkconvar(L, 1)->GetHelpText());
  return 1;
}

static int ConVar_GetInt (lua_State *L) {
  lua_pushinteger(L, luaL_checkconvar(L, 1)->GetInt());
  return 1;
}

static int ConVar_GetMax (lua_State *L) {
  float maxVal;
  lua_pushboolean(L, luaL_checkconvar(L, 1)->GetMax(maxVal));
  lua_pushnumber(L, maxVal);
  return 2;
}

static int ConVar_GetMin (lua_State *L) {
  float minVal;
  lua_pushboolean(L, luaL_checkconvar(L, 1)->GetMin(minVal));
  lua_pushnumber(L, minVal);
  return 2;
}

static int ConVar_GetName (lua_State *L) {
  lua_pushstring(L, luaL_checkconvar(L, 1)->GetName());
  return 1;
}

static int ConVar_GetString (lua_State *L) {
  lua_pushstring(L, luaL_checkconvar(L, 1)->GetString());
  return 1;
}

static int ConVar_IsCommand (lua_State *L) {
  lua_pushboolean(L, luaL_checkconvar(L, 1)->IsCommand());
  return 1;
}

static int ConVar_IsFlagSet (lua_State *L) {
  lua_pushboolean(L, luaL_checkconvar(L, 1)->IsFlagSet(luaL_checkint(L, 2)));
  return 1;
}

static int ConVar_IsRegistered (lua_State *L) {
  lua_pushboolean(L, luaL_checkconvar(L, 1)->IsRegistered());
  return 1;
}

static int ConVar_Revert (lua_State *L) {
  const_cast<ConVar *>(luaL_checkconvar(L, 1))->Revert();
  return 0;
}

static int ConVar___tostring (lua_State *L)
{
  lua_pushfstring(L, "ConVar: \"%s\" = \"%s\"", luaL_checkconvar(L, 1)->GetName(), luaL_checkconvar(L, 1)->GetString());
  return 1;
}


static const luaL_Reg ConVarmeta[] = {
  {"AddFlags", ConVar_AddFlags},
  {"GetBool", ConVar_GetBool},
  {"GetDefault", ConVar_GetDefault},
  {"GetFloat", ConVar_GetFloat},
  {"GetHelpText", ConVar_GetHelpText},
  {"GetInt", ConVar_GetInt},
  {"GetMax", ConVar_GetMax},
  {"GetMin", ConVar_GetMin},
  {"GetName", ConVar_GetName},
  {"GetString", ConVar_GetString},
  {"IsCommand", ConVar_IsCommand},
  {"IsFlagSet", ConVar_IsFlagSet},
  {"IsRegistered", ConVar_IsRegistered},
  {"Revert", ConVar_Revert},
  {"__tostring", ConVar___tostring},
  {NULL, NULL}
};


static int luasrc_ConVar (lua_State *L) {
  ConVar *pConVar = new ConVar(luaL_checkstring(L, 1), luaL_checkstring(L, 2), luaL_optint(L, 3, 0), luaL_optstring(L, 4, 0), luaL_optboolean(L, 5, 0), luaL_optnumber(L, 6, 0.0), luaL_optboolean(L, 7, 0), luaL_optnumber(L, 8, 0));
  cvar->RegisterConCommand(pConVar);
  lua_pushconvar(L, pConVar);
  return 1;
}


static const luaL_Reg ConVar_funcs[] = {
  {"ConVar", luasrc_ConVar},
  {NULL, NULL}
};


/*
** Open ConVar object
*/
int luaopen_ConVar (lua_State *L) {
  luaL_newmetatable(L, "ConVar");
  luaL_register(L, NULL, ConVarmeta);
  lua_pushvalue(L, -1);  /* push metatable */
  lua_setfield(L, -2, "__index");  /* metatable.__index = metatable */
  lua_pushstring(L, "convar");
  lua_setfield(L, -2, "__type");  /* metatable.__type = "convar" */
  lua_pop(L, 2);
  luaL_register(L, "_G", ConVar_funcs);
  return 1;
}

