--========= Copyleft © 2010, Team Sandbox, Some rights reserved. ============--
--
-- Purpose:
--
-- $NoKeywords: $
--===========================================================================--

local vgui = vgui;
local CBuildSubMenu = vgui.CBuildSubMenu
local ScreenWidth = UTIL.ScreenWidth
local ScreenHeight = UTIL.ScreenHeight

local CBuildMenu = {
	m_hFonts = {},
	m_lastx = nil,
	m_lasty = nil,
}

-------------------------------------------------------------------------------
-- Purpose: Constructor
-------------------------------------------------------------------------------
function CBuildMenu:Init(pViewPort)
	self:SetProportional(true);
	-- Make it screen sized
	self:SetBounds( 0, 0, ScreenWidth(), ScreenHeight() );


	self:SetAutoDelete( false );
	
	self.m_pViewPort = pViewPort;

if false then
	self.m_pMainMenu = CBuildSubMenu( self, "mainmenu" );
	self.m_pMainMenu:LoadControlSettings( "Resource/UI/MainBuyMenu.res" );
	self.m_pMainMenu:SetVisible( false );
end
	self:SetVisible( false )
end

function CBuildMenu:ApplySchemeSettings( pScheme )
	self:SetBgColor( Color(0, 0, 0, 80) );
end

-------------------------------------------------------------------------------
-- Purpose: shows/hides the build menu
-------------------------------------------------------------------------------
function CBuildMenu:ShowPanel(bShow)
	if ( self:IsVisible() == bShow ) then
		return;
	end

	if ( bShow ) then
		self:Update();

		self:SetMouseInputEnabled( true );
		self:SetVisible( true )
		self:MakePopup();

		if ( self.m_lastx and self.m_lasty ) then
			input.SetCursorPos( self.m_lastx, self.m_lasty )
		end
	else
		self.m_lastx, self.m_lasty = input.GetCursorPos()

		self:SetVisible( false );
		self:SetMouseInputEnabled( false );
	end
end


function CBuildMenu:Update()
end
function CBuildMenu:OnClose()
	self.BaseClass.OnClose( self );
end

vgui.register( CBuildMenu, "CBuildMenu", "Panel" )
