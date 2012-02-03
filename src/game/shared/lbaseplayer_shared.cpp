//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Implements shared baseplayer class functionality
//
// $NoKeywords: $
//=============================================================================//

#define lbaseplayer_shared_cpp

#include "cbase.h"
#include "luamanager.h"
#include "lbaseplayer_shared.h"
#include "lbasecombatweapon_shared.h"
#include "lbaseentity_shared.h"
#include "mathlib/lvector.h"
#include "lvphysics_interface.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

/*
** access functions (stack -> C)
*/


LUA_API lua_CBasePlayer *lua_toplayer (lua_State *L, int idx) {
  CBaseHandle *hPlayer = dynamic_cast<CBaseHandle *>((CBaseHandle *)lua_touserdata(L, idx));
  if (hPlayer == NULL)
    return NULL;
  return dynamic_cast<lua_CBasePlayer *>(hPlayer->Get());
}



/*
** push functions (C -> stack)
*/


LUA_API void lua_pushplayer (lua_State *L, CBasePlayer *pPlayer) {
  CBaseHandle *hPlayer = (CBaseHandle *)lua_newuserdata(L, sizeof(CBaseHandle));
  hPlayer->Set(pPlayer);
  luaL_getmetatable(L, "CBasePlayer");
  lua_setmetatable(L, -2);
}


LUALIB_API lua_CBasePlayer *luaL_checkplayer (lua_State *L, int narg) {
  lua_CBasePlayer *d = lua_toplayer(L, narg);
  if (d == NULL)  /* avoid extra test when d is not 0 */
    luaL_argerror(L, narg, "attempt to index a NULL entity");
  return d;
}


LUALIB_API lua_CBasePlayer *luaL_optplayer (lua_State *L, int narg,
                                                          CBasePlayer *def) {
  return luaL_opt(L, luaL_checkplayer, narg, def);
}


static int CBasePlayer_AbortReload (lua_State *L) {
  luaL_checkplayer(L, 1)->AbortReload();
  return 0;
}

static int CBasePlayer_AddToPlayerSimulationList (lua_State *L) {
  luaL_checkplayer(L, 1)->AddToPlayerSimulationList(luaL_checkentity(L, 2));
  return 0;
}

static int CBasePlayer_CacheVehicleView (lua_State *L) {
  luaL_checkplayer(L, 1)->CacheVehicleView();
  return 0;
}

static int CBasePlayer_ClearPlayerSimulationList (lua_State *L) {
  luaL_checkplayer(L, 1)->ClearPlayerSimulationList();
  return 0;
}

static int CBasePlayer_ClearZoomOwner (lua_State *L) {
  luaL_checkplayer(L, 1)->ClearZoomOwner();
  return 0;
}

static int CBasePlayer_CurrentCommandNumber (lua_State *L) {
  lua_pushinteger(L, luaL_checkplayer(L, 1)->CurrentCommandNumber());
  return 1;
}

static int CBasePlayer_DoMuzzleFlash (lua_State *L) {
  luaL_checkplayer(L, 1)->DoMuzzleFlash();
  return 0;
}

static int CBasePlayer_ExitLadder (lua_State *L) {
  luaL_checkplayer(L, 1)->ExitLadder();
  return 0;
}

static int CBasePlayer_EyeAngles (lua_State *L) {
  QAngle v = luaL_checkplayer(L, 1)->EyeAngles();
  lua_pushangle(L, &v);
  return 1;
}

static int CBasePlayer_EyePosition (lua_State *L) {
  Vector v = luaL_checkplayer(L, 1)->EyePosition();
  lua_pushvector(L, &v);
  return 1;
}

static int CBasePlayer_EyePositionAndVectors (lua_State *L) {
  Vector pPosition, pForward, pRight, pUp;
  luaL_checkplayer(L, 1)->EyePositionAndVectors(&pPosition, &pForward, &pRight, &pUp);
  lua_pushvector(L, &pPosition);
  lua_pushvector(L, &pForward);
  lua_pushvector(L, &pRight);
  lua_pushvector(L, &pUp);
  return 4;
}

static int CBasePlayer_EyeVectors (lua_State *L) {
  Vector pForward, pRight, pUp;
  luaL_checkplayer(L, 1)->EyeVectors(&pForward, &pRight, &pUp);
  lua_pushvector(L, &pForward);
  lua_pushvector(L, &pRight);
  lua_pushvector(L, &pUp);
  return 3;
}

static int CBasePlayer_FindUseEntity (lua_State *L) {
  CBaseEntity *pUseEntity = luaL_checkplayer(L, 1)->FindUseEntity();
  lua_pushentity(L, pUseEntity);
  return 1;
}

static int CBasePlayer_GetActiveWeapon (lua_State *L) {
  CBaseCombatWeapon *pWeapon = luaL_checkplayer(L, 1)->GetActiveWeapon();
  lua_pushweapon(L, pWeapon);
  return 1;
}

static int CBasePlayer_GetAutoaimVector (lua_State *L) {
  Vector v = luaL_checkplayer(L, 1)->GetAutoaimVector(luaL_checknumber(L, 2));
  lua_pushvector(L, &v);
  return 1;
}

static int CBasePlayer_GetBonusChallenge (lua_State *L) {
  lua_pushinteger(L, luaL_checkplayer(L, 1)->GetBonusChallenge());
  return 1;
}

static int CBasePlayer_GetBonusProgress (lua_State *L) {
  lua_pushinteger(L, luaL_checkplayer(L, 1)->GetBonusProgress());
  return 1;
}

static int CBasePlayer_GetDeathTime (lua_State *L) {
  lua_pushnumber(L, luaL_checkplayer(L, 1)->GetDeathTime());
  return 1;
}

static int CBasePlayer_GetDefaultFOV (lua_State *L) {
  lua_pushinteger(L, luaL_checkplayer(L, 1)->GetDefaultFOV());
  return 1;
}

static int CBasePlayer_GetEFNoInterpParity (lua_State *L) {
  lua_pushinteger(L, luaL_checkplayer(L, 1)->GetEFNoInterpParity());
  return 1;
}

static int CBasePlayer_GetFOV (lua_State *L) {
  lua_pushnumber(L, luaL_checkplayer(L, 1)->GetFOV());
  return 1;
}

static int CBasePlayer_GetFOVDistanceAdjustFactor (lua_State *L) {
  lua_pushnumber(L, luaL_checkplayer(L, 1)->GetFOVDistanceAdjustFactor());
  return 1;
}

static int CBasePlayer_GetFOVTime (lua_State *L) {
  lua_pushnumber(L, luaL_checkplayer(L, 1)->GetFOVTime());
  return 1;
}

static int CBasePlayer_GetHealth (lua_State *L) {
  lua_pushinteger(L, luaL_checkplayer(L, 1)->GetHealth());
  return 1;
}

static int CBasePlayer_GetImpulse (lua_State *L) {
  lua_pushinteger(L, luaL_checkplayer(L, 1)->GetImpulse());
  return 1;
}

static int CBasePlayer_GetLaggedMovementValue (lua_State *L) {
  lua_pushnumber(L, luaL_checkplayer(L, 1)->GetLaggedMovementValue());
  return 1;
}

static int CBasePlayer_GetLastKnownPlaceName (lua_State *L) {
  lua_pushstring(L, luaL_checkplayer(L, 1)->GetLastKnownPlaceName());
  return 1;
}

static int CBasePlayer_GetNextAttack (lua_State *L) {
  lua_pushnumber(L, luaL_checkplayer(L, 1)->GetNextAttack());
  return 1;
}

static int CBasePlayer_GetObserverMode (lua_State *L) {
  lua_pushinteger(L, luaL_checkplayer(L, 1)->GetObserverMode());
  return 1;
}

static int CBasePlayer_GetObserverTarget (lua_State *L) {
  lua_pushentity(L, luaL_checkplayer(L, 1)->GetObserverTarget());
  return 1;
}

static int CBasePlayer_GetOffset_m_Local (lua_State *L) {
  lua_pushinteger(L, CBasePlayer::GetOffset_m_Local());
  return 1;
}

static int CBasePlayer_GetPlayerMaxs (lua_State *L) {
  Vector v = luaL_checkplayer(L, 1)->GetPlayerMaxs();
  lua_pushvector(L, &v);
  return 1;
}

static int CBasePlayer_GetPlayerMins (lua_State *L) {
  Vector v = luaL_checkplayer(L, 1)->GetPlayerMins();
  lua_pushvector(L, &v);
  return 1;
}

static int CBasePlayer_GetPreviouslyPredictedOrigin (lua_State *L) {
  Vector v = luaL_checkplayer(L, 1)->GetPreviouslyPredictedOrigin();
  lua_pushvector(L, &v);
  return 1;
}

static int CBasePlayer_GetStepSoundVelocities (lua_State *L) {
  float velwalk, velrun;
  luaL_checkplayer(L, 1)->GetStepSoundVelocities(&velwalk, &velrun);
  lua_pushnumber(L, velwalk);
  lua_pushnumber(L, velrun);
  return 2;
}

static int CBasePlayer_GetSwimSoundTime (lua_State *L) {
  lua_pushnumber(L, luaL_checkplayer(L, 1)->GetSwimSoundTime());
  return 1;
}

static int CBasePlayer_GetTimeBase (lua_State *L) {
  lua_pushnumber(L, luaL_checkplayer(L, 1)->GetTimeBase());
  return 1;
}

static int CBasePlayer_GetTracerType (lua_State *L) {
  lua_pushstring(L, luaL_checkplayer(L, 1)->GetTracerType());
  return 1;
}

static int CBasePlayer_GetUseEntity (lua_State *L) {
  lua_pushentity(L, luaL_checkplayer(L, 1)->GetUseEntity());
  return 1;
}

static int CBasePlayer_GetUserID (lua_State *L) {
  lua_pushinteger(L, luaL_checkplayer(L, 1)->GetUserID());
  return 1;
}

static int CBasePlayer_GetViewModel (lua_State *L) {
  lua_pushentity(L, luaL_checkplayer(L, 1)->GetViewModel(luaL_optint(L, 2, 0)));
  return 1;
}

static int CBasePlayer_GetWaterJumpTime (lua_State *L) {
  lua_pushnumber(L, luaL_checkplayer(L, 1)->GetWaterJumpTime());
  return 1;
}

static int CBasePlayer_GetWeapon (lua_State *L) {
  lua_pushweapon(L, luaL_checkplayer(L, 1)->GetWeapon(luaL_checkint(L, 2)));
  return 1;
}

static int CBasePlayer_HintMessage (lua_State *L) {
  luaL_checkplayer(L, 1)->HintMessage(luaL_checkstring(L, 2));
  return 0;
}

static int CBasePlayer_IncrementEFNoInterpParity (lua_State *L) {
  luaL_checkplayer(L, 1)->IncrementEFNoInterpParity();
  return 0;
}

static int CBasePlayer_IsBot (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->IsBot());
  return 1;
}

static int CBasePlayer_IsHLTV (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->IsHLTV());
  return 1;
}

static int CBasePlayer_IsInAVehicle (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->IsInAVehicle());
  return 1;
}

static int CBasePlayer_IsObserver (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->IsObserver());
  return 1;
}

static int CBasePlayer_IsPlayer (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->IsPlayer());
  return 1;
}

static int CBasePlayer_IsPlayerUnderwater (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->IsPlayerUnderwater());
  return 1;
}

static int CBasePlayer_IsSuitEquipped (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->IsSuitEquipped());
  return 1;
}

static int CBasePlayer_IsUseableEntity (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->IsUseableEntity(luaL_checkentity(L, 2), luaL_checkint(L, 3)));
  return 1;
}

static int CBasePlayer_ItemPostFrame (lua_State *L) {
  luaL_checkplayer(L, 1)->ItemPostFrame();
  return 0;
}

static int CBasePlayer_ItemPreFrame (lua_State *L) {
  luaL_checkplayer(L, 1)->ItemPreFrame();
  return 0;
}

static int CBasePlayer_LeaveVehicle (lua_State *L) {
  luaL_checkplayer(L, 1)->LeaveVehicle();
  return 0;
}

static int CBasePlayer_LocalEyeAngles (lua_State *L) {
  QAngle v = luaL_checkplayer(L, 1)->LocalEyeAngles();
  lua_pushangle(L, &v);
  return 1;
}

static int CBasePlayer_MaxSpeed (lua_State *L) {
  lua_pushnumber(L, luaL_checkplayer(L, 1)->MaxSpeed());
  return 1;
}

static int CBasePlayer_MyCombatCharacterPointer (lua_State *L) {
  lua_pushplayer(L, (CBasePlayer *)luaL_checkplayer(L, 1)->MyCombatCharacterPointer());
  return 1;
}

static int CBasePlayer_OnRestore (lua_State *L) {
  luaL_checkplayer(L, 1)->OnRestore();
  return 0;
}

static int CBasePlayer_PhysicsSimulate (lua_State *L) {
  luaL_checkplayer(L, 1)->PhysicsSimulate();
  return 0;
}

static int CBasePlayer_PhysicsSolidMaskForEntity (lua_State *L) {
  lua_pushinteger(L, luaL_checkplayer(L, 1)->PhysicsSolidMaskForEntity());
  return 1;
}

static int CBasePlayer_PlayerUse (lua_State *L) {
  luaL_checkplayer(L, 1)->PlayerUse();
  return 0;
}

static int CBasePlayer_PlayStepSound (lua_State *L) {
  luaL_checkplayer(L, 1)->PlayStepSound(*luaL_checkvector(L, 2), &lua_tosurfacedata(L, 3), luaL_checknumber(L, 4), luaL_checkboolean(L, 5));
  return 0;
}

static int CBasePlayer_PostThink (lua_State *L) {
  luaL_checkplayer(L, 1)->PostThink();
  return 0;
}

static int CBasePlayer_PreThink (lua_State *L) {
  luaL_checkplayer(L, 1)->PreThink();
  return 0;
}

static int CBasePlayer_RemoveAllAmmo (lua_State *L) {
  luaL_checkplayer(L, 1)->RemoveAllAmmo();
  return 0;
}

static int CBasePlayer_RemoveAmmo (lua_State *L) {
  luaL_checkplayer(L, 1)->RemoveAmmo(luaL_checkint(L, 2), luaL_checkstring(L, 3));
  return 0;
}

static int CBasePlayer_RemoveFromPlayerSimulationList (lua_State *L) {
  luaL_checkplayer(L, 1)->RemoveFromPlayerSimulationList(luaL_checkentity(L, 2));
  return 0;
}

static int CBasePlayer_ResetAutoaim (lua_State *L) {
  luaL_checkplayer(L, 1)->ResetAutoaim();
  return 0;
}

static int CBasePlayer_ResetObserverMode (lua_State *L) {
  luaL_checkplayer(L, 1)->ResetObserverMode();
  return 0;
}

static int CBasePlayer_SelectItem (lua_State *L) {
  luaL_checkplayer(L, 1)->SelectItem(luaL_checkstring(L, 2), luaL_optint(L, 3, 0));
  return 0;
}

static int CBasePlayer_SelectLastItem (lua_State *L) {
  luaL_checkplayer(L, 1)->SelectLastItem();
  return 0;
}

static int CBasePlayer_SetAmmoCount (lua_State *L) {
  luaL_checkplayer(L, 1)->SetAmmoCount(luaL_checkint(L, 2), luaL_checkint(L, 3));
  return 0;
}

static int CBasePlayer_SetAnimation (lua_State *L) {
  luaL_checkplayer(L, 1)->SetAnimation((PLAYER_ANIM)luaL_checkint(L, 2));
  return 0;
}

static int CBasePlayer_SetAnimationExtension (lua_State *L) {
  luaL_checkplayer(L, 1)->SetAnimationExtension(luaL_checkstring(L, 2));
  return 0;
}

static int CBasePlayer_SetBloodColor (lua_State *L) {
  luaL_checkplayer(L, 1)->SetBloodColor(luaL_checkint(L, 2));
  return 0;
}

static int CBasePlayer_SetFOV (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->SetFOV(luaL_checkentity(L, 2), luaL_checkint(L, 3), luaL_checknumber(L, 4), luaL_optint(L, 5, 0)));
  return 1;
}

static int CBasePlayer_SetLadderNormal (lua_State *L) {
  luaL_checkplayer(L, 1)->SetLadderNormal(*luaL_checkvector(L, 2));
  return 0;
}

static int CBasePlayer_SetMaxSpeed (lua_State *L) {
  luaL_checkplayer(L, 1)->SetMaxSpeed(luaL_checknumber(L, 2));
  return 0;
}

static int CBasePlayer_SetNextAttack (lua_State *L) {
  luaL_checkplayer(L, 1)->SetNextAttack(luaL_checknumber(L, 2));
  return 0;
}

static int CBasePlayer_SetPlayerUnderwater (lua_State *L) {
  luaL_checkplayer(L, 1)->SetPlayerUnderwater(luaL_checkboolean(L, 2));
  return 0;
}

static int CBasePlayer_SetPreviouslyPredictedOrigin (lua_State *L) {
  luaL_checkplayer(L, 1)->SetPreviouslyPredictedOrigin(*luaL_checkvector(L, 2));
  return 0;
}

static int CBasePlayer_SetSwimSoundTime (lua_State *L) {
  luaL_checkplayer(L, 1)->SetSwimSoundTime(luaL_checknumber(L, 2));
  return 0;
}

static int CBasePlayer_SetWaterJumpTime (lua_State *L) {
  luaL_checkplayer(L, 1)->SetWaterJumpTime(luaL_checknumber(L, 2));
  return 0;
}

static int CBasePlayer_SharedSpawn (lua_State *L) {
  luaL_checkplayer(L, 1)->SharedSpawn();
  return 0;
}

static int CBasePlayer_ShouldShowHints (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->ShouldShowHints());
  return 1;
}

static int CBasePlayer_SimulatePlayerSimulatedEntities (lua_State *L) {
  luaL_checkplayer(L, 1)->SimulatePlayerSimulatedEntities();
  return 0;
}

static int CBasePlayer_SmoothViewOnStairs (lua_State *L) {
  luaL_checkplayer(L, 1)->SmoothViewOnStairs(*luaL_checkvector(L, 2));
  return 0;
}

static int CBasePlayer_Spawn (lua_State *L) {
  luaL_checkplayer(L, 1)->Spawn();
  return 0;
}

static int CBasePlayer_SwitchToNextBestWeapon (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->SwitchToNextBestWeapon(luaL_checkweapon(L, 2)));
  return 1;
}

static int CBasePlayer_UpdateClientData (lua_State *L) {
  luaL_checkplayer(L, 1)->UpdateClientData();
  return 0;
}

static int CBasePlayer_UpdateUnderwaterState (lua_State *L) {
  luaL_checkplayer(L, 1)->UpdateUnderwaterState();
  return 0;
}

static int CBasePlayer_UsingStandardWeaponsInVehicle (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->UsingStandardWeaponsInVehicle());
  return 1;
}

static int CBasePlayer_ViewPunch (lua_State *L) {
  luaL_checkplayer(L, 1)->ViewPunch(*luaL_checkangle(L, 2));
  return 0;
}

static int CBasePlayer_ViewPunchReset (lua_State *L) {
  luaL_checkplayer(L, 1)->ViewPunchReset(luaL_optnumber(L, 2, 0));
  return 0;
}

static int CBasePlayer_Weapon_CanSwitchTo (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->Weapon_CanSwitchTo(luaL_checkweapon(L, 2)));
  return 1;
}

static int CBasePlayer_Weapon_OwnsThisType (lua_State *L) {
  lua_pushweapon(L, luaL_checkplayer(L, 1)->Weapon_OwnsThisType(luaL_checkstring(L, 2), luaL_optint(L, 3, 0)));
  return 1;
}

static int CBasePlayer_Weapon_SetLast (lua_State *L) {
  luaL_checkplayer(L, 1)->Weapon_SetLast(luaL_checkweapon(L, 2));
  return 0;
}

static int CBasePlayer_Weapon_ShootPosition (lua_State *L) {
  Vector v = luaL_checkplayer(L, 1)->Weapon_ShootPosition();
  lua_pushvector(L, &v);
  return 1;
}

static int CBasePlayer_Weapon_ShouldSelectItem (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->Weapon_ShouldSelectItem(luaL_checkweapon(L, 2)));
  return 1;
}

static int CBasePlayer_Weapon_ShouldSetLast (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->Weapon_ShouldSetLast(luaL_checkweapon(L, 2), luaL_checkweapon(L, 3)));
  return 1;
}

static int CBasePlayer_Weapon_Switch (lua_State *L) {
  lua_pushboolean(L, luaL_checkplayer(L, 1)->Weapon_Switch(luaL_checkweapon(L, 2), luaL_optint(L, 3, 0)));
  return 1;
}

static int CBasePlayer_WeaponCount (lua_State *L) {
  lua_pushinteger(L, luaL_checkplayer(L, 1)->WeaponCount());
  return 1;
}

static int CBasePlayer___index (lua_State *L) {
  CBasePlayer *pPlayer = lua_toplayer(L, 1);
  if (pPlayer == NULL) {  /* avoid extra test when d is not 0 */
    lua_Debug ar1;
    lua_getstack(L, 1, &ar1);
    lua_getinfo(L, "fl", &ar1);
    lua_Debug ar2;
    lua_getinfo(L, ">S", &ar2);
	lua_pushfstring(L, "%s:%d: attempt to index a NULL entity", ar2.short_src, ar1.currentline);
	return lua_error(L);
  }
  const char *field = luaL_checkstring(L, 2);
  if (Q_strcmp(field, "m_afButtonLast") == 0)
    lua_pushinteger(L, pPlayer->m_afButtonLast);
  else if (Q_strcmp(field, "m_afButtonPressed") == 0)
    lua_pushinteger(L, pPlayer->m_afButtonPressed);
  else if (Q_strcmp(field, "m_afButtonReleased") == 0)
    lua_pushinteger(L, pPlayer->m_afButtonReleased);
  else if (Q_strcmp(field, "m_flNextAttack") == 0)
    lua_pushnumber(L, pPlayer->m_flNextAttack);
  else if (Q_strcmp(field, "m_fOnTarget") == 0)
    lua_pushboolean(L, pPlayer->m_fOnTarget);
  else if (Q_strcmp(field, "m_nButtons") == 0)
    lua_pushinteger(L, pPlayer->m_nButtons);
  else if (Q_strcmp(field, "m_StuckLast") == 0)
    lua_pushinteger(L, pPlayer->m_StuckLast);
  else if (Q_strcmp(field, "m_szAnimExtension") == 0)
    lua_pushstring(L, pPlayer->m_szAnimExtension);
  else {
    lua_getmetatable(L, 1);
    lua_pushvalue(L, 2);
    lua_gettable(L, -2);
    if (lua_isnil(L, -1)) {
      lua_pop(L, 1);
      luaL_getmetatable(L, "CBaseAnimating");
      lua_pushvalue(L, 2);
      lua_gettable(L, -2);
      if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        luaL_getmetatable(L, "CBaseEntity");
        lua_pushvalue(L, 2);
        lua_gettable(L, -2);
      }
    }
  }
  return 1;
}

static int CBasePlayer___newindex (lua_State *L) {
  CBasePlayer *pPlayer = lua_toplayer(L, 1);
  if (pPlayer == NULL) {  /* avoid extra test when d is not 0 */
    lua_Debug ar1;
    lua_getstack(L, 1, &ar1);
    lua_getinfo(L, "fl", &ar1);
    lua_Debug ar2;
    lua_getinfo(L, ">S", &ar2);
	lua_pushfstring(L, "%s:%d: attempt to index a NULL entity", ar2.short_src, ar1.currentline);
	return lua_error(L);
  }
  const char *field = luaL_checkstring(L, 2);
  if (Q_strcmp(field, "m_afButtonLast") == 0)
    pPlayer->m_afButtonLast = luaL_checkint(L, 3);
  else if (Q_strcmp(field, "m_afButtonPressed") == 0)
    pPlayer->m_afButtonPressed = luaL_checkint(L, 3);
  else if (Q_strcmp(field, "m_afButtonReleased") == 0)
    pPlayer->m_afButtonReleased = luaL_checkint(L, 3);
  else if (Q_strcmp(field, "m_flNextAttack") == 0)
    pPlayer->m_flNextAttack = luaL_checknumber(L, 3);
#ifdef CLIENT_DLL
  else if (Q_strcmp(field, "m_fOnTarget") == 0)
    pPlayer->m_fOnTarget = luaL_checkboolean(L, 3);
#else
  else if (Q_strcmp(field, "m_fOnTarget") == 0)
    pPlayer->m_fOnTarget.GetForModify() = (bool)luaL_checkboolean(L, 3);
#endif
  else if (Q_strcmp(field, "m_nButtons") == 0)
    pPlayer->m_nButtons = luaL_checkint(L, 3);
  else if (Q_strcmp(field, "m_StuckLast") == 0)
    pPlayer->m_StuckLast = luaL_checkint(L, 3);
  else if (Q_strcmp(field, "m_szAnimExtension") == 0)
    Q_strcpy(pPlayer->m_szAnimExtension, luaL_checkstring(L, 3));
  return 0;
}

static int CBasePlayer___eq (lua_State *L) {
  lua_pushboolean(L, lua_toplayer(L, 1) == lua_toplayer(L, 2));
  return 1;
}

static int CBasePlayer___tostring (lua_State *L) {
  CBasePlayer *pPlayer = lua_toplayer(L, 1);
  if (pPlayer == NULL)
    lua_pushstring(L, "NULL");
  else
    lua_pushfstring(L, "CBasePlayer: %d \"%s\"", pPlayer->GetUserID(), pPlayer->GetPlayerName());
  return 1;
}


static const luaL_Reg CBasePlayermeta[] = {
  {"AbortReload", CBasePlayer_AbortReload},
  {"AddToPlayerSimulationList", CBasePlayer_AddToPlayerSimulationList},
  {"ClearZoomOwner", CBasePlayer_ClearZoomOwner},
  {"CurrentCommandNumber", CBasePlayer_CurrentCommandNumber},
  {"DoMuzzleFlash", CBasePlayer_DoMuzzleFlash},
  {"ExitLadder", CBasePlayer_ExitLadder},
  {"EyeAngles", CBasePlayer_EyeAngles},
  {"EyePosition", CBasePlayer_EyePosition},
  {"EyePositionAndVectors", CBasePlayer_EyePositionAndVectors},
  {"EyeVectors", CBasePlayer_EyeVectors},
  {"FindUseEntity", CBasePlayer_FindUseEntity},
  {"GetActiveWeapon", CBasePlayer_GetActiveWeapon},
  {"GetAutoaimVector", CBasePlayer_GetAutoaimVector},
  {"GetBonusChallenge", CBasePlayer_GetBonusChallenge},
  {"GetBonusProgress", CBasePlayer_GetBonusProgress},
  {"GetDeathTime", CBasePlayer_GetDeathTime},
  {"GetDefaultFOV", CBasePlayer_GetDefaultFOV},
  {"GetEFNoInterpParity", CBasePlayer_GetEFNoInterpParity},
  {"GetFOV", CBasePlayer_GetFOV},
  {"GetFOVDistanceAdjustFactor", CBasePlayer_GetFOVDistanceAdjustFactor},
  {"GetFOVTime", CBasePlayer_GetFOVTime},
  {"GetHealth", CBasePlayer_GetHealth},
  {"GetImpulse", CBasePlayer_GetImpulse},
  {"GetLaggedMovementValue", CBasePlayer_GetLaggedMovementValue},
  {"GetLastKnownPlaceName", CBasePlayer_GetLastKnownPlaceName},
  {"GetNextAttack", CBasePlayer_GetNextAttack},
  {"GetObserverMode", CBasePlayer_GetObserverMode},
  {"GetObserverTarget", CBasePlayer_GetObserverTarget},
  {"GetOffset_m_Local", CBasePlayer_GetOffset_m_Local},
  {"GetPlayerMaxs", CBasePlayer_GetPlayerMaxs},
  {"GetPlayerMins", CBasePlayer_GetPlayerMins},
  {"GetPreviouslyPredictedOrigin", CBasePlayer_GetPreviouslyPredictedOrigin},
  {"GetStepSoundVelocities", CBasePlayer_GetStepSoundVelocities},
  {"GetSwimSoundTime", CBasePlayer_GetSwimSoundTime},
  {"GetTimeBase", CBasePlayer_GetTimeBase},
  {"GetTracerType", CBasePlayer_GetTracerType},
  {"GetUseEntity", CBasePlayer_GetUseEntity},
  {"GetUserID", CBasePlayer_GetUserID},
  {"GetViewModel", CBasePlayer_GetViewModel},
  {"GetWaterJumpTime", CBasePlayer_GetWaterJumpTime},
  {"GetWeapon", CBasePlayer_GetWeapon},
  {"HintMessage", CBasePlayer_HintMessage},
  {"IncrementEFNoInterpParity", CBasePlayer_IncrementEFNoInterpParity},
  {"IsBot", CBasePlayer_IsBot},
  {"IsHLTV", CBasePlayer_IsHLTV},
  {"IsInAVehicle", CBasePlayer_IsInAVehicle},
  {"IsObserver", CBasePlayer_IsObserver},
  {"IsPlayer", CBasePlayer_IsPlayer},
  {"IsPlayerUnderwater", CBasePlayer_IsPlayerUnderwater},
  {"IsSuitEquipped", CBasePlayer_IsSuitEquipped},
  {"IsUseableEntity", CBasePlayer_IsUseableEntity},
  {"ItemPostFrame", CBasePlayer_ItemPostFrame},
  {"ItemPreFrame", CBasePlayer_ItemPreFrame},
  {"LeaveVehicle", CBasePlayer_LeaveVehicle},
  {"LocalEyeAngles", CBasePlayer_LocalEyeAngles},
  {"MaxSpeed", CBasePlayer_MaxSpeed},
  {"MyCombatCharacterPointer", CBasePlayer_MyCombatCharacterPointer},
  {"OnRestore", CBasePlayer_OnRestore},
  {"PhysicsSimulate", CBasePlayer_PhysicsSimulate},
  {"PhysicsSolidMaskForEntity", CBasePlayer_PhysicsSolidMaskForEntity},
  {"PlayerUse", CBasePlayer_PlayerUse},
  {"PlayStepSound", CBasePlayer_PlayStepSound},
  {"PostThink", CBasePlayer_PostThink},
  {"PreThink", CBasePlayer_PreThink},
  {"RemoveAllAmmo", CBasePlayer_RemoveAllAmmo},
  {"RemoveAmmo", CBasePlayer_RemoveAmmo},
  {"RemoveFromPlayerSimulationList", CBasePlayer_RemoveFromPlayerSimulationList},
  {"ResetAutoaim", CBasePlayer_ResetAutoaim},
  {"ResetObserverMode", CBasePlayer_ResetObserverMode},
  {"SelectItem", CBasePlayer_SelectItem},
  {"SelectLastItem", CBasePlayer_SelectLastItem},
  {"SetAmmoCount", CBasePlayer_SetAmmoCount},
  {"SetAnimation", CBasePlayer_SetAnimation},
  {"SetAnimationExtension", CBasePlayer_SetAnimationExtension},
  {"SetBloodColor", CBasePlayer_SetBloodColor},
  {"SetFOV", CBasePlayer_SetFOV},
  {"SetLadderNormal", CBasePlayer_SetLadderNormal},
  {"SetMaxSpeed", CBasePlayer_SetMaxSpeed},
  {"SetNextAttack", CBasePlayer_SetNextAttack},
  {"SetPlayerUnderwater", CBasePlayer_SetPlayerUnderwater},
  {"SetPreviouslyPredictedOrigin", CBasePlayer_SetPreviouslyPredictedOrigin},
  {"SetSwimSoundTime", CBasePlayer_SetSwimSoundTime},
  {"SetWaterJumpTime", CBasePlayer_SetWaterJumpTime},
  {"SharedSpawn", CBasePlayer_SharedSpawn},
  {"ShouldShowHints", CBasePlayer_ShouldShowHints},
  {"SimulatePlayerSimulatedEntities", CBasePlayer_SimulatePlayerSimulatedEntities},
  {"SmoothViewOnStairs", CBasePlayer_SmoothViewOnStairs},
  {"Spawn", CBasePlayer_Spawn},
  {"SwitchToNextBestWeapon", CBasePlayer_SwitchToNextBestWeapon},
  {"UpdateClientData", CBasePlayer_UpdateClientData},
  {"UpdateUnderwaterState", CBasePlayer_UpdateUnderwaterState},
  {"UsingStandardWeaponsInVehicle", CBasePlayer_UsingStandardWeaponsInVehicle},
  {"ViewPunch", CBasePlayer_ViewPunch},
  {"ViewPunchReset", CBasePlayer_ViewPunchReset},
  {"Weapon_CanSwitchTo", CBasePlayer_Weapon_CanSwitchTo},
  {"Weapon_OwnsThisType", CBasePlayer_Weapon_OwnsThisType},
  {"Weapon_SetLast", CBasePlayer_Weapon_SetLast},
  {"Weapon_ShootPosition", CBasePlayer_Weapon_ShootPosition},
  {"Weapon_ShouldSelectItem", CBasePlayer_Weapon_ShouldSelectItem},
  {"Weapon_ShouldSetLast", CBasePlayer_Weapon_ShouldSetLast},
  {"Weapon_Switch", CBasePlayer_Weapon_Switch},
  {"WeaponCount", CBasePlayer_WeaponCount},
  {"__index", CBasePlayer___index},
  {"__newindex", CBasePlayer___newindex},
  {"__eq", CBasePlayer___eq},
  {"__tostring", CBasePlayer___tostring},
  {NULL, NULL}
};


/*
** Open CBasePlayer object
*/
int luaopen_CBasePlayer_shared (lua_State *L) {
  luaL_getmetatable(L, "CBasePlayer");
  if (lua_isnoneornil(L, -1)) {
    lua_pop(L, 1);
    luaL_newmetatable(L, "CBasePlayer");
  }
  luaL_register(L, NULL, CBasePlayermeta);
  lua_pushstring(L, "entity");
  lua_setfield(L, -2, "__type");  /* metatable.__type = "entity" */
  lua_pop(L, 1);
  return 1;
}

