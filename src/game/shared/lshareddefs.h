//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Definitions that are shared by the game DLL and the client DLL.
//
// $NoKeywords: $
//=============================================================================//

#ifndef LSHAREDDEFS_H
#define LSHAREDDEFS_H
#ifdef _WIN32
#pragma once
#endif


/* type for FireBulletsInfo_t in Lua */
typedef FireBulletsInfo_t lua_FireBulletsInfo_t;



/*
** access functions (stack -> C)
*/

LUA_API lua_FireBulletsInfo_t      (lua_tofirebulletsinfo) (lua_State *L, int idx);


#endif // LSHAREDDEFS_H
