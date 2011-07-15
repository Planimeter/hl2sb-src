//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: Functions dealing with the player.
//
//===========================================================================//

#define lplayer_cpp

#include "cbase.h"
#include "luamanager.h"
#include "lbaseplayer_shared.h"
#include "mathlib/lvector.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static int CBasePlayer_SetBodyPitch (lua_State *L) {
  luaL_checkplayer(L, 1)->SetBodyPitch(luaL_checknumber(L, 2));
  return 0;
}

static int CBasePlayer_CreateViewModel (lua_State *L) {
  luaL_checkplayer(L, 1)->CreateViewModel(luaL_optinteger(L, 2, 0));
  return 0;
}

static int CBasePlayer_HideViewModels (lua_State *L) {
  luaL_checkplayer(L, 1)->HideViewModels();
  return 0;
}

static int CBasePlayer_DestroyViewModels (lua_State *L) {
  luaL_checkplayer(L, 1)->DestroyViewModels();
  return 0;
}

static int CBasePlayer_RequiredEdictIndex (lua_State *L) {
  lua_pushinteger(L, luaL_checkplayer(L, 1)->RequiredEdictIndex());
  return 1;
}

static int CBasePlayer_LockPlayerInPlace (lua_State *L) {
  luaL_checkplayer(L, 1)->LockPlayerInPlace();
  return 0;
}

static int CBasePlayer_UnlockPlayer (lua_State *L) {
  luaL_checkplayer(L, 1)->UnlockPlayer();
  return 0;
}

static int CBasePlayer_DrawDebugGeometryOverlays (lua_State *L) {
  luaL_checkplayer(L, 1)->DrawDebugGeometryOverlays();
  return 0;
}

static int CBasePlayer_UpdateTransmitState (lua_State *L) {
  luaL_checkplayer(L, 1)->UpdateTransmitState();
  return 0;
}

static int CBasePlayer_ForceRespawn (lua_State *L) {
  luaL_checkplayer(L, 1)->ForceRespawn();
  return 0;
}

static int CBasePlayer_InitialSpawn (lua_State *L) {
  luaL_checkplayer(L, 1)->InitialSpawn();
  return 0;
}

static int CBasePlayer_InitHUD (lua_State *L) {
  luaL_checkplayer(L, 1)->InitHUD();
  return 0;
}

static int CBasePlayer_PlayerDeathThink (lua_State *L) {
  luaL_checkplayer(L, 1)->PlayerDeathThink();
  return 0;
}

static int CBasePlayer_Jump (lua_State *L) {
  luaL_checkplayer(L, 1)->Jump();
  return 0;
}

static int CBasePlayer_Duck (lua_State *L) {
  luaL_checkplayer(L, 1)->Duck();
  return 0;
}

static int CBasePlayer_ForceSimulation (lua_State *L) {
  luaL_checkplayer(L, 1)->ForceSimulation();
  return 0;
}

static int CBasePlayer_TakeHealth (lua_State *L) {
  lua_pushinteger(L, luaL_checkplayer(L, 1)->TakeHealth(luaL_checknumber(L, 2), luaL_checkinteger(L, 3)));
  return 1;
}

static int CBasePlayer_DamageEffect (lua_State *L) {
  luaL_checkplayer(L, 1)->DamageEffect(luaL_checknumber(L, 2), luaL_checkinteger(L, 3));
  return 0;
}

static int CBasePlayer_PauseBonusProgress (lua_State *L) {
  luaL_checkplayer(L, 1)->PauseBonusProgress(luaL_optboolean(L, 2, true));
  return 0;
}

static int CBasePlayer_SetBonusProgress (lua_State *L) {
  luaL_checkplayer(L, 1)->SetBonusProgress(luaL_checkinteger(L, 2));
  return 0;
}

static int CBasePlayer_SetBonusChallenge (lua_State *L) {
  luaL_checkplayer(L, 1)->SetBonusChallenge(luaL_checkinteger(L, 2));
  return 0;
}

static int CBasePlayer_GetBonusProgress (lua_State *L) {
  lua_pushinteger(L, luaL_checkplayer(L, 1)->GetBonusProgress());
  return 1;
}

static int CBasePlayer_GetBonusChallenge (lua_State *L) {
  lua_pushinteger(L, luaL_checkplayer(L, 1)->GetBonusChallenge());
  return 1;
}

static int CBasePlayer_SnapEyeAngles (lua_State *L) {
  luaL_checkplayer(L, 1)->SnapEyeAngles(luaL_checkangle(L, 2));
  return 0;
}


static const luaL_Reg CBasePlayermeta[] = {
  {"SetBodyPitch", CBasePlayer_SetBodyPitch},
  {"CreateViewModel", CBasePlayer_CreateViewModel},
  {"HideViewModels", CBasePlayer_HideViewModels},
  {"DestroyViewModels", CBasePlayer_DestroyViewModels},
  {"RequiredEdictIndex", CBasePlayer_RequiredEdictIndex},
  {"LockPlayerInPlace", CBasePlayer_LockPlayerInPlace},
  {"UnlockPlayer", CBasePlayer_UnlockPlayer},
  {"DrawDebugGeometryOverlays", CBasePlayer_DrawDebugGeometryOverlays},
  {"UpdateTransmitState", CBasePlayer_UpdateTransmitState},
  {"ForceRespawn", CBasePlayer_ForceRespawn},
  {"InitialSpawn", CBasePlayer_InitialSpawn},
  {"InitHUD", CBasePlayer_InitHUD},
  {"PlayerDeathThink", CBasePlayer_PlayerDeathThink},
  {"Jump", CBasePlayer_Jump},
  {"Duck", CBasePlayer_Duck},
  {"ForceSimulation", CBasePlayer_ForceSimulation},
  {"TakeHealth", CBasePlayer_TakeHealth},
  {"DamageEffect", CBasePlayer_DamageEffect},
  {"PauseBonusProgress", CBasePlayer_PauseBonusProgress},
  {"SetBonusProgress", CBasePlayer_SetBonusProgress},
  {"SetBonusChallenge", CBasePlayer_SetBonusChallenge},
  {"GetBonusProgress", CBasePlayer_GetBonusProgress},
  {"GetBonusChallenge", CBasePlayer_GetBonusChallenge},
  {"SnapEyeAngles", CBasePlayer_SnapEyeAngles},
  {NULL, NULL}
};


/*
** Open CBasePlayer object
*/
int luaopen_CBasePlayer (lua_State *L) {
  luaL_getmetatable(L, "CBasePlayer");
  if (lua_isnoneornil(L, -1)) {
    lua_pop(L, 1);
    luaL_newmetatable(L, "CBasePlayer");
  }
  luaL_register(L, NULL, CBasePlayermeta);
  lua_pop(L, 1);
  return 1;
}

