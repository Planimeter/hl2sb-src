--========== Copyleft © 2010, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: 
--
--===========================================================================--

include( "../includes/extensions/panel.lua" )

local FCVAR_CLIENTDLL = _E.FCVAR.CLIENTDLL

require( "concommand" )

local hContentDialog = INVALID_PANEL

local function PositionDialog(dlg)
	if ( dlg == INVALID_PANEL ) then
		return;
	end

	local x, y, ww, wt, wide, tall;
	x, y, ww, wt = surface.GetWorkspaceBounds();
	wide, tall = dlg:GetSize();

	-- Center it, keeping requested size
	dlg:SetPos(x + ((ww - wide) / 2), y + ((wt - tall) / 2));
end

local function OnOpenContentDialog()
	if ( ToPanel( hContentDialog ) == INVALID_PANEL ) then
		hContentDialog = vgui.CContentDialog(VGui_GetGameUIPanel(), "ContentDialog");
		PositionDialog( hContentDialog );
	end
	hContentDialog:Activate();
end

concommand.Create( "OpenContentDialog", OnOpenContentDialog, "Open content dialog.", FCVAR_CLIENTDLL )
