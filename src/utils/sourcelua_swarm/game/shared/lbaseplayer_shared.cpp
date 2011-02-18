//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Implements shared baseplayer class functionality
//
// $NoKeywords: $
//=============================================================================//

#define lbaseplayer_shared_cpp

#include "player.h"
#include "basecombatcharacter.h"
#include "../../LuaManager.h"
#include "lbaseplayer_shared.h"
#include "lbasecombatweapon_shared.h"
#include "lbaseentity_shared.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

/*
** access functions (stack -> C)
*/


LUA_API lua_CBasePlayer *lua_toplayer (lua_State *L, int idx) {
  lua_CBasePlayer **ppPlayer = (lua_CBasePlayer **)luaL_checkudata(L, idx, "CBasePlayer");
  return *ppPlayer;
}



/*
** push functions (C -> stack)
*/


LUA_API void lua_pushplayer (lua_State *L, CBasePlayer *pPlayer) {
  if (pPlayer == NULL)  /* avoid extra test when d is not 0 */
  {
    lua_pushnil(L);
  }
  else
  {
    lua_CBasePlayer **ppPlayer = (lua_CBasePlayer **)lua_newuserdata(L, sizeof(pPlayer));
    *ppPlayer = pPlayer;
    luaL_getmetatable(L, "CBasePlayer");
    lua_setmetatable(L, -2);
  }
}


LUALIB_API lua_CBasePlayer *luaL_checkplayer (lua_State *L, int narg) {
  lua_CBasePlayer *d = lua_toplayer(L, narg);
  if (d == NULL)  /* avoid extra test when d is not 0 */
    luaL_typerror(L, narg, "CBasePlayer");
  return d;
}


static int luasrc_CBasePlayer_GetActiveWeapon (lua_State *L) {
  CBaseCombatWeapon *pWeapon = CBaseCombatCharacter_GetActiveWeapon(reinterpret_cast<CBaseCombatCharacter *>(luaL_checkplayer(L, 1)));
  lua_pushweapon(L, pWeapon);
  return 1;
}

static int luasrc_CBasePlayer_GetUserID (lua_State *L) {
  lua_pushinteger(L, CBasePlayer_GetUserID(luaL_checkplayer(L, 1)));
  return 1;
}

static int luasrc_CBasePlayer_GiveNamedItem (lua_State *L) {
  lua_pushentity(L, CBasePlayer_GiveNamedItem(luaL_checkplayer(L, 1), luaL_checkstring(L, 2), luaL_optint(L, 3, 0)));
  return 1;
}

static int luasrc_CBasePlayer___index (lua_State *L) {
  lua_getmetatable(L, 1);
  lua_pushvalue(L, 2);
  lua_gettable(L, -2);
  if (lua_isnil(L, -1))
  {
    luaL_getmetatable(L, "CBaseEntity");
    lua_pushvalue(L, 2);
    lua_gettable(L, -2);
  }
  return 1;
}

static int luasrc_CBasePlayer___tostring (lua_State *L)
{
  lua_pushfstring(L, "CBasePlayer: %d", CBasePlayer_GetUserID(luaL_checkplayer(L, 1)));
  return 1;
}


static const luaL_Reg CBasePlayermeta[] = {
  {"GetActiveWeapon", luasrc_CBasePlayer_GetActiveWeapon},
  {"GetUserID", luasrc_CBasePlayer_GetUserID},
  {"GiveNamedItem", luasrc_CBasePlayer_GiveNamedItem},
  {"__index", luasrc_CBasePlayer___index},
  {"__tostring", luasrc_CBasePlayer___tostring},
  {NULL, NULL}
};


/*
** Open CBasePlayer object
*/
int luaopen_CBasePlayer (lua_State *L) {
  luaL_newmetatable(L, "CBasePlayer");
  luaL_register(L, NULL, CBasePlayermeta);
  lua_pushstring(L, "entity");
  lua_setfield(L, -2, "__type");  /* metatable.__type = "entity" */
  lua_pop(L, 1);
  return 1;
}

