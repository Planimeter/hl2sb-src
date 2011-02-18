//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ========//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#define ldbg_cpp

#include "dbg.h"
#include "Color.h"
#include "lua.hpp"
#include "lColor.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"



static int luasrc_Msg (lua_State *L) {
  Msg(luaL_checkstring(L, 1));
  return 0;
}

static int luasrc_Warning (lua_State *L) {
  Warning(luaL_checkstring(L, 1));
  return 0;
}

static int luasrc_Error (lua_State *L) {
  Error(luaL_checkstring(L, 1));
  return 0;
}

static int luasrc_DevMsg (lua_State *L) {
  DevMsg(luaL_checkstring(L, 1));
  return 0;
}

static int luasrc_DevWarning (lua_State *L) {
  DevWarning(luaL_checkstring(L, 1));
  return 0;
}

static int luasrc_ConColorMsg (lua_State *L) {
  ConColorMsg(luaL_checkcolor(L, 1), luaL_checkstring(L, 2));
  return 0;
}

static int luasrc_ConMsg (lua_State *L) {
  ConMsg(luaL_checkstring(L, 1));
  return 0;
}

static int luasrc_ConDMsg (lua_State *L) {
  ConDMsg(luaL_checkstring(L, 1));
  return 0;
}


static const luaL_Reg dbg_funcs[] = {
  {"Msg",  luasrc_Msg},
  {"Warning",  luasrc_Warning},
  {"Error",  luasrc_Error},
  {"DevMsg",  luasrc_DevMsg},
  {"DevWarning",  luasrc_DevWarning},
  {"ConColorMsg",  luasrc_ConColorMsg},
  {"ConMsg",  luasrc_ConMsg},
  {"ConDMsg",  luasrc_ConDMsg},
  {NULL, NULL}
};


int luaopen_dbg (lua_State *L) {
  luaL_register(L, "_G", dbg_funcs);
  return 1;
}

