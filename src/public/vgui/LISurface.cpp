//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#define LISurface_cpp

#include "cbase.h"
#include "vgui/isurface.h"
#include "vgui_controls/Controls.h"
#include "luamanager.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;



static int surface_AddBitmapFontFile (lua_State *L) {
  lua_pushboolean(L, surface()->AddBitmapFontFile(luaL_checkstring(L, 1)));
  return 1;
}

static int surface_AddCustomFontFile (lua_State *L) {
  lua_pushboolean(L, surface()->AddCustomFontFile(luaL_checkstring(L, 1)));
  return 1;
}

static int surface_ApplyChanges (lua_State *L) {
  surface()->ApplyChanges();
  return 0;
}

static int surface_CalculateMouseVisible (lua_State *L) {
  surface()->CalculateMouseVisible();
  return 0;
}

static int surface_ClearTemporaryFontCache (lua_State *L) {
  surface()->ClearTemporaryFontCache();
  return 0;
}

static int surface_CreateNewTextureID (lua_State *L) {
  lua_pushinteger(L, surface()->CreateNewTextureID(luaL_optboolean(L, 1, 0)));
  return 1;
}

static int surface_DrawFilledRect (lua_State *L) {
  surface()->DrawFilledRect(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4));
  return 0;
}

static int surface_DrawFilledRectFade (lua_State *L) {
  surface()->DrawFilledRectFade(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4), luaL_checkint(L, 5), luaL_checkint(L, 6), luaL_checkboolean(L, 7));
  return 0;
}

static int surface_DrawFlushText (lua_State *L) {
  surface()->DrawFlushText();
  return 0;
}

static int surface_DrawGetAlphaMultiplier (lua_State *L) {
  lua_pushnumber(L, surface()->DrawGetAlphaMultiplier());
  return 1;
}

static int surface_DrawGetTextPos (lua_State *L) {
  int x, y;
  surface()->DrawGetTextPos(x, y);
  lua_pushinteger(L, x);
  lua_pushinteger(L, y);
  return 2;
}

static int surface_DrawGetTextureFile (lua_State *L) {
  char * filename = "";
  lua_pushboolean(L, surface()->DrawGetTextureFile(luaL_checkint(L, 1), filename, luaL_checkint(L, 2)));
  lua_pushstring(L, filename);
  return 2;
}

static int surface_DrawGetTextureId (lua_State *L) {
  lua_pushinteger(L, surface()->DrawGetTextureId(luaL_checkstring(L, 1)));
  return 1;
}

static int surface_DrawGetTextureSize (lua_State *L) {
  int wide, tall;
  surface()->DrawGetTextureSize(luaL_checkint(L, 1), wide, tall);
  lua_pushinteger(L, wide);
  lua_pushinteger(L, tall);
  return 2;
}

static int surface_DrawLine (lua_State *L) {
  surface()->DrawLine(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4));
  return 0;
}

static int surface_DrawOutlinedCircle (lua_State *L) {
  surface()->DrawOutlinedCircle(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4));
  return 0;
}

static int surface_DrawOutlinedRect (lua_State *L) {
  surface()->DrawOutlinedRect(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4));
  return 0;
}

static int surface_DrawSetAlphaMultiplier (lua_State *L) {
  surface()->DrawSetAlphaMultiplier(luaL_checknumber(L, 1));
  return 0;
}

static int surface_DrawSetColor (lua_State *L) {
  surface()->DrawSetColor(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4));
  return 0;
}

static int surface_DrawSetTextColor (lua_State *L) {
  surface()->DrawSetTextColor(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4));
  return 0;
}

static int surface_DrawSetTextPos (lua_State *L) {
  surface()->DrawSetTextPos(luaL_checkint(L, 1), luaL_checkint(L, 2));
  return 0;
}

static int surface_DrawSetTextScale (lua_State *L) {
  surface()->DrawSetTextScale(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
  return 0;
}

static int surface_DrawSetTexture (lua_State *L) {
  surface()->DrawSetTexture(luaL_checkint(L, 1));
  return 0;
}

static int surface_DrawSetTextureFile (lua_State *L) {
  surface()->DrawSetTextureFile(luaL_checkint(L, 1), luaL_checkstring(L, 2), luaL_checkint(L, 3), luaL_checkboolean(L, 4));
  return 0;
}

static int surface_DrawTexturedRect (lua_State *L) {
  surface()->DrawTexturedRect(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4));
  return 0;
}

static int surface_DrawTexturedSubRect (lua_State *L) {
  surface()->DrawTexturedSubRect(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4), luaL_checknumber(L, 5), luaL_checknumber(L, 6), luaL_checknumber(L, 7), luaL_checknumber(L, 8));
  return 0;
}

static int surface_GetAbsoluteWindowBounds (lua_State *L) {
  int x, y, wide, tall;
  surface()->GetAbsoluteWindowBounds(x, y, wide, tall);
  lua_pushinteger(L, x);
  lua_pushinteger(L, y);
  lua_pushinteger(L, wide);
  lua_pushinteger(L, tall);
  return 4;
}

static int surface_GetBitmapFontName (lua_State *L) {
  lua_pushstring(L, surface()->GetBitmapFontName(luaL_checkstring(L, 1)));
  return 1;
}

static int surface_GetPopupCount (lua_State *L) {
  lua_pushinteger(L, surface()->GetPopupCount());
  return 1;
}

static int surface_GetProportionalBase (lua_State *L) {
  int width, height;
  surface()->GetProportionalBase(width, height);
  lua_pushinteger(L, width);
  lua_pushinteger(L, height);
  return 2;
}

static int surface_GetResolutionKey (lua_State *L) {
  lua_pushstring(L, surface()->GetResolutionKey());
  return 1;
}

static int surface_GetScreenSize (lua_State *L) {
  int wide, tall;
  surface()->GetScreenSize(wide, tall);
  lua_pushinteger(L, wide);
  lua_pushinteger(L, tall);
  return 2;
}

static int surface_GetWorkspaceBounds (lua_State *L) {
  int x, y, wide, tall;
  surface()->GetWorkspaceBounds(x, y, wide, tall);
  lua_pushinteger(L, x);
  lua_pushinteger(L, y);
  lua_pushinteger(L, wide);
  lua_pushinteger(L, tall);
  return 4;
}

static int surface_GetZPos (lua_State *L) {
  lua_pushnumber(L, surface()->GetZPos());
  return 1;
}

static int surface_HasCursorPosFunctions (lua_State *L) {
  lua_pushboolean(L, surface()->HasCursorPosFunctions());
  return 1;
}

static int surface_HasFocus (lua_State *L) {
  lua_pushboolean(L, surface()->HasFocus());
  return 1;
}

static int surface_IsCursorLocked (lua_State *L) {
  lua_pushboolean(L, surface()->IsCursorLocked());
  return 1;
}

static int surface_IsCursorVisible (lua_State *L) {
  lua_pushboolean(L, surface()->IsCursorVisible());
  return 1;
}

static int surface_IsTextureIDValid (lua_State *L) {
  lua_pushboolean(L, surface()->IsTextureIDValid(luaL_checkint(L, 1)));
  return 1;
}

static int surface_IsWithin (lua_State *L) {
  lua_pushboolean(L, surface()->IsWithin(luaL_checkint(L, 1), luaL_checkint(L, 2)));
  return 1;
}

static int surface_LockCursor (lua_State *L) {
  surface()->LockCursor();
  return 0;
}

static int surface_NeedKBInput (lua_State *L) {
  lua_pushboolean(L, surface()->NeedKBInput());
  return 1;
}

static int surface_OnScreenSizeChanged (lua_State *L) {
  surface()->OnScreenSizeChanged(luaL_checkint(L, 1), luaL_checkint(L, 2));
  return 0;
}

static int surface_PlaySound (lua_State *L) {
  surface()->PlaySound(luaL_checkstring(L, 1));
  return 0;
}

static int surface_RunFrame (lua_State *L) {
  surface()->RunFrame();
  return 0;
}

static int surface_SetAllowHTMLJavaScript (lua_State *L) {
  surface()->SetAllowHTMLJavaScript(luaL_checkboolean(L, 1));
  return 0;
}

static int surface_SetBitmapFontName (lua_State *L) {
  surface()->SetBitmapFontName(luaL_checkstring(L, 1), luaL_checkstring(L, 2));
  return 0;
}

static int surface_SetTranslateExtendedKeys (lua_State *L) {
  surface()->SetTranslateExtendedKeys(luaL_checkboolean(L, 1));
  return 0;
}

static int surface_SetWorkspaceInsets (lua_State *L) {
  surface()->SetWorkspaceInsets(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4));
  return 0;
}

static int surface_Shutdown (lua_State *L) {
  surface()->Shutdown();
  return 0;
}

static int surface_SupportsFeature (lua_State *L) {
  surface()->SupportsFeature((ISurface::SurfaceFeature_e)luaL_checkint(L, 1));
  return 0;
}

static int surface_SurfaceGetCursorPos (lua_State *L) {
  int x, y;
  surface()->SurfaceGetCursorPos(x, y);
  lua_pushinteger(L, x);
  lua_pushinteger(L, y);
  return 2;
}

static int surface_SurfaceSetCursorPos (lua_State *L) {
  surface()->SurfaceSetCursorPos(luaL_checkint(L, 1), luaL_checkint(L, 2));
  return 0;
}

static int surface_UnlockCursor (lua_State *L) {
  surface()->UnlockCursor();
  return 0;
}


static const luaL_Reg surfacelib[] = {
  {"AddBitmapFontFile",   surface_AddBitmapFontFile},
  {"AddCustomFontFile",   surface_AddCustomFontFile},
  {"ApplyChanges",   surface_ApplyChanges},
  {"CalculateMouseVisible",   surface_CalculateMouseVisible},
  {"ClearTemporaryFontCache",   surface_ClearTemporaryFontCache},
  {"CreateNewTextureID",   surface_CreateNewTextureID},
  {"DrawFilledRect",   surface_DrawFilledRect},
  {"DrawFilledRectFade",   surface_DrawFilledRectFade},
  {"DrawFlushText",   surface_DrawFlushText},
  {"DrawGetAlphaMultiplier",   surface_DrawGetAlphaMultiplier},
  {"DrawGetTextPos",   surface_DrawGetTextPos},
  {"DrawGetTextureFile",   surface_DrawGetTextureFile},
  {"DrawGetTextureId",   surface_DrawGetTextureId},
  {"DrawGetTextureSize",   surface_DrawGetTextureSize},
  {"DrawLine",   surface_DrawLine},
  {"DrawOutlinedCircle",   surface_DrawOutlinedCircle},
  {"DrawOutlinedRect",   surface_DrawOutlinedRect},
  {"DrawSetAlphaMultiplier",   surface_DrawSetAlphaMultiplier},
  {"DrawSetColor",   surface_DrawSetColor},
  {"DrawSetTextColor",   surface_DrawSetTextColor},
  {"DrawSetTextPos",   surface_DrawSetTextPos},
  {"DrawSetTextScale",   surface_DrawSetTextScale},
  {"DrawSetTexture",   surface_DrawSetTexture},
  {"DrawSetTextureFile",   surface_DrawSetTextureFile},
  {"DrawTexturedRect",   surface_DrawTexturedRect},
  {"DrawTexturedSubRect",   surface_DrawTexturedSubRect},
  {"GetAbsoluteWindowBounds",   surface_GetAbsoluteWindowBounds},
  {"GetBitmapFontName",   surface_GetBitmapFontName},
  {"GetPopupCount",   surface_GetPopupCount},
  {"GetProportionalBase",   surface_GetProportionalBase},
  {"GetResolutionKey",   surface_GetResolutionKey},
  {"GetScreenSize",   surface_GetScreenSize},
  {"GetWorkspaceBounds",   surface_GetWorkspaceBounds},
  {"GetZPos",   surface_GetZPos},
  {"HasCursorPosFunctions",   surface_HasCursorPosFunctions},
  {"HasFocus",   surface_HasFocus},
  {"IsCursorLocked",   surface_IsCursorLocked},
  {"IsCursorVisible",   surface_IsCursorVisible},
  {"IsTextureIDValid",   surface_IsTextureIDValid},
  {"IsWithin",   surface_IsWithin},
  {"LockCursor",   surface_LockCursor},
  {"NeedKBInput",   surface_NeedKBInput},
  {"OnScreenSizeChanged",   surface_OnScreenSizeChanged},
  {"PlaySound",   surface_PlaySound},
  {"RunFrame",   surface_RunFrame},
  {"SetAllowHTMLJavaScript",   surface_SetAllowHTMLJavaScript},
  {"SetBitmapFontName",   surface_SetBitmapFontName},
  {"SetTranslateExtendedKeys",   surface_SetTranslateExtendedKeys},
  {"SetWorkspaceInsets",   surface_SetWorkspaceInsets},
  {"Shutdown",   surface_Shutdown},
  {"SupportsFeature",   surface_SupportsFeature},
  {"SurfaceGetCursorPos",   surface_SurfaceGetCursorPos},
  {"SurfaceSetCursorPos",   surface_SurfaceSetCursorPos},
  {"UnlockCursor",   surface_UnlockCursor},
  {NULL, NULL}
};


/*
** Open surface library
*/
int luaopen_surface (lua_State *L) {
  luaL_register(L, "surface", surfacelib);
  lua_pop(L, 1);
  return 1;
}

