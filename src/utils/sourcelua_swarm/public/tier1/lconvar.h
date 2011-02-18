//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $NoKeywords: $
//===========================================================================//

#ifndef LCONVAR_H
#define LCONVAR_H

#if _WIN32
#pragma once
#endif

/* type for ConVar functions */
typedef ConVar lua_ConVar;



/*
** access functions (stack -> C)
*/

LUA_API const lua_ConVar     *(lua_toconvar) (lua_State *L, int idx);


/*
** push functions (C -> stack)
*/
LUA_API void  (lua_pushconvar) (lua_State *L, const lua_ConVar *pConVar);



LUALIB_API const lua_ConVar *(luaL_checkconvar) (lua_State *L, int narg);


int luaopen_ConVar(lua_State *L);


#endif // LCONVAR_H
