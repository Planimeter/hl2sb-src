//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "hl2mp_gamerules.h"
#include "viewport_panel_names.h"
#include "gameeventdefs.h"
#include <KeyValues.h>
#include "ammodef.h"

#ifdef LUA_SDK
#include "takedamageinfo.h"
#include "luamanager.h"
#include "lbasecombatweapon_shared.h"
#include "lbaseentity_shared.h"
#include "lbaseplayer_shared.h"
#include "ltakedamageinfo.h"
#include "mathlib/lvector.h"
#ifndef CLIENT_DLL
#include "ai_basenpc.h"
#endif
#endif

#ifdef CLIENT_DLL
#include "c_hl2mp_player.h"
#else

#include "eventqueue.h"
#include "player.h"
#include "gamerules.h"
#include "game.h"
#include "items.h"
#include "entitylist.h"
#include "mapentities.h"
#include "in_buttons.h"
#include <ctype.h>
#include "voice_gamemgr.h"
#include "iscorer.h"
#include "hl2mp_player.h"
#include "weapon_hl2mpbasehlmpcombatweapon.h"
#include "team.h"
#include "voice_gamemgr.h"
#include "hl2mp_gameinterface.h"
#include "hl2mp_cvars.h"

#if defined(DEBUG) || defined(LUA_SDK)
#include "hl2mp_bot_temp.h"
#endif

extern void respawn(CBaseEntity *pEdict, bool fCopyCorpse);

extern bool FindInList(const char **pStrings, const char *pToFind);

ConVar sv_hl2mp_weapon_respawn_time("sv_hl2mp_weapon_respawn_time", "20",
                                    FCVAR_GAMEDLL | FCVAR_NOTIFY);
ConVar sv_hl2mp_item_respawn_time("sv_hl2mp_item_respawn_time", "30",
                                  FCVAR_GAMEDLL | FCVAR_NOTIFY);
ConVar sv_report_client_settings("sv_report_client_settings", "0",
                                 FCVAR_GAMEDLL | FCVAR_NOTIFY);

extern ConVar mp_chattime;

extern CBaseEntity *g_pLastCombineSpawn;
extern CBaseEntity *g_pLastRebelSpawn;

#define WEAPON_MAX_DISTANCE_FROM_SPAWN 64

#endif

REGISTER_GAMERULES_CLASS(CHL2MPRules);

BEGIN_NETWORK_TABLE_NOBASE(CHL2MPRules, DT_HL2MPRules)

#ifdef CLIENT_DLL
RecvPropBool(RECVINFO(m_bTeamPlayEnabled)),
#else
SendPropBool(SENDINFO(m_bTeamPlayEnabled)),
#endif

    END_NETWORK_TABLE()

        LINK_ENTITY_TO_CLASS(hl2mp_gamerules, CHL2MPGameRulesProxy);
IMPLEMENT_NETWORKCLASS_ALIASED(HL2MPGameRulesProxy, DT_HL2MPGameRulesProxy)

static HL2MPViewVectors g_HL2MPViewVectors(
    Vector(0, 0, 64),  // VEC_VIEW (m_vView)

    Vector(-16, -16, 0),  // VEC_HULL_MIN (m_vHullMin)
    Vector(16, 16, 72),   // VEC_HULL_MAX (m_vHullMax)

    Vector(-16, -16, 0),  // VEC_DUCK_HULL_MIN (m_vDuckHullMin)
    Vector(16, 16, 36),   // VEC_DUCK_HULL_MAX	(m_vDuckHullMax)
    Vector(0, 0, 28),     // VEC_DUCK_VIEW		(m_vDuckView)

    Vector(-10, -10, -10),  // VEC_OBS_HULL_MIN	(m_vObsHullMin)
    Vector(10, 10, 10),     // VEC_OBS_HULL_MAX	(m_vObsHullMax)

    Vector(0, 0, 14),  // VEC_DEAD_VIEWHEIGHT (m_vDeadViewHeight)

    Vector(-16, -16, 0),  // VEC_CROUCH_TRACE_MIN (m_vCrouchTraceMin)
    Vector(16, 16, 60)    // VEC_CROUCH_TRACE_MAX (m_vCrouchTraceMax)
);

static const char *s_PreserveEnts[] = {
    "ai_network",
    "ai_hint",
    "hl2mp_gamerules",
    "team_manager",
    "player_manager",
    "env_soundscape",
    "env_soundscape_proxy",
    "env_soundscape_triggerable",
    "env_sun",
    "env_wind",
    "env_fog_controller",
    "func_brush",
    "func_wall",
    "func_buyzone",
    "func_illusionary",
    "infodecal",
    "info_projecteddecal",
    "info_node",
    "info_target",
    "info_node_hint",
    "info_player_deathmatch",
    "info_player_combine",
    "info_player_rebel",
    "info_map_parameters",
    "keyframe_rope",
    "move_rope",
    "info_ladder",
    "player",
    "point_viewcontrol",
    "scene_manager",
    "shadow_control",
    "sky_camera",
    "soundent",
    "trigger_soundscape",
    "viewmodel",
    "predicted_viewmodel",
    "worldspawn",
    "point_devshot_camera",
    "",  // END Marker
};

#ifdef CLIENT_DLL
void RecvProxy_HL2MPRules(const RecvProp *pProp, void **pOut, void *pData,
                          int objectID) {
    CHL2MPRules *pRules = HL2MPRules();
    Assert(pRules);
    *pOut = pRules;
}

BEGIN_RECV_TABLE(CHL2MPGameRulesProxy, DT_HL2MPGameRulesProxy)
RecvPropDataTable("hl2mp_gamerules_data", 0, 0,
                  &REFERENCE_RECV_TABLE(DT_HL2MPRules), RecvProxy_HL2MPRules)
    END_RECV_TABLE()
#else
void *SendProxy_HL2MPRules(const SendProp *pProp, const void *pStructBase,
                           const void *pData, CSendProxyRecipients *pRecipients,
                           int objectID) {
    CHL2MPRules *pRules = HL2MPRules();
    Assert(pRules);
    return pRules;
}

BEGIN_SEND_TABLE(CHL2MPGameRulesProxy, DT_HL2MPGameRulesProxy)
SendPropDataTable("hl2mp_gamerules_data", 0,
                  &REFERENCE_SEND_TABLE(DT_HL2MPRules), SendProxy_HL2MPRules)
    END_SEND_TABLE()
#endif

#ifdef HL2SB
extern ConVar sk_plr_health_drop_time;
extern ConVar sk_plr_grenade_drop_time;
#endif

#ifndef CLIENT_DLL

class CVoiceGameMgrHelper : public IVoiceGameMgrHelper {
public:
    virtual bool CanPlayerHearPlayer(CBasePlayer *pListener, CBasePlayer *pTalker,
                                  bool &bProximity) {
#if defined(LUA_SDK)
        BEGIN_LUA_CALL_HOOK("CanPlayerHearPlayer");
        lua_pushplayer(L, pListener);
        lua_pushplayer(L, pTalker);
        lua_pushboolean(L, bProximity);
        END_LUA_CALL_HOOK(3, 2);

        if (lua_isboolean(L, -2))
            bProximity = (bool)lua_toboolean(L, -2);
        lua_remove(L, -2);

        RETURN_LUA_BOOLEAN();
#endif

        return (pListener->GetTeamNumber() == pTalker->GetTeamNumber());
    }
};
CVoiceGameMgrHelper g_VoiceGameMgrHelper;
IVoiceGameMgrHelper *g_pVoiceGameMgrHelper = &g_VoiceGameMgrHelper;

#endif

// NOTE: the indices here must match TEAM_TERRORIST, TEAM_CT, TEAM_SPECTATOR,
// etc.
char *sTeamNames[] = {
    "Unassigned",
    "Spectator",
    "Combine",
    "Rebels",
};

CHL2MPRules::CHL2MPRules() {
#ifndef CLIENT_DLL
    // Create the team managers
    for (int i = 0; i < ARRAYSIZE(sTeamNames); i++) {
        CTeam *pTeam = static_cast<CTeam *>(CreateEntityByName("team_manager"));
        pTeam->Init(sTeamNames[i], i);

        g_Teams.AddToTail(pTeam);
    }

    m_bTeamPlayEnabled = teamplay.GetBool();
    m_flIntermissionEndTime = 0.0f;
    m_flGameStartTime = 0;

    m_hRespawnableItemsAndWeapons.RemoveAll();
    m_tmNextPeriodicThink = 0;
    m_flRestartGameTime = 0;
    m_bCompleteReset = false;
    m_bHeardAllPlayersReady = false;
    m_bAwaitingReadyRestart = false;
    m_bChangelevelDone = false;

#endif
}

const CViewVectors *CHL2MPRules::GetViewVectors() const {
    return &g_HL2MPViewVectors;
}

const HL2MPViewVectors *CHL2MPRules::GetHL2MPViewVectors() const {
    return &g_HL2MPViewVectors;
}

CHL2MPRules::~CHL2MPRules(void) {
#ifndef CLIENT_DLL
    // Note, don't delete each team since they are in the gEntList and will
    // automatically be deleted from there, instead.
    g_Teams.Purge();
#endif
}

void CHL2MPRules::CreateStandardEntities(void) {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("CreateStandardEntities");
    END_LUA_CALL_HOOK(0, 0);
#endif

#ifndef CLIENT_DLL
    // Create the entity that will send our data to the client.

    BaseClass::CreateStandardEntities();

    g_pLastCombineSpawn = NULL;
    g_pLastRebelSpawn = NULL;

#ifdef DBGFLAG_ASSERT
    CBaseEntity *pEnt =
#endif
        CBaseEntity::Create("hl2mp_gamerules", vec3_origin, vec3_angle);
    Assert(pEnt);
#endif
}

//=========================================================
// FlWeaponRespawnTime - what is the time in the future
// at which this weapon may spawn?
//=========================================================
float CHL2MPRules::FlWeaponRespawnTime(CBaseCombatWeapon *pWeapon) {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("FlWeaponRespawnTime");
    lua_pushweapon(L, pWeapon);
    END_LUA_CALL_HOOK(1, 1);

    RETURN_LUA_NUMBER();
#endif

#ifndef CLIENT_DLL
    if (weaponstay.GetInt() > 0) {
        // make sure it's only certain weapons
        if (!(pWeapon->GetWeaponFlags() & ITEM_FLAG_LIMITINWORLD)) {
            return 0;  // weapon respawns almost instantly
        }
    }

    return sv_hl2mp_weapon_respawn_time.GetFloat();
#endif

    return 0;  // weapon respawns almost instantly
}

bool CHL2MPRules::IsIntermission(void) {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("IsIntermission");
    END_LUA_CALL_HOOK(0, 1);

    RETURN_LUA_BOOLEAN();
#endif

#ifndef CLIENT_DLL
    return m_flIntermissionEndTime > gpGlobals->curtime;
#endif

    return false;
}

void CHL2MPRules::PlayerKilled(CBasePlayer *pVictim,
                               const CTakeDamageInfo &info) {
#if defined(LUA_SDK)
    CTakeDamageInfo linfo = info;

    BEGIN_LUA_CALL_HOOK("PlayerKilled");
    lua_pushplayer(L, pVictim);
    lua_pushdamageinfo(L, linfo);
    END_LUA_CALL_HOOK(2, 0);
#endif

#ifndef CLIENT_DLL
    if (IsIntermission())
        return;
    BaseClass::PlayerKilled(pVictim, info);
#endif
}

#ifndef CLIENT_DLL
#if defined(LUA_SDK)
bool CHL2MPRules::FPlayerCanTakeDamage(CBasePlayer *pPlayer,
                                       CBaseEntity *pAttacker) {
    CTakeDamageInfo info;
    BEGIN_LUA_CALL_HOOK("FPlayerCanTakeDamage");
    lua_pushplayer(L, pPlayer);
    lua_pushentity(L, pAttacker);
    lua_pushdamageinfo(L, info);
    END_LUA_CALL_HOOK(2, 1);

    RETURN_LUA_BOOLEAN();

    return BaseClass::FPlayerCanTakeDamage(pPlayer, pAttacker, info);
}

bool CHL2MPRules::AllowDamage(CBaseEntity *pVictim,
                              const CTakeDamageInfo &info) {
    CTakeDamageInfo lInfo = info;

    BEGIN_LUA_CALL_HOOK("AllowDamage");
    lua_pushentity(L, pVictim);
    lua_pushdamageinfo(L, lInfo);
    END_LUA_CALL_HOOK(2, 1);

    RETURN_LUA_BOOLEAN();

    return BaseClass::AllowDamage(pVictim, lInfo);
}

#ifdef HL2SB
//-----------------------------------------------------------------------------
// Purpose: Whether or not the NPC should drop a health vial
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CHL2MPRules::NPC_ShouldDropHealth(CBasePlayer *pRecipient) {
    // Can only do this every so often
    if (m_flLastHealthDropTime > gpGlobals->curtime)
        return false;

    // Try to throw dynamic health
    float healthPerc =
        ((float)pRecipient->m_iHealth / (float)pRecipient->m_iMaxHealth);

    if (random->RandomFloat(0.0f, 1.0f) > healthPerc * 1.5f)
        return true;

    return false;
}

//-----------------------------------------------------------------------------
// Purpose: Whether or not the NPC should drop a health vial
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CHL2MPRules::NPC_ShouldDropGrenade(CBasePlayer *pRecipient) {
    // Can only do this every so often
    if (m_flLastGrenadeDropTime > gpGlobals->curtime)
        return false;

    int grenadeIndex = GetAmmoDef()->Index("grenade");
    int numGrenades = pRecipient->GetAmmoCount(grenadeIndex);

    // If we're not maxed out on grenades and we've randomly okay'd it
    if ((numGrenades < GetAmmoDef()->MaxCarry(grenadeIndex)) &&
        (random->RandomInt(0, 2) == 0))
        return true;

    return false;
}

//-----------------------------------------------------------------------------
// Purpose: Update the drop counter for health
//-----------------------------------------------------------------------------
void CHL2MPRules::NPC_DroppedHealth(void) {
    m_flLastHealthDropTime =
        gpGlobals->curtime + sk_plr_health_drop_time.GetFloat();
}

//-----------------------------------------------------------------------------
// Purpose: Update the drop counter for grenades
//-----------------------------------------------------------------------------
void CHL2MPRules::NPC_DroppedGrenade(void) {
    m_flLastGrenadeDropTime =
        gpGlobals->curtime + sk_plr_grenade_drop_time.GetFloat();
}
#endif

void CHL2MPRules::PlayerThink(CBasePlayer *pPlayer) {
    BEGIN_LUA_CALL_HOOK("PlayerThink");
    lua_pushplayer(L, pPlayer);
    END_LUA_CALL_HOOK(1, 0);

    BaseClass::PlayerThink(pPlayer);
}

void CHL2MPRules::PlayerSpawn(CBasePlayer *pPlayer) {
    BEGIN_LUA_CALL_HOOK("PlayerSpawn");
    lua_pushplayer(L, pPlayer);
    END_LUA_CALL_HOOK(1, 1);

    RETURN_LUA_NONE();

    BaseClass::PlayerSpawn(pPlayer);
}

bool CHL2MPRules::FPlayerCanRespawn(CBasePlayer *pPlayer) {
    BEGIN_LUA_CALL_HOOK("FPlayerCanRespawn");
    lua_pushplayer(L, pPlayer);
    END_LUA_CALL_HOOK(1, 1);

    RETURN_LUA_BOOLEAN();

    return BaseClass::FPlayerCanRespawn(pPlayer);
}

float CHL2MPRules::FlPlayerSpawnTime(CBasePlayer *pPlayer) {
    BEGIN_LUA_CALL_HOOK("FlPlayerSpawnTime");
    lua_pushplayer(L, pPlayer);
    END_LUA_CALL_HOOK(1, 1);

    RETURN_LUA_NUMBER();

    return BaseClass::FlPlayerSpawnTime(pPlayer);
}
#endif
#endif

void CHL2MPRules::Think(void) {
#ifdef LUA_SDK
    BEGIN_LUA_CALL_HOOK("Think");
    END_LUA_CALL_HOOK(0, 0);
#endif

#ifndef CLIENT_DLL

    CGameRules::Think();

    if (g_fGameOver)  // someone else quit the game already
    {
        // check to see if we should change levels now
        if (m_flIntermissionEndTime < gpGlobals->curtime) {
            if (!m_bChangelevelDone) {
                ChangeLevel();  // intermission is over
                m_bChangelevelDone = true;
            }
        }

        return;
    }

    //	float flTimeLimit = mp_timelimit.GetFloat() * 60;
    float flFragLimit = fraglimit.GetFloat();

    if (GetMapRemainingTime() < 0) {
        GoToIntermission();
        return;
    }

    if (flFragLimit) {
        if (IsTeamplay() == true) {
            CTeam *pCombine = g_Teams[TEAM_COMBINE];
            CTeam *pRebels = g_Teams[TEAM_REBELS];

            if (pCombine->GetScore() >= flFragLimit ||
                pRebels->GetScore() >= flFragLimit) {
                GoToIntermission();
                return;
            }
        } else {
            // check if any player is over the frag limit
            for (int i = 1; i <= gpGlobals->maxClients; i++) {
                CBasePlayer *pPlayer = UTIL_PlayerByIndex(i);

                if (pPlayer && pPlayer->FragCount() >= flFragLimit) {
                    GoToIntermission();
                    return;
                }
            }
        }
    }

    if (gpGlobals->curtime > m_tmNextPeriodicThink) {
        CheckAllPlayersReady();
        CheckRestartGame();
        m_tmNextPeriodicThink = gpGlobals->curtime + 1.0;
    }

    if (m_flRestartGameTime > 0.0f &&
        m_flRestartGameTime <= gpGlobals->curtime) {
        RestartGame();
    }

    if (m_bAwaitingReadyRestart && m_bHeardAllPlayersReady) {
        UTIL_ClientPrintAll(
            HUD_PRINTCENTER,
            "All players ready. Game will restart in 5 seconds");
        UTIL_ClientPrintAll(
            HUD_PRINTCONSOLE,
            "All players ready. Game will restart in 5 seconds");

        m_flRestartGameTime = gpGlobals->curtime + 5;
        m_bAwaitingReadyRestart = false;
    }

    ManageObjectRelocation();

#endif
}

void CHL2MPRules::GoToIntermission(void) {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("GoToIntermission");
    END_LUA_CALL_HOOK(0, 0);
#endif

#ifndef CLIENT_DLL
    if (g_fGameOver)
        return;

    g_fGameOver = true;

    m_flIntermissionEndTime = gpGlobals->curtime + mp_chattime.GetInt();

    for (int i = 0; i < MAX_PLAYERS; i++) {
        CBasePlayer *pPlayer = UTIL_PlayerByIndex(i);

        if (!pPlayer)
            continue;

        pPlayer->ShowViewPortPanel(PANEL_SCOREBOARD);
        pPlayer->AddFlag(FL_FROZEN);
    }
#endif
}

bool CHL2MPRules::CheckGameOver() {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("CheckGameOver");
    END_LUA_CALL_HOOK(0, 1);

    RETURN_LUA_BOOLEAN();
#endif

#ifndef CLIENT_DLL
    if (g_fGameOver)  // someone else quit the game already
    {
        // check to see if we should change levels now
        if (m_flIntermissionEndTime < gpGlobals->curtime) {
            ChangeLevel();  // intermission is over
        }

        return true;
    }
#endif

    return false;
}

// when we are within this close to running out of entities,  items
// marked with the ITEM_FLAG_LIMITINWORLD will delay their respawn
#define ENTITY_INTOLERANCE 100

//=========================================================
// FlWeaponRespawnTime - Returns 0 if the weapon can respawn
// now,  otherwise it returns the time at which it can try
// to spawn again.
//=========================================================
float CHL2MPRules::FlWeaponTryRespawn(CBaseCombatWeapon *pWeapon) {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("FlWeaponTryRespawn");
    lua_pushweapon(L, pWeapon);
    END_LUA_CALL_HOOK(1, 1);

    RETURN_LUA_NUMBER();
#endif

#ifndef CLIENT_DLL
    if (pWeapon && (pWeapon->GetWeaponFlags() & ITEM_FLAG_LIMITINWORLD)) {
        if (gEntList.NumberOfEntities() <
            (gpGlobals->maxEntities - ENTITY_INTOLERANCE))
            return 0;

        // we're past the entity tolerance level,  so delay the respawn
        return FlWeaponRespawnTime(pWeapon);
    }
#endif
    return 0;
}

//=========================================================
// VecWeaponRespawnSpot - where should this weapon spawn?
// Some game variations may choose to randomize spawn locations
//=========================================================
Vector CHL2MPRules::VecWeaponRespawnSpot(CBaseCombatWeapon *pWeapon) {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("VecWeaponRespawnSpot");
    lua_pushweapon(L, pWeapon);
    END_LUA_CALL_HOOK(1, 1);

    RETURN_LUA_VECTOR();
#endif

#ifndef CLIENT_DLL
    CWeaponHL2MPBase *pHL2Weapon = dynamic_cast<CWeaponHL2MPBase *>(pWeapon);

    if (pHL2Weapon) {
        return pHL2Weapon->GetOriginalSpawnOrigin();
    }
#endif

    return pWeapon->GetAbsOrigin();
}

#ifndef CLIENT_DLL

CItem *IsManagedObjectAnItem(CBaseEntity *pObject) {
    return dynamic_cast<CItem *>(pObject);
}

CWeaponHL2MPBase *IsManagedObjectAWeapon(CBaseEntity *pObject) {
    return dynamic_cast<CWeaponHL2MPBase *>(pObject);
}

bool GetObjectsOriginalParameters(CBaseEntity *pObject, Vector &vOriginalOrigin,
                                  QAngle &vOriginalAngles) {
    if (CItem *pItem = IsManagedObjectAnItem(pObject)) {
        if (pItem->m_flNextResetCheckTime > gpGlobals->curtime)
            return false;

        vOriginalOrigin = pItem->GetOriginalSpawnOrigin();
        vOriginalAngles = pItem->GetOriginalSpawnAngles();

        pItem->m_flNextResetCheckTime =
            gpGlobals->curtime + sv_hl2mp_item_respawn_time.GetFloat();
        return true;
    } else if (CWeaponHL2MPBase *pWeapon = IsManagedObjectAWeapon(pObject)) {
        if (pWeapon->m_flNextResetCheckTime > gpGlobals->curtime)
            return false;

        vOriginalOrigin = pWeapon->GetOriginalSpawnOrigin();
        vOriginalAngles = pWeapon->GetOriginalSpawnAngles();

        pWeapon->m_flNextResetCheckTime =
            gpGlobals->curtime + sv_hl2mp_weapon_respawn_time.GetFloat();
        return true;
    }

    return false;
}

void CHL2MPRules::ManageObjectRelocation(void) {
    int iTotal = m_hRespawnableItemsAndWeapons.Count();

    if (iTotal > 0) {
        for (int i = 0; i < iTotal; i++) {
            CBaseEntity *pObject = m_hRespawnableItemsAndWeapons[i].Get();

            if (pObject) {
                Vector vSpawOrigin;
                QAngle vSpawnAngles;

                if (GetObjectsOriginalParameters(pObject, vSpawOrigin,
                                                 vSpawnAngles) == true) {
                    float flDistanceFromSpawn =
                        (pObject->GetAbsOrigin() - vSpawOrigin).Length();

                    if (flDistanceFromSpawn > WEAPON_MAX_DISTANCE_FROM_SPAWN) {
                        bool shouldReset = false;
                        IPhysicsObject *pPhysics = pObject->VPhysicsGetObject();

                        if (pPhysics) {
                            shouldReset = pPhysics->IsAsleep();
                        } else {
                            shouldReset = (pObject->GetFlags() & FL_ONGROUND)
                                              ? true
                                              : false;
                        }

                        if (shouldReset) {
                            pObject->Teleport(&vSpawOrigin, &vSpawnAngles,
                                              NULL);
                            pObject->EmitSound("AlyxEmp.Charge");

                            IPhysicsObject *pPhys =
                                pObject->VPhysicsGetObject();

                            if (pPhys) {
                                pPhys->Wake();
                            }
                        }
                    }
                }
            }
        }
    }
}

//=========================================================
// AddLevelDesignerPlacedWeapon
//=========================================================
void CHL2MPRules::AddLevelDesignerPlacedObject(CBaseEntity *pEntity) {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("AddLevelDesignerPlacedObject");
    lua_pushentity(L, pEntity);
    END_LUA_CALL_HOOK(1, 1);

    RETURN_LUA_NONE();
#endif

    if (m_hRespawnableItemsAndWeapons.Find(pEntity) == -1) {
        m_hRespawnableItemsAndWeapons.AddToTail(pEntity);
    }
}

//=========================================================
// RemoveLevelDesignerPlacedWeapon
//=========================================================
void CHL2MPRules::RemoveLevelDesignerPlacedObject(CBaseEntity *pEntity) {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("RemoveLevelDesignerPlacedObject");
    lua_pushentity(L, pEntity);
    END_LUA_CALL_HOOK(1, 1);

    RETURN_LUA_NONE();
#endif

    if (m_hRespawnableItemsAndWeapons.Find(pEntity) != -1) {
        m_hRespawnableItemsAndWeapons.FindAndRemove(pEntity);
    }
}

//=========================================================
// Where should this item respawn?
// Some game variations may choose to randomize spawn locations
//=========================================================
Vector CHL2MPRules::VecItemRespawnSpot(CItem *pItem) {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("VecItemRespawnSpot");
    lua_pushentity(L, pItem);
    END_LUA_CALL_HOOK(1, 1);

    RETURN_LUA_VECTOR();
#endif

    return pItem->GetOriginalSpawnOrigin();
}

//=========================================================
// What angles should this item use to respawn?
//=========================================================
QAngle CHL2MPRules::VecItemRespawnAngles(CItem *pItem) {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("VecItemRespawnAngles");
    lua_pushentity(L, pItem);
    END_LUA_CALL_HOOK(1, 1);

    RETURN_LUA_ANGLE();
#endif

    return pItem->GetOriginalSpawnAngles();
}

//=========================================================
// At what time in the future may this Item respawn?
//=========================================================
float CHL2MPRules::FlItemRespawnTime(CItem *pItem) {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("FlItemRespawnTime");
    lua_pushentity(L, pItem);
    END_LUA_CALL_HOOK(1, 1);

    RETURN_LUA_NUMBER();
#endif

    return sv_hl2mp_item_respawn_time.GetFloat();
}

#if defined(LUA_SDK)
//=========================================================
//=========================================================
void CHL2MPRules::PlayerGotItem(CBasePlayer *pPlayer, CItem *pItem) {
    BEGIN_LUA_CALL_HOOK("PlayerGotItem");
    lua_pushentity(L, pPlayer);
    lua_pushentity(L, pItem);
    END_LUA_CALL_HOOK(2, 0);
}

//=========================================================
//=========================================================
int CHL2MPRules::ItemShouldRespawn(CItem *pItem) {
    BEGIN_LUA_CALL_HOOK("ItemShouldRespawn");
    lua_pushentity(L, pItem);
    END_LUA_CALL_HOOK(1, 1);

    RETURN_LUA_INTEGER();

    return BaseClass::ItemShouldRespawn(pItem);
}
#endif

//=========================================================
// CanHaveWeapon - returns false if the player is not allowed
// to pick up this weapon
//=========================================================
bool CHL2MPRules::CanHavePlayerItem(CBasePlayer *pPlayer,
                                    CBaseCombatWeapon *pItem) {
#if !defined(LUA_SDK)
    if (weaponstay.GetInt() > 0) {
        if (pPlayer->Weapon_OwnsThisType(pItem->GetClassname(),
                                         pItem->GetSubType()))
            return false;
    }
#else
    BEGIN_LUA_CALL_HOOK("CanHavePlayerItem");
    lua_pushplayer(L, pPlayer);
    lua_pushweapon(L, pItem);
    END_LUA_CALL_HOOK(2, 1);

    RETURN_LUA_BOOLEAN();
#endif

    return BaseClass::CanHavePlayerItem(pPlayer, pItem);
}

#endif

//=========================================================
// WeaponShouldRespawn - any conditions inhibiting the
// respawning of this weapon?
//=========================================================
int CHL2MPRules::WeaponShouldRespawn(CBaseCombatWeapon *pWeapon) {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("WeaponShouldRespawn");
    lua_pushweapon(L, pWeapon);
    END_LUA_CALL_HOOK(1, 1);

    RETURN_LUA_INTEGER();
#endif

#ifndef CLIENT_DLL
    if (pWeapon->HasSpawnFlags(SF_NORESPAWN)) {
        return GR_WEAPON_RESPAWN_NO;
    }
#endif

    return GR_WEAPON_RESPAWN_YES;
}

//-----------------------------------------------------------------------------
// Purpose: Player has just left the game
//-----------------------------------------------------------------------------
void CHL2MPRules::ClientDisconnected(edict_t *pClient) {
#ifndef CLIENT_DLL
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("ClientDisconnected");
    lua_pushplayer(L, (CBasePlayer *)CBaseEntity::Instance(pClient));
    END_LUA_CALL_HOOK(1, 0);
#endif

    // Msg( "CLIENT DISCONNECTED, REMOVING FROM TEAM.\n" );

    CBasePlayer *pPlayer = (CBasePlayer *)CBaseEntity::Instance(pClient);
    if (pPlayer) {
        // Remove the player from his team
        if (pPlayer->GetTeam()) {
            pPlayer->GetTeam()->RemovePlayer(pPlayer);
        }
    }

    BaseClass::ClientDisconnected(pClient);

#endif
}

#ifndef CLIENT_DLL
#if defined(LUA_SDK)
float CHL2MPRules::FlPlayerFallDamage(CBasePlayer *pPlayer) {
    BEGIN_LUA_CALL_HOOK("FlPlayerFallDamage");
    lua_pushplayer(L, pPlayer);
    END_LUA_CALL_HOOK(1, 1);

    RETURN_LUA_NUMBER();

    return BaseClass::FlPlayerFallDamage(pPlayer);
}
#endif
#endif

//=========================================================
// Deathnotice.
//=========================================================
void CHL2MPRules::DeathNotice(CBasePlayer *pVictim,
                              const CTakeDamageInfo &info) {
#if defined(LUA_SDK)
    CTakeDamageInfo lInfo = info;

    BEGIN_LUA_CALL_HOOK("DeathNotice");
    lua_pushplayer(L, pVictim);
    lua_pushdamageinfo(L, lInfo);
    END_LUA_CALL_HOOK(2, 0);
#endif

#ifndef CLIENT_DLL
    // Work out what killed the player, and send a message to all clients about
    // it
    const char *killer_weapon_name =
        "world";  // by default, the player is killed by the world
    int killer_ID = 0;
#if defined(LUA_SDK)
    const char *killer_class_name =
        "class C_World";  // by default, the player is killed by the world
    const char *weapon_class_name = NULL;
#endif

    // Find the killer & the scorer
#if defined(LUA_SDK)
    CBaseEntity *pInflictor = lInfo.GetInflictor();
    CBaseEntity *pKiller = lInfo.GetAttacker();
#else
    CBaseEntity *pInflictor = info.GetInflictor();
    CBaseEntity *pKiller = info.GetAttacker();
#endif
    CBasePlayer *pScorer = GetDeathScorer(pKiller, pInflictor);

    // Custom kill type?
#if defined(LUA_SDK)
    if (lInfo.GetDamageCustom()) {
        killer_weapon_name = GetDamageCustomString(lInfo);
        killer_class_name = pKiller->GetClassname();
        weapon_class_name = pInflictor->GetClassname();
#else
    if (info.GetDamageCustom()) {
        killer_weapon_name = GetDamageCustomString(info);
#endif
        if (pScorer) {
            killer_ID = pScorer->GetUserID();
        }

#ifdef LUA_SDK
        if (!Q_strcmp(killer_class_name, weapon_class_name)) {
            // If the inflictor is the killer,  then it must be their current
            // weapon doing the damage
            CAI_BaseNPC *pNPC = pKiller->MyNPCPointer();
            if (pNPC && pNPC->GetActiveWeapon()) {
                killer_weapon_name = pNPC->GetActiveWeapon()->GetClassname();
                weapon_class_name = pNPC->GetActiveWeapon()->GetClassname();
            }
        }
#endif
    } else {
        // Is the killer a client?
        if (pScorer) {
            killer_ID = pScorer->GetUserID();

            if (pInflictor) {
                if (pInflictor == pScorer) {
                    // If the inflictor is the killer,  then it must be their
                    // current weapon doing the damage
                    if (pScorer->GetActiveWeapon()) {
                        killer_weapon_name =
                            pScorer->GetActiveWeapon()->GetClassname();
                    }
                } else {
                    killer_weapon_name =
                        pInflictor->GetClassname();  // it's just that easy
                }
            }
        } else {
            killer_weapon_name = pInflictor->GetClassname();
#ifdef LUA_SDK
            killer_class_name = pKiller->GetClassname();
            weapon_class_name = pInflictor->GetClassname();

            if (!Q_strcmp(killer_class_name, weapon_class_name)) {
                // If the inflictor is the killer,  then it must be their
                // current weapon doing the damage
                CAI_BaseNPC *pNPC = pKiller->MyNPCPointer();
                if (pNPC && pNPC->GetActiveWeapon()) {
                    killer_weapon_name =
                        pNPC->GetActiveWeapon()->GetClassname();
                    weapon_class_name = pNPC->GetActiveWeapon()->GetClassname();
                }
            }
#endif
        }

        // strip the NPC_* or weapon_* from the inflictor's classname
        if (strncmp(killer_weapon_name, "weapon_", 7) == 0) {
            killer_weapon_name += 7;
        } else if (strncmp(killer_weapon_name, "npc_", 4) == 0) {
            killer_weapon_name += 4;
        } else if (strncmp(killer_weapon_name, "func_", 5) == 0) {
            killer_weapon_name += 5;
        } else if (strstr(killer_weapon_name, "physics")) {
            killer_weapon_name = "physics";
        }

        if (strcmp(killer_weapon_name, "prop_combine_ball") == 0) {
            killer_weapon_name = "combine_ball";
        } else if (strcmp(killer_weapon_name, "grenade_ar2") == 0) {
            killer_weapon_name = "smg1_grenade";
        } else if (strcmp(killer_weapon_name, "satchel") == 0 ||
                   strcmp(killer_weapon_name, "tripmine") == 0) {
            killer_weapon_name = "slam";
        }
    }

    IGameEvent *event = gameeventmanager->CreateEvent("player_death");
    if (event) {
        event->SetInt("userid", pVictim->GetUserID());
        event->SetInt("attacker", killer_ID);
#ifdef LUA_SDK
        event->SetString("attackername", killer_class_name);
#endif
        event->SetString("weapon", killer_weapon_name);
#ifdef LUA_SDK
        event->SetString("weaponname", weapon_class_name);
#endif
        event->SetInt("priority", 7);
        gameeventmanager->FireEvent(event);
    }
#endif
}

void CHL2MPRules::ClientSettingsChanged(CBasePlayer *pPlayer) {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("ClientSettingsChanged");
    lua_pushplayer(L, pPlayer);
    END_LUA_CALL_HOOK(1, 0);
#endif

#ifndef CLIENT_DLL

    CHL2MP_Player *pHL2Player = ToHL2MPPlayer(pPlayer);

    if (pHL2Player == NULL)
        return;

    const char *pCurrentModel = modelinfo->GetModelName(pPlayer->GetModel());
    const char *szModelName = engine->GetClientConVarValue(
        engine->IndexOfEdict(pPlayer->edict()), "cl_playermodel");

//#ifdef HL2SB
//    // Andrew; Map our requested player model to the new model/player path.
//    char file[_MAX_PATH];
//    Q_strncpy(file, szModelName, sizeof(file));
//    if (Q_strnicmp(file, "models/player/", 14)) {
//        char *substring = strstr(file, "models/");
//        if (substring) {
//            // replace with new directory
//            const char *dirname = substring + strlen("models/");
//            *substring = 0;
//            char destpath[_MAX_PATH];
//            // player
//            Q_snprintf(destpath, sizeof(destpath), "models/player/%s", dirname);
//            szModelName = destpath;
//        }
//    }
//#endif

    // If we're different.
    if (stricmp(szModelName, pCurrentModel)) {
        // Too soon, set the cvar back to what it was.
        // Note: this will make this function be called again
        // but since our models will match it'll just skip this whole dealio.
        if (pHL2Player->GetNextModelChangeTime() >= gpGlobals->curtime) {
            char szReturnString[512];

            Q_snprintf(szReturnString, sizeof(szReturnString),
                       "cl_playermodel %s\n", pCurrentModel);
            engine->ClientCommand(pHL2Player->edict(), szReturnString);

            Q_snprintf(szReturnString, sizeof(szReturnString),
                       "Please wait %d more seconds before trying to switch.\n",
                       (int)(pHL2Player->GetNextModelChangeTime() -
                             gpGlobals->curtime));
            ClientPrint(pHL2Player, HUD_PRINTTALK, szReturnString);
            return;
        }

        if (HL2MPRules()->IsTeamplay() == false) {
            pHL2Player->SetPlayerModel();

            const char *pszCurrentModelName =
                modelinfo->GetModelName(pHL2Player->GetModel());

            char szReturnString[128];
            Q_snprintf(szReturnString, sizeof(szReturnString),
                       "Your player model is: %s\n", pszCurrentModelName);

            ClientPrint(pHL2Player, HUD_PRINTTALK, szReturnString);
        } else {
//#ifdef HL2SB
//            if (Q_stristr(szModelName, "models/player/human")) {
//#else
            if (Q_stristr(szModelName, "models/human")) {
            //#endif
                pHL2Player->ChangeTeam(TEAM_REBELS);
            } else {
                pHL2Player->ChangeTeam(TEAM_COMBINE);
            }
        }
    }
    if (sv_report_client_settings.GetInt() == 1) {
        UTIL_LogPrintf(
            "\"%s\" cl_cmdrate = \"%s\"\n", pHL2Player->GetPlayerName(),
            engine->GetClientConVarValue(pHL2Player->entindex(), "cl_cmdrate"));
    }

    BaseClass::ClientSettingsChanged(pPlayer);
#endif
}

int CHL2MPRules::PlayerRelationship(CBaseEntity *pPlayer,
                                    CBaseEntity *pTarget) {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("PlayerRelationship");
    lua_pushentity(L, pPlayer);
    lua_pushentity(L, pTarget);
    END_LUA_CALL_HOOK(2, 1);

    RETURN_LUA_INTEGER();
#endif

#ifndef CLIENT_DLL
    // half life multiplay has a simple concept of Player Relationships.
    // you are either on another player's team, or you are not.
    if (!pPlayer || !pTarget || !pTarget->IsPlayer() || IsTeamplay() == false)
        return GR_NOTTEAMMATE;

    if ((*GetTeamID(pPlayer) != '\0') && (*GetTeamID(pTarget) != '\0') &&
        !stricmp(GetTeamID(pPlayer), GetTeamID(pTarget))) {
        return GR_TEAMMATE;
    }
#endif

    return GR_NOTTEAMMATE;
}

#ifndef CLIENT_DLL
#if defined(LUA_SDK)
bool CHL2MPRules::PlayerCanHearChat(CBasePlayer *pListener,
                                    CBasePlayer *pSpeaker) {
    BEGIN_LUA_CALL_HOOK("PlayerCanHearChat");
    lua_pushplayer(L, pListener);
    lua_pushplayer(L, pSpeaker);
    END_LUA_CALL_HOOK(2, 1);

    RETURN_LUA_BOOLEAN();

    return BaseClass::PlayerCanHearChat(pListener, pSpeaker);
}

bool CHL2MPRules::ClientConnected(edict_t *pEntity, const char *pszName,
                                  const char *pszAddress, char *reject,
                                  int maxrejectlen) {
    BEGIN_LUA_CALL_HOOK("ClientConnected");
    lua_pushplayer(L, (CBasePlayer *)CBaseEntity::Instance(pEntity));
    lua_pushstring(L, pszName);
    lua_pushstring(L, pszAddress);
    lua_pushstring(L, reject);
    lua_pushinteger(L, maxrejectlen);
    END_LUA_CALL_HOOK(5, 1);

    RETURN_LUA_BOOLEAN();

    return BaseClass::ClientConnected(pEntity, pszName, pszAddress, reject,
                                      maxrejectlen);
}

void CHL2MPRules::InitHUD(CBasePlayer *pPlayer) {
    BEGIN_LUA_CALL_HOOK("InitHUD");
    lua_pushplayer(L, pPlayer);
    END_LUA_CALL_HOOK(1, 0);

    BaseClass::InitHUD(pPlayer);
}
#endif
#endif

const char *CHL2MPRules::GetGameDescription(void) {
#if !defined(LUA_SDK)
    if (IsTeamplay())
        return "Team Deathmatch";
#else
    BEGIN_LUA_CALL_HOOK("GetGameDescription");
    END_LUA_CALL_HOOK(0, 1);

    RETURN_LUA_STRING();
#endif

#if !defined(HL2SB)
    return "Deathmatch";
#else
	return "Half-Life 2 Sandbox";
#endif
}

bool CHL2MPRules::IsConnectedUserInfoChangeAllowed(CBasePlayer *pPlayer) {
    return true;
}

float CHL2MPRules::GetMapRemainingTime() {
#if !defined(LUA_SDK)
    // if timelimit is disabled, return 0
    if (mp_timelimit.GetInt() <= 0)
        return 0;
#else
    BEGIN_LUA_CALL_HOOK("GetMapRemainingTime");
    END_LUA_CALL_HOOK(0, 1);

    RETURN_LUA_NUMBER();
#endif

    // timelimit is in minutes

    float timeleft = (m_flGameStartTime + mp_timelimit.GetInt() * 60.0f) -
                     gpGlobals->curtime;

    return timeleft;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CHL2MPRules::Precache(void) {
#if !defined(LUA_SDK)
    CBaseEntity::PrecacheScriptSound("AlyxEmp.Charge");
#else
    BEGIN_LUA_CALL_HOOK("Precache");
    END_LUA_CALL_HOOK(0, 0);
#endif
}

bool CHL2MPRules::ShouldCollide(int collisionGroup0, int collisionGroup1) {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("ShouldCollide");
    lua_pushinteger(L, collisionGroup0);
    lua_pushinteger(L, collisionGroup1);
    END_LUA_CALL_HOOK(2, 1);

    RETURN_LUA_BOOLEAN();
#endif

    if (collisionGroup0 > collisionGroup1) {
        // swap so that lowest is always first
        V_swap(collisionGroup0, collisionGroup1);
    }

    if ((collisionGroup0 == COLLISION_GROUP_PLAYER ||
         collisionGroup0 == COLLISION_GROUP_PLAYER_MOVEMENT) &&
        collisionGroup1 == COLLISION_GROUP_WEAPON) {
        return false;
    }

    return BaseClass::ShouldCollide(collisionGroup0, collisionGroup1);
}

//#if !defined(CLIENT_DLL) && defined(HL2SB)
////-----------------------------------------------------------------------------
//// Returns whether or not Alyx cares about light levels in order to see.
////-----------------------------------------------------------------------------
//bool CHL2MPRules::IsAlyxInDarknessMode() {
//#ifdef HL2_EPISODIC
//    if (alyx_darkness_force.GetBool())
//        return true;
//
//    return (GlobalEntity_GetState("ep_alyx_darknessmode") == GLOBAL_ON);
//#else
//    return false;
//#endif  // HL2_EPISODIC
//}
//
//#endif

bool CHL2MPRules::ClientCommand(CBaseEntity *pEdict, const CCommand &args) {
#ifndef CLIENT_DLL
    if (BaseClass::ClientCommand(pEdict, args))
        return true;

    CHL2MP_Player *pPlayer = (CHL2MP_Player *)pEdict;

    if (pPlayer->ClientCommand(args))
        return true;
#endif

    return false;
}

// shared ammo definition
// JAY: Trying to make a more physical bullet response
#define BULLET_MASS_GRAINS_TO_LB(grains) (0.002285 * (grains) / 16.0f)
#define BULLET_MASS_GRAINS_TO_KG(grains) \
    lbs2kg(BULLET_MASS_GRAINS_TO_LB(grains))

// exaggerate all of the forces, but use real numbers to keep them consistent
#define BULLET_IMPULSE_EXAGGERATION 3.5
// convert a velocity in ft/sec and a mass in grains to an impulse in kg in/s
#define BULLET_IMPULSE(grains, ftpersec)                  \
    ((ftpersec) * 12 * BULLET_MASS_GRAINS_TO_KG(grains) * \
     BULLET_IMPULSE_EXAGGERATION)

CAmmoDef *GetAmmoDef() {
    static CAmmoDef def;
    static bool bInitted = false;

    if (!bInitted) {
        bInitted = true;

        def.AddAmmoType("AR2", DMG_BULLET, TRACER_LINE_AND_WHIZ, 0, 0, 60,
                        BULLET_IMPULSE(200, 1225), 0);
        def.AddAmmoType("AR2AltFire", DMG_DISSOLVE, TRACER_NONE, 0, 0, 3, 0, 0);
        def.AddAmmoType("Pistol", DMG_BULLET, TRACER_LINE_AND_WHIZ, 0, 0, 150,
                        BULLET_IMPULSE(200, 1225), 0);
        def.AddAmmoType("SMG1", DMG_BULLET, TRACER_LINE_AND_WHIZ, 0, 0, 225,
                        BULLET_IMPULSE(200, 1225), 0);
        def.AddAmmoType("357", DMG_BULLET, TRACER_LINE_AND_WHIZ, 0, 0, 12,
                        BULLET_IMPULSE(800, 5000), 0);
        def.AddAmmoType("XBowBolt", DMG_BULLET, TRACER_LINE, 0, 0, 10,
                        BULLET_IMPULSE(800, 8000), 0);
        def.AddAmmoType("Buckshot", DMG_BULLET | DMG_BUCKSHOT, TRACER_LINE, 0,
                        0, 30, BULLET_IMPULSE(400, 1200), 0);
        def.AddAmmoType("RPG_Round", DMG_BURN, TRACER_NONE, 0, 0, 3, 0, 0);
        def.AddAmmoType("SMG1_Grenade", DMG_BURN, TRACER_NONE, 0, 0, 3, 0, 0);
        def.AddAmmoType("Grenade", DMG_BURN, TRACER_NONE, 0, 0, 5, 0, 0);
        def.AddAmmoType("slam", DMG_BURN, TRACER_NONE, 0, 0, 5, 0, 0);
    }

    return &def;
}

#ifdef CLIENT_DLL

ConVar cl_autowepswitch(
    "cl_autowepswitch", "1", FCVAR_ARCHIVE | FCVAR_USERINFO,
    "Automatically switch to picked up weapons (if more powerful)");

#else

#if defined(DEBUG) || defined(LUA_SDK)

// Handler for the "bot" command.
void Bot_f() {
    // Look at -count.
    int count = 1;
    count = clamp(count, 1, 16);

    int iTeam = TEAM_COMBINE;

    // Look at -frozen.
    bool bFrozen = false;

    // Ok, spawn all the bots.
    while (--count >= 0) {
        BotPutInServer(bFrozen, iTeam);
    }
}

ConCommand cc_Bot("bot", Bot_f, "Add a bot.", FCVAR_CHEAT);

#endif

bool CHL2MPRules::FShouldSwitchWeapon(CBasePlayer *pPlayer,
                                      CBaseCombatWeapon *pWeapon) {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("FShouldSwitchWeapon");
    lua_pushplayer(L, pPlayer);
    lua_pushweapon(L, pWeapon);
    END_LUA_CALL_HOOK(2, 1);

    RETURN_LUA_BOOLEAN();
#endif

    if (pPlayer->GetActiveWeapon() && pPlayer->IsNetClient()) {
        // Player has an active item, so let's check cl_autowepswitch.
        const char *cl_autowepswitch = engine->GetClientConVarValue(
            engine->IndexOfEdict(pPlayer->edict()), "cl_autowepswitch");
        if (cl_autowepswitch && atoi(cl_autowepswitch) <= 0) {
            return false;
        }
    }

    return BaseClass::FShouldSwitchWeapon(pPlayer, pWeapon);
}

#endif

#ifndef CLIENT_DLL

void CHL2MPRules::RestartGame() {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("RestartGame");
    END_LUA_CALL_HOOK(0, 0);
#endif

    // bounds check
    if (mp_timelimit.GetInt() < 0) {
        mp_timelimit.SetValue(0);
    }
    m_flGameStartTime = gpGlobals->curtime;
    if (!IsFinite(m_flGameStartTime.Get())) {
        Warning("Trying to set a NaN game start time\n");
        m_flGameStartTime.GetForModify() = 0.0f;
    }

    CleanUpMap();

    // now respawn all players
    for (int i = 1; i <= gpGlobals->maxClients; i++) {
        CHL2MP_Player *pPlayer = (CHL2MP_Player *)UTIL_PlayerByIndex(i);

        if (!pPlayer)
            continue;

        if (pPlayer->GetActiveWeapon()) {
            pPlayer->GetActiveWeapon()->Holster();
        }
        pPlayer->RemoveAllItems(true);
        respawn(pPlayer, false);
        pPlayer->Reset();
    }

    // Respawn entities (glass, doors, etc..)

    CTeam *pRebels = GetGlobalTeam(TEAM_REBELS);
    CTeam *pCombine = GetGlobalTeam(TEAM_COMBINE);

    if (pRebels) {
        pRebels->SetScore(0);
    }

    if (pCombine) {
        pCombine->SetScore(0);
    }

    m_flIntermissionEndTime = 0;
    m_flRestartGameTime = 0.0;
    m_bCompleteReset = false;

    IGameEvent *event = gameeventmanager->CreateEvent("round_start");
    if (event) {
        event->SetInt("fraglimit", 0);
        event->SetInt("priority", 6);  // HLTV event priority, not transmitted

        event->SetString("objective", "DEATHMATCH");

        gameeventmanager->FireEvent(event);
    }
}

void CHL2MPRules::CleanUpMap() {
    // Recreate all the map entities from the map data (preserving their
    // indices), then remove everything else except the players.

    // Get rid of all entities except players.
    CBaseEntity *pCur = gEntList.FirstEnt();
    while (pCur) {
        CBaseHL2MPCombatWeapon *pWeapon =
            dynamic_cast<CBaseHL2MPCombatWeapon *>(pCur);
        // Weapons with owners don't want to be removed..
        if (pWeapon) {
            if (!pWeapon->GetPlayerOwner()) {
                UTIL_Remove(pCur);
            }
        }
        // remove entities that has to be restored on roundrestart (breakables
        // etc)
        else if (!FindInList(s_PreserveEnts, pCur->GetClassname())) {
            UTIL_Remove(pCur);
        }

        pCur = gEntList.NextEnt(pCur);
    }

    // Really remove the entities so we can have access to their slots below.
    gEntList.CleanupDeleteList();

    // Cancel all queued events, in case a func_bomb_target fired some delayed
    // outputs that could kill respawning CTs
    g_EventQueue.Clear();

    // Now reload the map entities.
    class CHL2MPMapEntityFilter : public IMapEntityFilter {
       public:
        virtual bool ShouldCreateEntity(const char *pClassname) {
            // Don't recreate the preserved entities.
            if (!FindInList(s_PreserveEnts, pClassname)) {
                return true;
            } else {
                // Increment our iterator since it's not going to call
                // CreateNextEntity for this ent.
                if (m_iIterator != g_MapEntityRefs.InvalidIndex())
                    m_iIterator = g_MapEntityRefs.Next(m_iIterator);

                return false;
            }
        }

        virtual CBaseEntity *CreateNextEntity(const char *pClassname) {
            if (m_iIterator == g_MapEntityRefs.InvalidIndex()) {
                // This shouldn't be possible. When we loaded the map, it should
                // have used CCSMapLoadEntityFilter, which should have built the
                // g_MapEntityRefs list with the same list of entities we're
                // referring to here.
                Assert(false);
                return NULL;
            } else {
                CMapEntityRef &ref = g_MapEntityRefs[m_iIterator];
                m_iIterator = g_MapEntityRefs.Next(
                    m_iIterator);  // Seek to the next entity.

                if (ref.m_iEdict == -1 ||
                    engine->PEntityOfEntIndex(ref.m_iEdict)) {
                    // Doh! The entity was delete and its slot was reused.
                    // Just use any old edict slot. This case sucks because we
                    // lose the baseline.
                    return CreateEntityByName(pClassname);
                } else {
                    // Cool, the slot where this entity was is free again (most
                    // likely, the entity was freed above). Now create an entity
                    // with this specific index.
                    return CreateEntityByName(pClassname, ref.m_iEdict);
                }
            }
        }

       public:
        int m_iIterator;  // Iterator into g_MapEntityRefs.
    };
    CHL2MPMapEntityFilter filter;
    filter.m_iIterator = g_MapEntityRefs.Head();

    // DO NOT CALL SPAWN ON info_node ENTITIES!

    MapEntity_ParseAllEntities(engine->GetMapEntitiesString(), &filter, true);

#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("CleanUpMap");
    END_LUA_CALL_HOOK(0, 0);
#endif
}

void CHL2MPRules::CheckChatForReadySignal(CHL2MP_Player *pPlayer,
                                          const char *chatmsg) {
#if defined(LUA_SDK)
    BEGIN_LUA_CALL_HOOK("CheckChatForReadySignal");
    lua_pushplayer(L, pPlayer);
    lua_pushstring(L, chatmsg);
    END_LUA_CALL_HOOK(2, 0);
#endif

    if (m_bAwaitingReadyRestart &&
        FStrEq(chatmsg, mp_ready_signal.GetString())) {
        if (!pPlayer->IsReady()) {
            pPlayer->SetReady(true);
        }
    }
}

void CHL2MPRules::CheckRestartGame(void) {
    // Restart the game if specified by the server
    int iRestartDelay = mp_restartgame.GetInt();

    if (iRestartDelay > 0) {
        if (iRestartDelay > 60)
            iRestartDelay = 60;

        // let the players know
        char strRestartDelay[64];
        Q_snprintf(strRestartDelay, sizeof(strRestartDelay), "%d",
                   iRestartDelay);
        UTIL_ClientPrintAll(HUD_PRINTCENTER, "Game will restart in %s1 %s2",
                            strRestartDelay,
                            iRestartDelay == 1 ? "SECOND" : "SECONDS");
        UTIL_ClientPrintAll(HUD_PRINTCONSOLE, "Game will restart in %s1 %s2",
                            strRestartDelay,
                            iRestartDelay == 1 ? "SECOND" : "SECONDS");

        m_flRestartGameTime = gpGlobals->curtime + iRestartDelay;
        m_bCompleteReset = true;
        mp_restartgame.SetValue(0);
    }

    if (mp_readyrestart.GetBool()) {
        m_bAwaitingReadyRestart = true;
        m_bHeardAllPlayersReady = false;

        const char *pszReadyString = mp_ready_signal.GetString();

        // Don't let them put anything malicious in there
        if (pszReadyString == NULL || Q_strlen(pszReadyString) > 16) {
            pszReadyString = "ready";
        }

        IGameEvent *event =
            gameeventmanager->CreateEvent("hl2mp_ready_restart");
        if (event)
            gameeventmanager->FireEvent(event);

        mp_readyrestart.SetValue(0);

        // cancel any restart round in progress
        m_flRestartGameTime = -1;
    }
}

void CHL2MPRules::CheckAllPlayersReady(void) {
    for (int i = 1; i <= gpGlobals->maxClients; i++) {
        CHL2MP_Player *pPlayer = (CHL2MP_Player *)UTIL_PlayerByIndex(i);

        if (!pPlayer)
            continue;
        if (!pPlayer->IsReady())
            return;
    }
    m_bHeardAllPlayersReady = true;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
const char *CHL2MPRules::GetChatFormat(bool bTeamOnly, CBasePlayer *pPlayer) {
    if (!pPlayer)  // dedicated server output
    {
        return NULL;
    }

    const char *pszFormat = NULL;

    // team only
    if (bTeamOnly == TRUE) {
        if (pPlayer->GetTeamNumber() == TEAM_SPECTATOR) {
            pszFormat = "HL2MP_Chat_Spec";
        } else {
            const char *chatLocation = GetChatLocation(bTeamOnly, pPlayer);
            if (chatLocation && *chatLocation) {
                pszFormat = "HL2MP_Chat_Team_Loc";
            } else {
                pszFormat = "HL2MP_Chat_Team";
            }
        }
    }
    // everyone
    else {
        if (pPlayer->GetTeamNumber() != TEAM_SPECTATOR) {
            pszFormat = "HL2MP_Chat_All";
        } else {
            pszFormat = "HL2MP_Chat_AllSpec";
        }
    }

    return pszFormat;
}

#endif
