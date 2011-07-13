//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Definitions that are shared by the game DLL and the client DLL.
//
// $NoKeywords: $
//=============================================================================//

#define lshareddefs_cpp

#include "cbase.h"
#include "ammodef.h"
#include "luamanager.h"
#include "lshareddefs.h"
#include "lbaseentity_shared.h"
#include "mathlib/lvector.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


/*
** access functions (stack -> C)
*/


LUA_API lua_FireBulletsInfo_t lua_tofirebulletsinfo (lua_State *L, int idx) {
  luaL_checktype(L, idx, LUA_TTABLE);
  FireBulletsInfo_t info;
  lua_getfield(L, idx, "m_bPrimaryAttack");
  if (!lua_isnil(L, -1))
    info.m_bPrimaryAttack = luaL_checkboolean(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, idx, "m_flDamageForceScale");
  if (!lua_isnil(L, -1))
    info.m_flDamageForceScale = luaL_checknumber(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, idx, "m_flDistance");
  if (!lua_isnil(L, -1))
    info.m_flDistance = luaL_checknumber(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, idx, "m_iAmmoType");
  if (!lua_isnil(L, -1))
    info.m_iAmmoType = luaL_checkinteger(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, idx, "m_iDamage");
  if (!lua_isnil(L, -1))
    info.m_iDamage = luaL_checkinteger(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, idx, "m_iPlayerDamage");
  if (!lua_isnil(L, -1))
    info.m_iPlayerDamage = luaL_checkinteger(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, idx, "m_iShots");
  if (!lua_isnil(L, -1))
    info.m_iShots = luaL_checkinteger(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, idx, "m_iTracerFreq");
  if (!lua_isnil(L, -1))
    info.m_iTracerFreq = luaL_checkinteger(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, idx, "m_nFlags");
  if (!lua_isnil(L, -1))
    info.m_nFlags = luaL_checkinteger(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, idx, "m_pAdditionalIgnoreEnt");
  if (!lua_isnil(L, -1))
    info.m_pAdditionalIgnoreEnt = lua_toentity(L, -1);
  lua_pop(L, 1);
  // Andrew; No idea why this is crashing.
  /*
  lua_getfield(L, idx, "m_pAttacker");
  if (!lua_isnil(L, -1))
    info.m_pAttacker = lua_toentity(L, -1);
  lua_pop(L, 1);
  */
  lua_getfield(L, idx, "m_vecDirShooting");
  if (!lua_isnil(L, -1))
    info.m_vecDirShooting = luaL_checkvector(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, idx, "m_vecSpread");
  if (!lua_isnil(L, -1))
    info.m_vecSpread = luaL_checkvector(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, idx, "m_vecSrc");
  if (!lua_isnil(L, -1))
    info.m_vecSrc = luaL_checkvector(L, -1);
  lua_pop(L, 1);
  return info;
}

