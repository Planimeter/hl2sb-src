//====== Copyright © 1996-2004, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#define lsteam_api_cpp

#include "cbase.h"
#include "steam_api.h"
#include "lua.hpp"
#include "listeamfriends.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static int steamapicontext_Clear (lua_State *L) {
  steamapicontext->Clear();
  return 0;
}

static int steamapicontext_Init (lua_State *L) {
  lua_pushboolean(L, steamapicontext->Init());
  return 1;
}

static int steamapicontext_SteamFriends (lua_State *L) {
  lua_pushsteamfriends(L, steamapicontext->SteamFriends());
  return 1;
}


static const luaL_Reg steamapicontextlib[] = {
  {"Clear",   steamapicontext_Clear},
  {"Init",   steamapicontext_Init},
  {"SteamFriends",   steamapicontext_SteamFriends},
  {NULL, NULL}
};


/*
** Open steamapicontext library
*/
LUALIB_API int luaopen_steamapicontext (lua_State *L) {
  luaL_register(L, "steamapicontext", steamapicontextlib);
  lua_pop(L, 1);
  return 1;
}

