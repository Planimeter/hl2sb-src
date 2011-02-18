//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Utility code.
//
// $NoKeywords: $
//=============================================================================//

#define lutil_cpp

#include "util.h"
#include "../../LuaManager.h"
#include "lbaseentity_shared.h"
#include "lbaseplayer_shared.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static int luasrc_UTIL_PlayerByIndex (lua_State *L) {
  lua_pushplayer(L, UTIL_PlayerByIndex(luaL_checkint(L, 1)));
  return 1;
}

static int luasrc_UTIL_Remove (lua_State *L) {
  UTIL_Remove(luaL_checkentity(L, 1));
  return 0;
}

static int luasrc_UTIL_ClientPrintAll (lua_State *L) {
  UTIL_ClientPrintAll(luaL_checkint(L, 1), luaL_checkstring(L, 2));
  return 0;
}


static const luaL_Reg util_funcs[] = {
  {"UTIL_PlayerByIndex",  luasrc_UTIL_PlayerByIndex},
  {"UTIL_Remove",  luasrc_UTIL_Remove},
  {"UTIL_ClientPrintAll",  luasrc_UTIL_ClientPrintAll},
  {NULL, NULL}
};


int luaopen_UTIL (lua_State *L) {
  luaL_register(L, "_G", util_funcs);
  return 1;
}

