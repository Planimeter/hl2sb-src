//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
//===========================================================================//

#define leiface_cpp

#include "Color.h"
#include "convar.h"
#include "lua.hpp"
#include "lColor.h"
#include "tier1/lconvar.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


static int cvar_AllocateDLLIdentifier (lua_State *L) {
  lua_pushinteger(L, cvar->AllocateDLLIdentifier());
  return 1;
}

static int cvar_ConsoleColorPrintf (lua_State *L) {
  cvar->ConsoleColorPrintf(luaL_checkcolor(L, 1), luaL_checkstring(L, 2));
  return 0;
}

static int cvar_ConsoleDPrintf (lua_State *L) {
  cvar->ConsoleDPrintf(luaL_checkstring(L, 1));
  return 0;
}

static int cvar_ConsolePrintf (lua_State *L) {
  cvar->ConsolePrintf(luaL_checkstring(L, 1));
  return 0;
}

static int cvar_FindVar (lua_State *L) {
  lua_pushconvar(L, const_cast<ConVar *>(cvar->FindVar(luaL_checkstring(L, 1))));
  return 1;
}

static int cvar_GetCommandLineValue (lua_State *L) {
  lua_pushstring(L, cvar->GetCommandLineValue(luaL_checkstring(L, 1)));
  return 1;
}

static int cvar_RevertFlaggedConVars (lua_State *L) {
  cvar->RevertFlaggedConVars(luaL_checkint(L, 1));
  return 0;
}


static const luaL_Reg cvarlib[] = {
  {"AllocateDLLIdentifier",   cvar_AllocateDLLIdentifier},
  {"ConsoleColorPrintf",   cvar_ConsoleColorPrintf},
  {"ConsoleDPrintf",   cvar_ConsoleDPrintf},
  {"ConsolePrintf",   cvar_ConsolePrintf},
  {"FindVar",   cvar_FindVar},
  {"GetCommandLineValue",   cvar_GetCommandLineValue},
  {"RevertFlaggedConVars",   cvar_RevertFlaggedConVars},
  {NULL, NULL}
};


/*
** Open cvar library
*/
int luaopen_cvar (lua_State *L) {
  luaL_register(L, "cvar", cvarlib);
  return 1;
}

