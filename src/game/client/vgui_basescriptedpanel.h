//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $NoKeywords: $
//=============================================================================//
#if !defined( VGUI_BASESCRIPTEDPANEL_H )
#define VGUI_BASESCRIPTEDPANEL_H
#ifdef _WIN32
#pragma once
#endif

#include <stdarg.h>
#include "vgui_BasePanel.h"
#include <vgui_controls/Panel.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/Controls.h>
#include <vgui/ISurface.h>

//-----------------------------------------------------------------------------
// Global interface allowing various rendering
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Purpose: Base Scripted Panel for scripted engine vgui panels ( can handle some drawing stuff )
//-----------------------------------------------------------------------------
class CBaseScriptedPanel : public CBasePanel
{
public:
	DECLARE_CLASS_GAMEROOT( CBaseScriptedPanel, CBasePanel );

					CBaseScriptedPanel( vgui::Panel *pParent, const char *panelName );
					CBaseScriptedPanel( vgui::Panel *pParent, const char *panelName, int x, int y, int w, int h );
	virtual			~CBaseScriptedPanel( void );

	// should this panel be drawn this frame?
	virtual bool	ShouldDraw( void ) { return true;}

	virtual void	PaintBackground( void );

	// If reflect mouse is true, then pass these up to parent
	virtual void	OnCursorMoved(int x,int y);
	virtual void	OnMousePressed(vgui::MouseCode code);
	virtual void	OnMouseDoublePressed(vgui::MouseCode code);
	virtual void	OnMouseReleased(vgui::MouseCode code);
	virtual void	OnMouseWheeled(int delta);

	virtual void	OnTick( void );

public:
	int				m_nRefCount;
};

//-----------------------------------------------------------------------------
// Purpose: Hud labels that use HUD scheme colors
//-----------------------------------------------------------------------------
class CScriptedHudLabel : public CHudLabel
{
	typedef CHudLabel BaseClass;
public:
	CScriptedHudLabel( vgui::Panel *parent, const char *panelName, const char *text );
	
	// Selection highlight
	void	SetSelected( bool bSelected );

	bool	m_bSelected;

private:
	CScriptedHudLabel( const CScriptedHudLabel & ); // not defined, not accessible
};

#endif // VGUI_BASESCRIPTEDPANEL_H
