//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: Functions dealing with the player.
//
//===========================================================================//

#define lplayer_cpp

#include "cbase.h"
#include "luamanager.h"
#include "lbasecombatweapon_shared.h"
#include "lbaseentity_shared.h"
#include "lbaseplayer_shared.h"
#include "ltakedamageinfo.h"
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
  luaL_checkplayer(L, 1)->SnapEyeAngles(*(QAngle *)luaL_checkangle(L, 2));
  return 0;
}

static int CBasePlayer_BodyAngles (lua_State *L) {
  QAngle v = luaL_checkplayer(L, 1)->BodyAngles();
  lua_pushangle(L, &v);
  return 1;
}

static int CBasePlayer_BodyTarget (lua_State *L) {
  Vector v = luaL_checkplayer(L, 1)->BodyTarget(*(Vector *)luaL_checkvector(L, 2), luaL_checkboolean(L, 3));
  lua_pushvector(L, &v);
  return 1;
}

static int CBasePlayer_ShouldFadeOnDeath (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->ShouldFadeOnDeath());
  return 1;
}

static int CBasePlayer_OnTakeDamage_Alive (lua_State *L) {
  lua_pushinteger(L, luaL_checkplayer(L, 1)->OnTakeDamage_Alive(*(CTakeDamageInfo *)luaL_checkdamageinfo(L, 2)));
  return 1;
}

static int CBasePlayer_Event_Killed (lua_State *L) {
  luaL_checkplayer(L, 1)->Event_Killed(*(CTakeDamageInfo *)luaL_checkdamageinfo(L, 2));
  return 0;
}

static int CBasePlayer_Event_KilledOther (lua_State *L) {
  luaL_checkplayer(L, 1)->Event_KilledOther(luaL_checkentity(L, 2), *(CTakeDamageInfo *)luaL_checkdamageinfo(L, 3));
  return 0;
}

static int CBasePlayer_Event_Dying (lua_State *L) {
  luaL_checkplayer(L, 1)->Event_Dying();
  return 0;
}

static int CBasePlayer_IsNetClient (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->IsNetClient());
  return 1;
}

static int CBasePlayer_IsFakeClient (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->IsFakeClient());
  return 1;
}

static int CBasePlayer_GetClientIndex (lua_State *L) {
  lua_pushinteger(L, luaL_checkplayer(L, 1)->GetClientIndex());
  return 1;
}

static int CBasePlayer_SetPlayerName (lua_State *L) {
  luaL_checkplayer(L, 1)->SetPlayerName(luaL_checkstring(L, 2));
  return 0;
}

static int CBasePlayer_GetNetworkIDString (lua_State *L) {
  lua_pushstring(L, luaL_checkplayer(L, 1)->GetNetworkIDString());
  return 1;
}

static int CBasePlayer_ShowViewModel (lua_State *L) {
  luaL_checkplayer(L, 1)->ShowViewModel(luaL_checkboolean(L, 2));
  return 0;
}

static int CBasePlayer_ShowCrosshair (lua_State *L) {
  luaL_checkplayer(L, 1)->ShowCrosshair(luaL_checkboolean(L, 2));
  return 0;
}

static int CBasePlayer_PackDeadPlayerItems (lua_State *L) {
  luaL_checkplayer(L, 1)->PackDeadPlayerItems();
  return 0;
}

static int CBasePlayer_RemoveAllItems (lua_State *L) {
  luaL_checkplayer(L, 1)->RemoveAllItems(luaL_checkboolean(L, 2));
  return 0;
}

static int CBasePlayer_IsDead (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->IsDead());
  return 1;
}

static int CBasePlayer_HasPhysicsFlag (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->HasPhysicsFlag(luaL_checkinteger(L, 2)));
  return 1;
}

static int CBasePlayer_Weapon_CanUse (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->Weapon_CanUse(luaL_checkweapon(L, 2)));
  return 1;
}

static int CBasePlayer_Weapon_Equip (lua_State *L) {
  luaL_checkplayer(L, 1)->Weapon_Equip(luaL_checkweapon(L, 2));
  return 0;
}

static int CBasePlayer_Weapon_Drop (lua_State *L) {
  luaL_checkplayer(L, 1)->Weapon_Drop(luaL_checkweapon(L, 2), luaL_checkvector(L, 3), luaL_checkvector(L, 4));
  return 0;
}

static int CBasePlayer_Weapon_DropSlot (lua_State *L) {
  luaL_checkplayer(L, 1)->Weapon_DropSlot(luaL_checkinteger(L, 2));
  return 0;
}

static int CBasePlayer_Weapon_GetLast (lua_State *L) {
  lua_pushweapon(L, luaL_checkplayer(L, 1)->Weapon_GetLast());
  return 1;
}

static int CBasePlayer_HasAnyAmmoOfType (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->HasAnyAmmoOfType(luaL_checkinteger(L, 2)));
  return 1;
}

static int CBasePlayer_RumbleEffect (lua_State *L) {
  luaL_checkplayer(L, 1)->RumbleEffect(luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4));
  return 0;
}

static int CBasePlayer_IsOnLadder (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->IsOnLadder());
  return 1;
}

static int CBasePlayer_SetFlashlightEnabled (lua_State *L) {
  luaL_checkplayer(L, 1)->SetFlashlightEnabled(luaL_checkboolean(L, 2));
  return 0;
}

static int CBasePlayer_FlashlightIsOn (lua_State *L) {
  lua_pushinteger(L, luaL_checkplayer(L, 1)->FlashlightIsOn());
  return 1;
}

static int CBasePlayer_FlashlightTurnOn (lua_State *L) {
  luaL_checkplayer(L, 1)->FlashlightTurnOn();
  return 0;
}

static int CBasePlayer_FlashlightTurnOff (lua_State *L) {
  luaL_checkplayer(L, 1)->FlashlightTurnOff();
  return 0;
}

static int CBasePlayer_IsIlluminatedByFlashlight (lua_State *L) {
  float flDot;
  lua_pushboolean(L, luaL_checkplayer(L, 1)->IsIlluminatedByFlashlight(luaL_checkentity(L, 2), &flDot));
  lua_pushnumber(L, flDot);
  return 2;
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
  {"BodyAngles", CBasePlayer_BodyAngles},
  {"BodyTarget", CBasePlayer_BodyTarget},
  {"ShouldFadeOnDeath", CBasePlayer_ShouldFadeOnDeath},
  {"OnTakeDamage_Alive", CBasePlayer_OnTakeDamage_Alive},
  {"Event_Killed", CBasePlayer_Event_Killed},
  {"Event_KilledOther", CBasePlayer_Event_KilledOther},
  {"Event_Dying", CBasePlayer_Event_Dying},
  {"IsNetClient", CBasePlayer_IsNetClient},
  {"IsFakeClient", CBasePlayer_IsFakeClient},
  {"GetClientIndex", CBasePlayer_GetClientIndex},
  {"SetPlayerName", CBasePlayer_SetPlayerName},
  {"GetNetworkIDString", CBasePlayer_GetNetworkIDString},
  {"ShowViewModel", CBasePlayer_ShowViewModel},
  {"ShowCrosshair", CBasePlayer_ShowCrosshair},
  {"PackDeadPlayerItems", CBasePlayer_PackDeadPlayerItems},
  {"RemoveAllItems", CBasePlayer_RemoveAllItems},
  {"IsDead", CBasePlayer_IsDead},
  {"HasPhysicsFlag", CBasePlayer_HasPhysicsFlag},
  {"Weapon_CanUse", CBasePlayer_Weapon_CanUse},
  {"Weapon_Equip", CBasePlayer_Weapon_Equip},
  {"Weapon_Drop", CBasePlayer_Weapon_Drop},
  {"Weapon_DropSlot", CBasePlayer_Weapon_DropSlot},
  {"Weapon_GetLast", CBasePlayer_Weapon_GetLast},
  {"HasAnyAmmoOfType", CBasePlayer_HasAnyAmmoOfType},
  {"RumbleEffect", CBasePlayer_RumbleEffect},
  {"IsOnLadder", CBasePlayer_IsOnLadder},
  {"SetFlashlightEnabled", CBasePlayer_SetFlashlightEnabled},
  {"FlashlightIsOn", CBasePlayer_FlashlightIsOn},
  {"FlashlightTurnOn", CBasePlayer_FlashlightTurnOn},
  {"FlashlightTurnOff", CBasePlayer_FlashlightTurnOff},
  {"IsIlluminatedByFlashlight", CBasePlayer_IsIlluminatedByFlashlight},
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

