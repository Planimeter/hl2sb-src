//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "vgui_BaseScriptedPanel.h"
#include <KeyValues.h>
#include <vgui/IScheme.h>
#include <vgui/IVGui.h>
#ifdef LUA_SDK
#include "luamanager.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CBaseScriptedPanel::CBaseScriptedPanel( vgui::Panel *pParent, const char *panelName ) : BaseClass( pParent, panelName )
{
#ifdef LUA_SDK
	m_nRefCount = LUA_NOREF;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : x - 
//			y - 
//			w - 
//			h - 
//-----------------------------------------------------------------------------
CBaseScriptedPanel::CBaseScriptedPanel( vgui::Panel *pParent, const char *panelName, int x, int y, int w, int h ) :
  CBasePanel( pParent, panelName, x, y, w, h )
{
#ifdef LUA_SDK
	m_nRefCount = LUA_NOREF;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CBaseScriptedPanel::~CBaseScriptedPanel( void )
{
#ifdef LUA_SDK
	lua_unref( L, m_nRefCount );
#endif
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseScriptedPanel::PaintBackground( void )
{
#ifdef LUA_SDK
#endif

	BaseClass::PaintBackground();
}

void CBaseScriptedPanel::OnCursorMoved(int x,int y)
{
#ifdef LUA_SDK
#endif

	BaseClass::OnCursorMoved(x, y);
}

void CBaseScriptedPanel::OnMousePressed(vgui::MouseCode code)
{
#ifdef LUA_SDK
#endif

	BaseClass::OnMousePressed(code);
}

void CBaseScriptedPanel::OnMouseDoublePressed(vgui::MouseCode code)
{
#ifdef LUA_SDK
#endif

	BaseClass::OnMouseDoublePressed(code);
}

void CBaseScriptedPanel::OnMouseReleased(vgui::MouseCode code)
{
#ifdef LUA_SDK
#endif

	BaseClass::OnMouseReleased(code);
}

void CBaseScriptedPanel::OnMouseWheeled(int delta)
{
#ifdef LUA_SDK
#endif

	BaseClass::OnMouseWheeled(delta);
}

void CBaseScriptedPanel::OnTick( void )
{
#ifdef LUA_SDK
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CScriptedHudLabel::CScriptedHudLabel( vgui::Panel *parent, const char *panelName, const char *text) :
	CHudLabel( parent,panelName,text )
{
}



