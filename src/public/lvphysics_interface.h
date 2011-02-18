//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Public interfaces to vphysics DLL
//
// $NoKeywords: $
//=============================================================================//

#ifndef LVPHYSICS_INTERFACE_H
#define LVPHYSICS_INTERFACE_H
#ifdef _WIN32
#pragma once
#endif

/* type for IPhysicsObject functions */
typedef IPhysicsObject lua_IPhysicsObject;



/*
** access functions (stack -> C)
*/

LUA_API lua_IPhysicsObject     *(lua_tophysicsobject) (lua_State *L, int idx);


/*
** push functions (C -> stack)
*/
LUA_API void  (lua_pushphysicsobject) (lua_State *L, lua_IPhysicsObject *pPhysicsObject);



LUALIB_API lua_IPhysicsObject *(luaL_checkphysicsobject) (lua_State *L, int narg);


int luaopen_physenv(lua_State *L);
int luaopen_IPhysicsObject(lua_State *L);

#endif // LVPHYSICS_INTERFACE_H
