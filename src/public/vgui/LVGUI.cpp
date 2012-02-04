//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Basic header for using vgui
//
// $NoKeywords: $
//=============================================================================//

#define lvgui_cpp

#include "cbase.h"
#include "lua.hpp"
#include "lvgui.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


using namespace vgui;


/*
** access functions (stack -> C)
*/


LUA_API lua_HFont lua_tohfont (lua_State *L, int idx) {
  lua_HFont hFont = *(lua_HFont *)lua_touserdata(L, idx);
  return hFont;
}



/*
** push functions (C -> stack)
*/


LUA_API void lua_pushhfont (lua_State *L, HFont hFont) {
  lua_HFont *phFont = (lua_HFont *)lua_newuserdata(L, sizeof(hFont));
  *phFont = hFont;
  luaL_getmetatable(L, "HFont");
  lua_setmetatable(L, -2);
}


LUALIB_API lua_HFont luaL_checkhfont (lua_State *L, int narg) {
  lua_HFont *d = (lua_HFont *)luaL_checkudata(L, narg, "HFont");
  return *d;
}


static int HFont___tostring (lua_State *L) {
  HFont hFont = luaL_checkhfont(L, 1);
  lua_pushfstring(L, "HFont: %d", hFont);
  return 1;
}


static const luaL_Reg HFontmeta[] = {
  {"__tostring", HFont___tostring},
  {NULL, NULL}
};


/*
** Open HFont object
*/
LUALIB_API int luaopen_HFont (lua_State *L) {
  luaL_newmetatable(L, "HFont");
  luaL_register(L, NULL, HFontmeta);
  lua_pushvalue(L, -1);  /* push metatable */
  lua_setfield(L, -2, "__index");  /* metatable.__index = metatable */
  lua_pushstring(L, "hfont");
  lua_setfield(L, -2, "__type");  /* metatable.__type = "hfont" */
  lua_pop(L, 1);
  lua_pushhfont(L, INVALID_FONT);
  lua_setglobal(L, "INVALID_FONT");  /* set global INVALID_FONT */
  return 1;
}
