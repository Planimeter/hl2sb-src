//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef VGUI_LCHECKBUTTON_H
#define VGUI_LCHECKBUTTON_H

#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/CheckButton.h>

namespace vgui
{

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class LCheckButton : public CheckButton
{
	DECLARE_CLASS_SIMPLE( LCheckButton, CheckButton );

public:
	LCheckButton(Panel *parent, const char *panelName, const char *text);
	virtual ~LCheckButton();

public:
#if defined( LUA_SDK )
	int m_nRefCount;
#endif
};

} // namespace vgui

/* type for CheckButton functions */
typedef vgui::CheckButton lua_CheckButton;



/*
** access functions (stack -> C)
*/

LUA_API lua_CheckButton     *(lua_tocheckbutton) (lua_State *L, int idx);


/*
** push functions (C -> stack)
*/
LUA_API void  (lua_pushcheckbutton) (lua_State *L, lua_CheckButton *pCheckButton);



LUALIB_API lua_CheckButton *(luaL_checkcheckbutton) (lua_State *L, int narg);

#endif // VGUI_LCHECKBUTTON_H
