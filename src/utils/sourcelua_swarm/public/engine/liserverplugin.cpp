//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//

#define liserverplugin_cpp

#include "edict.h"
#include "engine/iserverplugin.h"
#include "eiface.h"
#include "../../LuaManager.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern IServerPluginHelpers *helpers; // special 3rd party plugin helpers from the engine
extern IVEngineServer	*engine;

static int helpers_ClientCommand (lua_State *L) {
  edict_t *pEntity = engine->GetSplitScreenPlayerAttachToEdict(luaL_checkint(L, 1));
  helpers->ClientCommand(pEntity, luaL_checkstring(L, 2));
  return 0;
}

static int helpers_StartQueryCvarValue (lua_State *L) {
  edict_t *pEntity = engine->GetSplitScreenPlayerAttachToEdict(luaL_checkint(L, 1));
  lua_pushinteger(L, helpers->StartQueryCvarValue(pEntity, luaL_checkstring(L, 2)));
  return 1;
}


static const luaL_Reg helperslib[] = {
  {"ClientCommand",   helpers_ClientCommand},
  {"StartQueryCvarValue",   helpers_StartQueryCvarValue},
  {NULL, NULL}
};


/*
** Open helpers library
*/
int luaopen_helpers (lua_State *L) {
  luaL_register(L, "helpers", helperslib);
  return 1;
}

