//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#define lc_recipientfilter_cpp

#include "cbase.h"
#include "c_recipientfilter.h"
#include "luamanager.h"
#include "lc_recipientfilter.h"
#include "lbaseplayer_shared.h"
#include "mathlib/lvector.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

/*
** access functions (stack -> C)
*/


LUA_API lua_CRecipientFilter &lua_torecipientfilter (lua_State *L, int idx) {
  lua_CRecipientFilter *filter = (lua_CRecipientFilter *)lua_touserdata(L, idx);
  return *filter;
}



/*
** push functions (C -> stack)
*/


LUA_API void lua_pushrecipientfilter (lua_State *L, lua_CRecipientFilter &filter) {
  lua_CRecipientFilter *pFilter = (lua_CRecipientFilter *)lua_newuserdata(L, sizeof(lua_CRecipientFilter));
  *pFilter = filter;
  luaL_getmetatable(L, "CRecipientFilter");
  lua_setmetatable(L, -2);
}


LUALIB_API lua_CRecipientFilter &luaL_checkrecipientfilter (lua_State *L, int narg) {
  lua_CRecipientFilter *d = (lua_CRecipientFilter *)luaL_checkudata(L, narg, "CRecipientFilter");
  return *d;
}


static int C_RecipientFilter_AddAllPlayers (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).AddAllPlayers();
  return 0;
}

static int C_RecipientFilter_AddRecipient (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).AddRecipient(luaL_checkplayer(L, 2));
  return 0;
}

static int C_RecipientFilter_AddRecipientsByPAS (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).AddRecipientsByPAS(*luaL_checkvector(L, 2));
  return 0;
}

static int C_RecipientFilter_AddRecipientsByPVS (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).AddRecipientsByPVS(*luaL_checkvector(L, 2));
  return 0;
}

static int C_RecipientFilter_CopyFrom (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).CopyFrom(luaL_checkrecipientfilter(L, 2));
  return 0;
}

static int C_RecipientFilter_GetRecipientCount (lua_State *L) {
  lua_pushinteger(L, luaL_checkrecipientfilter(L, 1).GetRecipientCount());
  return 1;
}

static int C_RecipientFilter_GetRecipientIndex (lua_State *L) {
  lua_pushinteger(L, luaL_checkrecipientfilter(L, 1).GetRecipientIndex(luaL_checkint(L, 2)));
  return 1;
}

static int C_RecipientFilter_IgnorePredictionCull (lua_State *L) {
  lua_pushboolean(L, luaL_checkrecipientfilter(L, 1).IgnorePredictionCull());
  return 1;
}

static int C_RecipientFilter_IsInitMessage (lua_State *L) {
  lua_pushboolean(L, luaL_checkrecipientfilter(L, 1).IsInitMessage());
  return 1;
}

static int C_RecipientFilter_IsReliable (lua_State *L) {
  lua_pushboolean(L, luaL_checkrecipientfilter(L, 1).IsReliable());
  return 1;
}

static int C_RecipientFilter_IsUsingPredictionRules (lua_State *L) {
  lua_pushboolean(L, luaL_checkrecipientfilter(L, 1).IsUsingPredictionRules());
  return 1;
}

static int C_RecipientFilter_MakeReliable (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).MakeReliable();
  return 0;
}

static int C_RecipientFilter_RemoveRecipient (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).RemoveRecipient(luaL_checkplayer(L, 2));
  return 0;
}

static int C_RecipientFilter_Reset (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).Reset();
  return 0;
}

static int C_RecipientFilter_SetIgnorePredictionCull (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).SetIgnorePredictionCull(luaL_checkboolean(L, 2));
  return 0;
}

static int C_RecipientFilter_UsePredictionRules (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).UsePredictionRules();
  return 0;
}

static int C_RecipientFilter___tostring (lua_State *L) {
  lua_pushfstring(L, "CRecipientFilter: %p", luaL_checkudata(L, 1, "CRecipientFilter"));
  return 1;
}


static const luaL_Reg C_RecipientFiltermeta[] = {
  {"AddAllPlayers", C_RecipientFilter_AddAllPlayers},
  {"AddRecipient", C_RecipientFilter_AddRecipient},
  {"AddRecipientsByPAS", C_RecipientFilter_AddRecipientsByPAS},
  {"AddRecipientsByPVS", C_RecipientFilter_AddRecipientsByPVS},
  {"CopyFrom", C_RecipientFilter_CopyFrom},
  {"GetRecipientCount", C_RecipientFilter_GetRecipientCount},
  {"GetRecipientIndex", C_RecipientFilter_GetRecipientIndex},
  {"IgnorePredictionCull", C_RecipientFilter_IgnorePredictionCull},
  {"IsInitMessage", C_RecipientFilter_IsInitMessage},
  {"IsReliable", C_RecipientFilter_IsReliable},
  {"IsUsingPredictionRules", C_RecipientFilter_IsUsingPredictionRules},
  {"MakeReliable", C_RecipientFilter_MakeReliable},
  {"RemoveRecipient", C_RecipientFilter_RemoveRecipient},
  {"Reset", C_RecipientFilter_Reset},
  {"SetIgnorePredictionCull", C_RecipientFilter_SetIgnorePredictionCull},
  {"UsePredictionRules", C_RecipientFilter_UsePredictionRules},
  {"__tostring", C_RecipientFilter___tostring},
  {NULL, NULL}
};


static int luasrc_C_RecipientFilter (lua_State *L) {
  C_RecipientFilter filter;
  lua_pushrecipientfilter(L, filter);
  return 1;
}


static const luaL_Reg C_RecipientFilter_funcs[] = {
  {"CRecipientFilter", luasrc_C_RecipientFilter},
  {NULL, NULL}
};


/*
** Open CRecipientFilter object
*/
LUALIB_API int luaopen_CRecipientFilter (lua_State *L) {
  luaL_newmetatable(L, "CRecipientFilter");
  luaL_register(L, NULL, C_RecipientFiltermeta);
  lua_pushvalue(L, -1);  /* push metatable */
  lua_setfield(L, -2, "__index");  /* metatable.__index = metatable */
  lua_pushstring(L, "recipientfilter");
  lua_setfield(L, -2, "__type");  /* metatable.__type = "recipientfilter" */
  luaL_register(L, "_G", C_RecipientFilter_funcs);
  lua_pop(L, 2);
  return 1;
}