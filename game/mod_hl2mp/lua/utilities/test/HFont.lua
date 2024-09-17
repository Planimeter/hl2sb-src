--========== Copyleft © 2012, Team Sandbox, Some rights reserved. ===========--
--
-- Purpose: Tests the usage of HFont with surface library functions.
--
--===========================================================================--

if ( not bit ) then
  require( "bit" )
end

local bor = bit.bor
local vgui = require( "vgui" )
local Frame = vgui.Frame
local Panel = vgui.Panel

surface.AddCustomFontFile( "DIN-Light", "gamemodes\\sandbox\\content\\resource\\DINLi.ttf" )
local hTestFont = surface.CreateFont()
surface.SetFontGlyphSet( hTestFont,
                         "DIN-Light",
                         32,
                         0,
                         0,
                         0,
                         bor( 0x010, 0x100, 0x400 ) )

local strTextSample = "The five boxing wizards jump quickly."

g_hFontTestFrame = Frame()
g_hFontTestFrame:SetBounds( 0, 0, 408, 120 );
g_hFontTestFrame:SetSizeable( false )
g_hFontTestFrame:SetTitle( "Font Test", true )
g_hFontTestFrame:SetVisible( true )

g_hFontTestFrame.m_hFontSamples = Panel( g_hFontTestFrame, "FontSamples" )
g_hFontTestFrame.m_hFontSamples:SetProportional( true )

local iFontWide, iFontTall = surface.GetTextSize( hTestFont, strTextSample )
g_hFontTestFrame.m_hFontSamples:SetPos( 0, 120/2 - iFontTall/2 + 4 )
g_hFontTestFrame.m_hFontSamples:SetSize( 408, iFontTall + 8 )

function g_hFontTestFrame.m_hFontSamples:Paint()
  surface.DrawSetTextFont( hTestFont )
  surface.DrawPrintText( strTextSample )
end

-- move to the middle of the screen
g_hFontTestFrame:MoveToCenterOfScreen();
g_hFontTestFrame:InvalidateLayout();
g_hFontTestFrame:Activate();
