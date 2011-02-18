//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef LPERFORMANCE_H
#define LPERFORMANCE_H
#ifdef _WIN32
#pragma once
#endif

#pragma warning (disable : 4238)


/* type for physics_performanceparams_t in Lua */
typedef physics_performanceparams_t lua_Physics_performanceparams_t;



/*
** access functions (stack -> C)
*/

LUA_API lua_Physics_performanceparams_t      (lua_toperformanceparams) (lua_State *L, int idx);


/*
** set functions (stack -> Lua)
*/
LUA_API void  (lua_setperformanceparams) (lua_State *L, int idx, lua_Physics_performanceparams_t *pOutput);


#endif // LPERFORMANCE_H
