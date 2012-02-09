//========== Copyleft © 2011, Team Sandbox, Some rights reserved. ===========//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//


#define lsrcinit_cpp

#include "cbase.h"
#include "lua.hpp"

#ifdef CLIENT_DLL
#include "lc_baseanimating.h"
#include "lc_baseentity.h"
#include "lc_recipientfilter.h"
#include "lcdll_int.h"
#include "lcdll_util.h"
#include "liclientshadowmgr.h"
#include "lprediction.h"
#include "vgui/LISurface.h"
#include "vgui_controls/lPanel.h"
#else
#include "lbaseanimating.h"
#include "lbaseentity.h"
#include "leiface.h"
#include "linetchannelinfo.h"
#include "lplayer.h"
#include "lrecipientfilter.h"
#include "lutil.h"
#endif
#include "lbasecombatweapon_shared.h"
#include "lbaseentity_shared.h"
#include "lbaseplayer_shared.h"
#include "lColor.h"
#include "lconvar.h"
#include "leffect_dispatch_data.h"
#include "lfilesystem.h"
#include "lgametrace.h"
#include "lglobalvars_base.h"
#include "lhl2mp_player_shared.h"
#include "licvar.h"
#include "materialsystem/limaterial.h"
#include "limovehelper.h"
#include "steam/listeamfriends.h"
#include "engine/livdebugoverlay.h"
#include "lnetworkstringtabledefs.h"
#include "vstdlib/lrandom.h"
#include "steam/lsteam_api.h"
#include "ltakedamageinfo.h"
#include "lutil_shared.h"
#include "mathlib/lvector.h"
#include "lvphysics_interface.h"
#include "luasrclib.h"
#include "lauxlib.h"


static const luaL_Reg luasrclibs[] = {
  {"CBaseAnimating", luaopen_CBaseAnimating},
  {"engine", luaopen_engine},
  {"CBaseCombatWeapon", luaopen_CBaseCombatWeapon},
  {"CBaseEntity", luaopen_CBaseEntity},
  {"CBaseEntity", luaopen_CBaseEntity_shared},
  {"CRecipientFilter", luaopen_CRecipientFilter},
#ifdef GAME_DLL
  {"CBasePlayer", luaopen_CBasePlayer},
#endif
  {"CBasePlayer", luaopen_CBasePlayer_shared},
  {"Color", luaopen_Color},
  {"ConCommand", luaopen_ConCommand},
  {"ConVar", luaopen_ConVar},
  {"dbg", luaopen_dbg},
  {"CEffectData", luaopen_CEffectData},
  {"filesystem", luaopen_filesystem},
  {"CGameTrace", luaopen_CGameTrace},
  {"gpGlobals", luaopen_gpGlobals},
  {"CHL2MP_Player", luaopen_CHL2MP_Player_shared},
  {"cvar", luaopen_cvar},
  {"IMaterial", luaopen_IMaterial},
  {"IMoveHelper", luaopen_IMoveHelper},
  {"ISteamFriends", luaopen_ISteamFriends},
#ifdef GAME_DLL
  {"INetChannelInfo", luaopen_INetChannelInfo},
#endif
  {"debugoverlay", luaopen_debugoverlay},
  {"INetworkStringTable", luaopen_INetworkStringTable},
  {"networkstringtable", luaopen_networkstringtable},
  {"random", luaopen_random},
  {"steamapicontext", luaopen_steamapicontext},
  {"CTakeDamageInfo", luaopen_CTakeDamageInfo},
#ifdef CLIENT_DLL
  {"g_pClientShadowMgr", luaopen_g_pClientShadowMgr},
  {"enginevgui", luaopen_enginevgui},
  {"prediction", luaopen_prediction},
  {"surface", luaopen_surface},
  {"Panel", luaopen_Panel},
  {"vgui", luaopen_vgui},
#endif
  {"UTIL", luaopen_UTIL},
  {"UTIL", luaopen_UTIL_shared},
  {"Vector", luaopen_Vector},
  {"QAngle", luaopen_QAngle},
  {"physenv", luaopen_physenv},
  {"IPhysicsObject", luaopen_IPhysicsObject},
  {"IPhysicsSurfaceProps", luaopen_IPhysicsSurfaceProps},
  {NULL, NULL}
};


LUALIB_API void luasrc_openlibs (lua_State *L) {
  const luaL_Reg *lib = luasrclibs;
  for (; lib->func; lib++) {
    lua_pushcfunction(L, lib->func);
    lua_pushstring(L, lib->name);
    lua_call(L, 1, 0);
  }
}

