//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Basic header for using vgui
//
// $NoKeywords: $
//=============================================================================//

#ifndef LVGUI_H
#define LVGUI_H

#ifdef _WIN32
#pragma once
#endif

/* type for HFont functions */
typedef vgui::HFont lua_HFont;



/*
** access functions (stack -> C)
*/

LUA_API lua_HFont     (lua_tohfont) (lua_State *L, int idx);


/*
** push functions (C -> stack)
*/
LUA_API void  (lua_pushhfont) (lua_State *L, lua_HFont hFont);



LUALIB_API lua_HFont (luaL_checkhfont) (lua_State *L, int narg);


LUALIB_API int luaopen_HFont(lua_State *L);


#endif // LVGUI_H
