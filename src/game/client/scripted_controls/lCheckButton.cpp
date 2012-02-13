//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include <cbase.h>

#include <vgui_int.h>
#include <luamanager.h>
#include <vgui_controls/lPanel.h>
#include <lColor.h>

#include <scripted_controls/lCheckButton.h>

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
LCheckButton::LCheckButton(Panel *parent, const char *panelName, const char *text) : CheckButton(parent, panelName, text)
{
#if defined( LUA_SDK )
	m_nRefCount = LUA_NOREF;
#endif
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
LCheckButton::~LCheckButton()
{
#ifdef LUA_SDK
	lua_unref( L, m_nRefCount );
#endif
}

/*
** access functions (stack -> C)
*/


LUA_API lua_CheckButton *lua_tocheckbutton (lua_State *L, int idx) {
  PHandle *phPanel = dynamic_cast<PHandle *>((PHandle *)lua_touserdata(L, idx));
  if (phPanel == NULL)
    return NULL;
  return dynamic_cast<lua_CheckButton *>(phPanel->Get());
}



/*
** push functions (C -> stack)
*/


LUA_API void lua_pushcheckbutton (lua_State *L, CheckButton *pCheckButton) {
  PHandle *phPanel = (PHandle *)lua_newuserdata(L, sizeof(PHandle));
  phPanel->Set(pCheckButton);
  luaL_getmetatable(L, "CheckButton");
  lua_setmetatable(L, -2);
}


LUALIB_API lua_CheckButton *luaL_checkcheckbutton (lua_State *L, int narg) {
  lua_CheckButton *d = lua_tocheckbutton(L, narg);
  if (d == NULL)  /* avoid extra test when d is not 0 */
    luaL_argerror(L, narg, "attempt to index an INVALID_PANEL");
  return d;
}


static int CheckButton_ChainToAnimationMap (lua_State *L) {
  luaL_checkcheckbutton(L, 1)->ChainToAnimationMap();
  return 0;
}

static int CheckButton_ChainToMap (lua_State *L) {
  luaL_checkcheckbutton(L, 1)->ChainToMap();
  return 0;
}

static int CheckButton_GetDisabledBgColor (lua_State *L) {
  lua_pushcolor(L, luaL_checkcheckbutton(L, 1)->GetDisabledBgColor());
  return 1;
}

static int CheckButton_GetDisabledFgColor (lua_State *L) {
  lua_pushcolor(L, luaL_checkcheckbutton(L, 1)->GetDisabledFgColor());
  return 1;
}

static int CheckButton_GetPanelBaseClassName (lua_State *L) {
  lua_pushstring(L, luaL_checkcheckbutton(L, 1)->GetPanelBaseClassName());
  return 1;
}

static int CheckButton_GetPanelClassName (lua_State *L) {
  lua_pushstring(L, luaL_checkcheckbutton(L, 1)->GetPanelClassName());
  return 1;
}

static int CheckButton_KB_AddBoundKey (lua_State *L) {
  luaL_checkcheckbutton(L, 1)->KB_AddBoundKey(luaL_checkstring(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4));
  return 0;
}

static int CheckButton_KB_ChainToMap (lua_State *L) {
  luaL_checkcheckbutton(L, 1)->KB_ChainToMap();
  return 0;
}

static int CheckButton_SetCheckButtonCheckable (lua_State *L) {
  luaL_checkcheckbutton(L, 1)->SetCheckButtonCheckable(luaL_checkboolean(L, 2));
  return 0;
}

static int CheckButton_SetSelected (lua_State *L) {
  luaL_checkcheckbutton(L, 1)->SetSelected(luaL_checkboolean(L, 2));
  return 0;
}

static int CheckButton___index (lua_State *L) {
  CheckButton *pCheckButton = lua_tocheckbutton(L, 1);
  if (pCheckButton == NULL) {  /* avoid extra test when d is not 0 */
    lua_Debug ar1;
    lua_getstack(L, 1, &ar1);
    lua_getinfo(L, "fl", &ar1);
    lua_Debug ar2;
    lua_getinfo(L, ">S", &ar2);
	lua_pushfstring(L, "%s:%d: attempt to index an INVALID_PANEL", ar2.short_src, ar1.currentline);
	return lua_error(L);
  }
  LCheckButton *plCheckButton = dynamic_cast<LCheckButton *>(pCheckButton);
  if (plCheckButton && plCheckButton->m_nRefCount != LUA_NOREF) {
    lua_getref(L, plCheckButton->m_nRefCount);
    lua_pushvalue(L, 2);
    lua_gettable(L, -2);
    if (lua_isnil(L, -1)) {
      lua_pop(L, 1);
      lua_getmetatable(L, 1);
      lua_pushvalue(L, 2);
      lua_gettable(L, -2);
      if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        luaL_getmetatable(L, "Panel");
        lua_pushvalue(L, 2);
        lua_gettable(L, -2);
      }
    }
  } else {
    lua_getmetatable(L, 1);
    lua_pushvalue(L, 2);
    lua_gettable(L, -2);
    if (lua_isnil(L, -1)) {
      lua_pop(L, 1);
      luaL_getmetatable(L, "Panel");
      lua_pushvalue(L, 2);
      lua_gettable(L, -2);
    }
  }
  return 1;
}

static int CheckButton___newindex (lua_State *L) {
  CheckButton *pCheckButton = lua_tocheckbutton(L, 1);
  if (pCheckButton == NULL) {  /* avoid extra test when d is not 0 */
    lua_Debug ar1;
    lua_getstack(L, 1, &ar1);
    lua_getinfo(L, "fl", &ar1);
    lua_Debug ar2;
    lua_getinfo(L, ">S", &ar2);
    lua_pushfstring(L, "%s:%d: attempt to index an INVALID_PANEL", ar2.short_src, ar1.currentline);
    return lua_error(L);
  }
  LCheckButton *plCheckButton = dynamic_cast<LCheckButton *>(pCheckButton);
  if (plCheckButton) {
    if (plCheckButton->m_nRefCount == LUA_NOREF) {
      lua_newtable(L);
      plCheckButton->m_nRefCount = luaL_ref(L, LUA_REGISTRYINDEX);
    }
    lua_getref(L, plCheckButton->m_nRefCount);
    lua_pushvalue(L, 3);
    lua_setfield(L, -2, luaL_checkstring(L, 2));
    return 0;
  } else {
    lua_Debug ar1;
    lua_getstack(L, 1, &ar1);
    lua_getinfo(L, "fl", &ar1);
    lua_Debug ar2;
    lua_getinfo(L, ">S", &ar2);
    lua_pushfstring(L, "%s:%d: attempt to index a non-scripted panel", ar2.short_src, ar1.currentline);
    return lua_error(L);
  }
}

static int CheckButton___gc (lua_State *L) {
  LCheckButton *plCheckButton = dynamic_cast<LCheckButton *>(lua_tocheckbutton(L, 1));
  if (plCheckButton) {
    delete plCheckButton;
  }
  return 0;
}

static int CheckButton___eq (lua_State *L) {
  lua_pushboolean(L, lua_tocheckbutton(L, 1) == lua_tocheckbutton(L, 2));
  return 1;
}

static int CheckButton___tostring (lua_State *L) {
  CheckButton *pCheckButton = lua_tocheckbutton(L, 1);
  if (pCheckButton == NULL)
    lua_pushstring(L, "INVALID_PANEL");
  else {
    const char *pName = pCheckButton->GetName();
    if (Q_strcmp(pName, "") == 0)
      pName = "(no name)";
    lua_pushfstring(L, "CheckButton: \"%s\"", pName);
  }
  return 1;
}


static const luaL_Reg CheckButtonmeta[] = {
  {"ChainToAnimationMap", CheckButton_ChainToAnimationMap},
  {"ChainToMap", CheckButton_ChainToMap},
  {"GetDisabledBgColor", CheckButton_GetDisabledBgColor},
  {"GetDisabledFgColor", CheckButton_GetDisabledFgColor},
  {"GetPanelBaseClassName", CheckButton_GetPanelBaseClassName},
  {"GetPanelClassName", CheckButton_GetPanelClassName},
  {"KB_AddBoundKey", CheckButton_KB_AddBoundKey},
  {"KB_ChainToMap", CheckButton_KB_ChainToMap},
  {"SetCheckButtonCheckable", CheckButton_SetCheckButtonCheckable},
  {"SetSelected", CheckButton_SetSelected},
  {"__index", CheckButton___index},
  {"__newindex", CheckButton___newindex},
  {"__gc", CheckButton___gc},
  {"__eq", CheckButton___eq},
  {"__tostring", CheckButton___tostring},
  {NULL, NULL}
};


static int luasrc_CheckButton (lua_State *L) {
  CheckButton *pCheckButton = new LCheckButton(luaL_optpanel(L, 1, VGui_GetClientLuaRootPanel()), luaL_optstring(L, 2, NULL), luaL_optstring(L, 3, NULL));
  lua_pushcheckbutton(L, pCheckButton);
  return 1;
}


static const luaL_Reg CheckButton_funcs[] = {
  {"CheckButton", luasrc_CheckButton},
  {NULL, NULL}
};


/*
** Open CheckButton object
*/
LUALIB_API int luaopen_vgui_CheckButton(lua_State *L) {
  luaL_newmetatable(L, "CheckButton");
  luaL_register(L, NULL, CheckButtonmeta);
  lua_pushstring(L, "panel");
  lua_setfield(L, -2, "__type");  /* metatable.__type = "panel" */
  luaL_register(L, "vgui", CheckButton_funcs);
  lua_pop(L, 2);
  return 1;
}
