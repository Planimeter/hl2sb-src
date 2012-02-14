//========== Copyleft © 2011, Team Sandbox, Some rights reserved. ===========//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//


#ifndef LUASRCLIB_H
#define LUASRCLIB_H
#ifdef _WIN32
#pragma once
#endif


#define LUA_HL2MPPLAYERLIBNAME	"CHL2MP_Player"
LUALIB_API int (luaopen_CHL2MP_Player) (lua_State *L);

#define LUA_ENGINEVGUILIBNAME	"enginevgui"
LUALIB_API int (luaopen_enginevgui) (lua_State *L);

#define LUA_DBGLIBNAME			"dbg"
LUALIB_API int (luaopen_dbg) (lua_State *L);

#define LUA_PANELLIBNAME		"Panel"
LUALIB_API int (luaopen_Panel) (lua_State *L);

#define LUA_SURFACELIBNAME		"surface"
LUALIB_API int (luaopen_surface) (lua_State *L);

#define LUA_VGUILIBNAME			"vgui"
LUALIB_API int (luaopen_vgui) (lua_State *L);


/* open all Source Engine libraries */
LUALIB_API void (luasrc_openlibs) (lua_State *L); 



#endif // LUASRCLIB_H
