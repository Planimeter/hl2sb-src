//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#define lrecipientfilter_cpp

#include "cbase.h"
#include "recipientfilter.h"
#include "luamanager.h"
#include "lrecipientfilter.h"
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


static int CRecipientFilter_AddAllPlayers (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).AddAllPlayers();
  return 0;
}

static int CRecipientFilter_AddRecipient (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).AddRecipient(luaL_checkplayer(L, 2));
  return 0;
}

static int CRecipientFilter_AddRecipientsByPAS (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).AddRecipientsByPAS(luaL_checkvector(L, 2));
  return 0;
}

static int CRecipientFilter_AddRecipientsByPVS (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).AddRecipientsByPVS(luaL_checkvector(L, 2));
  return 0;
}

static int CRecipientFilter_CopyFrom (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).CopyFrom(luaL_checkrecipientfilter(L, 2));
  return 0;
}

static int CRecipientFilter_GetRecipientCount (lua_State *L) {
  lua_pushinteger(L, luaL_checkrecipientfilter(L, 1).GetRecipientCount());
  return 1;
}

static int CRecipientFilter_GetRecipientIndex (lua_State *L) {
  lua_pushinteger(L, luaL_checkrecipientfilter(L, 1).GetRecipientIndex(luaL_checkint(L, 2)));
  return 1;
}

static int CRecipientFilter_IgnorePredictionCull (lua_State *L) {
  lua_pushboolean(L, luaL_checkrecipientfilter(L, 1).IgnorePredictionCull());
  return 1;
}

static int CRecipientFilter_IsInitMessage (lua_State *L) {
  lua_pushboolean(L, luaL_checkrecipientfilter(L, 1).IsInitMessage());
  return 1;
}

static int CRecipientFilter_IsReliable (lua_State *L) {
  lua_pushboolean(L, luaL_checkrecipientfilter(L, 1).IsReliable());
  return 1;
}

static int CRecipientFilter_IsUsingPredictionRules (lua_State *L) {
  lua_pushboolean(L, luaL_checkrecipientfilter(L, 1).IsUsingPredictionRules());
  return 1;
}

static int CRecipientFilter_MakeInitMessage (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).MakeInitMessage();
  return 0;
}

static int CRecipientFilter_MakeReliable (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).MakeReliable();
  return 0;
}

static int CRecipientFilter_RemoveAllRecipients (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).RemoveAllRecipients();
  return 0;
}

static int CRecipientFilter_RemoveRecipient (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).RemoveRecipient(luaL_checkplayer(L, 2));
  return 0;
}

static int CRecipientFilter_RemoveRecipientByPlayerIndex (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).RemoveRecipientByPlayerIndex(luaL_checkint(L, 2));
  return 0;
}

static int CRecipientFilter_RemoveRecipientsByPVS (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).RemoveRecipientsByPVS(luaL_checkvector(L, 2));
  return 0;
}

static int CRecipientFilter_Reset (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).Reset();
  return 0;
}

static int CRecipientFilter_SetIgnorePredictionCull (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).SetIgnorePredictionCull(luaL_checkboolean(L, 2));
  return 0;
}

static int CRecipientFilter_UsePredictionRules (lua_State *L) {
  luaL_checkrecipientfilter(L, 1).UsePredictionRules();
  return 0;
}

static int CRecipientFilter___tostring (lua_State *L) {
  lua_pushfstring(L, "CRecipientFilter: %p", luaL_checkudata(L, 1, "CRecipientFilter"));
  return 1;
}


static const luaL_Reg CRecipientFiltermeta[] = {
  {"AddAllPlayers", CRecipientFilter_AddAllPlayers},
  {"AddRecipient", CRecipientFilter_AddRecipient},
  {"AddRecipientsByPAS", CRecipientFilter_AddRecipientsByPAS},
  {"AddRecipientsByPVS", CRecipientFilter_AddRecipientsByPVS},
  {"CopyFrom", CRecipientFilter_CopyFrom},
  {"GetRecipientCount", CRecipientFilter_GetRecipientCount},
  {"GetRecipientIndex", CRecipientFilter_GetRecipientIndex},
  {"IgnorePredictionCull", CRecipientFilter_IgnorePredictionCull},
  {"IsInitMessage", CRecipientFilter_IsInitMessage},
  {"IsReliable", CRecipientFilter_IsReliable},
  {"IsUsingPredictionRules", CRecipientFilter_IsUsingPredictionRules},
  {"MakeInitMessage", CRecipientFilter_MakeInitMessage},
  {"MakeReliable", CRecipientFilter_MakeReliable},
  {"RemoveAllRecipients", CRecipientFilter_RemoveAllRecipients},
  {"RemoveRecipient", CRecipientFilter_RemoveRecipient},
  {"RemoveRecipientByPlayerIndex", CRecipientFilter_RemoveRecipientByPlayerIndex},
  {"RemoveRecipientsByPVS", CRecipientFilter_RemoveRecipientsByPVS},
  {"Reset", CRecipientFilter_Reset},
  {"SetIgnorePredictionCull", CRecipientFilter_SetIgnorePredictionCull},
  {"UsePredictionRules", CRecipientFilter_UsePredictionRules},
  {"__tostring", CRecipientFilter___tostring},
  {NULL, NULL}
};


static int luasrc_CRecipientFilter (lua_State *L) {
  CRecipientFilter filter;
  lua_pushrecipientfilter(L, filter);
  return 1;
}


static const luaL_Reg CRecipientFilter_funcs[] = {
  {"CRecipientFilter", luasrc_CRecipientFilter},
  {NULL, NULL}
};


/*
** Open CRecipientFilter object
*/
LUALIB_API int luaopen_CRecipientFilter (lua_State *L) {
  luaL_newmetatable(L, "CRecipientFilter");
  luaL_register(L, NULL, CRecipientFiltermeta);
  lua_pushvalue(L, -1);  /* push metatable */
  lua_setfield(L, -2, "__index");  /* metatable.__index = metatable */
  lua_pushstring(L, "recipientfilter");
  lua_setfield(L, -2, "__type");  /* metatable.__type = "recipientfilter" */
  luaL_register(L, "_G", CRecipientFilter_funcs);
  lua_pop(L, 2);
  return 1;
}