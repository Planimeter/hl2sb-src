//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#define lperformance_cpp

#include "cbase.h"
#include "performance.h"
#include "lua.hpp"
#include "lperformance.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


/*
** access functions (stack -> C)
*/


LUA_API lua_Physics_performanceparams_t lua_toperformanceparams (lua_State *L, int idx) {
  luaL_checktype(L, idx, LUA_TTABLE);
  physics_performanceparams_t params;
  params.Defaults();
  lua_getfield(L, idx, "maxCollisionsPerObjectPerTimestep");
  if (!lua_isnil(L, -1))
    params.maxCollisionsPerObjectPerTimestep = luaL_checkint(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, idx, "maxCollisionChecksPerTimestep");
  if (!lua_isnil(L, -1))
    params.maxCollisionChecksPerTimestep = luaL_checkint(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, idx, "maxVelocity");
  if (!lua_isnil(L, -1))
    params.maxVelocity = luaL_checknumber(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, idx, "maxAngularVelocity");
  if (!lua_isnil(L, -1))
    params.maxAngularVelocity = luaL_checknumber(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, idx, "lookAheadTimeObjectsVsWorld");
  if (!lua_isnil(L, -1))
    params.lookAheadTimeObjectsVsWorld = luaL_checknumber(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, idx, "lookAheadTimeObjectsVsObject");
  if (!lua_isnil(L, -1))
    params.lookAheadTimeObjectsVsObject = luaL_checknumber(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, idx, "minFrictionMass");
  if (!lua_isnil(L, -1))
    params.minFrictionMass = luaL_checknumber(L, -1);
  lua_pop(L, 1);
  lua_getfield(L, idx, "maxFrictionMass");
  if (!lua_isnil(L, -1))
    params.maxFrictionMass = luaL_checknumber(L, -1);
  lua_pop(L, 1);
  return params;
}



/*
** set functions (stack -> Lua)
*/


LUA_API void lua_setperformanceparams (lua_State *L, int idx, lua_Physics_performanceparams_t *pOutput) {
  idx = idx - 2;
  lua_pushstring(L, "maxCollisionsPerObjectPerTimestep");
  lua_pushinteger(L, pOutput->maxCollisionChecksPerTimestep);
  lua_settable(L, idx);
  lua_pushstring(L, "maxCollisionChecksPerTimestep");
  lua_pushinteger(L, pOutput->maxCollisionChecksPerTimestep);
  lua_settable(L, idx);
  lua_pushstring(L, "maxVelocity");
  lua_pushnumber(L, pOutput->maxVelocity);
  lua_settable(L, idx);
  lua_pushstring(L, "maxAngularVelocity");
  lua_pushnumber(L, pOutput->maxAngularVelocity);
  lua_settable(L, idx);
  lua_pushstring(L, "lookAheadTimeObjectsVsWorld");
  lua_pushnumber(L, pOutput->lookAheadTimeObjectsVsWorld);
  lua_settable(L, idx);
  lua_pushstring(L, "lookAheadTimeObjectsVsObject");
  lua_pushnumber(L, pOutput->lookAheadTimeObjectsVsObject);
  lua_settable(L, idx);
  lua_pushstring(L, "minFrictionMass");
  lua_pushnumber(L, pOutput->minFrictionMass);
  lua_settable(L, idx);
  lua_pushstring(L, "maxFrictionMass");
  lua_pushnumber(L, pOutput->maxFrictionMass);
  lua_settable(L, idx);
}

