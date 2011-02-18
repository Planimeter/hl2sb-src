//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: This module defines the IVoiceServer interface, which is used by
//			game code to control which clients are listening to which other
//			clients' voice streams.
//
// $NoKeywords: $
//=============================================================================//

#define livoiceserver_cpp

#include "ivoiceserver.h"
#include "../LuaManager.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern IVoiceServer	*g_pVoiceServer;

static int g_pVoiceServer_GetClientListening (lua_State *L) {
  lua_pushboolean(L, g_pVoiceServer->GetClientListening(luaL_checkint(L, 1), luaL_checkint(L, 2)));
  return 1;
}

static int g_pVoiceServer_SetClientListening (lua_State *L) {
  lua_pushboolean(L, g_pVoiceServer->SetClientListening(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkboolean(L, 3)));
  return 1;
}

static int g_pVoiceServer_SetClientProximity (lua_State *L) {
  lua_pushboolean(L, g_pVoiceServer->SetClientProximity(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkboolean(L, 3)));
  return 1;
}


static const luaL_Reg g_pVoiceServerlib[] = {
  {"GetClientListening",   g_pVoiceServer_GetClientListening},
  {"SetClientListening",   g_pVoiceServer_SetClientListening},
  {"SetClientProximity",   g_pVoiceServer_SetClientProximity},
  {NULL, NULL}
};


/*
** Open g_pVoiceServer library
*/
int luaopen_g_pVoiceServer (lua_State *L) {
  luaL_register(L, "g_pVoiceServer", g_pVoiceServerlib);
  return 1;
}

