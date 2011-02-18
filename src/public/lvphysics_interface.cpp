//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Public interfaces to vphysics DLL
//
// $NoKeywords: $
//=============================================================================//

#define lvphysics_interface_cpp

#include "cbase.h"
#include "vphysics/performance.h"
#include "luamanager.h"
#include "lvphysics_interface.h"
#include "vphysics/lperformance.h"
#include "mathlib/lvector.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

/*
** access functions (stack -> C)
*/


LUA_API lua_IPhysicsObject *lua_tophysicsobject (lua_State *L, int idx) {
  lua_IPhysicsObject **ppPhysicsObject = (lua_IPhysicsObject **)luaL_checkudata(L, idx, "IPhysicsObject");
  return *ppPhysicsObject;
}



/*
** push functions (C -> stack)
*/


LUA_API void lua_pushphysicsobject (lua_State *L, lua_IPhysicsObject *pPhysicsObject) {
  if (pPhysicsObject == NULL)
    lua_pushnil(L);
  else {
    lua_IPhysicsObject **ppPhysicsObject = (lua_IPhysicsObject **)lua_newuserdata(L, sizeof(lua_IPhysicsObject));
    *ppPhysicsObject = pPhysicsObject;
    luaL_getmetatable(L, "IPhysicsObject");
    lua_setmetatable(L, -2);
  }
}


LUALIB_API lua_IPhysicsObject *luaL_checkphysicsobject (lua_State *L, int narg) {
  lua_IPhysicsObject *d = lua_tophysicsobject(L, narg);
  if (d == NULL)  /* avoid extra test when d is not 0 */
    luaL_argerror(L, narg, "attempt to index a NULL physics object");
  return d;
}


static int physenv_CleanupDeleteList (lua_State *L) {
  physenv->CleanupDeleteList();
  return 0;
}

static int physenv_ClearStats (lua_State *L) {
  physenv->ClearStats();
  return 0;
}

static int physenv_DebugCheckContacts (lua_State *L) {
  physenv->DebugCheckContacts();
  return 0;
}

static int physenv_GetActiveObjectCount (lua_State *L) {
  lua_pushnumber(L, physenv->GetActiveObjectCount());
  return 1;
}

static int physenv_GetAirDensity (lua_State *L) {
  lua_pushnumber(L, physenv->GetAirDensity());
  return 1;
}

static int physenv_GetGravity (lua_State *L) {
  Vector pGravityVector;
  physenv->GetGravity(&pGravityVector);
  lua_pushvector(L, pGravityVector);
  return 1;
}

static int physenv_GetNextFrameTime (lua_State *L) {
  lua_pushnumber(L, physenv->GetNextFrameTime());
  return 1;
}

static int physenv_GetPerformanceSettings (lua_State *L) {
  physics_performanceparams_t pOutput;
  physenv->GetPerformanceSettings(&pOutput);
  lua_newtable(L);
  lua_setperformanceparams(L, -1, &pOutput);
  return 1;
}

static int physenv_GetSimulationTime (lua_State *L) {
  lua_pushnumber(L, physenv->GetSimulationTime());
  return 1;
}

static int physenv_GetSimulationTimestep (lua_State *L) {
  lua_pushnumber(L, physenv->GetSimulationTimestep());
  return 1;
}

static int physenv_IsInSimulation (lua_State *L) {
  lua_pushboolean(L, physenv->IsInSimulation());
  return 1;
}

static int physenv_PostRestore (lua_State *L) {
  physenv->PostRestore();
  return 0;
}

static int physenv_ResetSimulationClock (lua_State *L) {
  physenv->ResetSimulationClock();
  return 0;
}

static int physenv_SetAirDensity (lua_State *L) {
  physenv->SetAirDensity(luaL_checknumber(L, 1));
  return 0;
}

static int physenv_SetGravity (lua_State *L) {
  physenv->SetGravity(luaL_checkvector(L, 1));
  return 0;
}

static int physenv_SetPerformanceSettings (lua_State *L) {
  physenv->SetPerformanceSettings(&lua_toperformanceparams(L, 1));
  return 0;
}

static int physenv_SetQuickDelete (lua_State *L) {
  physenv->SetQuickDelete(luaL_checkboolean(L, 1));
  return 0;
}

static int physenv_SetSimulationTimestep (lua_State *L) {
  physenv->SetSimulationTimestep(luaL_checknumber(L, 1));
  return 0;
}

static int physenv_Simulate (lua_State *L) {
  physenv->Simulate(luaL_checknumber(L, 1));
  return 0;
}


static const luaL_Reg physenvlib[] = {
  {"CleanupDeleteList",  physenv_CleanupDeleteList},
  {"ClearStats",  physenv_ClearStats},
  {"DebugCheckContacts",  physenv_DebugCheckContacts},
  {"GetActiveObjectCount",  physenv_GetActiveObjectCount},
  {"GetAirDensity",  physenv_GetAirDensity},
  {"GetGravity",  physenv_GetGravity},
  {"GetNextFrameTime",  physenv_GetNextFrameTime},
  {"GetPerformanceSettings",  physenv_GetPerformanceSettings},
  {"GetSimulationTime",  physenv_GetSimulationTime},
  {"GetSimulationTimestep",  physenv_GetSimulationTimestep},
  {"IsInSimulation",  physenv_IsInSimulation},
  {"PostRestore",  physenv_PostRestore},
  {"ResetSimulationClock",  physenv_ResetSimulationClock},
  {"SetAirDensity",  physenv_SetAirDensity},
  {"SetGravity",  physenv_SetGravity},
  {"SetPerformanceSettings",  physenv_SetPerformanceSettings},
  {"SetQuickDelete",  physenv_SetQuickDelete},
  {"SetSimulationTimestep",  physenv_SetSimulationTimestep},
  {"Simulate",  physenv_Simulate},
  {NULL, NULL}
};


/*
** Open physenv library
*/
int luaopen_physenv (lua_State *L) {
  luaL_register(L, "physenv", physenvlib);
  lua_pop(L, 1);
  return 1;
}


static int IPhysicsObject_ApplyForceCenter (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->ApplyForceCenter(luaL_checkvector(L, 2));
  return 0;
}

static int IPhysicsObject_ApplyForceOffset (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->ApplyForceOffset(luaL_checkvector(L, 2), luaL_checkvector(L, 3));
  return 0;
}

static int IPhysicsObject_ApplyTorqueCenter (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->ApplyTorqueCenter(luaL_checkvector(L, 2));
  return 0;
}

static int IPhysicsObject_BecomeHinged (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->BecomeHinged(luaL_checkinteger(L, 2));
  return 0;
}

static int IPhysicsObject_BecomeTrigger (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->BecomeTrigger();
  return 0;
}

static int IPhysicsObject_CalculateAngularDrag (lua_State *L) {
  lua_pushnumber(L, luaL_checkphysicsobject(L, 1)->CalculateAngularDrag(luaL_checkvector(L, 2)));
  return 1;
}

static int IPhysicsObject_CalculateForceOffset (lua_State *L) {
  Vector centerForce, centerTorque;
  luaL_checkphysicsobject(L, 1)->CalculateForceOffset(luaL_checkvector(L, 2), luaL_checkvector(L, 3), &centerForce, &centerTorque);
  lua_pushvector(L, centerForce);
  lua_pushvector(L, centerTorque);
  return 2;
}

static int IPhysicsObject_CalculateLinearDrag (lua_State *L) {
  lua_pushnumber(L, luaL_checkphysicsobject(L, 1)->CalculateLinearDrag(luaL_checkvector(L, 2)));
  return 1;
}

static int IPhysicsObject_CalculateVelocityOffset (lua_State *L) {
  Vector centerVelocity, centerAngularVelocity;
  luaL_checkphysicsobject(L, 1)->CalculateVelocityOffset(luaL_checkvector(L, 2), luaL_checkvector(L, 3), &centerVelocity, &centerAngularVelocity);
  lua_pushvector(L, centerVelocity);
  lua_pushvector(L, centerAngularVelocity);
  return 2;
}

static int IPhysicsObject_EnableCollisions (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->EnableCollisions(luaL_checkboolean(L, 2));
  return 0;
}

static int IPhysicsObject_EnableDrag (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->EnableDrag(luaL_checkboolean(L, 2));
  return 0;
}

static int IPhysicsObject_EnableGravity (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->EnableGravity(luaL_checkboolean(L, 2));
  return 0;
}

static int IPhysicsObject_EnableMotion (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->EnableMotion(luaL_checkboolean(L, 2));
  return 0;
}

static int IPhysicsObject_GetCallbackFlags (lua_State *L) {
  lua_pushinteger(L, luaL_checkphysicsobject(L, 1)->GetCallbackFlags());
  return 1;
}

static int IPhysicsObject_GetContactPoint (lua_State *L) {
  Vector contactPoint;
  IPhysicsObject *contactObject;
  lua_pushboolean(L, luaL_checkphysicsobject(L, 1)->GetContactPoint(&contactPoint, &contactObject));
  lua_pushvector(L, contactPoint);
  lua_pushphysicsobject(L, contactObject);
  return 3;
}

static int IPhysicsObject_GetContents (lua_State *L) {
  lua_pushinteger(L, luaL_checkphysicsobject(L, 1)->GetContents());
  return 1;
}

static int IPhysicsObject_GetDamping (lua_State *L) {
  float speed, rot;
  luaL_checkphysicsobject(L, 1)->GetDamping(&speed, &rot);
  lua_pushnumber(L, speed);
  lua_pushnumber(L, rot);
  return 2;
}

static int IPhysicsObject_GetEnergy (lua_State *L) {
  lua_pushnumber(L, luaL_checkphysicsobject(L, 1)->GetEnergy());
  return 1;
}

static int IPhysicsObject_GetGameFlags (lua_State *L) {
  lua_pushinteger(L, luaL_checkphysicsobject(L, 1)->GetGameFlags());
  return 1;
}

static int IPhysicsObject_GetGameIndex (lua_State *L) {
  lua_pushinteger(L, luaL_checkphysicsobject(L, 1)->GetGameIndex());
  return 1;
}

static int IPhysicsObject_GetImplicitVelocity (lua_State *L) {
  Vector velocity, angularVelocity;
  luaL_checkphysicsobject(L, 1)->GetImplicitVelocity(&velocity, &angularVelocity);
  lua_pushvector(L, velocity);
  lua_pushvector(L, angularVelocity);
  return 2;
}

static int IPhysicsObject_GetInertia (lua_State *L) {
  lua_pushvector(L, luaL_checkphysicsobject(L, 1)->GetInertia());
  return 1;
}

static int IPhysicsObject_GetInvInertia (lua_State *L) {
  lua_pushvector(L, luaL_checkphysicsobject(L, 1)->GetInvInertia());
  return 1;
}

static int IPhysicsObject_GetInvMass (lua_State *L) {
  lua_pushnumber(L, luaL_checkphysicsobject(L, 1)->GetInvMass());
  return 1;
}

static int IPhysicsObject_GetMass (lua_State *L) {
  lua_pushnumber(L, luaL_checkphysicsobject(L, 1)->GetMass());
  return 1;
}

static int IPhysicsObject_GetMassCenterLocalSpace (lua_State *L) {
  lua_pushvector(L, luaL_checkphysicsobject(L, 1)->GetMassCenterLocalSpace());
  return 1;
}

static int IPhysicsObject_GetMaterialIndex (lua_State *L) {
  lua_pushinteger(L, luaL_checkphysicsobject(L, 1)->GetMaterialIndex());
  return 1;
}

static int IPhysicsObject_GetName (lua_State *L) {
  lua_pushstring(L, luaL_checkphysicsobject(L, 1)->GetName());
  return 1;
}

static int IPhysicsObject_GetPosition (lua_State *L) {
  Vector worldPosition;
  QAngle angles;
  luaL_checkphysicsobject(L, 1)->GetPosition(&worldPosition, &angles);
  lua_pushvector(L, worldPosition);
  lua_pushangle(L, angles);
  return 2;
}

static int IPhysicsObject_GetShadowPosition (lua_State *L) {
  Vector position;
  QAngle angles;
  lua_pushinteger(L, luaL_checkphysicsobject(L, 1)->GetShadowPosition(&position, &angles));
  lua_pushvector(L, position);
  lua_pushangle(L, angles);
  return 3;
}

static int IPhysicsObject_GetSphereRadius (lua_State *L) {
  lua_pushnumber(L, luaL_checkphysicsobject(L, 1)->GetSphereRadius());
  return 1;
}

static int IPhysicsObject_GetVelocity (lua_State *L) {
  Vector velocity, angularVelocity;
  luaL_checkphysicsobject(L, 1)->GetVelocity(&velocity, &angularVelocity);
  lua_pushvector(L, velocity);
  lua_pushvector(L, angularVelocity);
  return 2;
}

static int IPhysicsObject_GetVelocityAtPoint (lua_State *L) {
  Vector pVelocity;
  luaL_checkphysicsobject(L, 1)->GetVelocityAtPoint(luaL_checkvector(L, 2), &pVelocity);
  lua_pushvector(L, pVelocity);
  return 1;
}

static int IPhysicsObject_IsAsleep (lua_State *L) {
  lua_pushboolean(L, luaL_checkphysicsobject(L, 1)->IsAsleep());
  return 1;
}

static int IPhysicsObject_IsAttachedToConstraint (lua_State *L) {
  lua_pushboolean(L, luaL_checkphysicsobject(L, 1)->IsAttachedToConstraint(luaL_checkboolean(L, 2)));
  return 1;
}

static int IPhysicsObject_IsCollisionEnabled (lua_State *L) {
  lua_pushboolean(L, luaL_checkphysicsobject(L, 1)->IsCollisionEnabled());
  return 1;
}

static int IPhysicsObject_IsDragEnabled (lua_State *L) {
  lua_pushboolean(L, luaL_checkphysicsobject(L, 1)->IsDragEnabled());
  return 1;
}

static int IPhysicsObject_IsFluid (lua_State *L) {
  lua_pushboolean(L, luaL_checkphysicsobject(L, 1)->IsFluid());
  return 1;
}

static int IPhysicsObject_IsGravityEnabled (lua_State *L) {
  lua_pushboolean(L, luaL_checkphysicsobject(L, 1)->IsGravityEnabled());
  return 1;
}

static int IPhysicsObject_IsHinged (lua_State *L) {
  lua_pushboolean(L, luaL_checkphysicsobject(L, 1)->IsHinged());
  return 1;
}

static int IPhysicsObject_IsMotionEnabled (lua_State *L) {
  lua_pushboolean(L, luaL_checkphysicsobject(L, 1)->IsMotionEnabled());
  return 1;
}

static int IPhysicsObject_IsMoveable (lua_State *L) {
  lua_pushboolean(L, luaL_checkphysicsobject(L, 1)->IsMoveable());
  return 1;
}

static int IPhysicsObject_IsStatic (lua_State *L) {
  lua_pushboolean(L, luaL_checkphysicsobject(L, 1)->IsStatic());
  return 1;
}

static int IPhysicsObject_IsTrigger (lua_State *L) {
  lua_pushboolean(L, luaL_checkphysicsobject(L, 1)->IsTrigger());
  return 1;
}

static int IPhysicsObject_LocalToWorld (lua_State *L) {
  Vector worldPosition;
  luaL_checkphysicsobject(L, 1)->LocalToWorld(&worldPosition, luaL_checkvector(L, 2));
  lua_pushvector(L, worldPosition);
  return 1;
}

static int IPhysicsObject_LocalToWorldVector (lua_State *L) {
  Vector worldVector;
  luaL_checkphysicsobject(L, 1)->LocalToWorldVector(&worldVector, luaL_checkvector(L, 2));
  lua_pushvector(L, worldVector);
  return 1;
}

static int IPhysicsObject_OutputDebugInfo (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->OutputDebugInfo();
  return 0;
}

static int IPhysicsObject_RecheckCollisionFilter (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->RecheckCollisionFilter();
  return 0;
}

static int IPhysicsObject_RecheckContactPoints (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->RecheckContactPoints();
  return 0;
}

static int IPhysicsObject_RemoveHinged (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->RemoveHinged();
  return 0;
}

static int IPhysicsObject_RemoveShadowController (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->RemoveShadowController();
  return 0;
}

static int IPhysicsObject_RemoveTrigger (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->RemoveTrigger();
  return 0;
}

static int IPhysicsObject_SetBuoyancyRatio (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->SetBuoyancyRatio(luaL_checknumber(L, 2));
  return 0;
}

static int IPhysicsObject_SetCallbackFlags (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->SetCallbackFlags(luaL_checkinteger(L, 2));
  return 0;
}

static int IPhysicsObject_SetContents (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->SetContents(luaL_checkinteger(L, 2));
  return 0;
}

static int IPhysicsObject_SetDragCoefficient (lua_State *L) {
  float pDrag = luaL_checknumber(L, 2);
  float pAngularDrag = luaL_checknumber(L, 3);
  luaL_checkphysicsobject(L, 1)->SetDragCoefficient(&pDrag, &pAngularDrag);
  return 0;
}

static int IPhysicsObject_SetGameFlags (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->SetGameFlags(luaL_checkinteger(L, 2));
  return 0;
}

static int IPhysicsObject_SetGameIndex (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->SetGameIndex(luaL_checkinteger(L, 2));
  return 0;
}

static int IPhysicsObject_SetMass (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->SetMass(luaL_checknumber(L, 2));
  return 0;
}

static int IPhysicsObject_SetMaterialIndex (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->SetMaterialIndex(luaL_checkinteger(L, 2));
  return 0;
}

static int IPhysicsObject_SetShadow (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->SetShadow(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checkboolean(L, 4), luaL_checkboolean(L, 5));
  return 0;
}

static int IPhysicsObject_Sleep (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->Sleep();
  return 0;
}

static int IPhysicsObject_Wake (lua_State *L) {
  luaL_checkphysicsobject(L, 1)->Wake();
  return 0;
}

static int IPhysicsObject_WorldToLocal (lua_State *L) {
  Vector localPosition;
  luaL_checkphysicsobject(L, 1)->WorldToLocal(&localPosition, luaL_checkvector(L, 2));
  lua_pushvector(L, localPosition);
  return 1;
}

static int IPhysicsObject_WorldToLocalVector (lua_State *L) {
  Vector localVector;
  luaL_checkphysicsobject(L, 1)->WorldToLocalVector(&localVector, luaL_checkvector(L, 2));
  lua_pushvector(L, localVector);
  return 1;
}

static int IPhysicsObject___tostring (lua_State *L) {
  lua_pushfstring(L, "IPhysicsObject: %p", luaL_checkudata(L, 1, "IPhysicsObject"));
  return 1;
}


static const luaL_Reg IPhysicsObjectmeta[] = {
  {"ApplyForceCenter", IPhysicsObject_ApplyForceCenter},
  {"ApplyForceOffset", IPhysicsObject_ApplyForceOffset},
  {"ApplyTorqueCenter", IPhysicsObject_ApplyTorqueCenter},
  {"BecomeHinged", IPhysicsObject_BecomeHinged},
  {"BecomeTrigger", IPhysicsObject_BecomeTrigger},
  {"CalculateAngularDrag", IPhysicsObject_CalculateAngularDrag},
  {"CalculateForceOffset", IPhysicsObject_CalculateForceOffset},
  {"CalculateLinearDrag", IPhysicsObject_CalculateLinearDrag},
  {"CalculateVelocityOffset", IPhysicsObject_CalculateVelocityOffset},
  {"EnableCollisions", IPhysicsObject_EnableCollisions},
  {"EnableDrag", IPhysicsObject_EnableDrag},
  {"EnableGravity", IPhysicsObject_EnableGravity},
  {"EnableMotion", IPhysicsObject_EnableMotion},
  {"GetCallbackFlags", IPhysicsObject_GetCallbackFlags},
  {"GetContactPoint", IPhysicsObject_GetContactPoint},
  {"GetContents", IPhysicsObject_GetContents},
  {"GetDamping", IPhysicsObject_GetDamping},
  {"GetEnergy", IPhysicsObject_GetEnergy},
  {"GetGameFlags", IPhysicsObject_GetGameFlags},
  {"GetGameIndex", IPhysicsObject_GetGameIndex},
  {"GetImplicitVelocity", IPhysicsObject_GetImplicitVelocity},
  {"GetInertia", IPhysicsObject_GetInertia},
  {"GetInvInertia", IPhysicsObject_GetInvInertia},
  {"GetInvMass", IPhysicsObject_GetInvMass},
  {"GetMass", IPhysicsObject_GetMass},
  {"GetMassCenterLocalSpace", IPhysicsObject_GetMassCenterLocalSpace},
  {"GetMaterialIndex", IPhysicsObject_GetMaterialIndex},
  {"GetName", IPhysicsObject_GetName},
  {"GetPosition", IPhysicsObject_GetPosition},
  {"GetShadowPosition", IPhysicsObject_GetShadowPosition},
  {"GetSphereRadius", IPhysicsObject_GetSphereRadius},
  {"GetVelocity", IPhysicsObject_GetVelocity},
  {"GetVelocityAtPoint", IPhysicsObject_GetVelocityAtPoint},
  {"IsAsleep", IPhysicsObject_IsAsleep},
  {"IsAttachedToConstraint", IPhysicsObject_IsAttachedToConstraint},
  {"IsCollisionEnabled", IPhysicsObject_IsCollisionEnabled},
  {"IsDragEnabled", IPhysicsObject_IsDragEnabled},
  {"IsFluid", IPhysicsObject_IsFluid},
  {"IsGravityEnabled", IPhysicsObject_IsGravityEnabled},
  {"IsHinged", IPhysicsObject_IsHinged},
  {"IsMotionEnabled", IPhysicsObject_IsMotionEnabled},
  {"IsMoveable", IPhysicsObject_IsMoveable},
  {"IsStatic", IPhysicsObject_IsStatic},
  {"IsTrigger", IPhysicsObject_IsTrigger},
  {"LocalToWorld", IPhysicsObject_LocalToWorld},
  {"LocalToWorldVector", IPhysicsObject_LocalToWorldVector},
  {"OutputDebugInfo", IPhysicsObject_OutputDebugInfo},
  {"RecheckCollisionFilter", IPhysicsObject_RecheckCollisionFilter},
  {"RecheckContactPoints", IPhysicsObject_RecheckContactPoints},
  {"RemoveHinged", IPhysicsObject_RemoveHinged},
  {"RemoveShadowController", IPhysicsObject_RemoveShadowController},
  {"RemoveTrigger", IPhysicsObject_RemoveTrigger},
  {"SetBuoyancyRatio", IPhysicsObject_SetBuoyancyRatio},
  {"SetCallbackFlags", IPhysicsObject_SetCallbackFlags},
  {"SetContents", IPhysicsObject_SetContents},
  {"SetDragCoefficient", IPhysicsObject_SetDragCoefficient},
  {"SetGameFlags", IPhysicsObject_SetGameFlags},
  {"SetGameIndex", IPhysicsObject_SetGameIndex},
  {"SetMass", IPhysicsObject_SetMass},
  {"SetMaterialIndex", IPhysicsObject_SetMaterialIndex},
  {"SetShadow", IPhysicsObject_SetShadow},
  {"Sleep", IPhysicsObject_Sleep},
  {"Wake", IPhysicsObject_Wake},
  {"WorldToLocal", IPhysicsObject_WorldToLocal},
  {"WorldToLocalVector", IPhysicsObject_WorldToLocalVector},
  {"__tostring", IPhysicsObject___tostring},
  {NULL, NULL}
};


/*
** Open IPhysicsObject object
*/
int luaopen_IPhysicsObject (lua_State *L) {
  luaL_newmetatable(L, "IPhysicsObject");
  luaL_register(L, NULL, IPhysicsObjectmeta);
  lua_pushvalue(L, -1);  /* push metatable */
  lua_setfield(L, -2, "__index");  /* metatable.__index = metatable */
  lua_pushstring(L, "physicsobject");
  lua_setfield(L, -2, "__type");  /* metatable.__type = "physicsobject" */
  lua_pop(L, 1);
  return 1;
}

